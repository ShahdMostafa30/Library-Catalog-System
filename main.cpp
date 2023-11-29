#include<bits/stdc++.h>
#include <fstream>
using namespace std;

//Files:
//For Primary index -> PrimaryIndexAuthor , PrimaryIndexBook -> ( ID, offset)
//For Secondary index -> SecondaryIndexAuthor ->(Name, Pointer)
//                    -> SecondaryIndexBook ->(AuthorID, pointer)
//For Linked List -> LLAuthor ->(#, ID(for author), Pointer)
//                -> LLBook   ->(#, ID(for book), Pointer)

bool getAuthorByID(int id, fstream &indexFile); // search for the author by id and print the author record
bool getBookByISBN(int isbn, fstream &indexFile); // search for the book by isbn and print the book record


int main() {

    fstream primary("PrimaryIndexBook.txt", ios::in);
    int n; cin >> n;
    getBookByISBN(n, primary);
    primary.close();
    return 0;
}
// A function that takes the authorID as a parameter and binary search for the authorID in the PrimaryIndexAuthor file
// and get the offset of the author record then seek to the offset in the Author file and read the author record
bool getAuthorByID(int id, fstream &indexFile) {
        // open the PrimaryIndexAuthor file in the read mode
        // count the number of records in the PrimaryIndexAuthor file
        int count = 0;
        string line;
        while (getline(indexFile, line))
        {
            count++;
        }
        // loop through the PrimaryIndexAuthor file
        // and get each id and compare it with the id parameter
        indexFile.clear();
        indexFile.seekg(0, ios::beg);
        // store records into a map to sort them
        vector<pair<int, int>> records;
        for(int i = 0; i < count; i++) {
            if(getline(indexFile, line)) {
                int currentID, currentOffset;
                // before the space is the id
                // after the space is the offset
                currentID = stoi(line.substr(0, line.find(' ')));
                currentOffset = stoi(line.substr(line.find(' ') + 1));
                // insert the record into the map
                records.emplace_back(currentID, currentOffset);
            }
        }
        indexFile.close();

        // binary search for the id
        int left = 0, right = count-1, mid;
        int offset;
        bool found = false;
        while(left <= right) {
            mid = left + (right - left) / 2;
            if(records[mid].first == id) {
                found = true;
                offset = records[mid].second;
                break;
            }
            else if (records[mid].first < id)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
        if(found) {
            cout << "Author is found!" << endl;
            // open the Author file in the read mode
            ifstream author("Author.txt", ios:: binary);
            // seek to the offset
            author.seekg(offset, ios::beg);
            // read the length of the author record
            char length[3];
            length[2] = '\0';
            author.read(length, 2);
            // convert the length to integer
            int len = stoi(length);
            // read about len bytes from the current position
            char* record = new char[len+1];
            record[len] = '\0';
            author.read(record, len);
            author.close();
            // parse the record
            stringstream ss(record);
            string ID, name, address;
            getline(ss, ID, '|');
            getline(ss, name, '|');
            getline(ss, address, '|');
            cout << "ID: " << ID << "\nName: "<< name << "\nAddress: " << address << endl;
        }
        else {
            cout << "Author is NOT found!" << endl;
        }
    return found;
}

// A function that takes the ISBN as a parameter and binary search for the ISBN in the PrimaryIndexBook file
bool getBookByISBN(int isbn, fstream &indexFile) {
    // open the PrimaryIndexAuthor file in the read mode
    // count the number of records in the PrimaryIndexAuthor file
    int count = 0;
    string line;
    while (getline(indexFile, line))
    {
        count++;
    }
    // loop through the PrimaryIndexAuthor file
    // and get each id and compare it with the id parameter
    indexFile.clear();
    indexFile.seekg(0, ios::beg);
    // store records into a map to sort them
    vector<pair<int, int>> records;
    for(int i = 0; i < count; i++) {
        if(getline(indexFile, line)) {
            int currentISBN, currentOffset;
            // before the space is the id
            // after the space is the offset
            currentISBN = stoi(line.substr(0, line.find(' ')));
            currentOffset = stoi(line.substr(line.find(' ') + 1));
            // insert the record into the map
            records.emplace_back(currentISBN, currentOffset);
        }
    }
    indexFile.close();

    // binary search for the id
    int left = 0, right = count-1, mid;
    int offset;
    bool found = false;
    while(left <= right) {
        mid = left + (right - left) / 2;
        if(records[mid].first == isbn) {
            found = true;
            offset = records[mid].second;
            break;
        }
        else if (records[mid].first < isbn)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    if(found) {
        cout << "Book is found!" << endl;
        // open the Author file in the read mode
        ifstream book("Book.txt", ios:: binary);
        // seek to the offset
        book.seekg(offset, ios::beg);
        // read the length of the author record
        char length[3];
        length[2] = '\0';
        book.read(length, 2);
        // convert the length to integer
        int len = stoi(length);
        // read about len bytes from the current position
        char* record = new char[len+1];
        record[len] = '\0';
        book.read(record, len);
        book.close();
        // parse the record
        stringstream ss(record);
        string ISBN, title, authorID;
        getline(ss, ISBN, '|');
        getline(ss, title, '|');
        getline(ss, authorID, '|');
        cout << "ID: " << ISBN << "\nName: " << title << "\nAddress: " << authorID << endl;
    }
    else {
        cout << "Book is NOT found!" << endl;
    }
    return found;
}