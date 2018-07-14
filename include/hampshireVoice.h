#pragma once
//
#include "hampshireGlobalParameterStorage.h"
#include "hampshireController.h"
#include "hampshireVoiceStatics.h"
#include "hampshireVoiceStaticsOnce.h"
#include "maximilian.h"
#include "voiceBase.h"
//
//#include "public.sdk/samples/vst/common/voicebase.h"
#include "public.sdk/samples/vst/common/logscale.h"
//
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/base/futils.h"
//
#include "base/source/fstreamer.h"
//
#include <cmath>
#include <algorithm>
//
#ifndef M_PI
#define M_PI			3.14159265358979323846
#endif
#ifndef M_PI_MUL_2
#define M_PI_MUL_2		6.28318530717958647692
#endif
//
namespace Carlsound
{
	namespace Hampshire
	{
		static Steinberg::uint64 currentParamStateVersion = 3;

		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		//
		class Voice : public Carlsound::Vst::VoiceBase
		<
			Carlsound::Hampshire::kNumParameters,
			SamplePrecision,
			Carlsound::Hampshire::kNumChannels,
			GlobalParameterStorage
		>
		{
		public:
			//-----------------------------------------------------------------------------
			Voice ()
			{
				m_voiceBase = std::make_shared
				<
					Carlsound::Vst::VoiceBase
					<
						kNumParameters,
						SamplePrecision,
						Carlsound::Hampshire::kNumChannels,
						GlobalParameterStorage
					>
				>();
				//filter = new Filter (Filter::kLowpass);
				m_oscillator = std::make_shared<maxiOsc>();
				m_oscillatorSettings = std::make_shared<maxiSettings>();
			}
			//
			//-----------------------------------------------------------------------------
			~Voice ()
			{
				//delete filter;
			}
			//
			//-----------------------------------------------------------------------------
			void setSampleRate
			(
				Steinberg::Vst::ParamValue sampleRate
			) 
				SMTG_OVERRIDE
			{
				//filter->setSampleRate (sampleRate);
				m_voiceBase->setSampleRate(sampleRate);
				m_oscillatorSettings->sampleRate = sampleRate;
			}
			//-----------------------------------------------------------------------------
			float getSampleRate() const
			{ 
				return m_voiceBase->getSampleRate();
			}
			//
			//-----------------------------------------------------------------------------
			void setNoteExpressionValue
			(
				Steinberg::int32 index,
				Steinberg::Vst::ParamValue value
			)
				SMTG_OVERRIDE
			{
				switch (index) // ivstnoteexpression.h enum NoteExpressionTypeIDs: kVolumTypeID, kPanTypeID, kTuningTypeID, kVibratoTypeID, kExpressionTypeID, kBrightnessTypeID, kTextTypeID, kPhonemeTypeID
				{
					//------------------------------
					case Steinberg::Vst::kVolumeTypeID:
					{
						/*
						Steinberg::Vst::ParamValue vol = VoiceStatics::normalizedLevel2Gain ((float) value);
						m_voiceBase->setNoteExpressionValue
							(
								kVolumeMod, 
								vol
							);
						*/
						break;
					}
					//------------------------------
					case Steinberg::Vst::kPanTypeID:{ break; }
					//------------------------------
					case Steinberg::Vst::kTuningTypeID:{ break; }
					//------------------------------
					case Steinberg::Vst::kVibratoTypeID:{ break; }
					//------------------------------
					case Steinberg::Vst::kExpressionTypeID:{ break; }
					//------------------------------
					case Steinberg::Vst::kBrightnessTypeID:{ break; }
					//------------------------------
					case Steinberg::Vst::kTextTypeID:{ break; }
					//------------------------------
					case Steinberg::Vst::kPhonemeTypeID:{ break; }
					//------------------------------
					default:
					{
						m_voiceBase->setNoteExpressionValue
						(
							index,
							value
						);
						break;
					}
				}
			}
			//
			//-----------------------------------------------------------------------------
			void noteOn 
			(
				Steinberg::int32 pitch, 
				Steinberg::Vst::ParamValue velocity, 
				float tuning, 
				Steinberg::int32 sampleOffset, 
				Steinberg::int32 nId
			) 
				SMTG_OVERRIDE
			{
				currentVolume = 0;
				//this->values[kVolumeMod] = 0;
				//levelFromVel = 1.f + this->globalParameters->velToLevel * (velocity - 1.);

				//currentSinusVolume = this->values[kSinusVolume] = this->globalParameters->sinusVolume;

				/*
				Steinberg::Vst::VoiceBase
				<
					kNumParameters,
					SamplePrecision, 
					2, 
					GlobalParameterStorage
				>::noteOn
				(
					_pitch, 
					velocity, 
					tuning, 
					sampleOffset, 
					nId
				);
				*/
				m_voiceBase->noteOn
				(
					pitch,
					velocity,
					tuning,
					sampleOffset,
					nId
				);
				this->noteOnSampleOffset++;
			}
			//
			//-----------------------------------------------------------------------------
			void noteOff 
			(
				Steinberg::Vst::ParamValue velocity, 
				Steinberg::int32 sampleOffset
			) 
				SMTG_OVERRIDE
			{
				/*
				Steinberg::Vst::VoiceBase
				<
					kNumParameters, 
					SamplePrecision, 
					Carlsound::Hampshire::kNumChannels,
					GlobalParameterStorage
				>::noteOff
				*/
				m_voiceBase->noteOff
				(
					velocity, 
					sampleOffset
				);
				this->noteOffSampleOffset++;

				/*
				Steinberg::Vst::ParamValue timeFactor = ::pow
				(
					100., 
				    this->values[kReleaseTimeMod]
				);
				*/

				//noteOffVolumeRamp = 1.0 / (timeFactor * this->sampleRate * ((this->globalParameters->releaseTime * MAX_RELEASE_TIME_SEC) + 0.005));
				if (currentVolume)
					noteOffVolumeRamp *= currentVolume;
			}
			//
			//-----------------------------------------------------------------------------
			bool process
			(
				SamplePrecision* outputBuffers[2],
				Steinberg::int32 numSamples
			)
			{
				for (Steinberg::int32 i = 0; i < numSamples; i++)
				{
					this->noteOnSampleOffset--;
					this->noteOffSampleOffset--;
					//
					if (this->noteOnSampleOffset <= 0)
					{
						// we are in Release
						if (this->noteOffSampleOffset == 0)
						{
							//volumeRamp = 0;
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
						//SamplePrecision osc = (SamplePrecision)sin(n * triangleFreq + trianglePhase);
						//sample += (SamplePrecision)(sin(n * sinusFreq + sinusPhase) * currentSinusVolume);

						n++;

						// store in output
						//outputBuffers[0][i] += (SamplePrecision)(sample * currentPanningLeft * currentVolume);
						//outputBuffers[1][i] += (SamplePrecision)(sample * currentPanningRight * currentVolume);

						// ramp parameters
						//currentVolume += volumeRamp;
					}
				}
				return true;
			}
			//
			//-----------------------------------------------------------------------------
			void reset 
			() 
				SMTG_OVERRIDE
			{
				n = 0;
				//sinusPhase = trianglePhase = 0.;
				//currentSinusF = currentTriangleF = -1.;
				//this->values[kVolumeMod] = 0.;

				noteOffVolumeRamp = 0.005;

				/*
				Steinberg::Vst::VoiceBase
				<
					kNumParameters, 
					SamplePrecision, 
					Carlsound::Hampshire::kNumChannels,
					GlobalParameterStorage
				>::reset();
				*/
				m_voiceBase->reset();
			}
			//
			//-----------------------------------------------------------------------------
			//
		protected:
			//-----------------------------------------------------------------------------
			Steinberg::uint32 n;
			//
			Steinberg::Vst::ParamValue currentVolume;
			//
			Steinberg::Vst::ParamValue levelFromVel;
			Steinberg::Vst::ParamValue noteOffVolumeRamp;
			//
			std::shared_ptr<maxiOsc> m_oscillator;
			std::shared_ptr<maxiSettings> m_oscillatorSettings;
			//
			std::shared_ptr
				<
				Carlsound::Vst::VoiceBase
				<
				kNumParameters,
				SamplePrecision,
				Carlsound::Hampshire::kNumChannels,
				GlobalParameterStorage
				>
				> m_voiceBase;
		};
	} // Hampshire
} // Carlsound
