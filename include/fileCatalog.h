#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>

// Структура для даты
struct Date {
    int day = 0;
    int month = 0;
    int year = 0;

    // Проверяет корректность даты
    bool isValid() const;

    // Оператор сравнения "меньше" для двух дат

    bool operator<(const Date& other) const;

    // Преобразует дату в строку формата "ДД.ММ.ГГГГ"

    std::string toString() const;
};

// Структура для записи файла каталога
struct FileEntry {
    std::string name;
    Date creationDate;
    long long accessCount = 0;
};

// Класс для управления каталогом файлов
class FileCatalog {
public:
    FileCatalog() = default;

    //Выводит часть текущего каталога в консоль для контекста
    void printContextCatalog() const;

    //Выводит весь каталог в форматированном виде
    void printCatalog() const;

    //Формирует каталог, считывая записи с клавиатуры (полная очистка перед началом)
    void initializeFromConsole();

    //Добавляет записи в конец текущего каталога с клавиатуры
    void appendToConsole();

    //Формирует каталог, считывая записи из файла (полная очистка перед началом)
    void initializeFromFile();

    //Удаляет файлы, дата создания которых МЕНЬШЕ заданной даты
    void deleteByDate();

    //Находит и выводит файл с наибольшим количеством обращений
    void findMostAccessed() const;

    //Сохраняет текущий каталог в указанный файл с проверкой перезаписи
    void saveCatalogToFile() const;

    //Проверяет, пуст ли каталог
    bool isEmpty() const {
        return catalog_.empty();
    }

private:
    std::list<FileEntry> catalog_;
};