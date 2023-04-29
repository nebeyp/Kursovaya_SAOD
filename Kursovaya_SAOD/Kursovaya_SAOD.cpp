// Kursovaya_SAOD.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <regex>
#include <sstream>

using namespace std;

bool validateDate(string dateStr)
{
    // Проверяем, соответствует ли строка формату XX.XX.23
    regex dateRegex("^\\d{2}\\.\\d{2}\\.23$");
    if (!regex_match(dateStr, dateRegex))
    {
        return false;
    }

    // Извлекаем день и месяц из строки
    int day = std::stoi(dateStr.substr(0, 2));
    int month = std::stoi(dateStr.substr(3, 2));

    // Проверяем, что месяц и день находятся в допустимых пределах
    if (month < 1 || month > 12)
    {
        return false;
    }
    if (day < 1 || day > 31)
    {
        return false;
    }

    // Проверяем, что день не превышает максимальное значение для данного месяца
    int maxDay;
    switch (month)
    {
    case 2:
        maxDay = 28; // В феврале 28 дней (без учета високосных годов)
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        maxDay = 30; // В апреле, июне, сентябре и ноябре 30 дней
        break;
    default:
        maxDay = 31; // В остальных месяцах 31 день
        break;
    }
    if (day > maxDay)
    {
        return false;
    }

    // Если мы добрались до сюда, значит дата прошла все проверки
    return true;
}

string pad_with_zeroes(int number) {
    // преобразуем число в строку
    string number_str = to_string(number);
    // добавляем нули в начало строки до нужной длины
    number_str = string(6 - number_str.length(), '0') + number_str;
    return number_str;
}

bool check_time_format(string time_str) {
    // Проверяем, что строка содержит ровно 5 символов
    if (time_str.length() != 5) {
        return false;
    }

    // Проверяем, что первые два символа - это число от 00 до 23
    string hours_str = time_str.substr(0, 2);
    try {
        int hours = stoi(hours_str);
        if (hours < 0 || hours > 23) {
            return false;
        }
    }
    catch (invalid_argument e) {
        return false;
    }

    // Проверяем, что третий символ - это двоеточие
    if (time_str[2] != ':') {
        return false;
    }

    // Проверяем, что четвертый и пятый символы - это число от 00 до 59
    string minutes_str = time_str.substr(3, 2);
    try {
        int minutes = stoi(minutes_str);
        if (minutes < 0 || minutes > 59) {
            return false;
        }
    }
    catch (invalid_argument e) {
        return false;
    }

    // Если все проверки прошли успешно, возвращаем true
    return true;
}

bool isTimeWithinRange(const string& timeStr, const string& rangeStr) {
    // Извлекаем значения часов и минут из строки времени
    istringstream timeStream(timeStr);
    int hours, minutes;
    char delimiter;
    timeStream >> hours >> delimiter >> minutes;

    // Извлекаем значения начального и конечного времени из строки временного промежутка
    istringstream rangeStream(rangeStr);
    int startHours, startMinutes, endHours, endMinutes;
    rangeStream >> startHours >> delimiter >> startMinutes >> delimiter >> endHours >> delimiter >> endMinutes;

    // Проверяем, что время входит в заданный промежуток
    if ((hours > startHours || (hours == startHours && minutes >= startMinutes)) &&
        (hours < endHours || (hours == endHours && minutes <= endMinutes))) {
        return true;
    }
    return false;
}

bool check_name(string str) {
    regex pattern("[A-Za-z]+ [A-Za-z]\\.[A-Za-z]\\.");
    return std::regex_match(str, pattern);
}

string capitals(string str) {
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] >= 97 && str[i] <= 122)
        {
            str[i] -= 32;
        }
    }
    return str;
}

bool year_in_range(int number) {
    return (number >= 1900 && number <= 2023);
}

int check(int m)
{
    while ((cin.fail()) || (cin.get() != '\n'))
    {
        cout << "Error: ошибка ввода." << endl;
        cout << "Попробуйте заново: " ;
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cin >> m;
    }
    return m;
}


const int MAX_SIZE = 100; // максимальный размер хеш-таблицы

struct Patient {
    string reg_num; // регистрационный номер - key
    string name; // ФИО
    int birth_year; // год рождения
    string address; // адрес
    string workplace; // место работы (учебы)

    Patient() {
        reg_num = "";
        name = "";
        birth_year = 0;
        address = "";
        workplace = "";
    }

    Patient(string name, int birth_year, string address, string workplace, string uchastok, string poryadok) {
        //MM - NNNNNN
        this->reg_num = uchastok + uchastok + "-" + poryadok;
        this->name = name;
        this->birth_year = birth_year;
        this->address = address;
        this->workplace = workplace;
    }
};

class HashTable {
private:
    Patient* table[MAX_SIZE];
    bool used[MAX_SIZE]; // массив флагов для отслеживания занятых ячеек
    int size; // текущий размер хеш-таблицы
public:
    int get_size()
    {
        return size;
    }
    HashTable();
    int hash(const string& ); // хеш-функция для регистрационного номера
    void add_patient(Patient*); // метод для добавления нового пациента в хеш-таблицу
    Patient* find_patient(const string&); // метод для поиска пациента по регистрационному номеру
    void delete_patient(const string&); // метод для удаления пациента по регистрационному номеру
    void print_table(); //вывод таблицы пациентов
    void clear_table(); //метод очищения данных о пациентах
    void find_patient_FIO(const string&);

};

void HashTable::clear_table() {
    size = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        used[i] = false;
    }
    cout << "Данные о пациентах очищены" << endl;
}

HashTable::HashTable()
{
    size = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        used[i] = false;
        table[i] = nullptr;
    }
}

int HashTable::hash(const string& reg_num)
{
    int h = 0;
    for (char c : reg_num) {
        h = (h * 31 + c) % MAX_SIZE;
    }
    return h;
}

