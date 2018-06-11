

#include "../include/noteExpressionSynthController.h"

#include "../include/panNoteExpressionType.h"
#include "../include/releaseTimeModNoteExpressionType.h"

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
    Steinberg::tresult result = EditController::initialize (context);
	if (result == Steinberg::kResultTrue)
	{
		//---Create Parameters------------
		/*
		parameters.addParameter (STR16 ("Bypass"), // title
			                     0, // units
			                     1, // stepCount
			                     0, // defaultValueNormalized
                                 Steinberg::Vst::ParameterInfo::kCanAutomate | Steinberg::Vst::ParameterInfo::kIsBypass, // flags
		                         HuntleyParams::kBypassId, // tag
			                     0, // unitUD
			                     STR16("Bypass")); // shortTitle

		parameters.addParameter (STR16 ("Speed"), // title
		                         STR16 ("sec"), // units
			                     99, // stepCount
			                     0.1, // defaultValueNormalized
                                 Steinberg::Vst::ParameterInfo::kCanAutomate, HuntleyParams::kParamSpeedId, // flags
			                     0, // unitID
		                         STR16 ("Speed")); // shortTitle
		

		
		m_speedRangeParameter = std::make_shared<Steinberg::Vst::RangeParameter> (STR16("Speed"), // title
			                                                                 HuntleyParams::kParamSpeedId, // ParamID
			                                                                 STR16("sec"), // units
			                                                                 0.1, // minPlain
			                                                                 10.0, // maxPlain
			                                                                 1.0, // defaultValuePlain
			                                                                 99, // stepCount
			                                                                 Steinberg::Vst::ParameterInfo::kCanAutomate, // flags
			                                                                 0, // unitID
			                                                                 STR16("Speed")); // shortTitle
		parameters.addParameter(m_speedRangeParameter->getInfo());
		*/
		/////////////////////////////////
		Steinberg::Vst::Parameter* param;
		/////////////////////////////////
		/*
		param = new Steinberg::Vst::RangeParameter(USTRING("Master Volume"),
			                                       kParamMasterVolume,
			                                       USTRING("%"), 
			                                       0, 
			                                       100, 
			                                       80);
		param->setPrecision(1);
		parameters.addParameter(param);
		*/
		/////////////////////////////////
		/////////////////////////////////
		// Init Note Expression Types
		/*
		noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::NoteExpressionType(Steinberg::Vst::kVolumeTypeID, 
			                                      Steinberg::String("Volume"), 
			                                      Steinberg::String("Vol"), 
			                                      0, 
			                                      -1, 
			                                      1., 
			                                      0., 
			                                      1., 
			                                      0, 
			                                      0));
		*/
	}
    return Steinberg::kResultTrue;
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Carlsound::Huntley::Controller::terminate()
{
	noteExpressionTypes.removeAll();
	return EditController::terminate();
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Controller::setComponentState (Steinberg::IBStream* state)
{
	// we receive the current state of the component (processor part)
	// we read our parameters and bypass value...
	GlobalParameterState gps;
	Steinberg::tresult result = gps.setState(state);

	if (result == Steinberg::kResultTrue)
	{
		/*
		setParamNormalized(kParamMasterVolume, 
			               gps.masterVolume);
		*/
	}
	return result;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Controller::setParamNormalized(Steinberg::Vst::ParamID tag,
		                                                     Steinberg::Vst::ParamValue value)
{
	bool newRange = false;
	if (tag == kParamTuningRange && getParamNormalized(tag) != value)
	{
		newRange = true;
		Steinberg::Vst::NoteExpressionType* net = noteExpressionTypes.getNoteExpressionType(Steinberg::Vst::kTuningTypeID);
		if (value > 0)
		{
			noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::NoteExpressionType(Controller::kTriangleSlopeTypeID, 
				                                                                             Steinberg::String("Triangle Slope"),
				                                                                             Steinberg::String("Tri Slope"),
				                                                                             Steinberg::String("%"),
				                                                                             -1,
				                                                                             getParameterObject(kParamTriangleSlop),
				                                                                             Steinberg::Vst::NoteExpressionTypeInfo::kIsAbsolute));
			if (net)
			{
				net->getInfo().valueDesc.minimum = 0.5 - 3 * VoiceStatics::kNormTuningOneTune;
				net->getInfo().valueDesc.maximum = 0.5 + 2 * VoiceStatics::kNormTuningOneTune;
			}
		}
		else
		{
			noteExpressionTypes.removeNoteExpressionType(Controller::kTriangleSlopeTypeID);
			if (net)
			{
				net->getInfo().valueDesc.minimum = 0.5 - VoiceStatics::kNormTuningOneOctave;
				net->getInfo().valueDesc.maximum = 0.5 + VoiceStatics::kNormTuningOneOctave;
			}
		}
	}

	Steinberg::tresult res = Steinberg::Vst::EditController::setParamNormalized(tag, value);

	if (newRange && componentHandler)
		componentHandler->restartComponent(Steinberg::Vst::kNoteExpressionChanged);

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
		return noteExpressionTypes.getNoteExpressionCount();
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
		return noteExpressionTypes.getNoteExpressionInfo(noteExpressionIndex, info);
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
		return noteExpressionTypes.getNoteExpressionStringByValue(id, valueNormalized, string);
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
		return noteExpressionTypes.getNoteExpressionValueByString(id, string, valueNormalized);

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
				id = kParamMasterTuning;
				break;
			}
			case Steinberg::Vst::kCtrlVolume: 
			{
				id = kParamMasterVolume;
				break;
			}
			case Steinberg::Vst::kCtrlFilterCutoff: 
			{
				id = kParamFilterFreq;
				break;
			}
			case Steinberg::Vst::kCtrlFilterResonance: 
			{
				id = kParamFilterQ;
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
