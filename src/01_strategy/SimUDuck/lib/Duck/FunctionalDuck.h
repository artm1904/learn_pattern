#pragma once

#include <functional>
#include <iostream>

class FunctionalDuck {
   public:
    using FlyStrategy = std::function<int()>;
    using QuackStrategy = std::function<void()>;
    using DanceStrategy = std::function<void()>;

    FunctionalDuck(FlyStrategy flyBeh, QuackStrategy quackBeh, DanceStrategy danceBeh)
        : m_flyBeh(flyBeh), m_quackBeh(quackBeh), m_danceBeh(danceBeh) {};

    void Fly() const {
        auto count = m_flyBeh();
        if (count != 0 && count % 2 == 0) {
            m_quackBeh();
        }
    }
    void Quack() const { m_quackBeh(); }
    void Dance() const { m_danceBeh(); }

    void SetFlyStrategy(FlyStrategy flyBeh) { m_flyBeh = std::move(flyBeh); }
    void SetQuackStrategy(QuackStrategy quackBeh) { m_quackBeh = std::move(quackBeh); }
    void SetDanceStrategy(DanceStrategy danceBeh) { m_danceBeh = std::move(danceBeh); }

    virtual void Display() const = 0;

    virtual ~FunctionalDuck() = default;

   private:
    FlyStrategy m_flyBeh;
    QuackStrategy m_quackBeh;
    DanceStrategy m_danceBeh;
};