void HashTable::add_patient(Patient* patient) {
    int h = hash(patient->reg_num); //получаем ключ
    int i = h;
    while (used[i]) { //если ячейка в таблице занята
        if (table[i]->reg_num == patient->reg_num) {
            // пациент с таким же регистрационным номером уже есть в таблице
            cout << "Error: пациент с регистрационным номером " << patient->reg_num << " уже существует." << endl;
            return;
        }
        i = (i + 1) % MAX_SIZE; // линейное опробование
        if (i == h) {
            // пройдены все ячейки таблицы, нет места для нового пациента
            cout << "Error: Хэш-таблица заполнена." << endl;
            return;
        }
    }
    table[i] = patient;
    used[i] = true;
    size++;
}

Patient* HashTable::find_patient(const string& reg_num)
{
    int h = hash(reg_num);
    int i = h;
    while (used[i]) { //если ячейка занята
        if (table[i]->reg_num == reg_num) {
            return table[i];
        }
        i = (i + 1) % MAX_SIZE; // линейное опробование
        if (i == h) {
            // пройдены все ячейки таблицы, пациент не найден
            cout << "Error: пациент с регистрационным номером " << reg_num << " не найден." << endl;
            return nullptr;
        }
    }
    cout << "Error: пациент с регимтрационным номером " << reg_num << " не найден." << endl;
    return nullptr;
}

void HashTable::find_patient_FIO(const string& FIO)
{
    int flag = 0;
    if (size == 0)
    {
        cout << "Таблица пуста" << endl;
    }
    else
    {
        cout << left << setw(15) << "Рег.номер" << setw(20) << "ФИО" << endl;
        for (int i = 0; i < MAX_SIZE; i++) {
            if (used[i]) { //если ячейка занята
                if (table[i]->name == FIO) {
                    cout << left << setw(15) << table[i]->reg_num << setw(20) << table[i]->name << endl;
                    flag = 1;
                }
                cout << "----------------------------------------" << endl;
            }
        }
        if (flag == 0)
        {
            cout << "----------------------------------------" << endl;
            cout << "Пациенты с таким ФИО не зарегистрированы" << endl;
            cout << "----------------------------------------" << endl;
        }
    }

}

void HashTable::delete_patient(const string& reg_num) {
    int h = hash(reg_num);
    int i = h;
    while (used[i]) { //если ячейка занята
        if (table[i]->reg_num == reg_num) {
            // найден пациент, удаляем его
            used[i] = false;
            size--;
            // проверяем, необходимо ли переместить последующие элементы
            int j = (i + 1) % MAX_SIZE;
            while (used[j]) { //если ячейка занята
                int k = hash(table[j]->reg_num);
                if ((j > i && (k <= i || k > j)) ||
                    (j < i && (k <= i && k > j))) {
                    // элемент j должен быть перемещен на место i
                    table[i] = table[j];
                    used[i] = true;
                    used[j] = false;
                    i = j;
                }
                j = (j + 1) % MAX_SIZE;
            }
            cout << "Данные о пациенте удалены" << endl;
            return;
        }
        i = (i + 1) % MAX_SIZE; // линейное опробование
        if (i == h) {
            // пройдены все ячейки таблицы, пациент не найден
            cout << "Error: пациент с регистрационным номером " << reg_num << " не найден." << endl;
            return;
        }
    }
    cout << "Error: пациент с регистрационным номером " << reg_num << " не найден." << endl;
}

void HashTable::print_table(){
    if (size == 0)
    {
        cout << "Таблица пуста" << endl;
    }
    else
    {
        cout << left << setw(15) << "Рег.номер" << setw(20) << "ФИО" << setw(20) << "Год рождения" << setw(30) << "Адрес" << setw(30) << "Место работы(учебы)" << endl;
        for (int i = 0; i < MAX_SIZE; i++) {
            if (used[i]) {
                cout << left << setw(15) << table[i]->reg_num << setw(20) << table[i]->name << setw(20) << table[i]->birth_year << setw(30) << table[i]->address << setw(30) << table[i]->workplace << endl;
            }
        }
    }
}

// Д Е Р Е В О
// узлы дерева
struct Doctor {
    string name; // ФИО врача - ключ
    string position; // Должность
    int room_number; // Номер кабинета
    string schedule; // График приема

    Doctor* left; // указатель на левое поддерево
    Doctor* right; // указатель на правое поддерево
    Doctor* prev; // указатель на предыдущий узел
    int height_node; //высота поддерева данного узла

    Doctor(string name, string position, int room_number, string schedule, Doctor* right = nullptr, Doctor* left = nullptr, Doctor* previous = nullptr, int height_node = 1)
    {
        this->name = name;
        this->position = position;
        this->room_number = room_number;
        this->schedule = schedule;
        this->right = right;
        this->left = left;
        this->height_node = height_node;
        this->prev = previous;
    }
};

class Tree
{
private:
    Doctor* root = nullptr;
public:
    int get_height(Doctor* node)
    {
        if (node != nullptr)
        {
            return node->height_node;
        }
        else
        {
            return 0;
        }
    }

    int balance_factor(Doctor* node)
    {
        return get_height(node->right) - get_height(node->left);
    }

    void fix_height(Doctor* node) {
        int height_left = get_height(node->left);
        int height_right = get_height(node->right);
        if (height_left > height_right)
        {
            node->height_node = height_left + 1;
        }
        else
        {
            node->height_node = height_right + 1;
        }
    }

    void right_rotate(Doctor* high)
    {
        Doctor* low = high->left;
        high->left = low->right;
        if (low->right != nullptr)
        {
            low->right->prev = high;
        }
        low->right = high;
        low->prev = high->prev;
        high->prev = low;
        if (low->prev != nullptr)
        {
            if (low->prev->name < low->name)
            {
                low->prev->right = low;
            }
            if (low->prev->name > low->name)
            {
                low->prev->left = low;
            }
        }

        fix_height(high);
        fix_height(low);
    }

