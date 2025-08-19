#include "WeatherData.h"

// int main() {
//     CWeatherData wdIn("In");
//     CWeatherData wdOut("Out");

//     CDisplay display;
//     wdIn.RegisterObserver(display, 10);
//     wdOut.RegisterObserver(display, 10);

//     wdIn.SetMeasurements(24, 65, 760);
//     // ... в консоли появится статистика только для "In"

//     wdOut.SetMeasurements(10, 80, 745);
//     // ... в консоли появится обновленная статистика и для "In", и для "Out"
//     return 0;
// }



// int main() {
//     CWeatherDataPro wdPro("Out (Pro)");
//     CWeatherData wdIn("In (Standard)");

//     CStatsDisplay statsDisplay;
//     wdPro.RegisterObserver(statsDisplay);
//     wdIn.RegisterObserver(statsDisplay);

//     std::cout << "--- First update from Pro station ---" << std::endl;
//     wdPro.SetMeasurements(10, 80, 745, 5.0, 90); // 5 м/с, ветер восточный

//     std::cout << "\n--- Update from Standard station ---" << std::endl;
//     wdIn.SetMeasurements(22, 60, 760);

//     std::cout << "\n--- Second update from Pro station ---" << std::endl;
//     wdPro.SetMeasurements(12, 78, 744, 7.0, 135); // 7 м/с, юго-восточный

//     return 0;
// }

int main() {
    CWeatherDataPro wdPro("Winter Fishing Spot");

    // Fisherman's display, only cares about Temp and Pressure
    CStatsDisplay fishermanDisplay;
    wdPro.RegisterObserver(fishermanDisplay, "Temperature", 10);
    wdPro.RegisterObserver(fishermanDisplay, "Pressure", 10);
    wdPro.RegisterObserver(fishermanDisplay, "Wind", 10);


    // A full display for testing, subscribed to all events
    CDisplay fullDisplay("Full Monitor");
    wdPro.RegisterObserver(fullDisplay, "Temperature");
    wdPro.RegisterObserver(fullDisplay, "Humidity");
    wdPro.RegisterObserver(fullDisplay, "Pressure");
    wdPro.RegisterObserver(fullDisplay, "Wind");

    std::cout << "--- Setting initial state ---" << std::endl;
    wdPro.SetMeasurements(-5, 80, 750, 2.0, 45);

    std::cout << "\n--- Wind changes, fisherman's stats display should NOT update stats, but full "
                 "display is notified ---"
              << std::endl;
    wdPro.SetMeasurements(-5, 80, 750, 15.0, 90);  // Only wind changed

    std::cout << "\n--- Temperature drops, BOTH displays should be notified ---" << std::endl;
    wdPro.SetMeasurements(-10, 80, 750, 15.0, 90);  // Temp changed

    std::cout << "\n--- Fisherman unsubscribes from Pressure ---" << std::endl;
    wdPro.RemoveObserver(fishermanDisplay, "Pressure");

    std::cout << "\n--- Pressure changes, fisherman's stats display should NOT update stats ---"
              << std::endl;
    wdPro.SetMeasurements(-10, 80, 740, 15.0, 90);  // Pressure changed

    return 0;
}