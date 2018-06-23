#include "../include/hampshireGlobalParameterStorage.h"

namespace Carlsound
{
	namespace Hampshire
	{
		//-----------------------------------------------------------------------------
		Steinberg::tresult GlobalParameterStorage::setState
		(
			Steinberg::IBStream* state
		)
		{
			Steinberg::IBStreamer streamer
			(
				state, 
				kLittleEndian
			);
			Steinberg::uint64 version = 0;
			double dDoubleParam = 0;
			// version 0
			if (!streamer.readInt64u(version))
			{
				return Steinberg::kResultFalse;
			}
			//
			if (!streamer.readDouble(dDoubleParam))
			{
				return Steinberg::kResultFalse;
			}
			//
			if (version >= 1)
			{

			}
			if (version >= 2)
			{
				//if (!s.readDouble(masterVolume)) return Steinberg::kResultFalse;
			}
			if (version >= 3)
			{

			}
			return Steinberg::kResultTrue;
		}

		//-----------------------------------------------------------------------------
		Steinberg::tresult GlobalParameterStorage::getState
		(
			Steinberg::IBStream* state
		)
		{
			Steinberg::IBStreamer streamer
			(
				state, 
				kLittleEndian
			);

			// version 0
			//if (!streamer.writeInt64u(currentParamStateVersion)) return Steinberg::kResultFalse;

			// version 1

			// version 2
			//if (!streamer.writeDouble(masterVolume)) return Steinberg::kResultFalse;

			// version 3

			return Steinberg::kResultTrue;
		}
	} // namespace Hampshire
} // namespace Carlsound