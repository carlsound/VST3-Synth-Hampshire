//------------------------------------------------------------------------
#pragma once
//
#define _USE_MATH_DEFINES 
#include <cmath>
//
#include "public.sdk/source/vst/vstaudioeffect.h"
//
#include "hampshireGlobalParameterStorage.h"
#include "hampshireVoice.h"
#include "hampshireIDs.h"
//
#include "maximilian.h"
#include <public.sdk/source/vst/vstparameters.h>
#include <public.sdk/samples/vst/common/voiceprocessor.h>
#include <pluginterfaces/base/smartpointer.h>
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
			
			Steinberg::tresult PLUGIN_API initialize
			(
				FUnknown* context
			) 
				SMTG_OVERRIDE;
			//
			Steinberg::tresult PLUGIN_API setBusArrangements
			(
				Steinberg::Vst::SpeakerArrangement* inputs,
				Steinberg::int32 numIns,
			   	Steinberg::Vst::SpeakerArrangement* outputs,
				Steinberg::int32 numOuts
			) 
				SMTG_OVERRIDE;
			//
			Steinberg::tresult PLUGIN_API setState
			(
				Steinberg::IBStream* state
			) 
				SMTG_OVERRIDE;
			//
			Steinberg::tresult PLUGIN_API getState
			(
				Steinberg::IBStream* state
			) 
				SMTG_OVERRIDE;
			//
			Steinberg::tresult PLUGIN_API canProcessSampleSize
			(
				Steinberg::int32 symbolicSampleSize
			) 
				SMTG_OVERRIDE;
			//
			Steinberg::tresult PLUGIN_API setActive 
			(
				Steinberg::TBool state
			) 
				SMTG_OVERRIDE;
			//
			Steinberg::tresult PLUGIN_API process
			(
				Steinberg::Vst::ProcessData& data
			) 
				SMTG_OVERRIDE;
			//
			//------------------------------------------------------------------------
			
			static FUnknown* createInstance (void*)
			{
				return (Steinberg::Vst::IAudioProcessor*)new Processor ();
			}
			
			/*
			template <class T> void bufferSampleGain
			(
				T inBuffer, 
				T outBuffer, 
				const int sampleLocation, 
				const double gainValue
			);
			*/
			
			protected:
			//-----------------------------------------------------------------------------
		    //Steinberg::IPtr<Steinberg::Vst::VoiceProcessor> m_voiceProcessor;
		    std::shared_ptr<Steinberg::Vst::VoiceProcessor> m_voiceProcessor;;
			//Steinberg::IPtr<GlobalParameterStorage> m_globalParameterStorage;
			std::shared_ptr<GlobalParameterStorage> m_globalParameterStorage;
		};
		
		//------------------------------------------------------------------------
	} // namespace Hampshire
} // namespace Carlsound
