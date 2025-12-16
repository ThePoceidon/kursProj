#include <iostream>
#include <cstdlib>
#include "utils.h"
#include "fileCatalog.h"

// Обработка формирования каталога
void handleInitializationMenu(FileCatalog& catalog) {
    int choice = -1;

    while (choice != 0) {
        system("cls"); // Очистка консоли
        catalog.printContextCatalog();

        std::cout << "\n\n==============================================\n";
        std::cout << "          МЕНЮ: ФОРМИРОВАНИЕ КАТАЛОГА         \n";
        std::cout << "==============================================\n";
        std::cout << "1. Создание каталога          ( с клавиатуры )\n";
        std::cout << "2. Дозапись в каталог         ( с клавиатуры ) \n";
        std::cout << "3. Создание каталога          (   из файла   ) \n";
        std::cout << "0. Вернуться в главное меню \n";
        std::cout << "----------------------------------------------\n";
        std::cout << "Выберите пункт (нажмите цифру): ";

        choice = Utils::getSubMenuChoice();

        switch (choice) {
        case 1:
            system("cls");
            catalog.initializeFromConsole();
            break;
        case 2:
            system("cls");
            catalog.appendToConsole();
            break;
        case 3:
            system("cls");
            catalog.initializeFromFile();
            break;
        case 0:
            // Выход из этого подменю
            return;
        default:
            break;
        }

        // Пауза перед возвратом в подменю
        std::cout << "\nНажмите любую клавишу для продолжения...\n";
        Utils::waitKeyPress();
    }
}

// Обработка вывода каталога
void handleDisplayCatalog(const FileCatalog& catalog) {
    system("cls");

    std::cout << "\n==============================================\n";
    std::cout << "                ВЫВОД КАТАЛОГА                \n";
    std::cout << "==============================================\n";

    catalog.printCatalog();

    std::cout << "\nНажмите любую клавишу для возврата в главное меню...\n";
    Utils::waitKeyPress();
}

// Обработка удаления
void handleDeletion(FileCatalog& catalog) {
    system("cls");
    catalog.printContextCatalog();

    std::cout << "\n\n==============================================\n";
    std::cout << "                  УДАЛЕНИЕ                    \n";
    std::cout << "==============================================\n";

    catalog.deleteByDate();

    std::cout << "\nНажмите любую клавишу для возврата в главное меню...\n";
    Utils::waitKeyPress();
}

// Обработка поиска
void handleFinding(const FileCatalog& catalog) {
    system("cls");
    catalog.printContextCatalog();

    std::cout << "\n==============================================\n";
    std::cout << "                    ПОИСК                     \n";
    std::cout << "==============================================\n";

    catalog.findMostAccessed();

    std::cout << "\nНажмите любую клавишу для возврата в главное меню...\n";
    Utils::waitKeyPress();
}

// Обработка сохранения
void handleSaving(const FileCatalog& catalog) {
    system("cls");
    catalog.printContextCatalog();

    std::cout << "\n==============================================\n";
    std::cout << "                  СОХРАНЕНИЕ                  \n";
    std::cout << "==============================================\n";

    catalog.saveCatalogToFile();

    std::cout << "\nНажмите любую клавишу для возврата в главное меню...\n";
    Utils::waitKeyPress();
}


// Главное меню
void displayMainMenu(const FileCatalog& catalog) {
    catalog.printContextCatalog();

    std::cout << "\n\n==============================================\n";
    std::cout << "                 ГЛАВНОЕ МЕНЮ                 \n";
    std::cout << "==============================================\n";
    std::cout << "1. Формирование каталога (меню) \n";
    std::cout << "2. Вывод текущего каталога \n";
    std::cout << "3. Удалить файлы по дате \n";
    std::cout << "4. Найти файл с наиб. количеством обращений \n";
    std::cout << "5. Сохранить каталог в файл \n";
    std::cout << "0. Выход \n";
    std::cout << "----------------------------------------------\n";
    std::cout << "Выберите пункт (нажмите цифру): ";
}

// Приветственный экран
void displaySplashScreen() {
    system("cls");

    std::cout << "\n======================================================\n";
    std::cout << "                   КУРСОВОЙ ПРОЕКТ                    \n";
    std::cout << "======================================================\n\n";
    std::cout << "              Выполнил: Помозин В. В.\n";
    std::cout << "                Группа: М7О-407С-22\n\n";
    std::cout << "------------------------------------------------------\n";
    std::cout << "\nНажмите любую клавишу для продолжения...\n";

    Utils::waitKeyPress();
    return;
}

int main() {
    // Настройка кодировки для Windows
    Utils::setupConsoleEncoding();

    displaySplashScreen();

    FileCatalog fileCatalog;
    int choice = -1;

    while (choice != 0) {
        system("cls");
        displayMainMenu(fileCatalog);
        choice = Utils::getMenuChoice();

        switch (choice) {
        case 1:
            handleInitializationMenu(fileCatalog);
            break;
        case 2:
            handleDisplayCatalog(fileCatalog);
            break;
        case 3:
            handleDeletion(fileCatalog);
            break;
        case 4:
            handleFinding(fileCatalog);
            break;
        case 5:
			handleSaving(fileCatalog);
            break;
        case 0:
            system("cls");
            std::cout << "Завершение работы программы.\n";
            break;
        default:
            std::cout << "\nНажмите любую клавишу для продолжения...\n";
            Utils::waitKeyPress();
            break;
        }
    }

    return 0;
}