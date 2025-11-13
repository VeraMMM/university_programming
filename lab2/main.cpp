#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <limits>
#include <memory> 


using namespace std;

bool isInt(int x) {

    if (cin.fail()) {
        cin.clear();
        cin.ignore(1000, '\n');
        return false;
    }

    if (cin.peek() != '\n') {
        cin.ignore(1000, '\n');
        return false;
    }

    return true;
}
bool isDouble(double x) {

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    if (cin.peek() != '\n') {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    return true;
}


class Contribution
{
private:
    double size;
    string currency;
    double rate_p;//проверка что не больше 100
    //double rate = (size * rate_p) / 100;


public:

    Contribution() : size(0), currency("RUB"), rate_p(0) {}

    void setSize(double size_b) { size = size_b; }//добавить деняг

    void setRate_p(double ratep_b) {
        if (ratep_b > 100) {
            cout << "Warning: Rate cannot exceed 100. Value capped at 100." << endl;
            rate_p = 100;
        }
        else {
            rate_p = ratep_b;
        }
    }
    double getRate() const {
        return (size * rate_p) / 100;
    }
    void displayContribution() const {
        cout << "Deposit: " << size << " " << currency << endl;
        cout << "Rate amount: " << getRate() << " " << currency << endl;
    }



};

class Person
{
private:
    string name;
    int age;
    Contribution contribution1;
    
    
public:
    
    string getName() const { return name; }
    int getAge() const { return age; }

    Person(const string& personName,const int& personAge) : name(personName),age(personAge) {}

    Contribution& getContribution() { return contribution1; }

    void displayPerson() const { 
        cout << "Person: " << name << endl;
        cout << "Age: " << age << endl;
        contribution1.displayContribution();
    }

};



class Menu {
private:
    Menu() {
        // Биндинг команд на enum значения
        commands[Command::CREATE] = [this]() { createPerson(); };
        commands[Command::CHANGESIZE] = [this]() { changeDepositSize(); };
        commands[Command::SUMM] = [this]() { calculateTotalInterest(); };
        commands[Command::INFO] = [this]() { showAllInfo(); };
        commands[Command::EXIT] = [this]() { exitProgram(); };
    }
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;

    int choiceIndex;
    enum class Command {
        CREATE = 1,
        CHANGESIZE = 2,
        SUMM = 3,
        INFO = 4,
        EXIT = 0
    };
    map<Command, function<void()>> commands;
    vector<unique_ptr<Person>> persons;
    
public:
    static Menu& getInstance() {
        static Menu instance;  
        return instance;
    }

    void showMenu() {
        cout << "\n=== Menu ===\n";
        cout << static_cast<int>(Command::CREATE) << " Добавить пользователя\n";
        cout << static_cast<int>(Command::CHANGESIZE) << " Изменить сумму вклада\n";
        cout << static_cast<int>(Command::SUMM) << " Сумма всех процентов вкладов в банке\n";
        cout << static_cast<int>(Command::INFO) << " Информация про юзеров\n";
        cout << static_cast<int>(Command::EXIT) << " Выход\n";
        
    }
    void showPersonList() {
        cout << "\nAvailable persons:" << endl;
        for (size_t i = 0; i < persons.size(); ++i) {
            cout << (i + 1) << ". " << persons[i]->getName()
                << " (Age: " << persons[i]->getAge() << ")" << endl;
        }
    }
    
    void createPerson() {
        try {
            string name23;
            int age23;
            
            while (true) {
                cout << "Введите имя:";
                //cin.ignore();
                cin >> name23;
                
                if (!name23.empty() && name23.length() < 15) {
                    break;
                }
                else {
                    cout << "Try again" << endl;
                }
            }
            while (true) {
                cout << "Введите возраст:";
                cin >> age23;
                if (isInt(age23) && (age23 < 100 && age23 > 0) ) {
                    break;
                }
                else {
                    cout << "Try again" << endl;
                }
            }





            auto person = make_unique<Person>(name23, age23);

            // Настраиваем вклад
            double deposit23, rate23;
            while (true) {
                cout << "Введите размер вкладов:";
                cin >> deposit23;
                if (isDouble(deposit23) && deposit23 > 0) {
                    break;
                }
                else {
                    cout << "Try again" << endl;
                }

            }
            while (true) {
                cout << "Введите размерную ставку:";
                cin >> rate23;
                if (isDouble(rate23) && rate23 > 0 && rate23 <= 100) {
                    break;
                }
                else {
                    cout << "Try again" << endl;
                }

            }

            person->getContribution().setSize(deposit23);
            person->getContribution().setRate_p(rate23);

            persons.push_back(move(person));
            cout << "Person created successfully!" << endl;

        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void changeDepositSize() {

        if (persons.empty()) {
            cout << "Для начала надо кого-то создать, бож" << endl;
            return;
        }
        showPersonList();
        //cout << "Select person (1-" << persons.size() << "): ";
        int index;
        while (true) {
            cout << "Выберете чяловека (1-" << persons.size() << "): ";
            cin >> index;
            if (isInt(index) && index <= persons.size()) {
                break;
            }
            else {
                cout << "Try again" << endl;
            }
        }

        double newSize;
        while (true) {
            cout << "Введите новый размер вклада:";
            cin >> newSize;
            if (isDouble(newSize) && newSize > 0) {
                break;
            }
            else {
                cout << "Try again" << endl;
            }

        }

        persons[index - 1]->getContribution().setSize(newSize);
        cout << "Всё, денежки упали!" << endl;


    };

    void calculateTotalInterest() {
        if (persons.empty()) {
            cout << "No persons available." << endl;
            return;
        }

        double totalInterest = 0;
        for (const auto& person : persons) {
            totalInterest += person->getContribution().getRate();
        }

        cout << "Всего деняг: " << totalInterest << " RUB" << endl;
    }
    void showAllInfo() {
        if (persons.empty()) {
            cout << "No persons available." << endl;
            return;
        }

        cout << "\n=== All Persons ===" << endl;
        for (size_t i = 0; i < persons.size(); ++i) {
            cout << "Person " << (i + 1) << ":" << endl;
            persons[i]->displayPerson();
            cout << "-------------------" << endl;
        }
    }
    void exitProgram() {
        cout << "Goodbye!" << endl;
        exit(0);
    }
    void executeCommand(int choice) {
        Command cmd = static_cast<Command>(choice);
        auto it = commands.find(cmd);
        if (it != commands.end()) {
            it->second();
        }
        else {
            cout << "Invalid command!" << endl;
        }
    }
    void run() {
        int choice;
        while (true) {
            showMenu();
            while (true) {
                cout << "Your choice: ";
                cin >> choice;
                if (isInt(choice) && choice > -1 && choice < 5) {
                    break;
                }
                else {
                    cout << "try again" << endl;
                }
            }
            
            executeCommand(choice);
        }
    }
};







int main()
{
    
    setlocale(LC_ALL, "RU");
    //locale::global(std::locale(""));
    Menu::getInstance().run();
    return 0;
}
