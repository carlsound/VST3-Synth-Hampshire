#pragma once

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "base/source/fdebug.h"
#include <algorithm>

#ifdef DEBUG_LOG
	#undef DEBUG_LOG
#endif

#define DEBUG_LOG	DEVELOPMENT

#ifndef VOICEPROCESSOR_BLOCKSIZE
	#define VOICEPROCESSOR_BLOCKSIZE	32
#endif

namespace Carlsound 
{
	namespace Vst 
	{
		//-----------------------------------------------------------------------------
		class VoiceProcessor
		{
			public:
			VoiceProcessor() : activeVoices(0), mClearOutputNeeded(true)
			{}
			
			virtual ~VoiceProcessor()
			{}
		
			virtual Steinberg::tresult process (Steinberg::Vst::ProcessData& data) = 0;
		
			/** Returns the number of active voices. */
			Steinberg::int32 getActiveVoices () const 
			{ 
				return activeVoices; 
			}
			
			void clearOutputNeeded (bool val) 
			{ 
				mClearOutputNeeded = val; 
			}
		/////////////////////////////////////////
		protected:
			Steinberg::int32 activeVoices;
			bool mClearOutputNeeded;
		};
	} // namespace Vst
} // namespace Carlsound
