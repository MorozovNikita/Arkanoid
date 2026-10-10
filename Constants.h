#pragma once

#include <string>

namespace Game
{
	using namespace std::literals;

	const std::string RESOURCES_PATH = "Resources/"s;

	const int SCREEN_WIDTH				= 800;
	const int SCREEN_HEIGHT				= 600;

	const float PLATFORM_WIDTH			= 140.f;
	const float PLATFORM_HEIGHT			= 18.f;
	const float PLATFORM_SPEED			= 520.f;
	const float PLATFORM_MARGIN_BOTTOM  = 28.f;

	const float BALL_RADIUS				= 8.f;
	const float BALL_SPEED				= 420.f;

	const float BLOCK_WIDTH				= 80.f;
	const float BLOCK_HEIGHT			= 18.f;
	const float BLOCK_GAP_X				= 10.f;
	const float BLOCK_GAP_Y				= 10.f;
	const int   BLOCK_COLUMNS			= 6;
	const int   BLOCK_ROWS				= 3;
	const float BLOCK_FIELD_TOP			= 64.f;
	const float SMOOTH_BLOCK_FADE_TIME	= 1.f;
	const int   DURABLE_BLOCK_HIT_POINTS = 3;

	const float PAUSE_COUNTDOWN			= 1.f;
}
