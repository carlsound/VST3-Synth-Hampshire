#pragma once

#include "noteExpressionSynthController.h"
#include "maximilian.h"

#include "public.sdk/samples/vst/common/voicebase.h"
#include "public.sdk/samples/vst/common/logscale.h"

#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/base/futils.h"

#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI			3.14159265358979323846
#endif
#ifndef M_PI_MUL_2
#define M_PI_MUL_2		6.28318530717958647692
#endif

namespace Carlsound
{
	namespace Hampshire
	{
		
		//#define MAX_VOICES				64
		//#define MAX_RELEASE_TIME_SEC	5.0
		//#define NUM_FILTER_TYPE			3
		//#define NUM_TUNING_RANGE		2 

		//-----------------------------------------------------------------------------
		struct GlobalParameterState
		{
			Steinberg::Vst::ParamValue masterVolume;	// [0, +1]
			//
			Steinberg::tresult setState (Steinberg::IBStream* stream);
			Steinberg::tresult getState (Steinberg::IBStream* stream);
		};

		//-----------------------------------------------------------------------------
		enum VoiceParameters
		{
			kVolumeMod
		};
		
		//-----------------------------------------------------------------------------
		class VoiceStatics
		{
			public:
			//------------------------------------------------------------------------
			static double normalizedLevel2Gain (float normalized)
			{
				double level;
				if (normalized >= 0.5)
					level = scaleHeadRoom * ::pow (10, (normalized - 0.5f) * 24 / 20.0f);
				else
					level = scaleNorm2GainC1 * ::pow (normalized, scaleNorm2GainC2);	
		
				return level;
			}	
		
			enum {
				kNumFrequencies = 128
			};
		
			static float freqTab[kNumFrequencies];
			static const float scaleHeadRoom;
			static const float scaleNorm2GainC1;
			static const float scaleNorm2GainC2;
			static Steinberg::Vst::LogScale<Steinberg::Vst::ParamValue> freqLogScale;
			static const double kNormTuningOneOctave;
			static const double kNormTuningOneTune;
		
		};
		
		//-----------------------------------------------------------------------------
		/** Example Note Expression Synth Voice Class
		
		\sa Steinberg::Vst::VoiceBase
		*/
		template<class SamplePrecision>
		class Voice : public Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>
		{
		public:
			Voice ();
			~Voice ();
		
			void setSampleRate (Steinberg::Vst::ParamValue sampleRate) SMTG_OVERRIDE;
			void noteOn (Steinberg::int32 pitch, Steinberg::Vst::ParamValue velocity, float tuning, Steinberg::int32 sampleOffset, Steinberg::int32 nId) SMTG_OVERRIDE;
			void noteOff (Steinberg::Vst::ParamValue velocity, Steinberg::int32 sampleOffset) SMTG_OVERRIDE;
			bool process (SamplePrecision* outputBuffers[2], Steinberg::int32 numSamples);
			void reset () SMTG_OVERRIDE;

			void setNoteExpressionValue (Steinberg::int32 index, Steinberg::Vst::ParamValue value) SMTG_OVERRIDE;
		
		protected:
			Steinberg::uint32 n;
			Steinberg::int32 noisePos;
			Steinberg::int32 noiseStep;
		
			Steinberg::Vst::ParamValue currentVolume;
		
