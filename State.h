#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"

namespace sf
{
	class RenderWindow;
}

namespace Game
{
	struct Settings;
}

class StateStack;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context
	{
		Context(sf::RenderWindow& window, FontHolder& fonts, SoundBufferHolder& soundBuffers, Game::Settings& settings);

		sf::RenderWindow&  window;
		FontHolder&		   fonts;
		SoundBufferHolder& soundBuffers;
		Game::Settings&	   settings;
	};

public:
	State(StateStack& stack, Context& context);
	virtual				~State();

	virtual void		draw() = 0;
	virtual bool		update(sf::Time dt) = 0;
	virtual bool		handleEvent(const sf::Event& event) = 0;

protected:
	void				requestStackPush(States::ID stateID);
	void				requestStackPop();
	void				requestStateClear();

	Context				getContext() const;

private:
	StateStack* mStack;
	Context&	mContext;
};
