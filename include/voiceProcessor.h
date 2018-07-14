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

namespace Carlsound {
namespace Vst {

//-----------------------------------------------------------------------------
class VoiceProcessor
{
public:
	VoiceProcessor () : activeVoices (0), mClearOutputNeeded (true) {}
	virtual ~VoiceProcessor () {}

	virtual Steinberg::tresult process (Steinberg::Vst::ProcessData& data) = 0;

	/** Returns the number of active voices. */
	Steinberg::int32 getActiveVoices () const { return activeVoices; }

	void clearOutputNeeded (bool val) { mClearOutputNeeded = val; }
protected:
	Steinberg::int32 activeVoices;
	bool mClearOutputNeeded;
};

//-----------------------------------------------------------------------------

template
<
	class Precision, 
	class VoiceClass, 
	Steinberg::int32 numChannels, 
	Steinberg::int32 maxVoices, 
	class GlobalParameterStorage
>
class VoiceProcessorImplementation : public VoiceProcessor
{
public:
	VoiceProcessorImplementation 
	(
		float sampleRate, 
		GlobalParameterStorage* globalParameters = 0
	);
	~VoiceProcessorImplementation ();

	Steinberg::tresult process (Steinberg::Vst::ProcessData& data);

protected:
	VoiceClass* getVoice (Steinberg::int32 noteId);
	VoiceClass* findVoice (Steinberg::int32 noteId);
	VoiceClass voices [maxVoices];
};


//-----------------------------------------------------------------------------
template
<
	class Precision, 
	class VoiceClass, 
	Steinberg::int32 numChannels, 
	Steinberg::int32 maxVoices, 
	class GlobalParameterStorage
>
VoiceProcessorImplementation
<
	Precision, 
	VoiceClass, 
	numChannels, 
	maxVoices, 
	GlobalParameterStorage
>::VoiceProcessorImplementation 
(
	float sampleRate, 
	GlobalParameterStorage* globalParameters
)
{
	for (Steinberg::int32 i = 0; i < maxVoices; i++)
	{
		voices[i].setGlobalParameterStorage (globalParameters);
		voices[i].setSampleRate (sampleRate);
		voices[i].reset ();
	}
}

//-----------------------------------------------------------------------------
template
<
	class Precision, 
	class VoiceClass, 
	Steinberg::int32 numChannels, 
	Steinberg::int32 maxVoices, 
	class GlobalParameterStorage
>
VoiceProcessorImplementation
<
	Precision, 
	VoiceClass, 
	numChannels, 
	maxVoices, 
	GlobalParameterStorage
>::~VoiceProcessorImplementation ()
{
}

//-----------------------------------------------------------------------------
template
<
	class Precision, 
	class VoiceClass, 
	Steinberg::int32 numChannels, 
	Steinberg::int32 maxVoices, 
	class GlobalParameterStorage
>
VoiceClass* VoiceProcessorImplementation
<
	Precision, 
	VoiceClass, 
	numChannels, 
	maxVoices, 
	GlobalParameterStorage
>::getVoice 
(
	Steinberg::int32 noteId
)
{
	VoiceClass* firstFreeVoice = 0;
	if (noteId != -1)
	{
		for (Steinberg::int32 i = 0; i < maxVoices; i++)
		{
			if (voices[i].getNoteId () == noteId)
			{
				return &voices[i];
			}
			else if (firstFreeVoice == 0 && voices[i].getNoteId () == -1)
			{
				firstFreeVoice = &voices[i];
			}
		}
	}
	return firstFreeVoice;
}

//-----------------------------------------------------------------------------
template
<
	class Precision, 
	class VoiceClass, 
	Steinberg::int32 numChannels, 
	Steinberg::int32 maxVoices, 
	class GlobalParameterStorage
>
VoiceClass* VoiceProcessorImplementation
<
	Precision, 
	VoiceClass, 
	numChannels, 
	maxVoices, 
	GlobalParameterStorage
>::findVoice 
(
	Steinberg::int32 noteId
)
{
	if (noteId != -1)
	{
		for (Steinberg::int32 i = 0; i < maxVoices; i++)
		{
			if (voices[i].getNoteId () == noteId)
			{
				return &voices[i];
			}
		}
	}
	return 0;
}

#if VOICEPROCESSOR_BLOCKSIZE <= 0	// voice processing happens in chunks of the block size
//-----------------------------------------------------------------------------
template
<
	class Precision, 
	class VoiceClass, 
	Steinberg::int32 numChannels, 
	Steinberg::int32 maxVoices, 
	class GlobalParameterStorage
>
Steinberg::tresult VoiceProcessorImplementation
<
	Precision, 
	VoiceClass, 
	numChannels, 
	maxVoices, 
	GlobalParameterStorage
>::process 
(
	Steinberg::Vst::ProcessData& data
)
{
	if (mClearOutputNeeded)
		for (Steinberg::int32 i = 0; i < numChannels; i++)
			memset (data.outputs[0].channelBuffers32[i], 0, data.numSamples * sizeof (Precision));

	Steinberg::Vst::IEventList* inputEvents = data.inputEvents;
	if (inputEvents)
	{
		Steinberg::Vst::Event e;
		Steinberg::int32 numEvents = inputEvents->getEventCount ();
		for (Steinberg::int32 i = 0; i < numEvents; i++)
		{
			if (inputEvents->getEvent (i, e) == Steinberg::kResultTrue)
			{
				switch (e.type)
				{
					//-----------------------
				case Steinberg::Vst::Event::kNoteOnEvent:
					{
						if (e.noteOn.noteId == -1) // for host which don't send unique noteId's
							e.noteOn.noteId = e.noteOn.pitch;
						VoiceClass* voice = getVoice (e.noteOn.noteId);
						if (voice)
						{
							voice->noteOn (e.noteOn.pitch, e.noteOn.velocity, e.noteOn.tuning, e.sampleOffset, e.noteOn.noteId);
							this->activeVoices++;
						}
						break;
					}
					//-----------------------
				case Steinberg::Vst::Event::kNoteOffEvent:
					{
						if (e.noteOff.noteId == -1) // for host which don't send unique noteId's
							e.noteOff.noteId = e.noteOff.pitch;
						VoiceClass* voice = findVoice (e.noteOff.noteId);
						if (voice)
						{
							voice->noteOff (e.noteOff.velocity, e.sampleOffset);
						}
					#if DEBUG_LOG
						else
						{
							FDebugPrint ("Voice for kNoteOffEvent not found : %d\n", e.noteOff.noteId);
						}
					#endif
						break;
					}
					//-----------------------
				case Steinberg::Vst::Event::kNoteExpressionValueEvent:
					{
						VoiceClass* voice = findVoice (e.noteExpressionValue.noteId);
						if (voice)
						{
							voice->setNoteExpressionValue (e.noteExpressionValue.typeId, e.noteExpressionValue.value);
						}
					#if DEBUG_LOG
						else
						{
							FDebugPrint ("Voice for kNoteExpressionValueEvent not found : %d\n", e.noteExpressionValue.noteId);
						}
					#endif
						break;
					}
				}
			}
		}
	}

	for (Steinberg::int32 i = 0; i < maxVoices; i++)
	{
		if (voices[i].getNoteId () != -1)
		{
			if (!voices[i].process ((Precision**)data.outputs[0].channelBuffers32, data.numSamples))
			{
				voices[i].reset ();
				this->activeVoices--;
			}
		}
	}
	return Steinberg::kResultTrue;
}

#else	// voice processing happens in chunks of VOICEPROCESSOR_BLOCKSIZE samples
//-----------------------------------------------------------------------------
template
<
	class Precision, 
	class VoiceClass, 
	Steinberg::int32 numChannels, 
	Steinberg::int32 maxVoices, 
	class GlobalParameterStorage
>
Steinberg::tresult VoiceProcessorImplementation
<
	Precision, 
	VoiceClass, 
	numChannels, 
	maxVoices, 
	GlobalParameterStorage
>::process 
(
	Steinberg::Vst::ProcessData& data
)
{	
	const Steinberg::int32 kBlockSize = VOICEPROCESSOR_BLOCKSIZE;

	Steinberg::int32 numSamples = data.numSamples;
	Steinberg::int32 samplesProcessed = 0;
	Steinberg::int32 i;

	Steinberg::Vst::IEventList* inputEvents = data.inputEvents;
	Steinberg::Vst::Event e = {0};
	Steinberg::Vst::Event* eventPtr = 0;
	Steinberg::int32 eventIndex = 0;
	Steinberg::int32 numEvents = inputEvents ? inputEvents->getEventCount () : 0;
	
	// get the first event
	if (numEvents)
	{
		inputEvents->getEvent (0, e);
		eventPtr = &e;
	}

	// initialize audio output buffers
	Precision* buffers[numChannels];
	for (i = 0; i < numChannels; i++)
	{
		buffers[i] = (Precision*)data.outputs[0].channelBuffers32[i];
		if (mClearOutputNeeded)
			memset (buffers[i], 0, data.numSamples * sizeof (Precision));
	}
	
	while (numSamples > 0)
	{
		Steinberg::int32 samplesToProcess = std::min<Steinberg::int32> (kBlockSize, numSamples);
		while (eventPtr != 0)
		{
			// if the event is not in the current processing block then adapt offset for next block
			if (e.sampleOffset > samplesToProcess)
			{
				e.sampleOffset -= samplesToProcess;
				break;
			}

			switch (e.type)
			{
				//-----------------------
				case Steinberg::Vst::Event::kNoteOnEvent:
				{
					if (e.noteOn.noteId == -1)
						e.noteOn.noteId = e.noteOn.pitch;
					VoiceClass* voice = getVoice (e.noteOn.noteId);
					if (voice)
					{
						voice->noteOn (e.noteOn.pitch, e.noteOn.velocity, e.noteOn.tuning, e.sampleOffset, e.noteOn.noteId);
						this->activeVoices++;
						//data.outputEvents->addEvent (e);
					}
					break;
				}
				//-----------------------
				case Steinberg::Vst::Event::kNoteOffEvent:
				{
					if (e.noteOff.noteId == -1)
						e.noteOff.noteId = e.noteOff.pitch;
					VoiceClass* voice = findVoice (e.noteOff.noteId);
					if (voice)
					{
						voice->noteOff (e.noteOff.velocity, e.sampleOffset);
						//data.outputEvents->addEvent (e);
					}
				#if DEBUG_LOG
					else
					{
						FDebugPrint ("Voice for kNoteOffEvent not found : %d\n", e.noteOff.noteId);
					}
				#endif
					break;
				}
				//-----------------------
				case Steinberg::Vst::Event::kNoteExpressionValueEvent:
				{
					VoiceClass* voice = findVoice (e.noteExpressionValue.noteId);
					if (voice)
					{
						voice->setNoteExpressionValue (e.noteExpressionValue.typeId, e.noteExpressionValue.value);
						//data.outputEvents->addEvent (e);
					}
				#if DEBUG_LOG
					else
					{
						FDebugPrint ("Voice for kNoteExpressionValueEvent not found : %d\n", e.noteExpressionValue.noteId);
					}
				#endif
					break;
				}
			}
			
			// get next event
			eventIndex++;
			if (eventIndex < numEvents)
			{
				if (inputEvents->getEvent (eventIndex, e) == Steinberg::kResultTrue)
				{
					e.sampleOffset -= samplesProcessed;
				}
				else
				{
					eventPtr = 0;
				}
			}
			else
			{
				eventPtr = 0;
			}
		}	// end while (event != 0)

		// now process the block
		for (i = 0; i < maxVoices; i++)
		{
			if (voices[i].getNoteId () != -1)
			{
				if (!voices[i].process (buffers, samplesToProcess))
				{
					voices[i].reset ();
					this->activeVoices--;
				}
			}
		}

		// update the counters
		for (i = 0; i < numChannels; i++)
			buffers[i] += samplesToProcess;
		numSamples -= samplesToProcess;
		samplesProcessed += samplesToProcess;

	} // end while (numSamples > 0)

	return Steinberg::kResultTrue;
}

#endif

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Carlsound
