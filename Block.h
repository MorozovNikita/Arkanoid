#pragma once

#include "Collidable.h"
#include "GameObject.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Time.hpp>

namespace Game
{
    enum class HitSound
    {
        Bonus,
        Glass,
        Wall
    };

    class Block : public GameObject, public Collidable
    {
    public:
        virtual void update(sf::Time dt);
        virtual void OnHit() = 0;
        virtual bool mustBeCleared() const;
        virtual HitSound hitSound() const { return HitSound::Bonus; }

        sf::FloatRect collisionBounds() const override;
        bool canCollide() const override;
        bool deflectsBall() const override;

    protected:
        Block(sf::Vector2f position, sf::Color fill, sf::Color outline);

        void setFillColor(sf::Color color);
        void setOutlineColor(sf::Color color);
        sf::Color fillColor() const;
        sf::Color outlineColor() const;

    private:
        sf::Shape& shape() override;
        const sf::Shape& shape() const override;

        sf::RectangleShape mShape;
    };
}
