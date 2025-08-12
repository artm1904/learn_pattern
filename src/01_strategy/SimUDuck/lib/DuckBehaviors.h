#pragma once

#include <functional>
#include <iostream>

using FlyStrategy = std::function<int()>;
using QuackStrategy = std::function<void()>;
using DanceStrategy = std::function<void()>;

// --- Фабрики стратегий полета ---
inline FlyStrategy CreateFlyNoWay() {
    return []() { return 0; };
}

inline FlyStrategy CreateFlyWithWings() {
    return [flyCount = 0]() mutable {
        flyCount++;
        std::cout << "Fly count: " << flyCount << std::endl;

        return flyCount;
    };
};

// --- Фабрики стратегий кряканья ---
inline QuackStrategy CreateMuteQuack() {
    return []() {};
}

inline QuackStrategy CreateQuackBehavior() {
    return []() { std::cout << "Quack Quack!!!" << std::endl; };
}

inline QuackStrategy CreateSqueakBehavior() {
    return []() { std::cout << "Squeek!!!" << std::endl; };
}

// --- Фабрики стратегий танца ---
inline DanceStrategy CreateDanceNoWay() {
    return []() {};
}

inline DanceStrategy CreateDanceWaltz() {
    return []() { std::cout << "Waltz" << std::endl; };
}

inline DanceStrategy CreateDanceMenuet() {
    return []() { std::cout << "Menuet" << std::endl; };
}
