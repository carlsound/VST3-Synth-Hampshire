#pragma once
//
#include "pluginterfaces/vst/vsttypes.h"
#include "public.sdk/samples/vst/common/logscale.h"
//
#include <cmath>
#include <algorithm>
//
namespace Carlsound
{
	namespace Hampshire
	{
		//---global variables----
		enum
		{
			kNumFrequencies = 128
		};
		//
		enum VoiceParameters
		{
			kParamNumHarmonicsId,
			//
			kNumParameters
		};
		//
		static const float scaleHeadRoom = (float)(pow(10.0, -12.0 / 20.0) * 0.70710677); // for 12 dB head room
		static const float scaleNorm2GainC1 = (float)(scaleHeadRoom * pow(10.0, 24.0 / 20.0));
        static const float scaleNorm2GainC2 = (float)(24.0 / 20.0 / 0.30102999566398119521373889472449); // Mathd::kLog2
		//
		static float freqTab[kNumFrequencies];
		//
		static const Steinberg::Vst::LogScale<Steinberg::Vst::ParamValue> freqLogScale(0., 1., 80., 18000., 0.5, 1800.);
		//
		static const double kNormTuningOneOctave = 12.0 / 240.0;	// full in VST 3 is +- 10 octaves
		static const double kNormTuningOneTune = 1.0 / 240.0;
		//
		//-----------------------------------------------------------------------------
		class VoiceStaticsOnce
		{
		public:
			VoiceStaticsOnce();
		};
		//
		//-----------------------------------------------------------------------------
		static VoiceStaticsOnce gVoiceStaticsOnce;
	} // namespace Hampshire
} // namespace Carlsound
