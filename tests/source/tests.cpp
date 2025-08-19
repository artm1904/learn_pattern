// #include <gmock/gmock.h>
// #include <gtest/gtest.h>

// #include "observer/WeatherStation/WeatherData.h"

// // Тестовый наблюдатель, который отписывается от уведомлений
// // прямо во время вызова своего метода Update.
// class SelfRemovingObserver : public IObserver<SWeatherInfo> {
//    public:
//     int m_updateCount = 0;

//     void Update(const SWeatherInfo& data, IObservable<SWeatherInfo>& subject) override {
//         m_updateCount++;
//         subject.RemoveObserver(*this);
//     }
// };

// // Обычный наблюдатель продолжать получать уведомления.
// class ControlObserver : public IObserver<SWeatherInfo> {
//    public:
//     int m_updateCount = 0;

//     void Update(const SWeatherInfo& data, IObservable<SWeatherInfo>& subject) override {
//         m_updateCount++;
//     }
// };

// TEST(WeatherStation, ObserverCanSafelyRemoveItselfDuringUpdate) {
//     CWeatherData wd("TestStation");
//     SelfRemovingObserver selfRemover;
//     ControlObserver control;

//     wd.RegisterObserver(selfRemover);
//     wd.RegisterObserver(control);

//     wd.SetMeasurements(10, 20, 30);

//     // Assert: Проверяем, что оба наблюдателя были уведомлены по одному разу.
//     ASSERT_EQ(selfRemover.m_updateCount, 1);
//     ASSERT_EQ(control.m_updateCount, 1);

//     wd.SetMeasurements(15, 25, 35);

//     // Assert: Проверяем, что самоудалившийся наблюдатель больше не получал уведомлений,
//     // а контрольный наблюдатель получил второе уведомление.
//     ASSERT_EQ(selfRemover.m_updateCount, 1);
//     ASSERT_EQ(control.m_updateCount, 2);
// }

// TEST(WeatherStation, NotifiesObserversAccordingToPriority) {
//     CWeatherData wd("TestStation");
//     std::vector<int> notificationOrder;

//     // Тестовый наблюдатель, который записывает свой ID в общий вектор при вызове.
//     class PriorityTestObserver : public IObserver<SWeatherInfo> {
//        public:
//         PriorityTestObserver(int id, std::vector<int>& order)
//             : m_id(id), m_notificationOrder(order) {}
//         void Update(const SWeatherInfo&, IObservable<SWeatherInfo>&) override {
//             m_notificationOrder.push_back(m_id);
//         }

//        private:
//         int m_id;
//         std::vector<int>& m_notificationOrder;
//     };

//     PriorityTestObserver obsHigh(4, notificationOrder);  // Приоритет: 100
//     PriorityTestObserver obsLow(1, notificationOrder);   // Приоритет: -10
//     PriorityTestObserver obsDef1(2, notificationOrder);  // Приоритет: 0
//     PriorityTestObserver obsDef2(3, notificationOrder);  // Приоритет: 0

//     // Регистрируем наблюдателей в произвольном порядке
//     wd.RegisterObserver(obsDef1, 0);
//     wd.RegisterObserver(obsHigh, 100);
//     wd.RegisterObserver(obsLow, -10);
//     wd.RegisterObserver(obsDef2, 0);

//     wd.SetMeasurements(1, 2, 3);

//     // Assert
//     // Ожидаемый порядок: obsHigh(4) -> obsDef1(2)/obsDef2(3) -> obsLow(1).
//     // Порядок между наблюдателями с одинаковым приоритетом не гарантирован.
//     ASSERT_EQ(notificationOrder.size(), 4);
//     ASSERT_EQ(notificationOrder.front(), 4);  // Высший приоритет (100) вызывается первым
//     ASSERT_EQ(notificationOrder.back(), 1);   // Низший приоритет (-10) вызывается последним
//     // Проверяем, что наблюдатели с приоритетом 0 находятся посередине, игнорируя их взаимный
//     // порядок
//     ASSERT_TRUE((notificationOrder[1] == 2 && notificationOrder[2] == 3) ||
//                 (notificationOrder[1] == 3 && notificationOrder[2] == 2));
// }

// TEST(WeatherStationDuo, TracksStatsSeparatelyForMultipleSubjects) {
   
//     CWeatherData wdIn("In");
//     CWeatherData wdOut("Out");
//     CStatsDisplay statsDisplay;

//     wdIn.RegisterObserver(statsDisplay);
//     wdOut.RegisterObserver(statsDisplay);

//     // Act: First measurement from "In"
//     wdIn.SetMeasurements(10, 80, 760);

    
//     const SLocationStats* inStats = statsDisplay.GetStatsFor("In");
//     const SLocationStats* outStats = statsDisplay.GetStatsFor("Out");

//     ASSERT_NE(inStats, nullptr);
//     EXPECT_EQ(inStats->temperature.GetCount(), 1);
//     EXPECT_EQ(inStats->temperature.GetMax(), 10);
//     EXPECT_EQ(inStats->humidity.GetMax(), 80);
//     EXPECT_EQ(inStats->pressure.GetMax(), 760);
//     ASSERT_EQ(outStats, nullptr); 

//     // Act: First measurement from "Out"
//     wdOut.SetMeasurements(5, 90, 755);

//     // Assert: Check "Out" stats, "In" stats should be unchanged
//     inStats = statsDisplay.GetStatsFor("In");
//     outStats = statsDisplay.GetStatsFor("Out");

//     ASSERT_NE(inStats, nullptr);
//     EXPECT_EQ(inStats->temperature.GetCount(), 1);  
//     ASSERT_NE(outStats, nullptr);
//     EXPECT_EQ(outStats->temperature.GetCount(), 1);
//     EXPECT_EQ(outStats->temperature.GetMax(), 5);
//     EXPECT_EQ(outStats->humidity.GetMax(), 90);
//     EXPECT_EQ(outStats->pressure.GetMax(), 755);


//     wdIn.SetMeasurements(20, 70, 765);


//     inStats = statsDisplay.GetStatsFor("In");
//     outStats = statsDisplay.GetStatsFor("Out");

//     ASSERT_NE(inStats, nullptr);
//     EXPECT_EQ(inStats->temperature.GetCount(), 2);
//     EXPECT_EQ(inStats->temperature.GetMin(), 10);
//     EXPECT_EQ(inStats->temperature.GetMax(), 20);
//     EXPECT_NEAR(inStats->temperature.GetAverage(), 15.0, 0.001);

//     ASSERT_NE(outStats, nullptr);
//     EXPECT_EQ(outStats->temperature.GetCount(), 1);  
// }
