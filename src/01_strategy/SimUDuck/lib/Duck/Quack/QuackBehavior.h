#ifndef QUACKBEHAVIOR_H
#define QUACKBEHAVIOR_H

#include "IQuakBehavior.h"
#include <iostream>

class QuackBehavior : public IQuackBehavior
{
public:
	void Quack() const override
	{
		std::cout << "Quack Quack!!!" << std::endl;
	}
};

#endif
