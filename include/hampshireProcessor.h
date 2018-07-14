//------------------------------------------------------------------------
#pragma once
//
#define _USE_MATH_DEFINES 
#include <cmath>
//
#include "hampshireIDs.h"
#include "hampshireGlobalParameterStorage.h"
#include "hampshireVoice.h"
#include "hampshireVoiceStaticsOnce.h"
#include "maximilian.h"
#include "voiceProcessor.h"
//
#include <algorithm>
//
#include <base/source/fstreamer.h>
//
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>
#include <pluginterfaces/base/smartpointer.h>
#include <pluginterfaces/base/ustring.h>
#include <pluginterfaces/vst/vsttypes.h>
//
//#include <public.sdk/samples/vst/common/voiceprocessor.h>
//
#include "public.sdk/source/vst/vstaudioeffect.h"
#include <public.sdk/source/vst/vstaudioprocessoralgo.h>
#include <public.sdk/source/vst/vstparameters.h>
//
#include <pluginterfaces/base/smartpointer.h>
#include <pluginterfaces/base/ustring.h>
//
namespace Carlsound
{
	namespace Hampshire
	{
		
		//-----------------------------------------------------------------------------
		class Processor : public Steinberg::Vst::AudioEffect
		{
			public:
			//-----------------------------------------------------------------------------
			Processor ();
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API initialize
			(
				FUnknown* context
			) 
				SMTG_OVERRIDE;
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API setBusArrangements
			(
				Steinberg::Vst::SpeakerArrangement* inputs,
				Steinberg::int32 numIns,
			   	Steinberg::Vst::SpeakerArrangement* outputs,
				Steinberg::int32 numOuts
			) 
				SMTG_OVERRIDE;
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API setState
			(
				Steinberg::IBStream* state
			) 
				SMTG_OVERRIDE;
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API getState
			(
				Steinberg::IBStream* state
			) 
				SMTG_OVERRIDE;
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API canProcessSampleSize
			(
				Steinberg::int32 symbolicSampleSize
			) 
				SMTG_OVERRIDE;
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API setActive 
			(
				Steinberg::TBool state
			) 
				SMTG_OVERRIDE;
			//--------------------------------------------
			/*
			template<class SamplePrecision>
			void processTemplate
			(
				SamplePrecision samplePrecision
			);
			*/
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API process
			(
				Steinberg::Vst::ProcessData& data
			) 
				SMTG_OVERRIDE;
			//--------------------------------------------
			static FUnknown* createInstance 
			(
				void*
			)
			{
				return (Steinberg::Vst::IAudioProcessor*)new Processor ();
			}
			//
		protected:
			//-----------------------------------------------------------------------------
			float m_sampleRate;
			std::shared_ptr<GlobalParameterStorage> m_globalParameterStorage;
			std::shared_ptr<Carlsound::Vst::VoiceProcessor> m_voiceProcessor;
			//
			template
				<
				class Precision,
				class VoiceClass,
				Steinberg::int32 numChannels,
				Steinberg::int32 maxVoices,
				class GlobalParameterStorage
				>
				static std::shared_ptr
				<
					Carlsound::Vst::VoiceProcessorImplementation
					<
						Precision,
						VoiceClass,
						kNumChannels,
						MAX_VOICES,
						GlobalParameterStorage
					>
				> m_voiceProcessorImplementation;
			//
			template
				<
				class SamplePrecision
				>
				static std::shared_ptr
				<
				Carlsound::Hampshire::Voice
				<
				SamplePrecision
				>
				> m_voice;
		};
		
		//------------------------------------------------------------------------
	} // namespace Hampshire
} // namespace Carlsound