			Steinberg::Vst::ParamValue levelFromVel;
			Steinberg::Vst::ParamValue noteOffVolumeRamp;
		};
		
		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		void Voice<SamplePrecision>::setNoteExpressionValue (Steinberg::int32 index, Steinberg::Vst::ParamValue value)
		{
			if (this->globalParameters->bypassSNA)
				return;
			
			switch (index)
			{
				//------------------------------
				case Steinberg::Vst::kVolumeTypeID:
				{
					Steinberg::Vst::ParamValue vol = VoiceStatics::normalizedLevel2Gain ((float)value);
					Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kVolumeMod, vol);
					break;
				}
				//------------------------------
				default:
				{
					Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (index, value);
					break;
				}
			}
		}
		
		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		bool Voice<SamplePrecision>::process (SamplePrecision* outputBuffers[2], Steinberg::int32 numSamples)
		{
			//---compute tuning-------------------------
		
			// main tuning
			Steinberg::Vst::ParamValue tuningInHz = 0.;
			if (this->values[kTuningMod] != 0. || this->globalParameters->masterTuning != 0 || this->tuning != 0)
			{
				tuningInHz = VoiceStatics::freqTab[this->pitch] * (::pow (2.0, (this->values[kTuningMod] * 10 + this->globalParameters->masterTuning * 2.0 / 12.0 + this->tuning)) - 1);
			}
	
			Steinberg::Vst::ParamValue triangleFreq = (VoiceStatics::freqTab[this->pitch] + tuningInHz) * M_PI_MUL_2 / this->getSampleRate () / 2.;
			if (currentTriangleF == -1)
				currentTriangleF = triangleFreq;
			// check for frequency changes and update the phase so that it is crackle free
			if (triangleFreq != currentTriangleF)
			{
				// update phase
				trianglePhase = (SamplePrecision)((currentTriangleF - triangleFreq) * n + trianglePhase);
				currentTriangleF = triangleFreq;
			}
			
			//---calculate parameter ramps
			Steinberg::Vst::ParamValue volumeRamp = 0.;
			Steinberg::Vst::ParamValue rampTime = std::max<Steinberg::Vst::ParamValue> ((Steinberg::Vst::ParamValue)numSamples, (this->sampleRate * 0.005));
			
			Steinberg::Vst::ParamValue wantedVolume = VoiceStatics::normalizedLevel2Gain ((float)Steinberg::Bound (0.0, 1.0, this->globalParameters->masterVolume * levelFromVel + this->values[kVolumeMod]));
			if (wantedVolume != currentVolume)
			{
				volumeRamp = (wantedVolume - currentVolume) / rampTime;
			}
		
			for (Steinberg::int32 i = 0; i < numSamples; i++)
			{
				this->noteOnSampleOffset--;
				this->noteOffSampleOffset--;
				
				if (this->noteOnSampleOffset <= 0)
				{
					// we are in Release
					if (this->noteOffSampleOffset == 0)
						{
 						volumeRamp = 0;
						if (currentVolume > 0)
						{
							// ramp note off
							currentVolume -= noteOffVolumeRamp;
							if (currentVolume < 0.)
								currentVolume = 0.;
							this->noteOffSampleOffset++;
						}
						else
						{
							this->noteOffSampleOffset = this->noteOnSampleOffset = -1;
							return false;
						}
					}
					SamplePrecision sample;
					SamplePrecision osc = (SamplePrecision)sin (n * triangleFreq + trianglePhase);
					sample += (SamplePrecision)(sin (n * sinusFreq + sinusPhase) * currentSinusVolume);
		
					n++;
					
					// store in output
					outputBuffers[0][i] += (SamplePrecision)(sample * currentPanningLeft * currentVolume);
					outputBuffers[1][i] += (SamplePrecision)(sample * currentPanningRight * currentVolume);
		
					// ramp parameters
					currentVolume += volumeRamp;
				}
			}
			return true;
		}
		
		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		void Voice<SamplePrecision>::noteOn (Steinberg::int32 _pitch, Steinberg::Vst::ParamValue velocity, float tuning, Steinberg::int32 sampleOffset, Steinberg::int32 nId)
		{	
			currentVolume = 0;
			this->values[kVolumeMod] = 0;
			levelFromVel = 1.f + this->globalParameters->velToLevel * (velocity - 1.);

			currentSinusVolume = this->values[kSinusVolume] = this->globalParameters->sinusVolume;
		
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::noteOn (_pitch, velocity, tuning, sampleOffset, nId);
			this->noteOnSampleOffset++;
		}
		
		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		void Voice<SamplePrecision>::noteOff (Steinberg::Vst::ParamValue velocity, Steinberg::int32 sampleOffset)
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::noteOff (velocity, sampleOffset);
			this->noteOffSampleOffset++;
		
			Steinberg::Vst::ParamValue timeFactor = ::pow (100., this->values[kReleaseTimeMod]);
			
			noteOffVolumeRamp = 1.0 / (timeFactor * this->sampleRate * ((this->globalParameters->releaseTime * MAX_RELEASE_TIME_SEC) + 0.005));
			if (currentVolume)
				noteOffVolumeRamp *= currentVolume;
		}
		
		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		void Voice<SamplePrecision>::reset ()
		{
			noiseStep = 1;
			noisePos = 0;
			n = 0;
			sinusPhase = trianglePhase = 0.;
			currentSinusF = currentTriangleF = -1.;
			this->values[kVolumeMod] = 0.;

			noteOffVolumeRamp = 0.005;
			
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::reset ();
		}
		
		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		void Voice<SamplePrecision>::setSampleRate (Steinberg::Vst::ParamValue sampleRate)
		{
			filter->setSampleRate (sampleRate);
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setSampleRate (sampleRate);
		}
		
		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		Voice<SamplePrecision>::~Voice ()
		{
			delete filter;
		}
		
	} // Hampshire
} // Carlsound
