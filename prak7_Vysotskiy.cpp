// prak7_Vysotskiy.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "Header.h"
#include <iostream>
#include <string>
#include <fstream>
#include <windows.h>
#include <stdexcept>

using namespace std;

const string FILE_NAME = "data.txt";


#ifdef _DEBUG
#define DEBUG(x) cout << "[DEBUG] " << x << endl;
#else
#define DEBUG(x)
#endif


int loadLibrary(Book books[])
{
    ifstream file(FILE_NAME);

    if (!file.is_open())
    {
        throw runtime_error("Не удалось открыть файл");
    }
    int count = 0;
    string line;

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        if (count >= MAX_BOOKS)
        {
            throw runtime_error("Слишком много книг");
        }
        try
        {
            int p1 = line.find('|');
            int p2 = line.find('|', p1 + 1);
            int p3 = line.find('|', p2 + 1);
            int p4 = line.find('|', p3 + 1);

            if (p1 == -1 || p2 == -1 || p3 == -1 || p4 == -1)
            {
                throw runtime_error("Неверный формат строки");
            }
            books[count].title = line.substr(0, p1);
            books[count].author = line.substr(p1 + 1, p2 - p1 - 1);
            books[count].year = stoi(line.substr(p2 + 1, p3 - p2 - 1));
            books[count].inventory = stoi(line.substr(p3 + 1, p4 - p3 - 1));

            string available = line.substr(p4 + 1);

            if (available[0] == '1')
            {
                books[count].available = true;
            }
            else if (available[0] == '0')
            {
                books[count].available = false;
            }
            else
            {
                throw runtime_error("Ошибка в наличии");
            }
            if (books[count].title.empty() || books[count].author.empty() ||
                books[count].year <= 0 || books[count].inventory <= 0)
            {
                throw runtime_error("Неправильные данные книги");
            }

            DEBUG("Загружена книга: " << books[count].title);

            count++;
        }
        catch (const invalid_argument&)
        {
            throw runtime_error("Ошибка в числовых данных");
        }
        catch (const out_of_range&)
        {
            throw runtime_error("Число слишком большое");
        }
    }
    file.close();
    return count;
}

void saveLibrary(Book books[], int count)
{
    ofstream file(FILE_NAME);

    if (!file.is_open())
    {
        throw runtime_error("Не удалось открыть файл для записи");
    }
    for (int i = 0; i < count; i++)
    {
        file << books[i].title << "|"
            << books[i].author << "|"
            << books[i].year << "|"
            << books[i].inventory << "|"
            << (books[i].available) << endl;
    }
    file.close();
    DEBUG("Данные сохранены");
}

void showBook(Book book)
{
    cout << "Название: " << book.title << endl;
    cout << "Автор: " << book.author << endl;
    cout << "Год: " << book.year << endl;
    cout << "Инвентарный номер: " << book.inventory << endl;
    cout << "Наличие: ";
    if (book.available)
    {
        cout << "есть";
    }
    else
    {
        cout << "выдана";
    }
    cout << endl;
}

void findTitle(Book books[], int count)
{
    string title;
    cout << "Введите название книги: ";
    getline(cin, title);
    bool found = false;
    for (int i = 0; i < count; i++)
    {
        cout << books[i].title << endl;
        if (books[i].title == title)
        {
            cout << "Книга: " << endl;
            showBook(books[i]);
            cout << "-----------" << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "Книга не найдена" << endl;
        cout << "-----------" << endl;
    }
}

void findAuthor(Book books[], int count)
{
    string author;
    cout << "Введите автора: ";
    getline(cin, author);
    bool found = false;
    for (int i = 0; i < count; i++)
    {
        if (books[i].author == author)
        {
            showBook(books[i]);
            cout << "-----------" << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "Книги этого автора не найдены" << endl;
        cout << "-----------" << endl;
    }
}

void showAvaible(Book books[], int count)
{
    bool found = false;
    cout << "Доступные книги:" << endl;
    for (int i = 0; i < count; i++)
    {
        if (books[i].available)
        {
            showBook(books[i]);
            cout << "-----------" << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "Доступных книг нет" << endl;
        cout << "-----------" << endl;
    }
}

void giveBook(Book books[], int count)
{
    int inventory;
    cout << "Введите инвентарный номер: ";
    cin >> inventory;
    cin.ignore();
    for (int i = 0; i < count; i++)
    {
        if (books[i].inventory == inventory)
        {
            if (!books[i].available)
            {
                cout << "Книга уже выдана" << endl;
                cout << "-----------" << endl;
                return;
            }

            books[i].available = false;

            cout << "Книга выдана" << endl;
            cout << "-----------" << endl;
            return;
        }
    }
    cout << "Книга не найдена" << endl;
    cout << "-----------" << endl;
}

void returnBook(Book books[], int count)
{
    int inventory;
    cout << "Введите инвентарный номер: ";
    cin >> inventory;
    cin.ignore();
    for (int i = 0; i < count; i++)
    {
        if (books[i].inventory == inventory)
        {
            if (books[i].available)
            {
                cout << "Книга уже находится в библиотеке" << endl;
                cout << "-----------" << endl;
                return;
            }
            books[i].available = true;
            cout << "Книга возвращена" << endl;
            cout << "-----------" << endl;
            return;
        }
    }
    cout << "Книга не найдена" << endl;
    cout << "-----------" << endl;
}

void library(Book books[], int count)
{
    int choice;
    do
    {
        try
        {
            cout << "Билбиотека" << endl;
            cout << "1. Поиск по названию" << endl;
            cout << "2. Поиск по автору" << endl;
            cout << "3. Доступные книги" << endl;
            cout << "4. Выдать книгу" << endl;
            cout << "5. Вернуть книгу" << endl;
            cout << "6. Сохранить" << endl;
            cout << "0. Выход" << endl;

            cout << "Выберите действие: ";
            cin >> choice;
            cin.ignore();
            cout << "-----------------------" << endl;
       
            if (choice == 1)findTitle(books, count);
            else if (choice == 2)findAuthor(books, count);
            else if (choice == 3)showAvaible(books, count);
            else if (choice == 4)giveBook(books, count);
            else if (choice == 5)returnBook(books, count);
            else if (choice == 6)saveLibrary(books, count);
            else if (choice == 0)saveLibrary(books, count);
            else cout << "Неверный выбор" << endl;
        }
        catch (const exception& e)
        {
            cout << "Ошибка: " << e.what() << endl;
        }
    } while (choice != 0);
}


int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, "RU");
    Book books[MAX_BOOKS];
    try
    {
        int count = loadLibrary(books);

        cout << "Загружено книг: " << count << endl;

        library (books, count);
    }
    catch (const exception& e)
    {
        cout << "Ошибка: " << e.what() << endl;
    }

    return 0;
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
