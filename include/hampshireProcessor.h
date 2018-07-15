//------------------------------------------------------------------------
#pragma once
//
#define _USE_MATH_DEFINES 
#include <algorithm>
#include <cmath>
#include <memory>
//
#include "hampshireGlobalParameterState.h"
#include "hampshireIDs.h"
#include "hampshireVoice.h"
#include "hampshireVoiceStaticsOnce.h"
#include "maximilian.h"
#include "hampshireVoiceProcessorImplementation32.h"
#include "hampshireVoiceProcessorImplementation64.h"
//
#include <base/source/fstreamer.h>
//
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/base/smartpointer.h>
#include <pluginterfaces/base/ustring.h>
//
#include <pluginterfaces/vst/ivstparameterchanges.h>
#include <pluginterfaces/vst/vsttypes.h>
//
//#include <public.sdk/samples/vst/common/voiceprocessor.h>
//
#include "public.sdk/source/vst/vstaudioeffect.h"
#include <public.sdk/source/vst/vstaudioprocessoralgo.h>
#include <public.sdk/source/vst/vstparameters.h>
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
			Processor()
			{
				// register its editor class
				setControllerClass
				(
					MyControllerUID
				);
				//m_voiceProcessorImplementation = NULL;
			}
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API initialize
			(
				FUnknown* context
			)
				SMTG_OVERRIDE
			{
				//---always initialize the parent-------
				Steinberg::tresult result = AudioEffect::initialize
				(
					context
				);
				if (result != Steinberg::kResultTrue)
					return Steinberg::kResultFalse;

				//---create Audio Out bus------
				addAudioOutput
				(
					STR16("AudioOutput"),
					Steinberg::Vst::SpeakerArr::kStereo
				);
				//
				addEventInput
				(
					STR16("Event Input"),
					1
				);
				//
				/*
				m_oscillator[0] = std::make_shared<maxiOsc>();
				m_oscillator[1] = std::make_shared<maxiOsc>();
				//
				m_oscillatorSettings = std::make_shared<maxiSettings>();
				//
				m_speedRangeParameter = std::make_shared
				<
				Steinberg::Vst::RangeParameter
				>
				(
				STR16("Speed"), // title
				HuntleyParams::kParamSpeedId, // ParamID
				STR16("sec"), // units
				0.1, // minPlain
				10.0, // maxPlain
				1.0, // defaultValuePlain
				99, // stepCount
				Steinberg::Vst::ParameterInfo::kCanAutomate, // flags
				0, // unitID
				STR16("Speed") // shortTitle
				);
				*/
				//
				m_globalParameterState = std::make_shared
					<
					GlobalParameterState
					>
					();
				//
				return Steinberg::kResultTrue;
			}
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API setBusArrangements
			(
				Steinberg::Vst::SpeakerArrangement* inputs,
				Steinberg::int32 numIns,
				Steinberg::Vst::SpeakerArrangement* outputs,
				Steinberg::int32 numOuts
			)
				SMTG_OVERRIDE
			{
				// we only support one stereo output bus
				if (numIns == 0
					&&
					numOuts == 1
					&&
					outputs[0] == Steinberg::Vst::SpeakerArr::kStereo)
				{
					return AudioEffect::setBusArrangements
					(
						inputs,
						numIns,
						outputs,
						numOuts
					);
				}
				return Steinberg::kResultFalse;
			}
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API setState
			(
				Steinberg::IBStream* state
			)
				SMTG_OVERRIDE
			{
				return m_globalParameterState->setState
				(
					state
				);
			}
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API getState
			(
				Steinberg::IBStream* state
			)
				SMTG_OVERRIDE
			{
				return m_globalParameterState->getState
				(
					state
				);
			}
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API canProcessSampleSize
			(
				Steinberg::int32 symbolicSampleSize
			)
				SMTG_OVERRIDE
			{
				if (symbolicSampleSize == Steinberg::Vst::kSample32 
					|| 
					symbolicSampleSize == Steinberg::Vst::kSample64)
				{
					return Steinberg::kResultTrue;
				}
				return Steinberg::kResultFalse;
			}
			//--------------------------------------------
			Steinberg::tresult PLUGIN_API setActive
			(
				Steinberg::TBool state
			)
				SMTG_OVERRIDE
			{
				{
					if (state) // Initialize
					{
						// Allocate Memory Here
						// Ex: algo.create ();
						if (
								(m_voiceProcessorImplementation32 == 0)
								&&
								(m_voiceProcessorImplementation32 == 0)
							)
						{
							if (processSetup.symbolicSampleSize == Steinberg::Vst::kSample32)
							{
								m_samplePrecisionType = Steinberg::Vst::kSample32;
								m_voiceProcessorImplementation32 = std::make_shared
								<
									VoiceProcessorImplementation32
								>
								(
									(float)processSetup.sampleRate, // sample rate
									&*m_globalParameterState       // GlobalParameterStorage
								);
								/*
								m_voiceProcessorImplementation = std::make_shared
								<
									Carlsound::Vst::VoiceProcessorImplementation
									<
										Voice<float>
									>
								>
								(
									(float) processSetup.sampleRate, // sample rate
									&*m_globalParameterState       // GlobalParameterStorage
								);
								*/
								/*
								m_voiceProcessorImplementation
								(
									new Carlsound::Vst::VoiceProcessorImplementation
									<
										Voice<float>
									>
									(
										(float)processSetup.sampleRate, // sample rate
										&*m_globalParameterState       // GlobalParameterStorage
									)
								);
								*/

							}
							else if (processSetup.symbolicSampleSize == Steinberg::Vst::kSample64)
							{
								m_samplePrecisionType = Steinberg::Vst::kSample64;
								m_voiceProcessorImplementation64 = std::make_shared
								<
									VoiceProcessorImplementation64
								>
								(
									(double)processSetup.sampleRate, // sample rate
									&*m_globalParameterState       // GlobalParameterStorage
								);
								/*
								m_voiceProcessorImplementation = std::make_shared
								<
									Carlsound::Vst::VoiceProcessorImplementation
									<
										Voice<double>
									>
								>
								(
									(double) processSetup.sampleRate, // sample rate
									&*m_globalParameterState        // GlobalParameterStorage
								);
								*/

							}
							else
							{
							return Steinberg::kInvalidArgument;
							}
						}
					}
					else // Release
					{
						// Free Memory if still allocated
						// Ex: if(algo.isCreated ()) { algo.destroy (); }
					}
					return AudioEffect::setActive(state);
				}
			}
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
				SMTG_OVERRIDE
			{
				//--- Read inputs parameter changes-----------
				if (data.inputParameterChanges)
				{
					Steinberg::int32 count = data.inputParameterChanges->getParameterCount();
					for (Steinberg::int32 i = 0; i < count; i++)
					{
						Steinberg::Vst::IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
						if (queue)
						{
							Steinberg::int32 sampleOffset;
							Steinberg::Vst::ParamValue value;
							Steinberg::Vst::ParamID pid = queue->getParameterId();
							if (queue->getPoint(
								queue->getPointCount() - 1,
								sampleOffset,
								value)
								==
								Steinberg::kResultTrue
								)
							{
								switch (pid)
								{
								case kParamNumHarmonicsId:
								{
									//m_globalParameterStorage->masterVolume = value;
									break;
								}
								}
							}
						}
					}
				}

				Steinberg::tresult result;

				// flush mode
				if (data.numOutputs < 1)
					result = Steinberg::kResultTrue;
				else if (Steinberg::Vst::kSample32 == m_samplePrecisionType)
				{
					result = m_voiceProcessorImplementation32->process(data);
				}
				else if (Steinberg::Vst::kSample64 == m_samplePrecisionType)
				{
					result = m_voiceProcessorImplementation64->process(data);
				}
				////////
				if (result == Steinberg::kResultTrue)
				{
					/*
					if (data.outputParameterChanges)
					{
					Steinberg::int32 index;
					Steinberg::Vst::IParamValueQueue* queue = data.outputParameterChanges->addParameterData
					(
					kParamActiveVoices,
					index
					);
					*/
					/*
					if (queue)
					{
						queue->addPoint
						(
							0,
							(Steinberg::Vst::ParamValue)m_voiceProcessor->getActiveVoices() / (Steinberg::Vst::ParamValue)MAX_VOICES,
							index
						);
					}
					*/
					if (
							(m_voiceProcessorImplementation32->getActiveVoices() == 0)
							&&
							(m_voiceProcessorImplementation64->getActiveVoices() == 0)
							&&
							(data.numOutputs > 0)
						)
					{
						data.outputs[0].silenceFlags = 0x11; // left and right channel are silent
					}
				}
				return result;
			}
			//--------------------------------------------
			static FUnknown* createInstance 
			(
				void*
			)
			{
				return (Steinberg::Vst::IAudioProcessor*)new Processor();
			}
			//
			//-----------------------------------------------------------------------------
		protected:
			//-----------------------------------------------------------------------------
			float m_sampleRate;
			//
			std::shared_ptr<GlobalParameterState> m_globalParameterState;
			//
			Steinberg::uint32 m_samplePrecisionType;
			//
			/*
			template
			<
				class VoiceClass
			>
			std::shared_ptr
			<
				Carlsound::Vst::VoiceProcessorImplementation
				<
					VoiceClass
				> 
			> m_voiceProcessorImplementation;
			*/
			//
			std::shared_ptr<VoiceProcessorImplementation32> m_voiceProcessorImplementation32;
			std::shared_ptr<VoiceProcessorImplementation64> m_voiceProcessorImplementation64;
			/*
			template
			<
				typename SamplePrecision
			> 
			std::shared_ptr
			<
				Carlsound::Hampshire::Voice
				<
					SamplePrecision
				> 
			> m_voice;
			*/
		};
		//------------------------------------------------------------------------
	} // namespace Hampshire
} // namespace Carlsound
