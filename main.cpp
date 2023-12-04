#include<bits/stdc++.h>
#include <fstream>
using namespace std;

//Files:
//For Primary index -> PrimaryIndexAuthor , PrimaryIndexBook -> ( ID, offset)
//For Secondary index -> SecondaryIndexAuthor ->(Name, Pointer)
//                    -> SecondaryIndexBook ->(AuthorID, pointer)
//For Linked List -> LLAuthor ->(#, ID(for author), Pointer)
//                -> LLBook   ->(#, ID(for book), Pointer)

int getAuthorByID(int id, fstream &indexFile); // search for the author by id and print the author record
int getBookByISBN(int isbn, fstream &indexFile); // search for the book by isbn and print the book record
void printAuthorByID(int offset); // print the author record by offset
void printBookByISBN(int offset); // print the book record by offset
vector<int> getBookByAuthorID(int id, fstream &secondaryIndexFile); // search for the books by author id and print the book records
void printBookByAuthorID(const vector<int>& v); // print the book records
vector<int> getAuthorByName(string name, fstream &secondaryIndexFile); // search for the authors by name and print the author records
void printAuthorByName(const vector<int>& v); // print the author records

int main() {
    fstream secondary("SecondaryIndexAuthor.txt", ios::in);
    string name; cin >> name;
    printAuthorByName(getAuthorByName(name, secondary));
    secondary.close();
    return 0;
}
// A function that takes the authorID as a parameter and binary search for the authorID in the PrimaryIndexAuthor file
// and get the offset of the author record then seek to the offset in the Author file and read the author record
int getAuthorByID(int id, fstream &indexFile) {
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
        int offset = -1;
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
    return offset;
}
void printAuthorByID(int offset) {
    if(offset == -1) {
        cout << "Author not found" << endl;
        return;
    }
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

// A function that takes the ISBN as a parameter and binary search for the ISBN in the PrimaryIndexBook file
int getBookByISBN(int isbn, fstream &indexFile) {
    // count the number of records in the PrimaryIndexBook file
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
    // store records into a vector to sort them
    vector<pair<int, int>> records;
    for(int i = 0; i < count; i++) {
        if (getline(indexFile, line)) {
            int currentISBN, currentOffset;
            string temp = line.substr(0, line.find(' '));
            string temp2 = line.substr(line.find(' ') + 1);

            try {
                currentISBN = stoi(temp);
                currentOffset = stoi(temp2);
                records.emplace_back(currentISBN, currentOffset);
            } catch (const std::exception& e) {
                cerr << "Error converting line " << i + 1 << ": " << e.what() << endl;
                // Handle or skip this line as needed
                continue; // Skip to the next iteration
            }
        }
    }
    indexFile.close();

    // binary search for the id
    int left = 0, right = count-1, mid;
    int offset = -1;
    while(left <= right) {
        mid = left + (right - left) / 2;
        if(records[mid].first == isbn) {
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
    return offset;
}
void printBookByISBN(int offset) {
    if(offset == -1) {
        cout << "Book not found" << endl;
        return;
    }
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
vector<int> getBookByAuthorID(int id, fstream &secondaryIndexFile) {
    // open the secondaryIndexFile
    // count the number of records in the secondaryIndexFile
    int count = 0;
    string line;
    while (getline(secondaryIndexFile, line))
    {
        count++;
    }
    secondaryIndexFile.clear();
    secondaryIndexFile.seekg(0, ios::beg);
    // store records of the secondary index file into a vector
    vector<pair<int, int>> records;
    for(int i = 0; i < count; i++) {
        if (getline(secondaryIndexFile, line)) {
            int currentID, currentPointer;
            string temp = line.substr(0, line.find(' '));
            string temp2 = line.substr(line.find(' ') + 1);

            try {
                currentID = stoi(temp);
                currentPointer = stoi(temp2);
                records.emplace_back(currentID, currentPointer);
            }
            catch (const std::exception& e) {
                cerr << "Error converting line " << i + 1 << ": " << e.what() << endl;
                // Handle or skip this line as needed
                continue; // Skip to the next iteration
            }
        }
    }
    // count the number of records in the linked list
    fstream linkedListFile("LLBook.txt", ios::in);
    int linkedListCount = 0;
    string line2;
    while (getline(linkedListFile, line2))
    {
        linkedListCount++;
    }
    // store the linked list in a vector of tuples
    vector<tuple<int, int, int>> linkedList;

    linkedListFile.clear();
    linkedListFile.seekg(0, ios::beg);

    for(int i = 0; i < linkedListCount; i++) {
        if (getline(linkedListFile, line2)) {
            int currentHashValue, currentISBN, currentPointer;
            string temp1 = line2.substr(0, line2.find(' '));
            string temp2 = line2.substr(line2.find(' ') + 1);
            string temp3 = temp2.substr(temp2.find(' ') + 1);

            try {
                currentHashValue = stoi(temp1);
                currentISBN = stoi(temp2);
                currentPointer = stoi(temp3);
                linkedList.emplace_back(currentHashValue, currentISBN, currentPointer);
            } catch (const std::exception& e) {
                cerr << "Error converting line " << i + 1 << ": " << e.what() << endl;
                // Handle or skip this line as needed
                continue; // Skip to the next iteration
            }
        }
    }

    // 1- binary search for the id to get the hash value
    int left = 0, right = count-1, mid;
    int hashValue = -1;
    while(left <= right) {
        mid = left + (right - left) / 2;
        if(records[mid].first == id) {
            hashValue = records[mid].second;
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
    // if the author is not found return empty vector
    if(hashValue == -1) {
        cout << "Author is NOT found!" << endl;
        return {};
    }
    // 2- binary search on the linked list to get all the books
    int pointer = hashValue;
    left = 0, right = linkedListCount-1;
    vector<int> ISBNs;
    while(pointer != -1) {
        while(left <= right) {
            mid = left + (right - left) / 2;
            if(get<0>(linkedList[mid]) == pointer) {
                ISBNs.push_back(get<1>(linkedList[mid]));
                pointer = get<2>(linkedList[mid]);
                break;
            }
            else if (get<0>(linkedList[mid]) < pointer)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
    }
    return ISBNs;
}
void printBookByAuthorID(const vector<int>& v) {
    // 3- print all the books
    for(int i = 0; i < v.size(); i++) {
        fstream primaryIndexFile("PrimaryIndexBook.txt", ios::in | ios::binary);
        int offset = getBookByISBN(v[i], primaryIndexFile);
        printBookByISBN(offset);
        primaryIndexFile.close();
    }
}
vector<int> getAuthorByName(string name, fstream &secondaryIndexFile) {
    // count the number of records in the secondaryIndexFile
    int count = 0;
    string line;
    while (getline(secondaryIndexFile, line))
    {
        count++;
    }
    secondaryIndexFile.clear();
    secondaryIndexFile.seekg(0, ios::beg);

    // store records of the secondary index file into a vector
    vector<pair<string, int>> records;
    // convert the hash value to integer
    for(int i = 0; i < count; i++) {
        if (getline(secondaryIndexFile, line)) {
            int hashValue;
            string Name = line.substr(0, line.find(' '));
            string temp = line.substr(line.find(' ') + 1);
            try {
                hashValue = stoi(temp);
                records.emplace_back(Name, hashValue);
            }
            catch (const std::exception& e) {
                cerr << "Error converting line " << i + 1 << ": " << e.what() << endl;
                // Handle or skip this line as needed
                continue; // Skip to the next iteration
            }
        }
    }
    // count the number of records in the linked list
    fstream linkedListFile("LLAuthor.txt", ios::in);
    int linkedListCount = 0;
    string line2;
    while (getline(linkedListFile, line2))
    {
        linkedListCount++;
    }

    linkedListFile.clear();
    linkedListFile.seekg(0, ios::beg);
    // store the linked list in a vector of tuples
    vector<tuple<int, int, int>> linkedList;
    for(int i = 0; i < linkedListCount; i++) {
        if (getline(linkedListFile, line2)) {
            int currentHashValue, currentID, currentPointer;
            string temp1 = line2.substr(0, line2.find(' '));
            string temp2 = line2.substr(line2.find(' ') + 1);
            string temp3 = temp2.substr(temp2.find(' ') + 1);
            try {
                currentHashValue = stoi(temp1);
                currentID = stoi(temp2);
                currentPointer = stoi(temp3);
                linkedList.emplace_back(currentHashValue, currentID, currentPointer);
            } catch (const std::exception& e) {
                cerr << "Error converting line " << i + 1 << ": " << e.what() << endl;
                // Handle or skip this line as needed
                continue; // Skip to the next iteration
            }
        }
    }
    // 1- binary search for the name to get the hash value
    int left = 0, right = count-1, mid;
    int hashValue = -1;
    while(left <= right) {
        mid = left + (right - left) / 2;
        if(records[mid].first == name) {
            hashValue = records[mid].second;
            break;
        }
        else if (records[mid].first < name)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    // if the author is not found return empty vector
    if(hashValue == -1) {
        cout << "Author is NOT found!" << endl;
        return {};
    }
    // 2- binary search on the linked list to get all the authors of the same name
    int pointer = hashValue;
    vector<int> AuthorIDs;
    while(pointer != -1) {
        left = 0, right = linkedListCount-1;
        while(left <= right) {
            mid = left + (right - left) / 2;
            if(get<0>(linkedList[mid]) == pointer) {
                AuthorIDs.push_back(get<1>(linkedList[mid]));
                pointer = get<2>(linkedList[mid]);
                break;
            }
            else if (get<0>(linkedList[mid]) < pointer)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
    }
    return AuthorIDs;
}
void printAuthorByName(const vector<int>& v) {
    // 3- print all the Author records
    for(int i = 0; i < v.size(); i++) {
        fstream primaryIndexFile("PrimaryIndexAuthor.txt", ios::in | ios::binary);
        int offset = getAuthorByID(v[i], primaryIndexFile);
        printAuthorByID(offset);
        primaryIndexFile.close();
        cout << endl;
    }
}