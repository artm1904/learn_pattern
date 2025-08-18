#include "WeatherData.h"

int main() {
    CWeatherData wdIn("In");
    CWeatherData wdOut("Out");

    CDisplay display;
    wdIn.RegisterObserver(display, 10);
    wdOut.RegisterObserver(display, 10);

    wdIn.SetMeasurements(24, 65, 760);
    // ... в консоли появится статистика только для "In"

    wdOut.SetMeasurements(10, 80, 745);
    // ... в консоли появится обновленная статистика и для "In", и для "Out"
    return 0;
}
