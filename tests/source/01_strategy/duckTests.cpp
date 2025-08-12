#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "lib/Duck/Dance/IDanceBehavior.h"
#include "lib/Duck/Duck.h"
#include "lib/Duck/Fly/FlyNoWay.h"
#include "lib/Duck/Fly/IFlyBehavior.h"
#include "lib/Duck/MallardDuck.h"
#include "lib/Duck/Quack/IQuakBehavior.h"
#include "lib/Duck/Quack/MuteQuackBehavior.h"

// Mock-реализация для IDanceBehavior с использованием GMock
class MockDanceBehavior : public IDanceBehavior {
   public:
    MOCK_METHOD(void, Dance, (), (const, override));
};

// Специальный класс утки для тестов, позволяющий внедрять mock-объекты
// через конструктор для проверки инициализации.
class TestDuck : public Duck {
   public:
    TestDuck(std::unique_ptr<IFlyBehavior>&& fly, std::unique_ptr<IQuackBehavior>&& quack,
             std::unique_ptr<IDanceBehavior>&& dance)
        : Duck(std::move(fly), std::move(quack), std::move(dance)) {}

    void Display() const override { /* Для тестов реализация не важна */ }
};

TEST(DuckDanceTest, DanceIsDelegatedToBehaviorFromConstructor) {
    auto mockBehavior = std::make_unique<testing::StrictMock<MockDanceBehavior>>();
    EXPECT_CALL(*mockBehavior, Dance()).Times(1);

    TestDuck duck(std::make_unique<FlyNoWay>(), std::make_unique<MuteQuackBehavior>(),
                  std::move(mockBehavior));

    duck.Dance();
}

TEST(DuckDanceTest, SetDanceBehaviorChangesBehavior) {
    // 1. Создаем реальный объект, который хотим протестировать.
    MallardDuck duck;  // Изначально танцует вальс

    // 2. Создаем наш "симулятор" - mock-объект.
    // Он реализует интерфейс IDanceBehavior, как и настоящие стратегии.
    auto mockBehavior = std::make_unique<testing::StrictMock<MockDanceBehavior>>();

    // 3. Программирование мока (Expectation)
    // Мы говорим: "Я ожидаю, что у этого mock-объекта будет вызван метод Dance()
    // ровно 1 раз. Если этого не произойдет, тест должен провалиться".
    EXPECT_CALL(*mockBehavior, Dance()).Times(1);

    // 3. Действие (Act)
    // Внедряем наш мок в утку, заменяя ее реальное поведение.
    duck.SetDanceBehavior(std::move(mockBehavior));

    // Вызываем тестируемый метод. Мы ожидаем, что внутри этого вызова
    // Duck делегирует работу своей стратегии, т.е. вызовет Dance() у нашего мока.
    duck.Dance();

    // 4. Проверка (Assert)
    // В случае с GMock, проверка происходит автоматически в конце теста.
    // Фреймворк сам убедится, что все ожидания, заданные через EXPECT_CALL, были выполнены.
    // Он проверяет контракт класса Duck: "При вызове Dance() утка должна вызвать метод Dance() у
    // своей текущей стратегии танца"
}