#include "fileCatalog.h"
#include "utils.h"
#include <algorithm>
#include <limits>

// Проверяет корректность даты
bool Date::isValid() const {
    if (year < 1900 || year > 2100) return false;   // Проверка диапазона года
    if (month < 1 || month > 12) return false;      // Проверка диапазона месяца
    if (day < 1 || day > 31) return false;          // Проверка диапазона дня

    // Специфическая проверка для Февраля
    if (month == 2) {
        
		bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); // Проверка на високосный год
		if (day > 29 || (!isLeap && day > 28)) return false; // Проверка для февраля
    }
    // Проверка для месяцев, в которых 30 дней (Апрель, Июнь, Сентябрь, Ноябрь)
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }
    // Если все проверки пройдены, дата корректна
    return true;
}

// Оператор сравнения "меньше" для двух дат
bool Date::operator<(const Date& other) const {
	if (year != other.year) return year < other.year;       // Сравнение по году
	if (month != other.month) return month < other.month;   // Сравнение по месяцу (если годы равны)
	return day < other.day;                                 // Сравнение по дню (если годы и месяцы равны)
}

// Преобразует дату в строку формата "ДД.ММ.ГГГГ"
std::string Date::toString() const {
    std::stringstream ss;
    // Форматироуем День и Месяц
    ss << std::setfill('0') << std::setw(2) << day << "."
        << std::setw(2) << month << "." << year;
    return ss.str();
}

// Выводит часть текущего каталога в консоль для контекста (первые 5 элементов)
void FileCatalog::printContextCatalog() const {
    std::cout << "\n===================== ТЕКУЩИЙ КАТАЛОГ ======================\n";

    if (catalog_.empty()) {
        std::cout << std::setw(33) << "(пусто)";
        std::cout << "\n" << std::string(60, '=') << "\n";
        return;
    }

    // Заголовки таблицы с форматированием для выравнивания
    std::cout << std::setw(30) << std::left << "Имя файла"
        << std::setw(15) << std::left << "Дата создания"
        << std::setw(15) << std::right << "Обращения" << "\n";
    std::cout << std::string(60, '-') << "\n";

    int count = 0;
    const int limit = 5; // Ограничение на вывод

    // Вывод ограниченного числа записей
    for (const auto& file : catalog_) {
        if (count >= limit) break;

        std::cout << std::setw(30) << std::left << (file.name)
            << std::setw(15) << std::left << file.creationDate.toString()
            << std::setw(15) << std::right << (std::to_string(file.accessCount)) << "\n";
        count++;
    }

    // Сообщение о пропущенных элементах
    long long remaining = catalog_.size() - count;

    if (remaining > 0) {
        std::cout << "\n... и еще " << remaining << " файл(ов) в каталоге.\n";
    }

    std::cout << std::string(60, '=') << "\n";
}


// Выводит весь каталог в форматированном виде
void FileCatalog::printCatalog() const {
    if (catalog_.empty()) {
        std::cout << "\nКаталог пуст.\n";
        return;
    }

    std::cout << "\n" << std::string(60, '=') << "\n";
    // Заголовки таблицы
    std::cout << std::setw(30) << std::left << "Имя файла"
        << std::setw(15) << std::left << "Дата создания"
        << std::setw(15) << std::right << "Обращения" << "\n";
    std::cout << std::string(60, '-') << "\n";

    // Итерация и вывод всех элементов
    for (const auto& file : catalog_) {
        std::cout << std::setw(30) << std::left << file.name
            << std::setw(15) << std::left << file.creationDate.toString()
            << std::setw(15) << std::right << file.accessCount << "\n";
    }
    std::cout << std::string(60, '=') << "\n";
}


