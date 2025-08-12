#ifndef DUCK_H
#define DUCK_H

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "Dance/IDanceBehavior.h"
#include "Fly/IFlyBehavior.h"
#include "Fly/FlyNoWay.h"
#include "Quack/IQuakBehavior.h"

class Duck {
   public:
    Duck(std::unique_ptr<IFlyBehavior>&& flyBehavior,
         std::unique_ptr<IQuackBehavior>&& quackBehavior,
         std::unique_ptr<IDanceBehavior>&& danceBehavior)
        : m_flyBehavior(std::move(flyBehavior)),
          m_quackBehavior(std::move(quackBehavior)),
          m_danceBehavior(std::move(danceBehavior)) {
        assert(m_flyBehavior);
        assert(m_quackBehavior);
        assert(m_danceBehavior);
    }

    void Quack() const { m_quackBehavior->Quack(); }

    void Swim() const { std::cout << "I'm swimming" << std::endl; }

    void Fly() const {
        m_flyBehavior->Fly();

        if (m_flyBehavior->IsFlying()) {
            m_countFly++;
            std::cout << "Fly count: " << m_countFly << std::endl;
        }
    }

    void Dance() const { m_danceBehavior->Dance(); }

    void SetFlyBehavior(std::unique_ptr<IFlyBehavior>&& flyBehavior) {
        assert(flyBehavior);
        m_flyBehavior = std::move(flyBehavior);
        m_countFly = 0;
    }

    void SetDanceBehavior(std::unique_ptr<IDanceBehavior>&& danceBehavior) {
        assert(danceBehavior);
        m_danceBehavior = std::move(danceBehavior);
    }

    virtual void Display() const = 0;
    virtual ~Duck() = default;

   private:
    std::unique_ptr<IFlyBehavior> m_flyBehavior;
    std::unique_ptr<IQuackBehavior> m_quackBehavior;
    std::unique_ptr<IDanceBehavior> m_danceBehavior;
    mutable int m_countFly = 0;
};

#endif
