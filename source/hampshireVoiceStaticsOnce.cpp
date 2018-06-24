#include "../include/hampshireVoiceStaticsOnce.h"

namespace Carlsound
{
	namespace Hampshire
	{
		//float freqTab[kNumFrequencies];
		//
		//Steinberg::Vst::LogScale<Steinberg::Vst::ParamValue> freqLogScale(0., 1., 80., 18000., 0.5, 1800.);
		//
		//const double kNormTuningOneOctave = 12.0 / 240.0;	// full in VST 3 is +- 10 octaves
		//const double kNormTuningOneTune = 1.0 / 240.0;
		//
		VoiceStaticsOnce::VoiceStaticsOnce()
		{
			// make frequency (Hz) table
			double k = 1.059463094359;	// 12th root of 2
			double a = 6.875;	// a
			a *= k;	// b
			a *= k;	// bb
			a *= k;	// c, frequency of midi note 0
			for (Steinberg::int32 i = 0; i < kNumFrequencies; i++)	// 128 midi notes
			{
				freqTab[i] = (float)a;
				a *= k;
			}
		}
	} // namespace Hampshire
} // namespace Carlsound
