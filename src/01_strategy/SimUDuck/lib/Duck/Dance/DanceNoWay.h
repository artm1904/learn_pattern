#pragma once

#include "IDanceBehavior.h"

class DanceNoWay : public IDanceBehavior {
   public:
    void Dance() const override {}
};