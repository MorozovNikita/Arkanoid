#pragma once

#include "Block.h"
#include "IDelayedAction.h"

namespace Game
{
    class SmoothDestroyableBlock : public Block, public IDelayedAction
    {
    public:
        SmoothDestroyableBlock(sf::Vector2f position, sf::Color color);

        void update(sf::Time dt) override;
        void OnHit() override;
        bool canCollide() const override;

    protected:
        void FinalAction() override;
        void EachTickAction(float deltaTime) override;

    private:
        bool mBreaking = false;
        sf::Color mFill;
        sf::Color mOutline;
    };
}
