#pragma once

#include "noteExpressionSynthVoiceStatistics.h"

namespace Carlsound
{
	namespace Hampshire
	{
		//-----------------------------------------------------------------------------
		class VoiceStaticsOnce
		{
		public:
			VoiceStaticsOnce()
			{
				// make frequency (Hz) table
				double k = 1.059463094359;	// 12th root of 2
				double a = 6.875;	// a
				a *= k;	// b
				a *= k;	// bb
				a *= k;	// c, frequency of midi note 0
				for (Steinberg::int32 i = 0; i < VoiceStatics::kNumFrequencies; i++)	// 128 midi notes
				{
					VoiceStatics::freqTab[i] = (float)a;
					a *= k;
				}
			}
		};

		//-----------------------------------------------------------------------------
		static VoiceStaticsOnce gVoiceStaticsOnce;
		//
		float VoiceStatics::freqTab[VoiceStatics::kNumFrequencies];
		const float VoiceStatics::scaleHeadRoom = (float)(pow(10.0, -12.0 / 20.0) * 0.70710677); // for 12 dB head room
		const float VoiceStatics::scaleNorm2GainC1 = (float)(VoiceStatics::scaleHeadRoom * pow(10.0, 24.0 / 20.0));
		const float VoiceStatics::scaleNorm2GainC2 = (float)(24.0 / 20.0 / 0.30102999566398119521373889472449); // Mathd::kLog2
		Steinberg::Vst::LogScale<Steinberg::Vst::ParamValue> VoiceStatics::freqLogScale(0., 1., 80., 18000., 0.5, 1800.);
		//
		const double VoiceStatics::kNormTuningOneOctave = 12.0 / 240.0;	// full in VST 3 is +- 10 octaves
		const double VoiceStatics::kNormTuningOneTune = 1.0 / 240.0;
	} // namespace Hampshire
} // namespace Carlsound