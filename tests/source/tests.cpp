#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "observer/WeatherStation/WeatherData.h"

// Тестовый наблюдатель, который отписывается от уведомлений
// прямо во время вызова своего метода Update.
class SelfRemovingObserver : public IObserver<SWeatherInfo>
{
public:
	int m_updateCount = 0;

	void Update(const SWeatherInfo& data, IObservable<SWeatherInfo>& subject) override
	{
		m_updateCount++;
		subject.RemoveObserver(*this);
	}
};

// Обычный наблюдатель продолжать получать уведомления.
class ControlObserver : public IObserver<SWeatherInfo>
{
public:
	int m_updateCount = 0;

	void Update(const SWeatherInfo& data, IObservable<SWeatherInfo>& subject) override
	{
		m_updateCount++;
	}
};

TEST(WeatherStation, ObserverCanSafelyRemoveItselfDuringUpdate)
{
	CWeatherData wd;
	SelfRemovingObserver selfRemover;
	ControlObserver control;

	wd.RegisterObserver(selfRemover);
	wd.RegisterObserver(control);

	

	wd.SetMeasurements(10, 20, 30);

	// Assert: Проверяем, что оба наблюдателя были уведомлены по одному разу.
	ASSERT_EQ(selfRemover.m_updateCount, 1);
	ASSERT_EQ(control.m_updateCount, 1);


	wd.SetMeasurements(15, 25, 35);

	// Assert: Проверяем, что самоудалившийся наблюдатель больше не получал уведомлений,
	// а контрольный наблюдатель получил второе уведомление.
	ASSERT_EQ(selfRemover.m_updateCount, 1);
	ASSERT_EQ(control.m_updateCount, 2);
}
