#include "State.h"

#include "Level.h"
#include "StateStack.h"

State::Context::Context(sf::RenderWindow& window, FontHolder& fonts, SoundBufferHolder& soundBuffers,
	Game::Settings& settings, Game::Level& level)
	: window(window)
	, fonts(fonts)
	, soundBuffers(soundBuffers)
	, settings(settings)
	, level(level)
{
}

State::State(StateStack& stack, Context& context)
	: mStack(&stack)
	, mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
	mStack->pushState(stateID);
}

void State::requestStackPop()
{
	mStack->popState();
}

void State::requestStateClear()
{
	mStack->clearStates();
}

State::Context State::getContext() const
{
	return mContext;
}
