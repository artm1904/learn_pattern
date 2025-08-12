#pragma once

#include <iostream>

#include "IDanceBehavior.h"

class DanceMenuet : public IDanceBehavior {
   public:
    void Dance() const override { std::cout << "I'm dancing menuet!!" << std::endl; }
};