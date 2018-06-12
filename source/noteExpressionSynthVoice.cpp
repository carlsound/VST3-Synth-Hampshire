#include "../include/noteExpressionSynthVoice.h"
#include "base/source/fstreamer.h"

namespace Carlsound
{
	namespace Hampshire
	{

		static Steinberg::uint64 currentParamStateVersion = 3;

//-----------------------------------------------------------------------------
Steinberg::tresult GlobalParameterState::setState (Steinberg::IBStream* stream)
{
	Steinberg::IBStreamer s (stream, kLittleEndian);
	Steinberg::uint64 version = 0;

	// version 0
	if (!s.readInt64u (version)) return Steinberg::kResultFalse;

	if (version >= 1)
	{
	
	}
	if (version >= 2)
	{
		if (!s.readDouble (masterVolume)) return Steinberg::kResultFalse;
	}
	if (version >= 3)
	{
		
	}
	return Steinberg::kResultTrue;
}

//-----------------------------------------------------------------------------
Steinberg::tresult GlobalParameterState::getState (Steinberg::IBStream* stream)
{
	Steinberg::IBStreamer s (stream, kLittleEndian);

	// version 0
	if (!s.writeInt64u (currentParamStateVersion)) return Steinberg::kResultFalse;
	
	// version 1
	
	// version 2
	if (!s.writeDouble (masterVolume)) return Steinberg::kResultFalse;

	// version 3

	return Steinberg::kResultTrue;
}

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
class VoiceStaticsOnce
{
public:
	VoiceStaticsOnce ()
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

static VoiceStaticsOnce gVoiceStaticsOnce;
*/
//-----------------------------------------------------------------------------


	} // namespace Hampshire
} // namespace Carlsound
