#pragma once

#include "Block.h"
#include "Constants.h"
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
        int score() const override { return SMOOTH_BLOCK_SCORE; }

    protected:
        void FinalAction() override;
        void EachTickAction(float deltaTime) override;

    private:
        bool mBreaking = false;
        sf::Color mFill;
        sf::Color mOutline;
    };
}
