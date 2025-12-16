#pragma once

#include <string>
#include <iostream>
#include <limits>
#include <conio.h>
#include <windows.h>
#include "fileCatalog.h"

// Класс для вспомогательных функций и работы с i/o
class Utils {
public:

    // Проверяет строку на наличие запрещенных символов
    static bool containsForbiddenChars(const std::string& name);

    // Удаляет начальные и конечные пробельные символы из строки
    static std::string trim(const std::string& str);

    // Запрашивает и контролирует ввод количества обращений (long long >= 0)
    static long long promptForAccessCount();

    // Запрашивает дату с клавиатуры в форматированном виде (ДД.ММ.ГГГГ)
    static Date promptForDateFormatted();

    // Считывает один символ с клавиатуры
    static char getInputCharNoEnter();

    // Запрашивает выбор пункта меню (0-5)
    static int getMenuChoice();

    // Запрашивает выбор пункта подменю (0-3)
    static int getSubMenuChoice();

    // Ожидает нажатия любой клавиши
    static void waitKeyPress();

    // Выполняет очистку буфера ввода
    static void clearInput();

    // Устанавливает кодировку консоли для корректного отображения русского языка
    static void setupConsoleEncoding();
};