    void left_rotate(Doctor* high)
    {
        Doctor* low = high->right;
        high->right = low->left;
        if (low->left != nullptr)
        {
            low->left->prev = high;
        }
        low->left = high;
        low->prev = high->prev;
        high->prev = low;
        if (low->prev != nullptr)
        {
            if (low->prev->name < low->name)
            {
                low->prev->right = low;
            }
            if (low->prev->name > low->name)
            {
                low->prev->left = low;
            }
        }
        fix_height(high);
        fix_height(low);
    }

    Doctor* get_root()
    {
        return root;
    }

    void balance(Doctor* node)
    {
        while (true)
        {
            fix_height(node);
            if (balance_factor(node) >= 2)
            {
                if (balance_factor(node->right) == 1 || balance_factor(node->right) == 0)
                {
                    left_rotate(node);
                }
                else
                {
                    right_rotate(node->right);
                    left_rotate(node);
                }
            }
            else if (balance_factor(node) <= -2)
            {
                if (balance_factor(node->left) == -1 || balance_factor(node->left) == 0)
                {
                    right_rotate(node);
                }
                else
                {
                    left_rotate(node->left);
                    right_rotate(node);
                }
            }
            if (node->prev)
                node = node->prev;
            else
            {
                root = node;
                break;
            }
            root = node;
        }
    }

