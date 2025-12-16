#include "utils.h"
#include <cctype>

void Utils::clearInput() {
    // Удаление лишних символов из буфера ввода после std::cin >> value
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

bool Utils::containsForbiddenChars(const std::string& name) {
    // Запрещенные символы в именах файлов Windows (кроме *.*): \ / : * ? " < > | *пробел*
    const std::string forbidden = "\\/:*?\"<>| ";

    for (char c : name) {
        if (forbidden.find(c) != std::string::npos) {
            return true;
        }
    }
    return false;
}

std::string Utils::trim(const std::string& str) {
    const char* whitespace = " \t\n\r\f\v";

    // Поиск первого непробельного символа
    size_t start = str.find_first_not_of(whitespace);
    if (std::string::npos == start) {
        return ""; // Строка состоит только из пробелов
    }

    // Поиск последнего непробельного символа
    size_t end = str.find_last_not_of(whitespace);

    // Возвращаем подстроку
    return str.substr(start, (end - start) + 1);
}

long long Utils::promptForAccessCount() {
    long long count;
    bool flag = true;
    while (flag) {
        std::cout << "\nВведите количество обращений (целое число >= 0): ";
        if (std::cin >> count) {
            if (count >= 0) {
                clearInput();
                flag = false;
                return count;
            }
            else {
                std::cout << "Количество обращений должно быть неотрицательным.\n";
            }
        }
        else {
            std::cout << "Ошибка ввода. Введите целое число.\n";
            std::cin.clear();
            clearInput();
        }
    }
    return 0; // Достижимо только в случае выхода из цикла
}

Date Utils::promptForDateFormatted() {
    Date date;
    std::string date_str = "XX.XX.XXXX";
    // Массив позиций курсора, которые нужно заполнить цифрами (0-7): 
    // 0, 1 (ДД), 3, 4 (ММ), 6, 7, 8, 9 (ГГГГ)
    int editable_positions[] = { 0, 1, 3, 4, 6, 7, 8, 9 };
    int cursor_index = 0; // Текущий индекс

    std::string prompt = "Введите дату (ДД.ММ.ГГГГ): ";

    // Выводим запрос и маску в одной строке
    std::cout << "\n" << prompt << date_str;

    // Возвращаем курсор на начало маски (10 символов назад)
    for (int i = 0; i < 10; ++i) {
        std::cout << "\b";
    }
    std::cout.flush();

    while (cursor_index < 8) { // Пока не введено 8 цифр
        int input = _getch();

        // Обработка цифр
        if (input >= '0' && input <= '9') {
            char digit = static_cast<char>(input);
            int current_pos = editable_positions[cursor_index];

            date_str[current_pos] = digit; // Обновляем внутреннюю строку

            // Выводим цифру
            std::cout << digit;
            cursor_index++;

            // Обработка разделителя
            if (cursor_index == 2 || cursor_index == 4) {
                // Выводим разделитель, чтобы курсор переместился
                std::cout << date_str[current_pos + 1];
            }
        }

        // Обработка Backspace
        else if (input == 8) {
            if (cursor_index > 0) {

                // Проверка на переход через разделитель
                if (cursor_index == 2 || cursor_index == 4) {
                    std::cout << "\b";
                }

                // Стираем текущую цифру
                cursor_index--;
                int current_pos = editable_positions[cursor_index];
                date_str[current_pos] = 'X';

                // Перемещаем курсор назад на 1 позицию, выводим 'X', и снова назад
                std::cout << "\bX\b";
            }
        }
        std::cout.flush();
    }

    // Очистка строки ввода
    std::cout << "\r";
    std::cout << std::string(prompt.length() + date_str.length() + 1, ' ');
    std::cout << "\r";

    // Парсинг введенных цифр
    try {
        date.day = std::stoi(date_str.substr(0, 2));
        date.month = std::stoi(date_str.substr(3, 2));
        date.year = std::stoi(date_str.substr(6, 4));
    }
    catch (...) {
        std::cout << prompt << "Ошибка парсинга даты. Попробуйте снова.\n";
        return promptForDateFormatted();
    }

    // Вывод результата
    std::cout << prompt << date.toString() << "\n";

    if (date.isValid()) {
        return date;
    }
    else {
        std::cout << "Некорректная дата. Попробуйте снова.\n";
        return promptForDateFormatted();
    }
}


char Utils::getInputCharNoEnter() {
    char input = _getch();
    // Отображаем выбранный пункт
    std::cout << input << "\n";
    return input;
}


int Utils::getMenuChoice() {
    int choice = -1;
    char input = _getch(); // Считывает символ без Enter

    // Проверяем, является ли символ цифрой
    if (input >= '0' && input <= '5') {
        choice = input - '0';
    }

    // Отображаем выбранный пункт
    std::cout << input << "\n";

    if (choice == -1) {
        std::cout << "\nНекорректный выбор. Пожалуйста, нажмите цифру от 0 до 5.";
    }
    return choice;
}

int Utils::getSubMenuChoice() {
    int choice = -1;
    char input = _getch(); // Считывает символ без Enter

    // Проверяем, является ли символ цифрой от 0 до 3
    if (input >= '0' && input <= '3') {
        choice = input - '0';
    }

    // Отображаем выбранный пункт
    std::cout << input << "\n";

    if (choice == -1) {
        std::cout << "\nНекорректный выбор. Пожалуйста, нажмите цифру от 0 до 3.";
    }
    return choice;
}

void Utils::waitKeyPress() {
    _getch();
}


void Utils::setupConsoleEncoding() {
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);         // Устанавливаем кодировку ввода
    SetConsoleOutputCP(1251);   // Устанавливаем кодировку вывода
}