//------------------------------------------------------------------------
#pragma once
#include "public.sdk/samples/vst/mda-vst3/source/mdaParameter.h"

namespace Carlsound
{
	namespace Hampshire
	{

		// HERE are defined the parameter Ids which are exported to the host
		enum HampshireParams : Steinberg::Vst::ParamID
		{
			//kNumHarmonicsId = 100
			//kParamSpeedId = 102
		};
		
		
		// HERE you have to define new unique class ids: for processor and for controller
		// you can use GUID creator tools like https://www.guidgenerator.com/

		static const Steinberg::FUID MyProcessorUID (0xAE5603A3, 0x76AE4279, 0xBC7D93BF, 0x91095BE5);
		static const Steinberg::FUID MyControllerUID (0xCFBE85e6, 0x11C64E55, 0x93520809, 0x6ED4B9C8);
		
		//------------------------------------------------------------------------
	} // namespace Hampshire
} // namespace Carlsound