    bool search(string name, string position)
    {
        if (root != nullptr)
        {
            Doctor* temp = root;
            while (temp->name != name)
            {
                if (name < temp->name)
                {
                    if (temp->left == nullptr)
                    {
                        return false;
                    }
                    else {
                        temp = temp->left;
                    }
                }
                else
                {
                    if (temp->right == nullptr)
                    {
                        return false;
                    }
                    else
                    {
                        temp = temp->right;
                    }
                }
            }
            if (temp->position == position)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else {
            return false;
        }
    }

    void add(string name, string position, int room_number, string schedule)
    {
        if (!(this->search(name, position)))
        {
            if (root == nullptr)
            {
                root = new Doctor(name, position, room_number, schedule);
            }
            else
            {
                Doctor* temp = root;
                while (true)
                {
                    if (name < temp->name)
                    {
                        if (temp->left == nullptr)
                        {
                            temp->left = new Doctor(name, position, room_number, schedule);
                            temp->left->prev = temp;

                            break;
                        }
                        else
                        {
                            temp = temp->left;
                        }

                    }
                    else
                    {
                        if (temp->right == nullptr)
                        {
                            temp->right = new Doctor(name, position, room_number, schedule);
                            temp->right->prev = temp;
                            break;
                        }
                        else
                        {
                            temp = temp->right;
                        }
                    }
                }
                balance(temp);

            }
            system("cls");
            cout << endl << "Регистрация прошла успешно." << endl<< endl;
        }
        else
        {
            cout << "Нельзя добавить существующий элемент!" << endl;
        }

    }

    void print_tree_inorder(Doctor* node) //симметричный обход дерева.
    {
        if (node == nullptr) return;

        print_tree_inorder(node->left);
        cout << left << setw(20) << node->name << setw(20) << node->position << setw(15) << node->room_number << setw(15) << node->schedule << endl;
        print_tree_inorder(node->right);
    }

    void remove(string data, string position)
    {
        if (this->search(data, position))
        {
            Doctor* temp = root;
            while (true)
            {
                if (data == temp->name)
                {
                    if (get_height(temp) == 1)
                    {
                        if (temp->prev == nullptr)
                        {
                            delete temp;
                            temp = nullptr;
                            root = nullptr;
                        }
                        else if (data < temp->prev->name)
                        {
                            temp->prev->left = nullptr;
                            Doctor* prev_for_temp = temp->prev;
                            delete temp;
                            temp = nullptr;
                            balance(prev_for_temp);
                        }
                        else
                        {
                            temp->prev->right = nullptr;
                            Doctor* prev_for_temp = temp->prev;
                            delete temp;
                            temp = nullptr;
                            balance(prev_for_temp);
                        }
                        break;
                    }
                    if (temp->right == nullptr || temp->left == nullptr)
                    {
                        if (temp->prev == nullptr)
                        {
                            if (temp->left == nullptr)
                            {
                                root = temp->right;
                                temp->right->prev = nullptr;
                            }
                            else if (temp->right == nullptr)
                            {
                                root = temp->left;
                                temp->left->prev = nullptr;
                            }
                            delete temp;
                            temp = nullptr;
                            this->balance(root);
                        }
                        else if (data < temp->prev->name)
                        {
                            if (temp->left == nullptr)
                            {
                                temp->right->prev = temp->prev;
                                temp->prev->left = temp->right;

                                Doctor* another_temp = temp->prev;
                                delete temp;
                                temp = nullptr;
                                this->balance(another_temp);
                            }
                            else if (temp->right == nullptr)
                            {
                                temp->left->prev = temp->prev;
                                temp->prev->left = temp->left;

                                Doctor* another_temp = temp->prev;
                                delete temp;
                                temp = nullptr;
                                this->balance(another_temp);
                            }
                        }
                        else
                        {
                            if (temp->right != nullptr)
                            {
                                temp->right->prev = temp->prev;
                                temp->prev->right = temp->right;

                                Doctor* another_temp = temp->prev;
                                delete temp;
                                temp = nullptr;
                                this->balance(another_temp);
                            }
                            else if (temp->left != nullptr)
                            {
                                temp->left->prev = temp->prev;
                                temp->prev->right = temp->left;

                                Doctor* another_temp = temp->prev;
                                delete temp;
                                temp = nullptr;
                                this->balance(another_temp);
                            }
                        }
                        break;
                    }

                    if (temp->right != nullptr && temp->left != nullptr)
                    {
                        Doctor* most_right = temp->left;
                        while (most_right->right != nullptr)
                        {
                            most_right = most_right->right;
                        }
                        if (most_right == temp->left)
                        {
                            most_right->prev = temp->prev;
                            if (temp->prev != nullptr)
                            {
                                if (data < temp->prev->name)
                                {
                                    temp->prev->left = most_right;
                                }
                                else
                                {
                                    temp->prev->right = most_right;
                                }

                            }
                            most_right->right = temp->right;
                            temp->right->prev = most_right;
                            delete temp;
                            temp = nullptr;
                            this->balance(most_right);
                            break;
                        }
                        else
                        {
                            Doctor* prev = most_right->prev;
                            prev->right = most_right->left;
                            if (most_right->left != nullptr)
                            {
                                most_right->left->prev = prev;
                            }
                            most_right->right = temp->right;
                            most_right->left = temp->left;
                            temp->right->prev = most_right;
                            temp->left->prev = most_right;
                            most_right->prev = temp->prev;
                            if (temp->prev != nullptr)
                            {
                                if (data < temp->prev->name)
                                {
                                    temp->prev->left = most_right;
                                }
                                else
                                {
                                    temp->prev->right = most_right;
                                }
                            }
                            delete temp;
                            temp = nullptr;
                            this->balance(prev);
                            break;
                        }
                    }

                }
                else if (data < temp->name)
                {
                    temp = temp->left;
                }
                else if (data > temp->name)
                {
                    temp = temp->right;
                }
            }
            cout << "Данные о враче удалены." << endl;
        }
        else
        {
        system("cls");
            cout << endl << "Врача нет в базе данных!" << endl;
        }
    };

    void clearTree(Doctor* node)
    {
        if (node == nullptr) {
            return;
        }
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

    void clear()
    {
        clearTree(root);
        root = nullptr;
    }

    void search_text(string word)
    {
        if (root == nullptr)
        {
            cout << endl << "Error: База данных о врачах пуста." << endl << endl;
        }
        else
        {
            bool flag = 0;
            searchHelper(root, word, flag);
            if (flag == 0)
            {
                system("cls");
                cout << endl << "Совпадений не найдено" << endl<< endl;
            }
        }
    }

    void searchHelper(Doctor* node, string word, bool &flag)
    {
        if (node != nullptr)
        {
            //прямой алгоритм поиска слова в тексте
            int n = node->position.length();
            int m = word.length();

            for (int i = 0; i <= n - m; i++)
            {
                int j = 0;
                while (j < m && node->position[i + j] == word[j])
                {
                    j++;
                }

                if (j == m)
                {
                    flag = 1;
                    cout << left << setw(20) << node->name << setw(20) << node->position << setw(15) << node->room_number << setw(15) << node->schedule << endl;
                }
            }

            // Рекурсивный обход левого поддерева
            searchHelper(node->left, word, flag);

            // Рекурсивный обход правого поддерева
            searchHelper(node->right, word, flag);
        }

    }

    Doctor* searchDoctor(string name, string position)
    {
        Doctor* current = root; // начинаем поиск с корня дерева
        while (current != nullptr) {
            // если ФИО и должность совпадают, возвращаем найденный узел
            if (current->name == name && current->position == position) {
                return current;
            }
            // иначе, если ФИО и должность больше текущего узла, идем в правое поддерево
            else if (name > current->name) {
                current = current->right;
            }
            // иначе, если ФИО и должность меньше текущего узла, идем в левое поддерево
            else {
                current = current->left;
            }
        }
        // если не нашли узел, возвращаем nullptr
        return nullptr;
    }

    bool search_kab(int kabinet)
    {
        if (root != nullptr)
        {
            Doctor* temp = root;
            while (temp->room_number != kabinet)
            {
                if (kabinet < temp->room_number)
                {
                    if (temp->left == nullptr)
                    {
                        return false;
                    }
                    else {
                        temp = temp->left;
                    }
                }
                else
                {
                    if (temp->right == nullptr)
                    {
                        return false;
                    }
                    else
                    {
                        temp = temp->right;
                    }
                }
            }
            
            return true;
            
        }
        else {
            return false;
        }
    }


};

 //С П И С О К
 //ячейка списка
struct Direction {

    string reg_number;  // Регистрационный номер
    string doctor_name; // ФИО врача
    string position;    // Должность
    string date;        // Дата направления
    string time;        // Время направления

    Direction* prev;    // Указатель на предыдущую запись в списке
    Direction* next;    // Указатель на следующую запись в списке

    Direction(string reg_number, string doctor_name, string date, string time, string position) {
        this->reg_number = reg_number;
        this->doctor_name = doctor_name;
        this->date = date;
        this->time = time;
        this->position = position;
        prev = nullptr;
        next = nullptr;
    }
};

class List {
private:
    Direction* head;
    Direction* tail;
public:
    List() {
        head = nullptr;
        tail = nullptr;
    }

    Direction* get_head()
    {
        return head;
    }

    void add(string reg_number, string doctor_name, string date, string time, string position) {
        Direction* new_direction = new Direction(reg_number, doctor_name, date, time, position);
        if (head == nullptr) {
            head = new_direction;
            tail = new_direction;
            new_direction->prev = tail;
            new_direction->next = head;
        }
        else {
            tail->next = new_direction;
            new_direction->prev = tail;
            new_direction->next = head;
            head->prev = new_direction;
            tail = new_direction;
        }
    }

    void print_list() {
        if (head == nullptr) {
            cout << "Список пуст" << endl;
            return;
        }

        Direction* current = head;
        do {
            cout << left << setw(10) << current->reg_number << setw(25) << current->doctor_name << setw(15) << current->position << setw(10) << current->date << setw(10) << current->time << endl;
            current = current->next;
        } while (current != head);
        cout << endl;
    }

    Direction* find(string doctor_name, string date, string time, string position) {

        if (head == nullptr)
        {
            return nullptr;
        }
        else
        {
            Direction* current = head;

            do {
                if (current->doctor_name == doctor_name && current->date == date && current->time == time && current->position == position) {
                    return current;
                }
                current = current->next;
            } while (current != head);

            return nullptr; // если элемент не найден
        }
    }

    Direction* find_by_reg_number(string reg_number) {
        if (head == nullptr)
        {
            return nullptr;
        }
        else
        {
            Direction* current = head;
            while (current != nullptr) {
                if (current->reg_number == reg_number) {
                    return current;
                }
                current = current->next;
                if (current == head) {
                    break; // Прошли весь список и вернулись на начало
                }
            }
            return nullptr; // Не нашли элемент с заданным номером
        }
    }

    void print_with_patents_date(Patient* ppp) {
        bool flag = 0;
        if (head == nullptr)
        {
            system("cls");
            cout << "База данных записей пуста." << endl;
        }
        else
        {
            Direction* current = head;
            while (current != nullptr) {
                if (current->reg_number == ppp->reg_num) {
                    cout << left << setw(25) << current->doctor_name << setw(20) << ppp->reg_num << setw(25) << ppp->name << setw(10) << ppp->birth_year << setw(20) << ppp->address << setw(20) << ppp->workplace << endl;
                    flag = 1;
                }
                current = current->next;
                if (current == head) {
                    break; // Прошли весь список и вернулись на начало
                }
            }
            if (flag == 0)
            {
                system("cls");
                cout << endl << "Записей к врачу у пациента нет" << endl;
            }
        }
    }

    void print_with_doctors_date(Doctor* ddd, HashTable patients) {
        bool flag = 0;
        if (head == nullptr)
        {
            cout << "База данных записей пуста." << endl;
        }
        else
        {
            Direction* current = head;
            while (current != nullptr) {
                if (current->doctor_name == ddd->name) {
                    Patient* ppp = patients.find_patient(current->reg_number);
                    cout << left << setw(25) << ddd->name << setw(20) << ddd->position << setw(15) << ddd->room_number << setw(15) << ddd->schedule << "| " << setw(25) << ppp->name << setw(10) << ppp->reg_num << endl;
                    flag = 1;
                }
                current = current->next;
                if (current == head) {
                    break; // Прошли весь список и вернулись на начало
                }
            }
            if (flag == 0)
            {
                cout << endl << "У врача нет записей." << endl;
            }
        }
    }
    
    void remove(string reg_number) {
        if (head == nullptr) {
            return; // список пустой
        }

        Direction* current = head;

        do {
            if (current->reg_number == reg_number) {
                if (head == tail) {
                    // В списке всего один элемент
                    delete current;
                    head = nullptr;
                    tail = nullptr;
                    return;
                }
                else if (current == head) {
                    // Удаляемый элемент является головой списка
                    head = current->next;
                    head->prev = tail;
                    tail->next = head;
                }
                else if (current == tail) {
                    // Удаляемый элемент является концом списка
                    tail = current->prev;
                    tail->next = head;
                    head->prev = tail;
                }
                else {
                    // Удаляемый элемент находится где-то в середине списка
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }

                Direction* next = current->next;
                delete current;
                current = next;
            }
            else {
                current = current->next;
            }
        } while (current != head);
    }

    void clear() {
        if (head == nullptr) {
            return;
        }

        Direction* current = head;
        while (current != tail) {
            Direction* temp = current;
            current = current->next;
            delete temp;
        }

        delete tail;
        head = nullptr;
        tail = nullptr;
    }

    void remove_2(string uchastok, string place_of_work) {
        if (head == nullptr) {
            return; // список пустой
        }

        Direction* current = head;

        do {
            if (current->doctor_name == uchastok && current->position == place_of_work) {
                if (head == tail) {
                    // В списке всего один элемент
                    delete current;
                    head = nullptr;
                    tail = nullptr;
                    return;
                }
                else if (current == head) {
                    // Удаляемый элемент является головой списка
                    head = current->next;
                    head->prev = tail;
                    tail->next = head;
                }
                else if (current == tail) {
                    // Удаляемый элемент является концом списка
                    tail = current->prev;
                    tail->next = head;
                    head->prev = tail;
                }
                else {
                    // Удаляемый элемент находится где-то в середине списка
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }

                Direction* next = current->next;
                delete current;
                current = next;
            }
            else {
                current = current->next;
            }
        } while (current != head);
    }

    Direction* operator[] (const int index)
    {
        int counter = 0;
        Direction* current = this->head;
        while (current != tail)
        {
            if (counter == index)
            {
                return current;
            }
            current = current->next;
            counter++;
        }
        return current;

    }

    void sort(List& directions)
    {
        int count = 0;
        Direction* current = head;
        while (current != tail) {
            count++;
            current = current->next;
        }
        count++;

        string name, pos, rn, t, d;

        for (int i = 1; i < count; i++) {
            Direction* key = directions[i];
            name = key->doctor_name;
            pos = key->position;
            rn = key->reg_number;
            t = key->time;
            d = key->date;

            int j = i - 1;
            while (j >= 0 && directions[j]->doctor_name > key->doctor_name) {
                directions[j + 1]->doctor_name = directions[j]->doctor_name;
                directions[j + 1]->position = directions[j]->position;
                directions[j + 1]->reg_number = directions[j]->reg_number;
                directions[j + 1]->time = directions[j]->time;
                directions[j + 1]->date = directions[j]->date;

                directions[j]->doctor_name = name;
                directions[j]->position = pos;
                directions[j]->reg_number = rn;
                directions[j]->time = t;
                directions[j]->date = d;


                j--;

            }
        }
    }

    bool isSorted()
    {
        Direction* cur = head;
        while (cur->next != tail)
        {
            if (cur->doctor_name > cur->next->doctor_name)
                return false;
            cur = cur->next;
        }
        if (cur->doctor_name > cur->next->doctor_name)
            return false;
        return true;
    }
};



int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "Russian");

