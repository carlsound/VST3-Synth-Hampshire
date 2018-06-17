

#include "../include/hampshireController.h"

//#include "../include/panNoteExpressionType.h"
//#include "../include/releaseTimeModNoteExpressionType.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include <string>

#include "public.sdk/source/vst/hosting/stringconvert.h"
#include "public.sdk/samples/vst/mda-vst3/source/mdaMultiBandController.h"

#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/base/futils.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "base/source/fstring.h"
#include <public.sdk/samples/vst/common/logscale.h>


namespace Carlsound
{
	namespace Hampshire
	{
		//-----------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Controller::initialize (FUnknown* context)
		{
			Steinberg::tresult result = Steinberg::Vst::EditController::initialize (context);
			if (result == Steinberg::kResultTrue)
			{
			//---Create Parameters------------
		
			/////////////////////////////////
			Steinberg::Vst::Parameter* param;
			/////////////////////////////////
			param = new Steinberg::Vst::Parameter(USTRING("Harmonics"), // title
				                                  kParamNumHarmonicsId, // Parameter ID tag
				                                  USTRING("#"), // units
				                                  0, // unit ID
				                                  1, // step count
				                                  Steinberg::Vst::ParameterInfo::kCanAutomate, // flags
				                                  0, // default value
				USTRING("Harmonics")); // short title

			//param->setPrecision(1);
			parameters.addParameter(param);
			
			/////////////////////////////////
			/////////////////////////////////
			// Init Note Expression Types
			m_noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::NoteExpressionType(Steinberg::Vst::kVolumeTypeID, 
			                                      Steinberg::String("Volume"), 
			                                      Steinberg::String("Vol"), 
			                                      0, 
			                                      -1, 
			                                      1., 
			                                      0., 
			                                      1., 
			                                      0, 
			                                      0));
			}
			return Steinberg::kResultTrue;
		}
		
		//-----------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Controller::terminate()
		{
			m_noteExpressionTypes.removeAll();
			return EditController::terminate();
		}
		
		//------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Controller::setComponentState (Steinberg::IBStream* state)
		{
			// we receive the current state of the component (processor part)
			// we read our parameters and bypass value...
			if (setState(state) == Steinberg::kResultTrue)
			{
				/*
				setParamNormalized(kParamMasterVolume, 
			                       gps.masterVolume);
				*/
			}
			return setState(state);
		}
		
		//------------------------------------------------------------------------

		Steinberg::tresult PLUGIN_API Controller::setParamNormalizedFromFile(Steinberg::Vst::ParamID tag, 
			                                                                 Steinberg::Vst::ParamValue value)
		{
			// --- get the parameter
			Steinberg::Vst::Parameter* pParam = Steinberg::Vst::EditController::getParameterObject(tag);

			// --- verify pointer
			if (!pParam) return Steinberg::kResultFalse;

			//  --- convert serialized value to normalized (raw)
			return setParamNormalized(tag, pParam->toNormalized(value));
		}

		//------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Controller::setState(Steinberg::IBStream* state)
		{
			Steinberg::IBStreamer streamer(state, kLittleEndian);

			Steinberg::uint64 version = 0;
			double dDoubleParam = 0;
			//
			//
			//
			if (!streamer.readInt64u(version))
			{
				 return Steinberg::kResultFalse;
			}
			//
			if (!streamer.readDouble(dDoubleParam))
			{
				return Steinberg::kResultFalse;
			}
			else
			{
				setParamNormalizedFromFile(kParamNumHarmonicsId, dDoubleParam);
			}
			//
			//
			//
			if (version >= 1)
			{
				//
			}

			return Steinberg::kResultTrue;
		}
		
		//------------------------------------------------------------------------
		
		Steinberg::tresult PLUGIN_API Controller::getState(Steinberg::IBStream* state)
		{
			Steinberg::IBStreamer streamer(state, kLittleEndian);

			return Steinberg::kResultTrue;
		}
				
