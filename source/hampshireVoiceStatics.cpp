#include "../include/hampshireVoiceStatics.h"

namespace Carlsound
{
	namespace Hampshire
	{
		//-----------------------------------------------------------------------------
		double VoiceStatics::normalizedLevel2Gain(float normalized)
		{
			double level;
			if (normalized >= 0.5)
				level = scaleHeadRoom * ::pow(10, 
				                              (normalized - 0.5f) * 24 / 20.0f);
			else
				level = scaleNorm2GainC1 * ::pow(normalized, 
					                             scaleNorm2GainC2);

			return level;
		}
	} // namespace Hampshire
} // namespace Carlsound
