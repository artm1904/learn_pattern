#pragma once

#include <functional>
#include <map>
#include <set>
#include <unordered_map>
#include <vector>

template <typename T>
class IObservable;

/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс,
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/
template <typename T>
class IObserver {
   public:
    virtual void Update(T const& data, IObservable<T>& subject, const std::string& eventType) = 0;
    virtual ~IObserver() = default;
};

/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T>
class IObservable {
   public:
    virtual void RegisterObserver(IObserver<T>& observer, const std::string& eventType, int priority = 0) = 0;
    virtual void NotifyObservers() = 0;
    virtual void RemoveObserver(IObserver<T>& observer, const std::string& eventType) = 0;
    virtual void RemoveObserver(IObserver<T>& observer) = 0;
    virtual std::string GetLocation() const = 0;
    virtual ~IObservable() = default;
};

// Реализация интерфейса IObservable
template <class T>
class CObservable : public IObservable<T> {
   public:
    typedef IObserver<T> ObserverType;

    void RegisterObserver(ObserverType& observer, const std::string& eventType, int priority = 0) override {
        m_subscriptionsByObserver[&observer][eventType] = priority;
        m_observersByEvent[eventType][priority].insert(&observer);
    }

    void NotifyObservers() override {
        T data = GetChangedData();
        auto events = GetChangedEvents();

        using Notification = std::pair<ObserverType*, std::string>;
        std::map<int, std::vector<Notification>, std::greater<int>> notificationsByPriority;

        for (const auto& eventType : events) {
            if (m_observersByEvent.count(eventType)) {
                const auto& observersForEvent = m_observersByEvent.at(eventType);
                for (const auto& [priority, observersSet] : observersForEvent) {
                    for (auto* obs : observersSet) {
                        notificationsByPriority[priority].push_back({obs, eventType});
                    }
                }
            }
        }

        std::vector<Notification> finalNotificationList;
        for (const auto& [priority, notifications] : notificationsByPriority) {
            finalNotificationList.insert(finalNotificationList.end(), notifications.begin(), notifications.end());
        }

        for (const auto& notification : finalNotificationList) {
            auto* observer = notification.first;
            const auto& eventType = notification.second;

            auto it = m_subscriptionsByObserver.find(observer);
            if (it != m_subscriptionsByObserver.end() && it->second.count(eventType)) {
                observer->Update(data, *this, eventType);
            }
        }
    }

    void RemoveObserver(ObserverType& observer, const std::string& eventType) override {
        auto sub_it = m_subscriptionsByObserver.find(&observer);
        if (sub_it == m_subscriptionsByObserver.end()) return;

        auto& events = sub_it->second;
        auto event_it = events.find(eventType);
        if (event_it == events.end()) return;

        int priority = event_it->second;
        events.erase(event_it);
        if (events.empty()) {
            m_subscriptionsByObserver.erase(sub_it);
        }

        auto& observersForPriority = m_observersByEvent.at(eventType).at(priority);
        observersForPriority.erase(&observer);

        if (observersForPriority.empty()) {
            m_observersByEvent.at(eventType).erase(priority);
        }
        if (m_observersByEvent.at(eventType).empty()) {
            m_observersByEvent.erase(eventType);
        }
    }

    void RemoveObserver(ObserverType& observer) override {
        auto it = m_subscriptionsByObserver.find(&observer);
        if (it != m_subscriptionsByObserver.end()) {
            auto eventTypes = it->second;
            for (const auto& [eventType, priority] : eventTypes) {
                RemoveObserver(observer, eventType);
            }
        }
    }

   protected:
    // Классы-наследники должны перегрузить данный метод,
    // в котором возвращать информацию об изменениях в объекте
    virtual T GetChangedData() const = 0;
    virtual std::vector<std::string> GetChangedEvents() const = 0;

   private:
    // event -> priority -> observers
    std::map<std::string, std::map<int, std::set<ObserverType*>, std::greater<int>>> m_observersByEvent;

    // observer -> event -> priority
    std::unordered_map<ObserverType*, std::map<std::string, int>> m_subscriptionsByObserver;
};
