#pragma once

#include <algorithm>
#include <memory>
#include <vector>

class IObservable;

class IObserver
{
public:
    virtual ~IObserver() = default;
    virtual void Notify(std::shared_ptr<IObservable> observable) = 0;
};

class IObservable : public std::enable_shared_from_this<IObservable>
{
public:
    IObservable() = default;
    IObservable(const IObservable&) = delete;
    IObservable& operator=(const IObservable&) = delete;

    virtual ~IObservable() = default;

    void AddObserver(std::weak_ptr<IObserver> observer)
    {
        observers.push_back(std::move(observer));
    }

protected:
    void Emit()
    {
        const auto self = shared_from_this();
        std::erase_if(observers, [&self](const std::weak_ptr<IObserver>& observer)
        {
            const auto lockedObserver = observer.lock();
            if (!lockedObserver)
                return true;

            lockedObserver->Notify(self);
            return false;
        });
    }

private:
    std::vector<std::weak_ptr<IObserver>> observers;
};