		//------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Controller::setParamNormalized(Steinberg::Vst::ParamID tag,
																	 Steinberg::Vst::ParamValue value)
		{
			Steinberg::tresult res = Steinberg::Vst::EditController::setParamNormalized(tag, value);
			
			return res;
		}
		
		//------------------------------------------------------------------------
		/*
		Steinberg::Vst::ParamValue PLUGIN_API Controller::normalizedParamToPlain(Steinberg::Vst::ParamID tag,
				                                                                 Steinberg::Vst::ParamValue valueNormalized)
		{
			if(kParamSpeedId == tag)
			{
				return ((valueNormalized * ((10.0 - 0.1) / 1.0)) + 0.10);
			}
			else
			{
				return valueNormalized;
			}
		}
		*/
		
		//------------------------------------------------------------------------
		/*
		Steinberg::Vst::ParamValue PLUGIN_API Controller::plainParamToNormalized(Steinberg::Vst::ParamID tag,
																			     Steinberg::Vst::ParamValue value)
		{
			if (kParamSpeedId == tag)
			{
				return (value / 10.0);
			}
			else
			{
				return value;
			}
		}
		*/
		
		//------------------------------------------------------------------------
		/*
		Steinberg::tresult Controller::getParamStringByValue(Steinberg::Vst::ParamID tag,
															 Steinberg::Vst::ParamValue valueNormalized,
															 Steinberg::Vst::String128 string)
		{
			
			if(HuntleyParams::kBypassId == tag)
			{
				std::string str;
				if(valueNormalized)
				{
					str = "On";
				}
				else
				{
					str = "Off";
				}
				string = new TCHAR[str.size() + 1];
				string[str.size()] = 0;
				std::copy(str.begin(), str.end(), string);
			}
			if (HuntleyParams::kParamSpeedId == tag)
			{
				std::string str;
				str = (std::to_string((valueNormalized * ((10.0 - 0.1) / 1.0)) + 0.10));
				string = new TCHAR[str.size() + 1];
				string[str.size()] = 0;
				std::copy(str.begin(), str.end(), string);
			}
			return Steinberg::kResultOk;
		}
		*/
		
		//------------------------------------------------------------------------
		Steinberg::int32 PLUGIN_API Controller::getNoteExpressionCount(Steinberg::int32 busIndex,
			                                                           Steinberg::int16 channel)
		{
			if (busIndex == 0 && channel == 0)
			{
				return m_noteExpressionTypes.getNoteExpressionCount();
			}
			return 0;
		}
		
		//------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Controller::getNoteExpressionInfo(Steinberg::int32 busIndex,
																		Steinberg::int16 channel,
																		Steinberg::int32 noteExpressionIndex,
																		Steinberg::Vst::NoteExpressionTypeInfo& info /*out*/)
		{
			if (busIndex == 0 && channel == 0)
			{
				return m_noteExpressionTypes.getNoteExpressionInfo(noteExpressionIndex, info);
			}
			return Steinberg::kResultFalse;
		}
		
		//------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Controller::getNoteExpressionStringByValue(Steinberg::int32 busIndex,
			                                                                     Steinberg::int16 channel,
			                                                                     Steinberg::Vst::NoteExpressionTypeID id,
			                                                                     Steinberg::Vst::NoteExpressionValue valueNormalized /*in*/,
			                                                                     Steinberg::Vst::String128 string /*out*/)
		{
			if (busIndex == 0 && channel == 0)
			{
				return m_noteExpressionTypes.getNoteExpressionStringByValue(id, valueNormalized, string);
			}
			return Steinberg::kResultFalse;
		}
		
		//------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Controller::getNoteExpressionValueByString(Steinberg::int32 busIndex,
			                                                                     Steinberg::int16 channel,
			                                                                     Steinberg::Vst::NoteExpressionTypeID id,
			                                                                     const Steinberg::Vst::TChar* string /*in*/,
				                                                                  Steinberg::Vst::NoteExpressionValue& valueNormalized /*out*/)
		{
			if (busIndex == 0 && channel == 0)
			{
				return m_noteExpressionTypes.getNoteExpressionValueByString(id, string, valueNormalized);

			}
			return Steinberg::kResultFalse;
		}
		
		//------------------------------------------------------------------------
		Steinberg::tresult PLUGIN_API Controller::getMidiControllerAssignment(Steinberg::int32 busIndex,
																			 Steinberg::int16 channel,
																			 Steinberg::Vst::CtrlNumber midiControllerNumber,
																			 Steinberg::Vst::ParamID& id/*out*/)
{
			if (busIndex == 0 && channel == 0)
			{
				id = 0;
				switch (midiControllerNumber)
				{
					case Steinberg::Vst::kPitchBend: 
					{
						//id = Steinberg::Vst::kParamMasterTuning;
						break;
					}
				}
				return id != 0 ? Steinberg::kResultTrue : Steinberg::kResultFalse;
			}
			return Steinberg::kResultFalse;
		}
				
		//------------------------------------------------------------------------
	} // namespace Hampshire
} // namespace Carlsound