// Формирует каталог, считывая записи с клавиатуры
void FileCatalog::initializeFromConsole() {
    // Очистка каталога перед началом формирования
    catalog_.clear();
    char choice;
    std::cout << "\n--- Формирование каталога с клавиатуры ---\n";

    do {
        FileEntry newFile;

        std::string name;
        bool flag = true;
        // Цикл ввода имени файла с проверкой корректности
        while (flag) {
            std::cout << "\nВведите имя файла (не может содержать \\ / : * ? \" < > | или пробелы): ";
            std::getline(std::cin, name);

            if (name.empty()) {
                std::cout << "Имя файла не может быть пустым.\n";
            }
            // Проверка на запрещенные символы Windows
            else if (Utils::containsForbiddenChars(name)) {
                std::cout << "Ошибка: Имя файла содержит запрещенные символы.\n";
            }
            else {
                newFile.name = name;
                flag = false;
            }
        }

        // Ввод даты и количества обращений с использованием вспомогательных функций
        newFile.creationDate = Utils::promptForDateFormatted();
        newFile.accessCount = Utils::promptForAccessCount();

        // Добавление новой записи в каталог
        catalog_.push_back(newFile);

        // Запрос на добавление еще одного файла
        flag = true;
        do {
            std::cout << "\nДобавить еще файл? (y/n): ";
            choice = Utils::getInputCharNoEnter(); // Считывание символа без Enter
            char lower_choice = std::tolower(choice);

            if (lower_choice == 'y' || lower_choice == 'n') {
                flag = false;
            }
            else {
                std::cout << "Ошибка: Некорректный ввод. Пожалуйста, нажмите 'y' или 'n'.\n";
            }
        } while (flag);

    } while (std::tolower(choice) == 'y');

    std::cout << "Формирование каталога завершено. Добавлено файлов: " << catalog_.size() << "\n";
}


// Добавляет записи в конец текущего каталога с клавиатуры
void FileCatalog::appendToConsole() {
    char choice;
    std::cout << "\n--- Дозапись в каталог с клавиатуры ---\n";

    do {
        FileEntry newFile;

        std::string name;
        bool flag = true;
        // Цикл ввода имени файла с проверкой корректности
        while (flag) {
            std::cout << "\nВведите имя файла (не может содержать \\ / : * ? \" < > | или пробелы): ";
            std::getline(std::cin, name);

            if (name.empty()) {
                std::cout << "Имя файла не может быть пустым.\n";
            }
            else if (Utils::containsForbiddenChars(name)) {
                std::cout << "Ошибка: Имя файла содержит запрещенные символы.\n";
            }
            else {
                newFile.name = name;
                flag = false;
            }
        }

        // Ввод даты и количества обращений
        newFile.creationDate = Utils::promptForDateFormatted();
        newFile.accessCount = Utils::promptForAccessCount();

        // Добавление новой записи в конец списка
        catalog_.push_back(newFile);

        // Запрос на добавление еще одного файла
        flag = true;
        do {
            std::cout << "\nДобавить еще файл? (y/n): ";
            choice = Utils::getInputCharNoEnter();

            char lower_choice = std::tolower(choice);

            if (lower_choice == 'y' || lower_choice == 'n') {
                flag = false;
            }
            else {
                std::cout << "Ошибка: Некорректный ввод. Пожалуйста, нажмите 'y' или 'n'.\n";
            }
        } while (flag);

    } while (std::tolower(choice) == 'y');

    std::cout << "Дозапись в каталог завершена. Общее количество файлов: " << catalog_.size() << "\n";
}


