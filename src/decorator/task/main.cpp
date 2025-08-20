#include "Beverages.h"
#include "Condiments.h"

#include <iostream>
#include <string>
#include <functional>

using namespace std;


/*
Возвращает функцию, декорирующую напиток определенной добавкой

Параметры шаблона: 
	Condiment - класс добавки, конструктор которого в качестве первого аргумента
				принимает IBeveragePtr&& оборачиваемого напитка
	Args - список типов прочих параметров конструктора (возможно, пустой)
*/
template <typename Condiment, typename... Args>
auto MakeCondiment(const Args&...args)
{
	// Возвращаем функцию, декорирующую напиток, переданный ей в качестве аргумента
	// Дополнительные аргументы декоратора, захваченные лямбда-функцией, передаются
	// конструктору декоратора через make_unique
	return [=](auto && b) {
		// Функции make_unique передаем b вместе со списком аргументов внешней функции
		return make_unique<Condiment>(forward<decltype(b)>(b), args...);
	};
}

/*
Перегруженная версия оператора <<, которая предоставляет нам синтаксический сахар
для декорирования компонента

Позволяет создать цепочку оборачивающих напиток декораторов следующим образом:
auto beverage = make_unique<CConcreteBeverage>(a, b, c)
					<< MakeCondimentA(d, e, f)
					<< MakeCondimentB(g, h);
*/
template <typename Component, typename Decorator>
auto operator << (Component && component, const Decorator & decorate)
{
	return decorate(forward<Component>(component));
}

void DialogWithUser()
{
	cout << "Type 1 for coffee or 2 for tea\n";
	int beverageChoice;
	cin >> beverageChoice;

	unique_ptr<IBeverage> beverage;

	if (beverageChoice == 1)
	{
		beverage = make_unique<CCoffee>();
	}
	else if (beverageChoice == 2)
	{
		beverage = make_unique<CTea>();
	}
	else
	{
		return;
	}

	int condimentChoice;
	for (;;)
	{
		cout << "1 - Lemon, 2 - Cinnamon, 0 - Checkout" << endl;
		cin >> condimentChoice;

		if (condimentChoice == 1)
		{
			//beverage = make_unique<CLemon>(move(beverage));
			beverage = move(beverage) << MakeCondiment<CLemon>(2);
		}
		else if (condimentChoice == 2)
		{
			//beverage = make_unique<CCinnamon>(move(beverage));
			beverage = move(beverage) << MakeCondiment<CCinnamon>();
		}
		else if (condimentChoice == 0)
		{
			break;
		}
		else
		{
			return;
		}
	}



	cout << beverage->GetDescription() << ", cost: " << beverage->GetCost() << endl;
}


int main()
{
	DialogWithUser();
	cout << endl;
	{
		// Наливаем чашечку латте
		auto latte = make_unique<CLatte>();
		// добавляем корицы
		auto cinnamon = make_unique<CCinnamon>(move(latte));
		// добавляем пару долек лимона
		auto lemon = make_unique<CLemon>(move(cinnamon), 2);
		// добавляем пару кубиков льда
		auto iceCubes = make_unique<CIceCubes>(move(lemon), 2, IceCubeType::Dry);
		// добавляем 2 грамма шоколадной крошки
		auto beverage = make_unique<CChocolateCrumbs>(move(iceCubes), 2);

		// Выписываем счет покупателю
		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}



	// Подробнее рассмотрим работу MakeCondiment и оператора <<
	{
		// lemon - функция, добавляющая "2 дольки лимона" к любому напитку
		auto lemon2 = MakeCondiment<CLemon>(2);
		// iceCubes - функция, добавляющая "3 кусочка льда" к любому напитку
		auto iceCubes3 = MakeCondiment<CIceCubes>(3, IceCubeType::Water);
		
		auto tea = make_unique<CTea>();

		// декорируем чай двумя дольками лимона и тремя кусочками льда
		auto lemonIceTea = iceCubes3(lemon2(move(tea)));	
	

		auto oneMoreLemonIceTea =
			make_unique<CTea>()	// Берем чай
			<< MakeCondiment<CLemon>(2)	// добавляем пару долек лимона
			<< MakeCondiment<CIceCubes>(3, IceCubeType::Water); // и 3 кубика льда
	
	}

}
