#include "../include/globalParameterState.h"

namespace Carlsound
{
	namespace Hampshire
	{
		//-----------------------------------------------------------------------------
		Steinberg::tresult GlobalParameterState::setState(Steinberg::IBStream* stream)
		{
			Steinberg::IBStreamer s(stream, kLittleEndian);
			Steinberg::uint64 version = 0;

			// version 0
			if (!s.readInt64u(version)) return Steinberg::kResultFalse;

			if (version >= 1)
			{

			}
			if (version >= 2)
			{
				if (!s.readDouble(masterVolume)) return Steinberg::kResultFalse;
			}
			if (version >= 3)
			{

			}
			return Steinberg::kResultTrue;
		}

		//-----------------------------------------------------------------------------
		Steinberg::tresult GlobalParameterState::getState(Steinberg::IBStream* stream)
		{
			Steinberg::IBStreamer s(stream, kLittleEndian);

			// version 0
			if (!s.writeInt64u(currentParamStateVersion)) return Steinberg::kResultFalse;

			// version 1

			// version 2
			if (!s.writeDouble(masterVolume)) return Steinberg::kResultFalse;

			// version 3

			return Steinberg::kResultTrue;
		}
	} // namespace Hamshire
} // namespace Carlsound