// Формирует каталог, считывая записи из файла (полная очистка перед началом)
void FileCatalog::initializeFromFile() {
    // Очистка текущего каталога
    catalog_.clear();
    std::string filename;

    std::cout << "\n--- Запуск формирования из файла ---\n";
    bool flag = true;
    // Цикл для ввода имени файла
    while (flag) {
        std::cout << "\nВведите имя файла каталога (например, input.txt или input): ";
        std::getline(std::cin, filename);

        // Проверка на пустой ввод или ввод, состоящий только из пробелов
        if (Utils::trim(filename).empty()) {
            std::cout << "Имя файла не может быть пустым. Пожалуйста, введите корректное имя.\n";
        }
        else {
            flag = false;
        }
    }

    // Добавление расширения .txt, если оно отсутствует
    if (filename.length() < 4 || filename.substr(filename.length() - 4) != ".txt") {
        filename += ".txt";
    }

    std::ifstream file(filename);
    int count = 0;

    std::cout << "\n--- Формирование каталога из файла (" << filename << ") ---\n";

    // Проверка успешности открытия файла
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << filename << ". Убедитесь, что он существует и доступен.\n";
        return;
    }

    std::string line;
    // Построчное чтение файла
    while (std::getline(file, line)) {

        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> seglist;

        // Разделение строки на сегменты по разделителю '/'
        while (std::getline(ss, segment, '/')) {
            // Удаление начальных/конечных пробелов из каждого сегмента
            seglist.push_back(Utils::trim(segment));
        }

        // Проверка, что считано ровно 3 поля (Имя / Дата / Обращения)
        if (seglist.size() != 3) {
            std::cerr << "Предупреждение: Некорректный формат строки (ожидается 3 поля, разделенных '/'): \"" << line << "\". Пропуск записи.\n";
            continue;
        }

        FileEntry newFile;
        newFile.name = seglist[0];

        // Парсинг даты (ДД.ММ.ГГГГ)
        try {
            std::string date_str = seglist[1];

            // Базовая проверка формата даты
            if (date_str.length() != 10 || date_str[2] != '.' || date_str[5] != '.') {
                throw std::runtime_error("Incorrect date format length or separators.");
            }

            // Извлечение компонентов даты
            newFile.creationDate.day = std::stoi(date_str.substr(0, 2));
            newFile.creationDate.month = std::stoi(date_str.substr(3, 2));
            newFile.creationDate.year = std::stoi(date_str.substr(6, 4));

            // Проверка логической корректности даты
            if (!newFile.creationDate.isValid()) {
                throw std::runtime_error("Date is not valid.");
            }
        }
        catch (const std::exception&) {
            std::cerr << "Предупреждение: Некорректный формат или значение даты: \"" << seglist[1] << "\". Пропуск записи.\n";
            continue;
        }

        // Парсинг количества обращений
        try {
            // Преобразование строки в long long
            newFile.accessCount = std::stoll(seglist[2]);
            // Проверка на неотрицательное значение
            if (newFile.accessCount < 0) {
                throw std::out_of_range("Negative count");
            }
        }
        catch (const std::exception&) {
            std::cerr << "Предупреждение: Некорректное число обращений: \"" << seglist[2] << "\". Пропуск записи.\n";
            continue;
        }

        // Добавление успешно распарсенной записи
        catalog_.push_back(newFile);
        count++;
    }

    std::cout << "Чтение из файла завершено. Добавлено файлов: " << count << "\n";
}


// Удаляет файлы, дата создания которых МЕНЬШЕ заданной даты
void FileCatalog::deleteByDate() {
    if (catalog_.empty()) {
        std::cout << "\nКаталог пуст, удалять нечего.\n";
        return;
    }

    std::cout << "\n--- Удаление файлов по дате ---\n";
    std::cout << "Будут удалены все файлы, дата создания которых МЕНЬШЕ заданной.\n";
    // Запрос даты-ограничения
    Date limitDate = Utils::promptForDateFormatted();

    std::vector<std::string> deletedNames;
    // Временный список для хранения элементов, которые останутся
    std::list<FileEntry> newCatalog;

    // Итерация по старому каталогу и фильтрация
    for (const auto& file : catalog_) {
        // Если дата создания меньше лимита, имя добавляется в список удаленных
        if (file.creationDate < limitDate) {
            deletedNames.push_back(file.name);
        }
        // В противном случае элемент переносится в новый каталог
        else {
            newCatalog.push_back(file);
        }
    }

    // Замена старого каталога новым
    catalog_ = std::move(newCatalog);

    int deletedCount = deletedNames.size();
    std::cout << "Удалено файлов: " << deletedCount << ".\n";

    // Вывод списка удаленных файлов, если таковые были
    if (deletedCount > 0) {
        std::cout << "Удаленные файлы:\n";
        for (const auto& name : deletedNames) {
            std::cout << " - " << name << "\n";
        }
    }
}


