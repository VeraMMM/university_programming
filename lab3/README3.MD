# Лабораторная работа №3
## Банковская система с наследованием и стратегиями бонусов

### Обзор системы

Реализована банковская система, демонстрирующая ключевые принципы объектно-ориентированного программирования:
- **Наследование** - иерархия классов Person → Depositor
- **Полиморфизм** - виртуальные методы interact() и display1()
- **Шаблон Strategy** - система бонусов в зависимости от пола клиента
- **Динамическое приведение типов** - работа с разными типами объектов

### Ключевые компоненты

#### 1. Стратегия бонусов (Strategy Pattern)

```cpp
class BonusStrategy {
public:
    virtual double getOpeningBonus() const = 0;
    virtual double getDepositBonus() const = 0;
    virtual ~BonusStrategy() = default;
};

class MaleBonusStrategy : public BonusStrategy {
public:
    double getOpeningBonus() const override { return -420000; }  // Штраф
    double getDepositBonus() const override { return -15000; }   // Штраф
};

class FemaleBonusStrategy : public BonusStrategy {
public:
    double getOpeningBonus() const override { return 420000; }   // Бонус
    double getDepositBonus() const override { return 15000; }    // Бонус
};

#### 2. Базовый класс
class Person {
protected:
    string name;
    int age;
    string gender;
public:
    virtual bool interact();        // Виртуальный метод
    virtual void display1() const;  // Виртуальный метод
    virtual ~Person() = default;    // Виртуальный деструктор
};
#### 3. Производный класс 
class Depositor : public Person {
private:
    Contribution contribution;
    bool hasInteracted;
public:
    bool interact() override;       // Переопределение метода
    void display1() const override; // Переопределение метода
    void makeDeposit(double size);
};
#### 4. Полиморфизм в действии 
// В классе Bank - полиморфная коллекция
vector<unique_ptr<Person>> people;

// Полиморфные вызовы
for (const auto& person : people) {
    person->interact();    // Вызовется правильная версия метода
    person->display1();    // в зависимости от фактического типа объекта
}
#### 5. Динамическое определение типов
// Проверка типа объекта во время выполнения
Depositor* depositor = dynamic_cast<Depositor*>(people[index].get());
if (depositor) {
    // Это вкладчик - можно работать с вкладом
    depositor->makeDeposit(amount);
} else {
    // Это обычный человек
    cout << "Этот человек еще не вкладчик!" << endl;
}
