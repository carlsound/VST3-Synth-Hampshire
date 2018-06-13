#pragma once

#include "public.sdk/samples/vst/common/voicebase.h"
#include "public.sdk/samples/vst/common/logscale.h"

#include <cmath>
#include <algorithm>

namespace Carlsound
{
	namespace Hampshire
	{
		//-----------------------------------------------------------------------------
		class VoiceStatics
		{
		public:
			//------------------------------------------------------------------------
			static double normalizedLevel2Gain(float normalized);

			//---member variables----
			enum {
				kNumFrequencies = 128
			};
		};
	} // namespace Hampshire
} // namespace Carlsound