#include "IDelayedAction.h"

namespace Game
{
    void IDelayedAction::start(float durationSeconds)
    {
        if (mRunning)
            return;

        mDuration = durationSeconds;
        mElapsed = 0.f;
        mRunning = true;
    }

    void IDelayedAction::UpdateTimer(float deltaTime)
    {
        if (!mRunning)
            return;

        mElapsed += deltaTime;
        EachTickAction(deltaTime);

        if (mElapsed < mDuration)
            return;

        mRunning = false;
        FinalAction();
    }
}
