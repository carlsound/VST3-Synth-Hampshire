#include "../include/hampshireVoice.h"

#include "base/source/fstreamer.h"

namespace Carlsound
{
	namespace Hampshire
	{

		static Steinberg::uint64 currentParamStateVersion = 3;



//-----------------------------------------------------------------------------
/*
float VoiceStatics::freqTab[VoiceStatics::kNumFrequencies];
const float VoiceStatics::scaleHeadRoom = (float)(pow (10.0, -12.0 / 20.0) * 0.70710677); // for 12 dB head room
const float VoiceStatics::scaleNorm2GainC1 = (float)(VoiceStatics::scaleHeadRoom * pow (10.0, 24.0 / 20.0));
const float VoiceStatics::scaleNorm2GainC2 = (float)(24.0 / 20.0 / 0.30102999566398119521373889472449); // Mathd::kLog2
Steinberg::Vst::LogScale<Steinberg::Vst::ParamValue> VoiceStatics::freqLogScale (0., 1., 80., 18000., 0.5, 1800.);

const double VoiceStatics::kNormTuningOneOctave = 12.0 / 240.0;	// full in VST 3 is +- 10 octaves
const double VoiceStatics::kNormTuningOneTune	= 1.0 / 240.0;
 */

//-----------------------------------------------------------------------------
/*

*/
		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		Voice<SamplePrecision>::~Voice()
		{
			//delete filter;
		}

		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		void Voice<SamplePrecision>::setSampleRate(Steinberg::Vst::ParamValue sampleRate)
		{
			//filter->setSampleRate (sampleRate);
			Steinberg::Vst::VoiceBase<kNumParameters, 
	                                  SamplePrecision, 
	                                  2, GlobalParameterState>::setSampleRate(sampleRate);
			//
			m_oscillatorSettings->sampleRate = sampleRate;
		}

		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		void Voice<SamplePrecision>::noteOn(Steinberg::int32 _pitch, Steinberg::Vst::ParamValue velocity, float tuning, Steinberg::int32 sampleOffset, Steinberg::int32 nId)
		{
			currentVolume = 0;
			this->values[kVolumeMod] = 0;
			levelFromVel = 1.f + this->globalParameters->velToLevel * (velocity - 1.);

			//currentSinusVolume = this->values[kSinusVolume] = this->globalParameters->sinusVolume;

			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::noteOn(_pitch, velocity, tuning, sampleOffset, nId);
			this->noteOnSampleOffset++;
		}

		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		void Voice<SamplePrecision>::noteOff(Steinberg::Vst::ParamValue velocity, Steinberg::int32 sampleOffset)
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::noteOff(velocity, sampleOffset);
			this->noteOffSampleOffset++;

			//Steinberg::Vst::ParamValue timeFactor = ::pow (100., this->values[kReleaseTimeMod]);

			//noteOffVolumeRamp = 1.0 / (timeFactor * this->sampleRate * ((this->globalParameters->releaseTime * MAX_RELEASE_TIME_SEC) + 0.005));
			if (currentVolume)
				noteOffVolumeRamp *= currentVolume;
		}

		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		bool Voice<SamplePrecision>::process(SamplePrecision* outputBuffers[2], Steinberg::int32 numSamples)
		{
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
					SamplePrecision osc = (SamplePrecision)sin(n * triangleFreq + trianglePhase);
					sample += (SamplePrecision)(sin(n * sinusFreq + sinusPhase) * currentSinusVolume);

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
		void Voice<SamplePrecision>::reset()
		{
			noiseStep = 1;
			noisePos = 0;
			n = 0;
			sinusPhase = trianglePhase = 0.;
			currentSinusF = currentTriangleF = -1.;
			this->values[kVolumeMod] = 0.;

			noteOffVolumeRamp = 0.005;

			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::reset();
		}

		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		void Voice<SamplePrecision>::setNoteExpressionValue(Steinberg::int32 index, Steinberg::Vst::ParamValue value)
		{
			if (this->globalParameters->bypassSNA)
				return;

			switch (index)
			{
				//------------------------------
				case Steinberg::Vst::kVolumeTypeID:
				{
					//Steinberg::Vst::ParamValue vol = VoiceStatics::normalizedLevel2Gain ((float)value);
					//Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kVolumeMod, vol);
					break;
				}
				//------------------------------
				default:
				{
					Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue(index, value);
					break;
				}
			}
		}

		//-----------------------------------------------------------------------------

	} // namespace Hampshire
} // namespace Carlsound
