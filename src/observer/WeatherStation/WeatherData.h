#pragma once
#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

#include "Observer.h"

struct SWeatherInfo {
    double temperature = 0;
    double humidity = 0;
    double pressure = 0;
};

class CDisplay : public IObserver<SWeatherInfo> {
   private:
    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
            Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
            остается публичным
    */
    void Update(SWeatherInfo const& data, IObservable<SWeatherInfo>& subject) override {
        if (subject.GetLocation() == "In") {
            std::cout << "In sensor data" << std::endl;
        } else if (subject.GetLocation() == "Out") {
            std::cout << "Out sensor data" << std::endl;
        }

        std::cout << "Current Temp " << data.temperature << std::endl;
        std::cout << "Current Hum " << data.humidity << std::endl;
        std::cout << "Current Pressure " << data.pressure << std::endl;
        std::cout << "----------------" << std::endl;
    }
};

// Вспомогательный класс для сбора статистики по одному показателю (температура, влажность и т.д.)
// Это позволяет избежать дублирования кода для каждого типа измерений.
class CStatsIndicator {
   public:
    void Update(double data) {
        if (m_min > data) {
            m_min = data;
        }
        if (m_max < data) {
            m_max = data;
        }
        m_acc += data;
        ++m_count;
    }

    void Print(const std::string& name) const {
        if (m_count > 0) {
            std::cout << "Max " << name << " " << m_max << std::endl;
            std::cout << "Min " << name << " " << m_min << std::endl;
            std::cout << "Average " << name << " " << (m_acc / m_count) << std::endl;
        }
    }

    // Getters for testing
    double GetMin() const { return m_min; }
    double GetMax() const { return m_max; }
    double GetAverage() const { return (m_count == 0) ? 0 : m_acc / m_count; }
    unsigned GetCount() const { return m_count; }

   private:
    double m_min = std::numeric_limits<double>::infinity();
    double m_max = -std::numeric_limits<double>::infinity();
    double m_acc = 0;
    unsigned m_count = 0;
};

// Структура для хранения всех индикаторов для одной локации
struct SLocationStats {
    CStatsIndicator temperature;
    CStatsIndicator humidity;
    CStatsIndicator pressure;
};

class CStatsDisplay : public IObserver<SWeatherInfo> {
   public:
    // Getter for testing
    const SLocationStats* GetStatsFor(const std::string& location) const {
        auto it = m_statsByLocation.find(location);
        if (it != m_statsByLocation.end()) {
            return &it->second;
        }
        return nullptr;
    }

   private:
    void Update(SWeatherInfo const& data, IObservable<SWeatherInfo>& subject) override {
        const std::string location = subject.GetLocation();

        m_statsByLocation[location].temperature.Update(data.temperature);
        m_statsByLocation[location].humidity.Update(data.humidity);
        m_statsByLocation[location].pressure.Update(data.pressure);

        // Выводим всю известную статистику
        for (const auto& [loc, stats] : m_statsByLocation) {
            std::cout << "--- Stats for " << loc << " ---" << std::endl;
            stats.temperature.Print("Temp");
            stats.humidity.Print("Humidity");
            stats.pressure.Print("Pressure");
            std::cout << "----------------" << std::endl;
        }
    }

    std::map<std::string, SLocationStats> m_statsByLocation;
};

class CWeatherData : public CObservable<SWeatherInfo> {
   public:
    CWeatherData() = default;
    CWeatherData(std::string location) : m_location(std::move(location)) {}

    // Температура в градусах Цельсия
    double GetTemperature() const { return m_temperature; }
    // Относительная влажность (0...100)
    double GetHumidity() const { return m_humidity; }
    // Атмосферное давление (в мм.рт.ст)
    double GetPressure() const { return m_pressure; }

    void MeasurementsChanged() { NotifyObservers(); }

    void SetMeasurements(double temp, double humidity, double pressure) {
        m_humidity = humidity;
        m_temperature = temp;
        m_pressure = pressure;

        MeasurementsChanged();
    }

   protected:
    std::string GetLocation() const override { return m_location; }

    SWeatherInfo GetChangedData() const override {
        SWeatherInfo info;
        info.temperature = GetTemperature();
        info.humidity = GetHumidity();
        info.pressure = GetPressure();
        return info;
    }

   private:
    std::string m_location;
    double m_temperature = 0.0;
    double m_humidity = 0.0;
    double m_pressure = 760.0;
};
