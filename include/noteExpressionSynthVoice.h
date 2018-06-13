#pragma once

#include "globalParameterState.h"
#include "noteExpressionSynthController.h"
#include "noteExpressionSynthVoiceStatistics.h"
#include "noteExpressionSynthVoiceStatisticsOnce.h"
#include "maximilian.h"

#include "public.sdk/samples/vst/common/voicebase.h"
#include "public.sdk/samples/vst/common/logscale.h"

#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/base/futils.h"

#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI			3.14159265358979323846
#endif
#ifndef M_PI_MUL_2
#define M_PI_MUL_2		6.28318530717958647692
#endif

namespace Carlsound
{
	namespace Hampshire
	{
		
		//#define MAX_VOICES				64
		//#define MAX_RELEASE_TIME_SEC	5.0
		//#define NUM_FILTER_TYPE			3
		//#define NUM_TUNING_RANGE		2 

		//-----------------------------------------------------------------------------
		enum VoiceParameters
		{
			kVolumeMod
		};
		
		//-----------------------------------------------------------------------------
		template<class SamplePrecision>
		class Voice : public Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>
		{
		public:
			Voice ();
			~Voice ();
		
			void setSampleRate (Steinberg::Vst::ParamValue sampleRate) SMTG_OVERRIDE;
			void noteOn (Steinberg::int32 pitch, Steinberg::Vst::ParamValue velocity, float tuning, Steinberg::int32 sampleOffset, Steinberg::int32 nId) SMTG_OVERRIDE;
			void noteOff (Steinberg::Vst::ParamValue velocity, Steinberg::int32 sampleOffset) SMTG_OVERRIDE;
			bool process (SamplePrecision* outputBuffers[2], Steinberg::int32 numSamples);
			void reset () SMTG_OVERRIDE;
			void setNoteExpressionValue (Steinberg::int32 index, Steinberg::Vst::ParamValue value) SMTG_OVERRIDE;
		
		protected:
			Steinberg::uint32 n;
			Steinberg::int32 noisePos;
			Steinberg::int32 noiseStep;
		
			Steinberg::Vst::ParamValue currentVolume;
		
			Steinberg::Vst::ParamValue levelFromVel;
			Steinberg::Vst::ParamValue noteOffVolumeRamp;

			std::shared_ptr<maxiOsc> m_oscillator;
			std::shared_ptr<maxiSettings> m_oscillatorSettings;
		};
	} // Hampshire
} // Carlsound
