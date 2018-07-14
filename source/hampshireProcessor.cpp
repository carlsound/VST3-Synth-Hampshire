#include "../include/hampshireProcessor.h"
//
namespace Carlsound
{
	namespace Hampshire
	{
		//-----------------------------------------------------------------------------
		Processor::Processor ()
		{
			// register its editor class
			setControllerClass
			(
				MyControllerUID
			);
			m_voiceProcessor = NULL;
		}
		//-----------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Processor::initialize 
		(
			FUnknown* context
		)
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
				STR16 ("AudioOutput"), 
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
			m_voiceProcessor = std::make_shared
			<
				Steinberg::Vst::VoiceProcessor
			>
			(

			);
			*/
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
			m_globalParameterStorage = std::make_shared
			<
				GlobalParameterStorage
			>
			();
			//
			return Steinberg::kResultTrue;
		}
		//-----------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Processor::setBusArrangements
		(
			Steinberg::Vst::SpeakerArrangement* inputs,
            Steinberg::int32 numIns,
            Steinberg::Vst::SpeakerArrangement* outputs,
            Steinberg::int32 numOuts
		)
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
		//-----------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Processor::setState
		(
			Steinberg::IBStream* state
		)
		{
			return m_globalParameterStorage->setState
			(
				state
			);
		}
		//------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Processor::getState
		(
			Steinberg::IBStream* state
		)
		{
			return m_globalParameterStorage->getState
			(
				state
			);
		}

		//-----------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Processor::canProcessSampleSize
		(
			Steinberg::int32 symbolicSampleSize
		)
		{
			if (symbolicSampleSize == Steinberg::Vst::kSample32 
				|| 
				symbolicSampleSize == Steinberg::Vst::kSample64)
			{
				return Steinberg::kResultTrue;
			}
			return Steinberg::kResultFalse;
		}
		//-----------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Processor::setActive
		(
			Steinberg::TBool state
		)
		{
			if (state) // Initialize
			{
				// Allocate Memory Here
				// Ex: algo.create ();
				if (m_voiceProcessor == 0)
				{
					if (processSetup.symbolicSampleSize == Steinberg::Vst::kSample32)
					{
						
						m_voiceProcessorImplementation
						<
							float,
							Voice<float>,
							kNumChannels,
							MAX_VOICES,
							GlobalParameterStorage
						>
							= std::make_shared
						<
							Carlsound::Vst::VoiceProcessorImplementation
							<
								float, // precision
						        Voice<float>, // voice class
						        kNumChannels, // numChannels
						        MAX_VOICES, // maxVoices
						        GlobalParameterStorage
							>
						>
						(
							(float)processSetup.sampleRate, // sample rate
							&*m_globalParameterStorage       // GlobalParameterStorage
						);
						
					}
					else if (processSetup.symbolicSampleSize == Steinberg::Vst::kSample64)
					{
						
						m_voiceProcessorImplementation
						<
							double,
							Voice<double>,
							kNumChannels,
							MAX_VOICES,
							GlobalParameterStorage
						>
							= std::make_shared
						<
							Carlsound::Vst::VoiceProcessorImplementation
							<
								double, // precision
								Voice<double>, // voice class
								kNumChannels, // numChannels
								MAX_VOICES, // maxVoices
								GlobalParameterStorage
							>
						>
						(
							(double)processSetup.sampleRate, // sample rate
							&*m_globalParameterStorage        // GlobalParameterStorage
						);
						
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
			return AudioEffect::setActive (state);
		}
		//-----------------------------------------------------------------------------
		/*
		template<class T>
		inline void Carlsound::Huntley::Processor::bufferSampleGain
		(
			T inBuffer, 
		    T outBuffer, 
			const int sampleLocation, 
			const double gainValue
		)
		{
			inBuffer = inBuffer + sampleLocation;  // pointer arithmetic
			outBuffer = outBuffer + sampleLocation; // pointer arithmetic
			//
			*outBuffer = *inBuffer * gainValue;
		}
		*/
		//-----------------------------------------------------------------------------
		/*
		template<class SamplePrecision>
		inline void processTemplate
		(
			SamplePrecision samplePrecision
		)
		{

		}
		*/
		//-----------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Processor::process
		(
			Steinberg::Vst::ProcessData& data
		)
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
			else
				result = m_voiceProcessor->process(data);
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
					if (queue)
					{
						queue->addPoint(
							0, 
							(Steinberg::Vst::ParamValue)m_voiceProcessor->getActiveVoices() / (Steinberg::Vst::ParamValue)MAX_VOICES, 
							index);
					}
				}
				*/
				if (m_voiceProcessor->getActiveVoices() == 0 
					&& 
					data.numOutputs > 0)
				{
					data.outputs[0].silenceFlags = 0x11; // left and right channel are silent
				}
			}
			return result;
		}
		//------------------------------------------------------------------------
	} // namespace Hampshire
} // namespace Carlsound