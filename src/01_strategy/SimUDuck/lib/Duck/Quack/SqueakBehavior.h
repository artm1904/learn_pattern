#ifndef SQUEAKBEHAVIOR_H
#define SQUEAKBEHAVIOR_H

#include "IQuakBehavior.h"
#include <iostream>

class SqueakBehavior : public IQuackBehavior
{
public:
	void Quack() const override
	{
		std::cout << "Squeek!!!" << std::endl;
	}
};

#endif
