#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

#include "Observer.h"

struct SWeatherInfo {
    double temperature = 0;
    double humidity = 0;
    double pressure = 0;
    double windSpeed = 0;
    double windDirection = 0;
};

class CDisplay : public IObserver<SWeatherInfo> {
   public:
    CDisplay(std::string name = "Generic Display") : m_name(std::move(name)) {}

   private:
    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
            Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
            остается публичным
    */
    void Update(SWeatherInfo const& data, IObservable<SWeatherInfo>& subject,
                const std::string& eventType) override {
        std::cout << "--- [" << m_name << "] Event '" << eventType << "' from " << subject.GetLocation() << " ---"
                  << std::endl;
        std::cout << "Current Temp: " << data.temperature << ", Hum: " << data.humidity
                  << ", Press: " << data.pressure << ", Wind: " << data.windSpeed << " m/s at "
                  << data.windDirection << " deg" << std::endl;
        std::cout << "----------------" << std::endl;
    }

    std::string m_name;
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

// Специализированный класс для сбора статистики по направлению ветра.
// Использует векторную алгебру для корректного усреднения углов.
class CWindDirectionStatsIndicator {
   public:
    void Update(double directionInDegrees) {
        constexpr double PI = 3.1415926535;
        double directionInRadians = directionInDegrees * PI / 180.0;
        m_x_sum += std::cos(directionInRadians);
        m_y_sum += std::sin(directionInRadians);
        m_count++;
    }

    void Print(const std::string& name) const {
        if (m_count > 0) {
            constexpr double PI = 3.1415926535;
            double avg_rad = std::atan2(m_y_sum, m_x_sum);
            double avg_deg = avg_rad * 180.0 / PI;
            if (avg_deg < 0) {
                avg_deg += 360.0;
            }
            std::cout << name << " " << avg_deg << " degrees" << std::endl;
        }
    }

   private:
    double m_x_sum = 0.0;
    double m_y_sum = 0.0;
    unsigned m_count = 0;
};

// Структура для хранения всех индикаторов для одной локации
struct SLocationStats {
    CStatsIndicator temperature;
    CStatsIndicator humidity;
    CStatsIndicator pressure;
    CStatsIndicator windSpeed;
    CWindDirectionStatsIndicator windDirection;
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
    void Update(SWeatherInfo const& data, IObservable<SWeatherInfo>& subject,
                const std::string& eventType) override {
        const std::string location = subject.GetLocation();

        if (eventType == "Temperature") {
            m_statsByLocation[location].temperature.Update(data.temperature);
        } else if (eventType == "Humidity") {
            m_statsByLocation[location].humidity.Update(data.humidity);
        } else if (eventType == "Pressure") {
            m_statsByLocation[location].pressure.Update(data.pressure);
        } else if (eventType == "Wind") {
            m_statsByLocation[location].windSpeed.Update(data.windSpeed);
            m_statsByLocation[location].windDirection.Update(data.windDirection);
        }

        // Выводим статистику только для той локации, от которой пришло уведомление
        const auto& stats = m_statsByLocation.at(location);
        std::cout << "--- Stats for " << location << " ---" << std::endl;
        stats.temperature.Print("Temp");
        stats.humidity.Print("Humidity");
        stats.pressure.Print("Pressure");
        stats.windSpeed.Print("Wind Speed");
        stats.windDirection.Print("Avg Wind Dir");
        std::cout << "----------------" << std::endl;
    }

    std::map<std::string, SLocationStats> m_statsByLocation;
};

class CWeatherData : public CObservable<SWeatherInfo> {
   public:
    CWeatherData(std::string location) : m_location(std::move(location)) {}

    // Температура в градусах Цельсия
    double GetTemperature() const { return m_temperature; }
    // Относительная влажность (0...100)
    double GetHumidity() const { return m_humidity; }
    // Атмосферное давление (в мм.рт.ст)
    double GetPressure() const { return m_pressure; }

    void MeasurementsChanged() { NotifyObservers(); }

    void SetMeasurements(double temp, double humidity, double pressure) {
        m_changedEvents.clear();
        if (m_temperature != temp) {
            m_changedEvents.push_back("Temperature");
        }
        if (m_humidity != humidity) {
            m_changedEvents.push_back("Humidity");
        }
        if (m_pressure != pressure) {
            m_changedEvents.push_back("Pressure");
        }

        m_humidity = humidity;
        m_temperature = temp;
        m_pressure = pressure;

        if (!m_changedEvents.empty()) {
            MeasurementsChanged();
        }
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

    std::vector<std::string> GetChangedEvents() const override { return m_changedEvents; }

    std::vector<std::string> m_changedEvents;
    double m_temperature = 0.0;
    double m_humidity = 0.0;
    double m_pressure = 760.0;

   private:
    std::string m_location;
};

class CWeatherDataPro : public CWeatherData {
   public:
    CWeatherDataPro(std::string location) : CWeatherData(std::move(location)) {}

    double GetWindSpeed() const { return m_windSpeed; }
    double GetWindDirection() const { return m_windDirection; }

    void SetMeasurements(double temp, double humidity, double pressure, double windSpeed,
                         double windDirection) {
        m_changedEvents.clear();

        if (m_temperature != temp) m_changedEvents.push_back("Temperature");
        if (m_humidity != humidity) m_changedEvents.push_back("Humidity");
        if (m_pressure != pressure) m_changedEvents.push_back("Pressure");
        if (m_windSpeed != windSpeed || m_windDirection != windDirection)
            m_changedEvents.push_back("Wind");

        m_temperature = temp;
        m_humidity = humidity;
        m_pressure = pressure;
        m_windSpeed = windSpeed;
        m_windDirection = windDirection;

        if (!m_changedEvents.empty()) {
            MeasurementsChanged();
        }
    }

   protected:
    SWeatherInfo GetChangedData() const override {
        // Получаем базовую информацию
        SWeatherInfo info = CWeatherData::GetChangedData();
        // Дополняем ее данными о ветре
        info.windSpeed = GetWindSpeed();
        info.windDirection = GetWindDirection();
        return info;
    }

   private:
    double m_windSpeed = 0;
    double m_windDirection = 0;
};
