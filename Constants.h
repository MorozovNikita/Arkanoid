#pragma once

#include <string>

namespace Game
{
	using namespace std::literals;

	const std::string RESOURCES_PATH = "Resources/"s;

	const int SCREEN_WIDTH  = 800;
	const int SCREEN_HEIGHT = 600;

	const float PLATFORM_WIDTH = 140.f;
	const float PLATFORM_HEIGHT = 18.f;
	const float PLATFORM_SPEED = 520.f;
	const float PLATFORM_MARGIN_BOTTOM = 28.f;

	const float BALL_RADIUS = 8.f;
	const float BALL_SPEED = 420.f;

	const float PAUSE_COUNTDOWN = 1.f;
}
