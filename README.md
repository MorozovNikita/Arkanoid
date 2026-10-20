# Arkanoid

Игра на C++ и SFML 3. Ниже схема классов. `State`, `StateStack`, `IObserver` и `IObservable` лежат в глобальном пространстве имён, остальные классы — в `Game`.

## Объекты на поле

`Block` и `Ball` сообщают о себе через `Emit()`. `GameState` подписан как `IObserver`. Бонус при подборе вызывает метод `BonusTarget`, этот интерфейс тоже реализует `GameState`.

```mermaid
classDiagram
    class GameObject {
        +draw()
        +bounds()
        +isAlive() bool
        +destroy()
    }
    class Collidable {
        <<interface>>
        +collisionBounds()
        +canCollide() bool
        +deflectsBall() bool
    }
    class IObservable {
        +AddObserver()
        #Emit()
    }
    class IObserver {
        <<interface>>
        +Notify()
    }
    class IDelayedAction {
        <<interface>>
        +UpdateTimer()
        #EachTickAction()
        #FinalAction()
    }
    class BonusTarget {
        <<interface>>
        +grantExtraBall()
        +grantWidePlatform()
        +grantExtraLife()
    }

    GameObject <|-- Platform
    GameObject <|-- Ball
    GameObject <|-- Block
    GameObject <|-- Bonus
    Collidable <|.. Block
    IObservable <|-- Ball
    IObservable <|-- Block
    IDelayedAction <|.. SmoothDestroyableBlock

    Block <|-- GlassBlock
    Block <|-- SmoothDestroyableBlock
    Block <|-- DurableBlock
    Block <|-- UnbreakableBlock

    Bonus <|-- ExtraBallBonus
    Bonus <|-- WidePlatformBonus
    Bonus <|-- ExtraLifeBonus
    Bonus ..> BonusTarget : apply
    IObservable --> IObserver : Emit
```

Очки за разрушение: стекло 1, исчезающий блок 2, крепкий блок — столько, сколько у него ударов.

## Состояния

`Application` владеет стеком состояний, текущим уровнем и таблицей рекордов. Конец партии `GameState` не рисует сам: он вызывает `onSessionFinished`, а экран рекордов только читает `Leaderboard`.

```mermaid
classDiagram
    class Application
    class StateStack
    class State {
        <<abstract>>
        +draw()
        +update()
        +handleEvent()
    }
    class IObserver {
        <<interface>>
        +Notify()
    }
    class BonusTarget {
        <<interface>>
    }
    class ISessionObserver {
        <<interface>>
        +onSessionFinished()
    }
    class Leaderboard

    Application *-- StateStack
    Application *-- Level
    Application *-- Leaderboard
    StateStack o-- State

    State <|-- MenuState
    State <|-- LevelSelectState
    State <|-- GameState
    State <|-- PauseState
    State <|-- SettingsState
    State <|-- LeaderboardState

    IObserver <|.. GameState
    BonusTarget <|.. GameState
    ISessionObserver <|.. Leaderboard
    GameState ..> ISessionObserver : конец партии
    LeaderboardState ..> Leaderboard

    GameState *-- Platform
    GameState o-- Ball
    GameState o-- Block
    GameState o-- Bonus
```

## Фабрики

Какая фабрика сработает, выбирается по строке типа блока или случайно для бонуса. Сам объект собирает наследник в `CreateBlock()` или `create()`.

```mermaid
classDiagram
    class BlockFactory {
        <<abstract>>
        +CreateBlock() Block
        +type()
    }
    class BonusFactory {
        <<abstract>>
        +create() Bonus
    }
    class Level
    class GameState

    BlockFactory <|-- SmoothDestroyableBlockFactory
    BlockFactory <|-- DurableBlockFactory
    BlockFactory <|-- GlassBlockFactory
    BlockFactory <|-- UnbreakableBlockFactory

    BonusFactory <|-- ExtraBallBonusFactory
    BonusFactory <|-- WidePlatformBonusFactory
    BonusFactory <|-- ExtraLifeBonusFactory

    Level ..> BlockFactory : тип из JSON
    GameState ..> BonusFactory : шанс 10%
    SmoothDestroyableBlockFactory ..> SmoothDestroyableBlock
    DurableBlockFactory ..> DurableBlock
    GlassBlockFactory ..> GlassBlock
    UnbreakableBlockFactory ..> UnbreakableBlock
    ExtraBallBonusFactory ..> ExtraBallBonus
    WidePlatformBonusFactory ..> WidePlatformBonus
    ExtraLifeBonusFactory ..> ExtraLifeBonus
```
