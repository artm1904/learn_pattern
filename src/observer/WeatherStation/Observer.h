#pragma once

#include <set>
#include <functional>


template <typename T>
class IObservable;


/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс, 
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/
template <typename T>
class IObserver
{
public:
	virtual void Update(T const& data, IObservable<T> &subject) = 0;
	virtual ~IObserver() = default;
};

/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void RegisterObserver(IObserver<T> & observer) = 0;
	virtual void NotifyObservers() = 0;
	virtual void RemoveObserver(IObserver<T> & observer) = 0;
};

// Реализация интерфейса IObservable
template <class T>
class CObservable : public IObservable<T>
{
public:
	typedef IObserver<T> ObserverType;

	void RegisterObserver(ObserverType & observer) override
	{
		m_observers.insert(&observer);
	}

	void NotifyObservers() override
	{
		T data = GetChangedData();
		// Создаем копию списка наблюдателей, чтобы избежать проблем с инвалидацией итераторов,
		// если один из наблюдателей удалит себя из списка внутри метода Update.
		auto observersCopy = m_observers;
		for (auto & observer : observersCopy)
		{
			// Передаем не только данные, но и ссылку на себя (this),
			// чтобы наблюдатель при необходимости мог отписаться.
			observer->Update(data, *this);
		}
	}

	void RemoveObserver(ObserverType & observer) override
	{
		m_observers.erase(&observer);
	}

protected:
	// Классы-наследники должны перегрузить данный метод, 
	// в котором возвращать информацию об изменениях в объекте
	virtual T GetChangedData()const = 0;

private:
	std::set<ObserverType *> m_observers;
};
