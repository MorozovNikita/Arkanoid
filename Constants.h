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
	const float BLOCK_FIELD_TOP			= 64.f;
	const float SMOOTH_BLOCK_FADE_TIME	= 1.f;
	const int   DURABLE_BLOCK_HIT_POINTS = 3;

	const std::string LEVELS_PATH		= "Levels/"s;
	const std::string LEADERBOARD_PATH	= "leaderboard.txt"s;
	const int LEVEL_COLUMN_COUNT		= 9;
	const int LEVEL_ROW_COUNT			= 8;

	const float PAUSE_COUNTDOWN			= 1.f;

	const int   GLASS_BLOCK_SCORE		= 1;
	const int   SMOOTH_BLOCK_SCORE		= 2;
	const int   STARTING_LIVES			= 1;

	const float BONUS_DROP_CHANCE		= 0.1f;
	const float BONUS_FALL_SPEED		= 180.f;
	const float BONUS_SIZE				= 20.f;
	const float WIDE_PLATFORM_TIME		= 10.f;
	const float WIDE_PLATFORM_SCALE		= 1.5f;
}
