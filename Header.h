#pragma once

#include <stdexcept>
#include <string>

using namespace std;

const int MAX_BOOKS = 100;

struct Book
{
    string title;
    string author;
    int year;
    int inventory;
    bool available;
};

int loadLibrary(Book books[]);

void saveLibrary(Book books[], int count);

void findTitle(Book books[], int count);

void findAuthor(Book books[], int count);

void showAvaible(Book books[], int count);

void giveBook(Book books[], int count);

void returnBook(Book books[], int count);

void library(Book books[], int count);;