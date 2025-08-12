#pragma once

struct IDanceBehavior
{
	virtual ~IDanceBehavior() {}
	virtual void Dance() const  = 0;
};