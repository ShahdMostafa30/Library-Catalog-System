#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include<bits/stdc++.h>
#include<cctype>

using namespace std;


struct Author {
    char authorID[16];
    char authorName[31];
    char address[31];
};

struct Book {
    char ISBN[16];
    char bookTitle[31];
    char authorID[16];
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