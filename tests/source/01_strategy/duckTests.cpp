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

class MockFlyBehavior : public IFlyBehavior {
   public:
    MOCK_METHOD(void, Fly, (), (const, override));
    MOCK_METHOD(bool, IsFlying, (), (const, override));
};

class MockQuackBehavior : public IQuackBehavior {
   public:
    MOCK_METHOD(void, Quack, (), (const, override));
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

TEST(DuckFlyAndQuackInteractionTest, FlyingDuckDoesNotQuackOnFirstFlight) {
    // Arrange: Создаем утку с mock-поведениями
    auto mockFly = std::make_unique<testing::StrictMock<MockFlyBehavior>>();
    auto mockQuack = std::make_unique<testing::StrictMock<MockQuackBehavior>>();
    auto* rawMockFly = mockFly.get();

    TestDuck duck(std::move(mockFly), std::move(mockQuack), std::make_unique<MockDanceBehavior>());

    // Expect: Ожидаем один вызов Fly() и что утка умеет летать.
    // Quack() не должен быть вызван (StrictMock провалит тест, если это произойдет).
    EXPECT_CALL(*rawMockFly, Fly()).Times(1);
    EXPECT_CALL(*rawMockFly, IsFlying()).WillOnce(testing::Return(true));

    // Act
    duck.Fly();
}

TEST(DuckFlyAndQuackInteractionTest, FlyingDuckQuacksOnSecondFlight) {
    auto mockFly = std::make_unique<testing::StrictMock<MockFlyBehavior>>();
    auto mockQuack = std::make_unique<testing::StrictMock<MockQuackBehavior>>();
    auto* rawMockFly = mockFly.get();
    auto* rawMockQuack = mockQuack.get();

    TestDuck duck(std::move(mockFly), std::move(mockQuack), std::make_unique<MockDanceBehavior>());

    // Expect: Ожидаем, что Fly() вызовется дважды, и оба раза IsFlying() вернет true.
    // Quack() должен быть вызван ровно один раз.
    EXPECT_CALL(*rawMockFly, Fly()).Times(2);
    EXPECT_CALL(*rawMockFly, IsFlying()).WillRepeatedly(testing::Return(true));
    EXPECT_CALL(*rawMockQuack, Quack()).Times(1);

    // Act
    duck.Fly(); // Полет #1
    duck.Fly(); // Полет #2, здесь должен быть вызван Quack()
}

TEST(DuckFlyAndQuackInteractionTest, NonFlyingDuckNeverQuacksAfterFlying) {
    auto mockFly = std::make_unique<testing::StrictMock<MockFlyBehavior>>();
    auto mockQuack = std::make_unique<testing::StrictMock<MockQuackBehavior>>();
    auto* rawMockFly = mockFly.get();

    TestDuck duck(std::move(mockFly), std::move(mockQuack), std::make_unique<MockDanceBehavior>());

    // Expect: Fly() вызывается, но IsFlying() возвращает false.
    // Quack() не должен быть вызван ни разу.
    EXPECT_CALL(*rawMockFly, Fly()).Times(2);
    EXPECT_CALL(*rawMockFly, IsFlying()).WillRepeatedly(testing::Return(false));

    // Act
    duck.Fly();
    duck.Fly();
}

TEST(DuckFlyAndQuackInteractionTest, FlightCounterResetsWhenFlyBehaviorIsSet) {
    auto mockQuack = std::make_unique<testing::StrictMock<MockQuackBehavior>>();
    auto* rawMockQuack = mockQuack.get();
    TestDuck duck(std::make_unique<FlyNoWay>(), std::move(mockQuack), std::make_unique<MockDanceBehavior>());

    // Act 1: Устанавливаем первое летающее поведение
    auto mockFly1 = std::make_unique<testing::StrictMock<MockFlyBehavior>>();
    EXPECT_CALL(*mockFly1, Fly());
    EXPECT_CALL(*mockFly1, IsFlying()).WillOnce(testing::Return(true));
    duck.SetFlyBehavior(std::move(mockFly1));
    duck.Fly(); // Первый полет, счетчик = 1. Не крякает.

    // Act 2: Устанавливаем второе летающее поведение, счетчик должен сброситься
    auto mockFly2 = std::make_unique<testing::StrictMock<MockFlyBehavior>>();
    EXPECT_CALL(*mockFly2, Fly()).Times(2);
    EXPECT_CALL(*mockFly2, IsFlying()).WillRepeatedly(testing::Return(true));
    duck.SetFlyBehavior(std::move(mockFly2));

    // Expect: После второго полета с НОВЫМ поведением утка должна крякнуть.
    EXPECT_CALL(*rawMockQuack, Quack()).Times(1);

    duck.Fly(); // Первый полет с новым поведением, счетчик = 1. Не крякает.
    duck.Fly(); // Второй полет с новым поведением, счетчик = 2. Крякает!
}
