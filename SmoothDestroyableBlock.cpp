#include "SmoothDestroyableBlock.h"

#include "Constants.h"

#include <algorithm>

namespace Game
{
    SmoothDestroyableBlock::SmoothDestroyableBlock(sf::Vector2f position, sf::Color color)
        : Block(position, color, sf::Color(24, 28, 36))
    {
    }

    void SmoothDestroyableBlock::update(sf::Time dt)
    {
        UpdateTimer(dt.asSeconds());
    }

    void SmoothDestroyableBlock::OnHit()
    {
        if (mBreaking)
            return;

        mBreaking = true;
        mFill = fillColor();
        mOutline = outlineColor();
        start(SMOOTH_BLOCK_FADE_TIME);
    }

    bool SmoothDestroyableBlock::canCollide() const
    {
        return isAlive() && !mBreaking;
    }

    void SmoothDestroyableBlock::EachTickAction(float)
    {
        const float progress = duration() > 0.f ? elapsed() / duration() : 1.f;
        const float remain = std::clamp(1.f - progress, 0.f, 1.f);

        auto fade = [remain](sf::Color color)
        {
            color.a = static_cast<std::uint8_t>(static_cast<float>(color.a) * remain);
            return color;
        };

        setFillColor(fade(mFill));
        setOutlineColor(fade(mOutline));
    }

    void SmoothDestroyableBlock::FinalAction()
    {
        destroy();
    }
}
