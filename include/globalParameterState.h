#pragma once

namespace Carlsound
{
	namespace Hampshire
	{
		//-----------------------------------------------------------------------------
		struct GlobalParameterState
		{
			Steinberg::Vst::ParamValue masterVolume;	// [0, +1]
														//
			Steinberg::tresult setState(Steinberg::IBStream* stream);
			Steinberg::tresult getState(Steinberg::IBStream* stream);
		};
	} // namespace Hampshire
} // namespace Carlsound