#include<bits/stdc++.h>
#include <fstream>
using namespace std;

//Files:
//For Primary index -> PrimaryIndexAuthor , PrimaryIndexBook -> ( ID, offset)
//For Secondary index -> SecondaryIndexAuthor ->(Name, Pointer)
//                    -> SecondaryIndexBook ->(AuthorID, pointer)
//For Linked List -> LLAuthor ->(#, ID(for author), Pointer)
//                -> LLBook   ->(#, ID(for book), Pointer)


//struct Author {
//    char authorID[15];
//    char authorName[30];
//    char address[30];
//};
//
//struct Book {
//    char ISBN[15];
//    char bookTitle[30];
//    char authorID[30];
//};
//
//struct ReferenceNode {
//    int idPointer;
//    int nextReferenceOffset;
//};
//
//struct SecondaryIndexNode {
//    char name[30];
//    int referenceListOffset;
//};

// Function declarations

//bool doesIDExist(fstream& primary, int x);
//void insertAuthorPrimary(char id[], short offset);
//void insertBookPrimary(char id[], short offset);
//void insertAuthorName(char name[30], char ID[15]);
//void sortSecondaryIndex(const char* filename);
//void insertAuthorName(char name[30], char ID[15]);
void getAuthorByID(int id, fstream &inFile);


int main() {
    fstream primary("PrimaryIndexAuthor.txt", ios::in);
    int n; cin >> n;
    getAuthorByID(n, primary);
    primary.close();

    return 0;
}
// a function that takes the authorID as a parameter
// and binary search for the authorID in the PrimaryIndexAuthor file
// and get the offset of the author record
// then seek to the offset in the Author file and read the author record
void getAuthorByID(int id, fstream &inFile) {
        // open the PrimaryIndexAuthor file in the read mode
        // count the number of records in the PrimaryIndexAuthor file
        int count = 0;
        string line;
        while (getline(inFile, line))
        {
            count++;
        }
        // loop through the PrimaryIndexAuthor file
        // and get each id and compare it with the id parameter
        inFile.clear();
        inFile.seekg(0, ios::beg);
        // store records into a map to sort them
        vector<pair<int, int>> records;
        for(int i = 0; i < count; i++) {
            if(getline(inFile, line)) {
                int currentID, currentOffset;
                // before the space is the id
                // after the space is the offset
                currentID = stoi(line.substr(0, line.find(' ')));
                currentOffset = stoi(line.substr(line.find(' ') + 1));
                // insert the record into the map
                records.emplace_back(currentID, currentOffset);
            }
        }
        inFile.close();

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
            cout << "Author is not found!" << endl;
        }
}