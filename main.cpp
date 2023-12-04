
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


// Function declarations
bool doesIDExist(ifstream& primary, int x);
void insertAuthorPrimary(char id[], short offset);
void insertBookPrimary(char id[], short offset);
void insertAuthorName(char name[], char ID[]);
void insertAuthorID(char authorID[], char ISBN[]);
void addAuthor(Author author);
void addBook(Book book);

//////////////////////////////
void deleteBookPrimary(char id[]);
void deleteAuthorPrimary(char id[]);



int main() {
    

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
/**
 * Inserts a new record into the linked list and secondary index files for authors.
 *
 * @param name The name of the author to be inserted
 * @param ID The ID of the author
 */

void insertAuthorName(char name[], char ID[]) {
    vector<pair<string, string>> data;
    ifstream file("SecondaryIndexAuthor.txt", ios::ate);

    // Check if the file is empty
    if (file.tellg() == 0) {
        // If file is empty, create and write initial records
        ofstream secFile("SecondaryIndexAuthor.txt");
        secFile << name << ' ' << 0 << '\n';
        secFile.close();

        ofstream llFile("LLAuthor.txt");
        llFile << 0 << ' ' << ID << ' ' << -1 << '\n';
        llFile.close();
    } else {
        // File not empty, read and process records
        ifstream secFile("SecondaryIndexAuthor.txt");
        ifstream llFile("LLAuthor.txt");
        string n, id;
        int secPointer, llPointer, x;

        // Loop through records in SecondaryIndexAuthor.txt
        while (secFile >> n >> secPointer) {
            llFile.clear();
            llFile.seekg(0, ios::beg);

            // Match found in SecondaryIndexAuthor.txt
            while (llFile >> llPointer >> id >> x) {
                if (secPointer == llPointer) {
                    data.push_back(make_pair(n, id));

                    // If x is not -1, continue pushing IDs until x is -1
                    while (x != -1) {
                        llFile >> llPointer >> id >> x;
                        data.push_back(make_pair(n, id));
                    }

                    break; // Stop after processing IDs with x = -1
                }
            }
        }

        secFile.close();
        llFile.close();

        // Add the new name and ID
        data.push_back(make_pair(name, ID));

        // Define a lambda comparator to sort based on pair.first
        auto compareFirst = [](const auto& a, const auto& b) {
            return a.first < b.first;
        };
        // Sort the vector based on pair.first
        sort(data.begin(), data.end(), compareFirst);

        // Write sorted data back to files
        ofstream secFile1("SecondaryIndexAuthor.txt", ios::trunc);
        ofstream llFile1("LLAuthor.txt", ios::trunc);

        // Write to SecondaryIndexAuthor.txt and LLAuthor.txt
        for (int i = 0; i < data.size(); ++i) {
            if (i == 0 || data[i].first != data[i - 1].first)
                secFile1 << data[i].first << ' ' << i << '\n';

            if (data[i].first == data[i + 1].first)
                llFile1 << i << ' ' << data[i].second << ' ' << i + 1 << '\n';
            else
                llFile1 << i << ' ' << data[i].second << ' ' << -1 << '\n';
        }

        // Display the sorted data
        for (int i = 0; i < data.size(); ++i) {
            cout << data[i].first << ' ' << data[i].second << '\n';
        }
    }
}
/**
 * Inserts a new record into the linked list and secondary index files for books.
 *
 * @param authorID The ID of the author to be inserted
 * @param ISBN The ISBN of the book associated with the author
 */

void insertAuthorID(char authorID[], char ISBN[]) {
    vector<pair<string, string>> data;
    ifstream file("SecondaryIndexBook.txt", ios::ate);

    // Check if the file is empty
    if (file.tellg() == 0) {
        // If file is empty, create and write initial records
        ofstream secFile("SecondaryIndexBook.txt");
        secFile << authorID << ' ' << 0 << '\n';
        secFile.close();

        ofstream llFile("LLBook.txt");
        llFile << 0 << ' ' << ISBN << ' ' << -1 << '\n';
        llFile.close();
    } else {
        // File not empty, read and process records
        ifstream secFile("SecondaryIndexBook.txt");
        ifstream llFile("LLBook.txt");
        string autID, isbn;
        int secPointer, llPointer, x;

        // Loop through records in SecondaryIndexAuthor.txt
        while (secFile >> autID >> secPointer) {
            llFile.clear();
            llFile.seekg(0, ios::beg);

            // Match found in SecondaryIndexAuthor.txt
            while (llFile >> llPointer >> isbn >> x) {
                if (secPointer == llPointer) {
                    data.push_back(make_pair(autID, isbn));

                    // If x is not -1, continue pushing IDs until x is -1
                    while (x != -1) {
                        llFile >> llPointer >> isbn >> x;
                        data.push_back(make_pair(autID, isbn));
                    }

                    break; // Stop after processing IDs with x = -1
                }
            }
        }

        secFile.close();
        llFile.close();

        // Add the new name and ID
        data.push_back(make_pair(authorID, ISBN));

        // Define a lambda comparator to sort based on pair.first
        auto compareFirst = [](const auto& a, const auto& b) {
            return a.first < b.first;
        };
        // Sort the vector based on pair.first
        sort(data.begin(), data.end(), compareFirst);

        // Write sorted data back to files
        ofstream secFile1("SecondaryIndexBook.txt", ios::trunc);
        ofstream llFile1("LLBook.txt", ios::trunc);

        // Write to SecondaryIndexBook.txt and LLBook.txt
        for (int i = 0; i < data.size(); ++i) {
            if (i == 0 || data[i].first != data[i - 1].first)
                secFile1 << data[i].first << ' ' << i << '\n';

            if (data[i].first == data[i + 1].first)
                llFile1 << i << ' ' << data[i].second << ' ' << i + 1 << '\n';
            else
                llFile1 << i << ' ' << data[i].second << ' ' << -1 << '\n';
        }

        // Display the sorted data
        for (int i = 0; i < data.size(); ++i) {
            cout << data[i].first << ' ' << data[i].second << '\n';
        }
    }
}

void addAuthor(Author author){

}

void deleteBookPrimary(char id[]){
    ifstream primary("PrimaryIndexBook.txt");
    int x = atoi(id);

    if (!doesIDExist(primary, x)) {
        cout << "ID doesn't exist!" << endl;
        return;
    }

    primary.close();

    // Delete from PrimaryIndexBook.txt
    vector<pair<int, int>> data;
    ifstream readPrimary("PrimaryIndexBook.txt");
    int ID, of;

    while (readPrimary >> ID >> of) {
        if (ID != x)
            data.push_back(make_pair(ID, of));
    }
    readPrimary.close();

    ofstream writePrimary("PrimaryIndexBook.txt", ios::trunc);

    for (int i = 0; i < data.size(); ++i) {
        writePrimary << data[i].first << ' ' << data[i].second << '\n';
    }
    writePrimary.close();
}


void deleteAuthorPrimary(char id[]){
    ifstream primary("PrimaryIndexAuthor.txt");
    int x = stoi(id);
    if(!doesIDExist(primary , x)){
        cout << "ID doesn't exsit!";
        return;
    }

    primary.close();

    vector<pair<int , int>> data;
    ifstream readPrimary("PrimaryIndexAuthor.txt");
    int ID , of;
    while(readPrimary >> ID >> of){
        if (ID != x)
            data.push_back(make_pair(ID, of));
    }
    readPrimary.close();


    ofstream writePrimary("PrimaryIndexAuthor.txt");
    for(int i = 0 ; i < data.size(); i++){
        writePrimary << data[i].first << ' ' << data[i].second << '\n';
    }
    writePrimary.close();
}