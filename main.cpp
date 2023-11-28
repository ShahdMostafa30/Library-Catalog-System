
#include<bits/stdc++.h>

using namespace std;

//Files:
//For Primary index -> PrimaryIndexAuthor , PrimaryIndexBook -> ( ID, offset)
//For Secondary index -> SecondaryIndexAuthor ->(Name, Pointer)
//                    -> SecondaryIndexBook ->(AuthorID, pointer)
//For Linked List -> LLAuthor ->(#, ID(for author), Pointer)
//                -> LLBook   ->(#, ID(for book), Pointer)


struct Author {
    char authorID[15];
    char authorName[30];
    char address[30];
};

struct Book {
    char ISBN[15];
    char bookTitle[30];
    char authorID[30];
};

struct ReferenceNode {
    int idPointer;
    int nextReferenceOffset;
};

struct SecondaryIndexNode {
    char name[30];
    int referenceListOffset;
};

// Function declarations

bool doesIDExist(fstream& primary, int x);
void insertAuthorPrimary(char id[], short offset);
void insertBookPrimary(char id[], short offset);
void insertAuthorName(char name[30], char ID[15]);
void sortSecondaryIndex(const char* filename);
void insertAuthorName(char name[30], char ID[15]);


int main() {
    char ID[15] = "0";
        insertAuthorPrimary(ID, 20);
    return 0;
}
/**
 * Checks if a given ID already exists in the provided primary index file.
 *
 * @param primary The ifstream of the primary index file
 * @param x The ID to check for existence
 * @return true if the ID already exists, false otherwise
 */
bool doesIDExist(ifstream& primary, int x) {
    int tmp = 0;
    short of = 0;

    primary.seekg(0, ios::beg);

    while (primary >> tmp >> of) {
        if (tmp == x) {
            return true; // ID already exists
        }
    }

    return false; // ID does not exist
}
/**
 * Inserts a new record into the primary index file for authors.
 *
 * @param id The ID of the author to be inserted
 * @param offset The offset of the author record
 */
void insertAuthorPrimary(char id[], short offset) {
    ifstream primary("PrimaryIndexAuthor.txt");
    int x = atoi(id);

    if (doesIDExist(primary, x)) {
        cout << "ID already exists!" << endl;
        return;
    }

    primary.close();

    // Read all records
    vector<pair<int, short>> records;

    ifstream readPrimary("PrimaryIndexAuthor.txt");
    int tmp;
    short of;

    while (readPrimary >> tmp >> of) {
        records.push_back(make_pair(tmp, of));
    }

    readPrimary.close();

    // Insert the new record into the correct position
    auto it = records.begin();
    while (it != records.end() && it->first < x) {
        ++it;
    }

    records.insert(it, make_pair(x, offset));

    // Rewrite all records into the file in sorted order
    ofstream writePrimary("PrimaryIndexAuthor.txt", ios::trunc);

    for (const auto& record : records) {
        writePrimary << record.first << ' ' << record.second << '\n';
    }

    writePrimary.close();
}
/**
 * Inserts a new record into the primary index file for books.
 *
 * @param id The ID of the book to be inserted
 * @param offset The offset of the book record
 */
void insertBookPrimary(char id[], short offset){
    ifstream primary("PrimaryIndexBook.txt");
    int x = atoi(id);

    if (doesIDExist(primary, x)) {
        cout << "ID already exists!" << endl;
        return;
    }

    primary.close();

    // Read all records
    vector<pair<int, short>> records;

    ifstream readPrimary("PrimaryIndexBook.txt");
    int tmp;
    short of;

    while (readPrimary >> tmp >> of) {
        records.push_back(make_pair(tmp, of));
    }

    readPrimary.close();

    // Insert the new record into the correct position
    auto it = records.begin();
    while (it != records.end() && it->first < x) {
        ++it;
    }

    records.insert(it, make_pair(x, offset));

    // Rewrite all records into the file in sorted order
    ofstream writePrimary("PrimaryIndexBook.txt", ios::trunc);

    for (const auto& record : records) {
        writePrimary << record.first << ' ' << record.second << '\n';
    }

    writePrimary.close();
}


bool isExist(char name[30]){
    ifstream readSec("SecondaryIndexAuthor");
    char nameRec[30];
    int pointer;
    while(readSec >> nameRec >> pointer){
        if(strcmp(name, nameRec) == 0)
            readSec.close();
            return true;
    }
    readSec.close();
    return false;
}
void insertAuthorName(char name[30], char ID[15]) {
    vector<pair<string,int>>linkedList;
    //load vector from file
    ifstream load("LLAuthor");
    int hash,pointer;
    string idRec;
    while(load >> hash >> idRec >> pointer){
        linkedList.push_back(make_pair(idRec, pointer));
    }
    load.close();

    int id = atoi(ID);

    if(isExist(name)){

     }
    else{


     }
}

