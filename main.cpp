#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include<bits/stdc++.h>
#include<cctype>

using namespace std;


struct Author {
    char authorID[15];
    char authorName[30];
    char address[30];
};

struct Book {
    char ISBN[15];
    char bookTitle[30];
    char authorID[15];
};

void addAuthor(const Author& author);
void addBook(const Book& book);
void updateAuthorName(const char* authorID, const char* newName);
void updateBookTitle(const char* ISBN, const char* newTitle);
void deleteAuthor(const char* authorID);
void deleteBook(const char* ISBN);
void printAuthor(const char* authorID);
void printBook(const char* ISBN);
void writeQuery(const char* query);





int main() {
    cout<<"Hello World"<<endl;

    return 0;
}