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
			double twelfthRootOfTwo = 1.059463094359;	// 12th root of 2
			double noteFrequency = 6.875;	// note A (octave -6)
			noteFrequency *= twelfthRootOfTwo;	// note B (octave -6)
			noteFrequency *= twelfthRootOfTwo;	// note Bb / C# (octave -6)
			noteFrequency *= twelfthRootOfTwo;	// note C (octave -5), frequency of midi note 0
			for (Steinberg::int32 i = 0; i < kNumFrequencies; i++)	// 128 midi notes
			{
				midiNotesFrequencyLookupTable[i] = (float) noteFrequency;
				noteFrequency *= twelfthRootOfTwo;
			}
		}
	} // namespace Hampshire
} // namespace Carlsound
