#include <cstdlib>

#include "lib/Duck/Dance/DanceMenuet.h"
#include "lib/Duck/Dance/DanceValse.h"
#include "lib/Duck/DecoyDuck.h"
#include "lib/Duck/MallardDuck.h"
#include "lib/Duck/ModelDuck.h"
#include "lib/Duck/RedheadDuck.h"
#include "lib/Duck/RubberDuck.h"
#include "lib/DuckFunctions.h"

int main() {
    MallardDuck mallardDuck;
    PlayWithDuck(mallardDuck);

    std::cout << "Update MallardDuck behavior, now she can dance menuet" << std::endl;
    mallardDuck.SetDanceBehavior(std::make_unique<DanceMenuet>());
    PlayWithDuck(mallardDuck);

    RedheadDuck redheadDuck;
    redheadDuck.SetDanceBehavior(std::make_unique<DanceMenuet>());
    PlayWithDuck(redheadDuck);

    RubberDuck rubberDuck;
    PlayWithDuck(rubberDuck);

    DecoyDuck decoyDuck;
    PlayWithDuck(decoyDuck);

    ModelDuck modelDuck;
    PlayWithDuck(modelDuck);

    modelDuck.SetFlyBehavior(std::make_unique<FlyWithWings>());
    PlayWithDuck(modelDuck);

    return EXIT_SUCCESS;
}