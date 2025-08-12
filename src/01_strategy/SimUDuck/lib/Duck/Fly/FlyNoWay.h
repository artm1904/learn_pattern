#ifndef FLYNOWAY_H
#define FLYNOWAY_H

#include "IFlyBehavior.h"

class FlyNoWay : public IFlyBehavior
{
public:
	void Fly() const  override {}
	bool IsFlying() const override { return false; }
};

#endif
