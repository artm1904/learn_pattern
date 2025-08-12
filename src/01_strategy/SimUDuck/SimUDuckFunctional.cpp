#include <iostream>
#include <memory>

#include "lib/Duck/FunctionalDuck.h"
#include "lib/DuckBehaviors.h"


void PlayWithDuck(const FunctionalDuck& duck)
{
	duck.Display();
	duck.Quack();
	duck.Fly();
	duck.Fly(); 
	duck.Dance();
	std::cout << "----------------" << std::endl;
}

class MallardDuck : public FunctionalDuck
{
public:
	MallardDuck()
		: FunctionalDuck(
			// Создаем поведение полета, передавая ему текущее поведение кряканья
			CreateFlyWithWings(),
			CreateQuackBehavior(),
			CreateDanceWaltz())
	{
	}
	void Display() const override { std::cout << "I'm a mallard duck" << std::endl; }
};

class RubberDuck : public FunctionalDuck
{
public:
	RubberDuck()
		: FunctionalDuck(CreateFlyNoWay(), CreateQuackBehavior(), CreateDanceNoWay())
	{
	}
	void Display() const override { std::cout << "I'm a rubber duck" << std::endl; }
};


int main()
{
	MallardDuck mallard;
	PlayWithDuck(mallard);

	RubberDuck rubber;
	PlayWithDuck(rubber);

	std::cout << ">> Changing mallard's fly behavior at runtime <<" << std::endl;
	// При смене поведения мы просто создаем новое замыкание.
	// Старое замыкание со своим счетчиком уничтожается, новое создается с `flyCount = 0`.
	// Это полностью соответствует требованию сброса счетчика.
	mallard.SetFlyStrategy(CreateFlyWithWings());
	std::cout << "Flying again after behavior change:" << std::endl;
	mallard.Fly(); // Полет #1 с новым поведением
	mallard.Fly(); // Полет #2 с новым поведением

	return 0;
}