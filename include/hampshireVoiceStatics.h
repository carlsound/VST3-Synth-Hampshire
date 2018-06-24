#pragma once
//
#include "hampshireVoiceStaticsOnce.h"
//
#include "public.sdk/samples/vst/common/voicebase.h"
#include "public.sdk/samples/vst/common/logscale.h"
#include "public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_voice.h"
//
#include <cmath>
#include <algorithm>
//
namespace Carlsound
{
	namespace Hampshire
	{
		//-----------------------------------------------------------------------------
		class VoiceStatics
		{
		public:
			//------------------------------------------------------------------------
			static double normalizedLevel2Gain
			(
				float normalized
			);
		};
	} // namespace Hampshire
} // namespace Carlsound