// Находит и выводит файл с наибольшим количеством обращений
void FileCatalog::findMostAccessed() const {
    if (catalog_.empty()) {
        std::cout << "\nКаталог пуст.\n";
        return;
    }

    // Поиск элемента с максимальным количеством обращений
    auto maxIt = std::max_element(catalog_.begin(), catalog_.end(),
        [](const FileEntry& a, const FileEntry& b) {
            return a.accessCount < b.accessCount; // Компаратор для поиска максимума
        });

    // Проверка, что каталог не пуст
    if (maxIt != catalog_.end()) {
        std::cout << "\n--- Файл с наибольшим количеством обращений ---\n";
        std::cout << "Имя файла: " << maxIt->name << "\n";
        std::cout << "Дата создания: " << maxIt->creationDate.toString() << "\n";
        std::cout << "Количество обращений: " << maxIt->accessCount << "\n";
        std::cout << "------------------------------------------------\n";
    }
}


// Сохраняет текущий каталог в указанный файл с проверкой перезаписи
void FileCatalog::saveCatalogToFile() const {
    if (catalog_.empty()) {
        std::cout << "\nКаталог пуст, сохранять нечего.\n";
        return;
    }

    std::string filename;
    std::cout << "\n--- Сохранение каталога в файл ---\n";

    bool flag = true;
    // Цикл для ввода имени файла
    while (flag) {
        std::cout << "Введите имя файла для сохранения (например, output.txt или output): ";
        std::getline(std::cin, filename);

        // Проверка, что ввод не состоит только из пробелов
        if (Utils::trim(filename).empty()) {
            std::cout << "Имя файла не может быть пустым. Пожалуйста, введите корректное имя.\n\n";
        }
        else {
            flag = false;
        }
    }

    // Добавление расширения .txt, если отсутствует
    if (filename.length() < 4 || filename.substr(filename.length() - 4) != ".txt") {
        filename += ".txt";
    }

    // Проверка существования файла путем попытки его открытия для чтения
    std::ifstream checkFile(filename);
    if (checkFile.is_open()) {
        checkFile.close(); // Закрываем файл, так как он открыт только для проверки

        char choice;
        // Запрос подтверждения на перезапись
        flag = true;
        do {
            std::cout << "Файл '" << filename << "' уже существует. Перезаписать? (y/n): ";
            choice = Utils::getInputCharNoEnter();
            char lower_choice = std::tolower(choice);

            if (lower_choice == 'n') {
                std::cout << "\nСохранение отменено.\n";
                return;
            }
            else if (lower_choice == 'y') {
                flag = false;
            }
            else {
                std::cout << "Ошибка: Некорректный ввод. Пожалуйста, нажмите 'y' или 'n'.\n";
            }
        } while (flag);
    }

    // Открытие файла для записи
    std::ofstream file(filename);
    int count = 0;

    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось создать или открыть файл " << filename << " для записи.\n";
        return;
    }

    // Запись данных в файл в формате: Имя / ДД.ММ.ГГГГ / Обращения
    for (const auto& entry : catalog_) {
        file << entry.name << " / "
            << entry.creationDate.toString() << " / "
            << entry.accessCount << "\n";
        count++;
    }

    file.close(); // Закрытие файла
    std::cout << "\nКаталог успешно сохранен в файл: " << filename << ". Создано записей: " << count << ".\n";
}