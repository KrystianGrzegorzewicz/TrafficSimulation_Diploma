#pragma once

struct ConflictParameters
{
	// HUMAN
	float humanBaseMargin = 1.5f;
	float humanAggressionMargin = 1.5f;
	float humanStopBuffer = 3.0f;
	float humanYieldHoldTime = 1.0f;

	// AV
	float avMargin = 0.8f;
	float avHumanExtraMargin = 1.0f;
	float avStopBuffer = 2.0f;
	float avHumanStopBuffer = 4.0f;

	// hysteresis
	float resumeMargin = 0.75f;
};