    int choose = -1;

    HashTable patients;
    Tree doctors;
    List directions;

    string FIO;
    int b_year = 0;
    string uchastok;
    string street;
    string place_of_work;

    string poryadok;
    string date;

    string begin = "0";
    string end = "0";

    doctors.add("AGAPOV A.A.", "Хирург", 12, "09:00-17:00");
    doctors.add("BAGAPOV A.A.", "Терапевт", 13, "09:00-17:00");
    doctors.add("CAGAPOV A.A.", "Дерматолог", 14, "09:00-17:00");
    doctors.add("DAGAPOV A.A.", "Хирург", 15, "09:00-17:00");

    patients.add_patient(new Patient("LIPOV A.A.", 2002, "Никольская", "SUAI", "2", "000001"));


    cout << "РЕГИСТРАЦИЯ. ПОЛИКЛИННИКА" << endl << endl;

    while (choose != 0)
    {
        cout << "--------------------МЕНЮШКА-------------------" << endl;
        cout << "Регистрация нового пациента - 1" << endl;
        cout << "Удаление данных о пациенте - 2" << endl;
        cout << "Просмотр всех пациентов - 3" << endl;
        cout << "Очистка данных о пациентах - 4" << endl;
        cout << "Посмотреть к какому врачу записан - 5" << endl;
        cout << "Поиск пациента по ФИО - 6" << endl;
        cout << "----------------------------------------------" << endl;
        cout << "Добавление нового врача - 7" << endl;
        cout << "Удаление сведений о враче - 8" << endl;
        cout << "Просмотр всех врачей - 9" << endl;
        cout << "Очистка данных о врачах - 10" << endl;
        cout << "Показать пациентов у врача - 11" << endl;
        cout << "Поиск врача по фрагментам 'Должность' - 12" << endl;
        cout << "----------------------------------------------" << endl;
        cout << "Запись на прием - 13" << endl;
        cout << "Отмена записи на прием - 14" << endl;
        cout << "Просмотр всех записей - 15" << endl;
        cout << "----------------------------------------------" << endl;
        cin >> choose;
        system("cls");

        switch (choose)
        {
        case 1:
        {
            cout << "ФИО (например, Ivanon I.I.): ";
            cin.ignore(); // очистить буфер ввода
            getline(cin, FIO);
            while (!(check_name(FIO)))
            {
                cout << "Error: ошибка ввода." << endl;
                cout << "ФИО (например, Ivanon I.I.): ";
                getline(cin, FIO);
            }

            FIO = capitals(FIO);


            cout << "Год рождения (от 1900): ";
            cin >> b_year;
            b_year = check(b_year);
            while (!(year_in_range(b_year)))
            {
                cout << "Error: ошибка ввода." << endl;
                cout << "Год рождения (от 1900): ";
                cin >> b_year;
                b_year = check(b_year);
            }
        

            cout << "Улица проживания: " << endl;
            cout << "Первомайская - 1" << endl;
            cout << "Никольская - 2" << endl;
            cout << "Октябрьская - 3" << endl;
            //cin.ignore(); // очистить буфер ввода
            getline(cin, uchastok);
            if (uchastok == "1")
            {
                street = "Первомайская";
            }
            else if (uchastok == "2")
            {
                street = "Никольская";
            }
            else if (uchastok == "3")
            {
                street = "Октябрьская";
            }
            else
            {
                cout << "Регистрация не удалась, попробуйте заново" << endl;
                break;
            }

            cout << "Место работы (вводите буквы и пробелы): ";
            getline(cin, place_of_work);
            place_of_work = capitals(place_of_work);


            poryadok = pad_with_zeroes(patients.get_size() + 1);

            Patient* bolnoy = new Patient(FIO, b_year, street, place_of_work, uchastok, poryadok);
            patients.add_patient(bolnoy);
            system("cls");
            cout << endl << "Регистрация прошла успешно" << endl << endl;
            break;
        }
        case 2:
        {
            if (patients.get_size() == 0)
            {
                cout << "Error: База данных пациента пустая.";
                break;
            }
            patients.print_table();
            cout << "Введите регистрационный номер: ";
            cin.ignore();
            getline(cin, uchastok);
            Patient* patient = patients.find_patient(uchastok);
            if (patient == nullptr)
            {
                break;
            }
            patients.delete_patient(uchastok);
            directions.remove(poryadok);
            cout << "Данные о записях обновлены." << endl;

            break;
            //должны удалиться данные о записи в списке
        }
        case 3:
        {
            patients.print_table();
            break;
        }
        case 4:
        {
            if (patients.get_size() == 0)
            {
                cout << "Error: База данных пациентов пустая.";
                break;
            }
            patients.clear_table();
            directions.clear();
            cout << "Данные о записях обновлены." << endl;
            break;
            //должны удалиться данные о записях в списке
        }
        case 5:
        {
            if (patients.get_size() == 0)
            {
                cout << "Error: База данных пациентов пустая." << endl;
                break;
            }
            cout << "ПАЦИЕНТЫ" << endl;
            patients.print_table();
            cout << endl << "Введите рег.номер пациента: ";
            cin.ignore();
            getline(cin, poryadok);
            Patient* ppp = patients.find_patient(poryadok);
            if (ppp == nullptr)
            {
                system("cls");
                cout << "Error: Пациента с таким рег.номером в базе нет." << endl;
                break;
            }
            else
            {
                cout << endl << left << setw(25) << "Врач" << setw(20) << "Пациент,номер" << setw(25) << "ФИО" << setw(10) << "Год р." << setw(20) << "Адрес" << setw(20) << "Место работы" << endl << endl;;
                directions.print_with_patents_date(ppp);
                cout << endl;
                break;
            }
        }
        case 6:
        {
            cout << "Введите ФИО: ";
            cin.ignore();
            getline(cin, FIO);
            patients.find_patient_FIO(FIO);
            break;
        }
        case 7:
        {
            cout << "ФИО (например, Ivanon I.I.): ";
            cin.ignore(); // очистить буфер ввода
            getline(cin, FIO);
            while (!(check_name(FIO)))
            {
                cout << "Error: ошибка ввода." << endl;
                cout << "ФИО (например, Ivanon I.I.): ";
                getline(cin, FIO);
            }
            FIO = capitals(FIO);

            cout << "Выберите должность: " << endl;
            cout << "Терапевт - 1" << endl;
            cout << "Окулист - 2" << endl;
            cout << "Хирург - 3" << endl;
            cout << "Дерматолог - 4" << endl;
            //cin.ignore(); // очистить буфер ввода
            getline(cin, uchastok);
            if (uchastok == "1")
            {
                uchastok = "Терапевт";
            }
            else if (uchastok == "2")
            {
                uchastok = "Окулист";
            }
            else if (uchastok == "3")
            {
                uchastok = "Хирург";
            }
            else if (uchastok == "4")
            {
                uchastok = "Дерматолог";
            }
            else
            {
                system("cls");
                cout << endl << "Error: Возникла ошибка, попробуйте заново" << endl << endl;
                break;
            }

            cout << "Номер кабинета (от 1 до 20): ";
            cin >> b_year;
            b_year = check(b_year);
            while (!(b_year > 0 && (b_year < 21)))
            {
                cout << "Error: ошибка ввода." << endl;
                cout << "Номер кабинета (от 1 до 20): ";
                cin >> b_year;
                b_year = check(b_year);
            }
            if (doctors.search_kab(b_year) == true) // если кабинет занят
            {
                system("cls");
                cout << endl << "Error: кабинет занят." << endl<<endl;
                break;
            }


            cout << "ГРАФИК ПРИЁМА (График поликлинники 8:00 - 17:00)" << endl;
            cout << "Начало приёма (формат - XX:XX): ";
            //cin.ignore();
            getline(cin, begin);
            while (!(check_time_format(begin)))
            {
                cout << "Error: введите корректный час." << endl;
                cout << "Начало приёма: ";
                getline(cin, begin);
            }
            cout << "Конец приёма (формат - XX:XX): ";
            getline(cin, end);
            while (!(check_time_format(end)))
            {
                cout << "Error: введите корректный час." << endl;
                cout << "Конец приёма: ";
                getline(cin, end);
            }
            if ((begin < "08:00" || begin >= "16:50") || (end < "08:10" || end > "17:00") || (begin > end))
            {
                system("cls");
                cout << endl << "Такой график невозможен" << endl;
                cout << "Error: Возникла ошибка, попробуйте заново" << endl << endl;
                break;
            }

            place_of_work = begin + "-" + end;

            poryadok = pad_with_zeroes(patients.get_size() + 1);

            doctors.add(FIO, uchastok, b_year, place_of_work);
            break;
        }
        case 8:
        {
            if (doctors.get_root() == nullptr)
            {
                cout << "Error: База данных врачей пустая.";
                break;
            }
            cout << left << setw(20) << "ФИО" << setw(20) << "Должность" << setw(15) << "Кабинет" << setw(15) << "График приема" << endl;
            doctors.print_tree_inorder(doctors.get_root());
            cout << "Введите ФИО врача: ";
            cin.ignore();
            getline(cin, uchastok);
            uchastok = capitals(uchastok);
            cout << "Выберите должность: " << endl;
            cout << "Терапевт - 1" << endl;
            cout << "Окулист - 2" << endl;
            cout << "Хирург - 3" << endl;
            cout << "Дерматолог - 4" << endl;
            //cin.ignore(); // очистить буфер ввода
            getline(cin, place_of_work);
            if (place_of_work == "1")
            {
                place_of_work = "Терапевт";
            }
            else if (place_of_work == "2")
            {
                place_of_work = "Окулист";
            }
            else if (place_of_work == "3")
            {
                place_of_work = "Хирург";
            }
            else if (place_of_work == "4")
            {
                place_of_work = "Дерматолог";
            }
            else
            {
                cout << "Error: Возникла ошибка, попробуйте заново" << endl;
                break;
            }
            doctors.remove(uchastok, place_of_work);
            directions.remove_2(uchastok, place_of_work);
            cout << "Данные о записях обновлены." << endl<<endl;
            break;
            //должны удалиться данные о записи в списке
        }
        case 9:
        {
            Doctor* root = doctors.get_root();
            if (root == nullptr)
            {
                cout << endl << "Error: База данных врачей пуста." << endl << endl;
                break;
            }
            else
            {
                cout << left << setw(20) << "ФИО" << setw(20) << "Должность" << setw(15) << "Кабинет" << setw(15) << "График" << endl;
                doctors.print_tree_inorder(root);
                cout << endl;
                break;
            }
        }
        case 10:
        {
            Doctor* root = doctors.get_root();
            if (root == nullptr)
            {
                cout << endl << "Error: База данных врачей пуста." << endl << endl;
                break;
            }
            else
            {
                doctors.clear();
                cout << endl << "База данных врачей очищена" << endl;
                directions.clear();
                cout << "Данные о записях обновлены." << endl << endl;
            }
            //должны удалиться данные о записях в списке
        }
        case 11:
        {
            if (doctors.get_root() == nullptr)
            {
                system("cls");
                cout << "Error: База данных врачей пуста." << endl;
                break;
            }
            else
            {
                cout << "ВРАЧИ" << endl;
                cout << left << setw(20) << "ФИО" << setw(20) << "Должность" << setw(15) << "Кабинет" << setw(15) << "График" << endl;
                Doctor* root = doctors.get_root();
                doctors.print_tree_inorder(root);
            }
            cout << endl << "Введите ФИО врача: ";
            cin.ignore();
            getline(cin, FIO);
            FIO = capitals(FIO);

            cout << "Выберите должность: " << endl;
            cout << "Терапевт - 1" << endl;
            cout << "Окулист - 2" << endl;
            cout << "Хирург - 3" << endl;
            cout << "Дерматолог - 4" << endl;
            //cin.ignore(); // очистить буфер ввода
            getline(cin, uchastok);
            if (uchastok == "1")
            {
                uchastok = "Терапевт";
            }
            else if (uchastok == "2")
            {
                uchastok = "Окулист";
            }
            else if (uchastok == "3")
            {
                uchastok = "Хирург";
            }
            else if (uchastok == "4")
            {
                uchastok = "Дерматолог";
            }
            else
            {
                cout << endl << "Error: Возникла ошибка, попробуйте заново" << endl << endl;
                break;
            }

            Doctor* ddd = doctors.searchDoctor(FIO, uchastok);

            if (ddd == nullptr)
            {
                system("cls");
                cout << endl << "Error: Врача с такими данными нет." << endl << endl;
                break;
            }
            else
            {
                system("cls");
                cout << left << setw(25) << "Врач" << setw(20) << "Должность" << setw(15) << "Кабинет" << setw(15) << "График" << "| " << setw(25) << "Пациент" << setw(10) << "Рег.номер" << endl;
                directions.print_with_doctors_date(ddd, patients);
                cout << endl;
                break;
            }
        }
        case 12:
        {
            Doctor* root = doctors.get_root();
            if (root == nullptr)
            {
                cout << "Error: База данных врачей пуста." << endl;
                break;
            }
            else
            {
                cout << "Введите фрагмент: ";
                cin.ignore();
                getline(cin, uchastok);
                cout << left << setw(20) << "ФИО" << setw(20) << "Должность" << setw(15) << "Кабинет" << setw(15) << "График приема" << endl;
                doctors.search_text(uchastok);
                cout << endl;
                break;
            }
        }
        case 13:
        {
            if (patients.get_size() == 0 || doctors.get_root() == nullptr)
            {
                system("cls");
                cout << endl << "Error: База данных клиентов или врачей пуста." << endl << endl;
                break;
            }
            else
            {
                cout << "ПАЦИЕНТЫ" << endl;
                patients.print_table();
                cout << endl << "Введите рег.номер пациента: ";
                cin.ignore();
                getline(cin, poryadok);
                Patient* ppp = patients.find_patient(poryadok);
                if (ppp == nullptr)
                {
                    system("cls");
                    cout << endl << "Error: Пациента с таким рег.номером в базе нет." << endl;
                    break;
                }
                else
                {
                    cout << endl;
                    cout << "ВРАЧИ" << endl;
                    Doctor* root = doctors.get_root();
                    doctors.print_tree_inorder(root);
                    cout << endl << "Введите ФИО доктора: " << endl;
                    //cin.ignore();
                    getline(cin, FIO);
                    FIO = capitals(FIO);

                    cout << "Выберите должность: " << endl;
                    cout << "Терапевт - 1" << endl;
                    cout << "Окулист - 2" << endl;
                    cout << "Хирург - 3" << endl;
                    cout << "Дерматолог - 4" << endl;
                    //cin.ignore(); // очистить буфер ввода
                    getline(cin, place_of_work);
                    if (place_of_work == "1")
                    {
                        place_of_work = "Терапевт";
                    }
                    else if (place_of_work == "2")
                    {
                        place_of_work = "Окулист";
                    }
                    else if (place_of_work == "3")
                    {
                        place_of_work = "Хирург";
                    }
                    else if (place_of_work == "4")
                    {
                        place_of_work = "Дерматолог";
                    }
                    else
                    {
                        system("cls");
                        cout << endl << "Error: Возникла ошибка, попробуйте заново" << endl << endl;
                        break;
                    }

                    Doctor* cur = doctors.searchDoctor(FIO, place_of_work);
                    if (cur == nullptr)
                    {
                        system("cls");
                        cout << endl << "Error: Такого доктора в базе нет." << endl << endl;
                        break;
                    }
                    else
                    {
                        cout << "Введите дату(формат: XX:XX:23): ";
                       // cin.ignore();
                        getline(cin, date);
                        while (!(validateDate(date)))
                        {
                            cout << "Error: введите существующую дату." << endl;
                            cout << "Введите дату(формат: XX:XX:23): ";
                            getline(cin, date);
                        }

                        cout << "Введите время записи: ";
                        //cin.ignore();
                        getline(cin, begin);
                        while (!(check_time_format(begin)))
                        {
                            cout << "Error: введите корректный час." << endl;
                            cout << "Введите время записи: ";
                            getline(cin, begin);
                        }

                        if (isTimeWithinRange(begin, cur->schedule) == false)
                        {
                            system("cls");
                            cout << endl << "Error: Врач не ведет прием в это время." << endl << endl;
                            break;
                        }
                        else
                        {
                            //получили все необходимые данные для списка
                            //нужно проверить что записи на это время и дату к врачу нет
                            //если ее нет, то добавляем
                            if (directions.find(FIO, date, begin, place_of_work) == nullptr)
                            {
                                directions.add(poryadok, FIO, date, begin, place_of_work);
                                system("cls");
                                cout << endl << "Запись оформлена." << endl << endl;
                                break;
                            }
                            else
                            {
                                system("cls");
                                cout << endl << "Error: На данную дату и время уже есть запись." << endl << endl;
                                break;
                            }

                        }

                    }

                }

            }

            break;
        }
        case 14:
        {
            if (directions.get_head() == nullptr)
            {
                cout << endl << "Error: База данных записей пустая." << endl << endl;
                break;
            }
            system("cls");
            directions.print_list();
            cout << "Введите рег.номер: ";
            cin.ignore();
            getline(cin, poryadok);
            directions.remove(poryadok);
            system("cls");
            cout  << endl << "Данные о записях обновлены." << endl << endl;
            break;
        }
        case 15:
        {
            if (directions.get_head() == nullptr)
            {
                cout << endl << "База данных записей пустая." << endl << endl;
                break;
            }
            while (!(directions.isSorted()))
            {
                directions.sort(directions);
            }
            system("cls");
            cout << left << setw(10) << "Пациент" << setw(25) << "ФИО врача" << setw(15) << "Должность" << setw(10) << "Дата" << setw(10) << "Время" << endl;
            directions.print_list();
            break;
        }
        case 0:
        {
            break;
        }
        default:
        {
            cout << endl << "Error: Ошибка, попробуйте заново." << endl << endl;
            break;
        }

        }
    }
}



    // Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
    // Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

    // Советы по началу работы 
    //   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
    //   2. В окне Team Explorer можно подключиться к системе управления версиями.
    //   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
    //   4. В окне "Список ошибок" можно просматривать ошибки.
    //   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
    //   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.