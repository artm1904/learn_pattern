#pragma once

#include "IBeverage.h"

// Базовый класс декоратора
class CBeverageDecorator : public IBeverage {
   public:
    CBeverageDecorator(std::unique_ptr<IBeverage>&& beverage) : m_beverage(std::move(beverage)) {}

    std::string GetDescription() const override { return m_beverage->GetDescription(); }

    double GetCost() const override { return m_beverage->GetCost(); }

   private:
    std::unique_ptr<IBeverage> m_beverage;
};

// Декоратор для двойной порции
class CDoublePortion : public CBeverageDecorator {
   public:
    CDoublePortion(std::unique_ptr<IBeverage>&& beverage)
        : CBeverageDecorator(std::move(beverage)) {}

    std::string GetDescription() const override {
        return "Double " + CBeverageDecorator::GetDescription();
    }

    double GetCost() const override {
        // Двойной капучино: 80 -> 120 (+40)
        // Двойной латте: 90 -> 130 (+40)
        // Добавка за двойную порцию составляет 40 рублей.
        return CBeverageDecorator::GetCost() + 40;
    }
};

// Базовая реализация напитка, предоставляющая его описание
class CBeverage : public IBeverage {
   public:
    CBeverage(const std::string& description) : m_description(description) {}

    std::string GetDescription() const override final { return m_description; }

   private:
    std::string m_description;
};

// Кофе
class CCoffee : public CBeverage {
   public:
    CCoffee(const std::string& description = "Coffee") : CBeverage(description) {}

    double GetCost() const override { return 60; }
};

// Капуччино
class CCappuccino : public CCoffee {
   public:
    CCappuccino() : CCoffee("Cappuccino") {}

    double GetCost() const override { return 80; }
};

// Латте
class CLatte : public CCoffee {
   public:
    CLatte() : CCoffee("Latte") {}

    double GetCost() const override { return 90; }
};

// Чай
class CTea : public CBeverage {
   public:
    CTea(const std::string& description = "Tea") : CBeverage(description) {}

    double GetCost() const override { return 30; }
};

class CBlackTea : public CTea {
   public:
    CBlackTea() : CTea("Black Tea") {}
};

class CGreenTea : public CTea {
   public:
    CGreenTea() : CTea("Green Tea") {}
};

class CWhiteTea : public CTea {
   public:
    CWhiteTea() : CTea("White Tea") {}
};

// Молочный коктейль
class CMilkshake : public CBeverage {
   public:
    CMilkshake() : CBeverage("Milkshake") {}

    double GetCost() const override { return 80; }
};
