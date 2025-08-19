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



int main() {
    CWeatherDataPro wdPro("Out (Pro)");
    CWeatherData wdIn("In (Standard)");

    CStatsDisplay statsDisplay;
    wdPro.RegisterObserver(statsDisplay);
    wdIn.RegisterObserver(statsDisplay);

    std::cout << "--- First update from Pro station ---" << std::endl;
    wdPro.SetMeasurements(10, 80, 745, 5.0, 90); // 5 м/с, ветер восточный

    std::cout << "\n--- Update from Standard station ---" << std::endl;
    wdIn.SetMeasurements(22, 60, 760);

    std::cout << "\n--- Second update from Pro station ---" << std::endl;
    wdPro.SetMeasurements(12, 78, 744, 7.0, 135); // 7 м/с, юго-восточный

    return 0;
}