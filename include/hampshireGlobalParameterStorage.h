#pragma once
//
#include "../include/hampshireIDs.h"
#include "../include/hampshireVoiceStaticsOnce.h"
//
#include "pluginterfaces/base/ftypes.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/vsttypes.h"
#include "base/source/fstreamer.h"
#include "public.sdk/source/vst/vsteditcontroller.h"
//
namespace Carlsound
{
	namespace Hampshire
	{
		#define MAX_VOICES				64
		#define MAX_RELEASE_TIME_SEC	5.0
		#define NUM_FILTER_TYPE			3
		#define NUM_TUNING_RANGE		2 

		//-----------------------------------------------------------------------------
		struct GlobalParameterStorage
		{
			Steinberg::Vst::ParamValue paramValueQtyHarmonics;	// [0, +1]
			//
			Steinberg::tresult setState
			(
				Steinberg::IBStream* stream
			);
			//
			Steinberg::tresult getState
			(
				Steinberg::IBStream* stream
			);
		};
	} // namespace Hampshire
} // namespace Carlsound