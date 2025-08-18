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
    virtual void Update(T const& data, IObservable<T>& subject) = 0;
    virtual ~IObserver() = default;
};

/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T>
class IObservable {
   public:
    virtual ~IObservable() = default;
    virtual void RegisterObserver(IObserver<T>& observer, int priority = 0) = 0;
    virtual void NotifyObservers() = 0;
    virtual void RemoveObserver(IObserver<T>& observer) = 0;
};

// Реализация интерфейса IObservable
template <class T>
class CObservable : public IObservable<T> {
   public:
    typedef IObserver<T> ObserverType;

    void RegisterObserver(ObserverType& observer, int priority = 0) override {
        // Если наблюдатель уже подписан, ничего не делаем.
        // Проверка за O(1) в среднем.
        if (m_priorityByObserver.find(&observer) == m_priorityByObserver.end()) {
            m_priorityByObserver[&observer] = priority;
            m_observersByPriority[priority].insert(&observer);
        }
    }

    void NotifyObservers() override {
        T data = GetChangedData();

        std::vector<ObserverType*> observersToNotify;

        for (auto const& [priority, observers] : m_observersByPriority) {
            observersToNotify.insert(observersToNotify.end(), observers.begin(), observers.end());
        }

        for (auto* observer : observersToNotify) {
            observer->Update(data, *this);
        }
    }

    void RemoveObserver(ObserverType& observer) override {
        auto it = m_priorityByObserver.find(&observer);
        // Если наблюдатель найден
        if (it != m_priorityByObserver.end()) {
            int priority = it->second;
            m_priorityByObserver.erase(it);

            auto& observersForPriority = m_observersByPriority.at(priority);
            observersForPriority.erase(&observer);

            // Если для данного приоритета не осталось наблюдателей, удаляем и сам приоритет
            if (observersForPriority.empty()) {
                m_observersByPriority.erase(priority);
            }
        }
    }

   protected:
    // Классы-наследники должны перегрузить данный метод,
    // в котором возвращать информацию об изменениях в объекте
    virtual T GetChangedData() const = 0;

   private:
    // Основное хранилище: map<приоритет, set<наблюдатели>>
    // Ключи (приоритеты) отсортированы по убыванию.
    std::map<int, std::set<ObserverType*>, std::greater<int>> m_observersByPriority;

    // Вспомогательное хранилище для быстрого поиска приоритета по наблюдателю.
    std::unordered_map<ObserverType*, int> m_priorityByObserver;
};
