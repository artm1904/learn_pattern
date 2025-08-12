#ifndef FLYWITHWINGS_H
#define FLYWITHWINGS_H

#include <iostream>

#include "IFlyBehavior.h"

class FlyWithWings : public IFlyBehavior {
   public:
    void Fly() const override { std::cout << "I'm flying with wings!!" << std::endl; }
};

#endif
