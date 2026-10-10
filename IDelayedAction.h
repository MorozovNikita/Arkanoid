#pragma once

namespace Game
{
    class IDelayedAction
    {
    public:
        virtual ~IDelayedAction() = default;

        void UpdateTimer(float deltaTime);

    protected:
        void start(float durationSeconds);
        bool isRunning() const { return mRunning; }
        float elapsed() const { return mElapsed; }
        float duration() const { return mDuration; }

        virtual void FinalAction() = 0;
        virtual void EachTickAction(float deltaTime) = 0;

    private:
        float mDuration = 0.f;
        float mElapsed = 0.f;
        bool mRunning = false;
    };
}
