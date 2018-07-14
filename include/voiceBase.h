#pragma once

#include "pluginterfaces/vst/vsttypes.h"
#include "base/source/fdebug.h"

#ifdef DEBUG_LOG
	#undef DEBUG_LOG
#endif

#define DEBUG_LOG	DEVELOPMENT

namespace Carlsound {
namespace Vst {

//-----------------------------------------------------------------------------
template
<
	Steinberg::uint32 numValues, 
	class SamplePrecision, 
	Steinberg::uint32 numChannels, 
	class GlobalParameterStorage
> 
class VoiceBase
{
public:
	VoiceBase();
	VoiceBase(const VoiceBase& vb);
	~VoiceBase();

	/** Returns the current note id of this voice. */
	Steinberg::int32 getNoteId () const { return noteId; }

	/** Sets a new GlobalParameterStorage. */
	virtual void setGlobalParameterStorage (GlobalParameterStorage* globalParameters) 
	{ 
		this->globalParameters = globalParameters; 
	}
	/** Sets the sampleRate. */
	virtual void setSampleRate (Steinberg::Vst::ParamValue sampleRate) 
	{ 
		this->sampleRate = sampleRate; 
	}
	/** Returns the sampleRate. */
	float getSampleRate () const 
	{ 
		return (float)sampleRate; 
	}

	virtual void setNoteExpressionValue (Steinberg::int32 index, Steinberg::Vst::ParamValue value) 
	{ 
		if (index < numValues)
		{
			values[index] = value;
		}
	}

	virtual void noteOn 
	(
		Steinberg::int32 pitch, 
		Steinberg::Vst::ParamValue velocity, 
		float tuning, 
		Steinberg::int32 sampleOffset, 
		Steinberg::int32 noteId
	);
	
	virtual void noteOff 
	(
		Steinberg::Vst::ParamValue velocity, 
		Steinberg::int32 sampleOffset
	);

	virtual void reset () 
	{ 
		noteOnSampleOffset = -1;
		noteOffSampleOffset = -1;
		noteId = -1;
		tuning = 0;
	}

	GlobalParameterStorage* globalParameters;

	Steinberg::int32 noteId;
	Steinberg::int32 pitch;
	Steinberg::int32 noteOnSampleOffset;
	Steinberg::int32 noteOffSampleOffset;
	float tuning;

	Steinberg::Vst::ParamValue sampleRate;
	Steinberg::Vst::ParamValue noteOnVelocity;
	Steinberg::Vst::ParamValue noteOffVelocity;
	Steinberg::Vst::ParamValue values[numValues];

//-----------------------------------------------------------------------------
protected:
	
};

//-----------------------------------------------------------------------------
template
<
	Steinberg::uint32 numValues, 
	class SamplePrecision, 
	Steinberg::uint32 numChannels, 
	class GlobalParameterStorage
>
VoiceBase
<
	numValues, 
	SamplePrecision, 
	numChannels, 
	GlobalParameterStorage
>::VoiceBase ()
: globalParameters (0)
, noteId (-1)
, pitch (-1)
, noteOnSampleOffset (0)
, noteOffSampleOffset (0)
, sampleRate (44100.)
, noteOnVelocity (0.)
, noteOffVelocity (0.)
{
}

//-----------------------------------------------------------------------------
template
<
	Steinberg::uint32 numValues, 
	class SamplePrecision, 
	Steinberg::uint32 numChannels, 
	class GlobalParameterStorage
>
VoiceBase
<
	numValues, 
	SamplePrecision,
	numChannels, 
	GlobalParameterStorage
>::VoiceBase 
(
	const VoiceBase<numValues, 
	SamplePrecision, 
	numChannels, 
	GlobalParameterStorage>& vb
)
: globalParameters (vb.globalParameters)
, noteId (vb.noteId)
, pitch (vb.pitch)
, noteOnSampleOffset (vb.noteOnSampleOffset)
, noteOffSampleOffset (vb.noteOffSampleOffset)
, noteOnVelocity (vb.noteOnVelocity)
, noteOffVelocity (vb.noteOffVelocity)
, sampleRate (vb.sampleRate)
, values (0)
{
	for (Steinberg::uint32 i = 0; i < numValues; i++)
	{
		values[i] = vb.values[i];
	}
}

//-----------------------------------------------------------------------------
template
<
	Steinberg::uint32 numValues, 
	class SamplePrecision, 
	Steinberg::uint32 numChannels, 
	class GlobalParameterStorage
>
VoiceBase
<
	numValues, 
	SamplePrecision, 
	numChannels, 
	GlobalParameterStorage
>::~VoiceBase ()
{
}

//-----------------------------------------------------------------------------
template
<
	Steinberg::uint32 numValues, 
	class SamplePrecision, 
	Steinberg::uint32 numChannels, 
	class GlobalParameterStorage
>
void VoiceBase
<
	numValues, 
	SamplePrecision, 
	numChannels, 
	GlobalParameterStorage
>::noteOn 
(Steinberg::int32 pitch, 
	Steinberg::Vst::ParamValue velocity,  
	float tuning, 
	Steinberg::int32 sampleOffset, 
	Steinberg::int32 nId
)
{
	this->pitch = pitch;
	noteOnVelocity = velocity;
	noteOnSampleOffset = sampleOffset;
	noteId = nId;
	this->tuning = tuning;

#if DEBUG_LOG
	FDebugPrint ("NoteOn :%d\n", nId);
#endif
}

//-----------------------------------------------------------------------------
template
<
	Steinberg::uint32 numValues, 
	class SamplePrecision, 
	Steinberg::uint32 numChannels, 
	class GlobalParameterStorage
>
void VoiceBase
<
	numValues, 
	SamplePrecision, 
	numChannels, 
	GlobalParameterStorage
>::noteOff 
(
	Steinberg::Vst::ParamValue velocity, 
	Steinberg::int32 sampleOffset
)
{
	noteOffVelocity = velocity;
	noteOffSampleOffset = sampleOffset;

#if DEBUG_LOG
	FDebugPrint ("NoteOff:%d\n", this->noteId);
#endif
}

//------------------------------------------------------------------------
} // namespace Vst
} // namespace Carlsound
