
#include<bits/stdc++.h>

#include <fstream>
#include <string>

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




int main() {
//    try insertAuthorID function
//    insertAuthorID("1", "1");
//    insertAuthorID("1", "2");
//    insertAuthorID("1", "3");
//    insertAuthorID("2", "4");
//    insertAuthorID("2", "5");
//    insertAuthorID("3", "6");
//    insertAuthorID("3", "7");

//    //try insertAuthorName function
//    insertAuthorName("Ahmed", "1");
//    insertAuthorName("Ali", "2");
//    insertAuthorName("Ahmed", "3");
//    insertAuthorName("Mohamed", "4");

//    Author author;
//    cout<<"Name: \n";
//    cin>>author.authorName;
//    cout<<"ID: \n";
//    cin>>author.authorID;
//    cout<<"Address: \n";
//    cin>>author.address;
//
//    addAuthor(author);


// try deleteAuthorID function
//    deleteAuthorID("1");
//    deleteAuthorID("2");
//    deleteAuthorID("3");

//    //try deleteAuthorName function
//    deleteAuthorName("Ahmed");
//    deleteAuthorName("Ali");
//    deleteAuthorName("Mohamed");
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
void parseQuery(const string& query) {
    // Select all from Authors where Author ID= number;
    if(query.find("Select all from Authors where Author ID=") != string::npos) {
        string temp = query.substr(query.find('=') + 1);
        try {
            int ID = stoi(temp);
            fstream primaryIndexFile("PrimaryIndexAuthor.txt", ios::in | ios::binary);
            int offset = getAuthorByID(ID, primaryIndexFile);
            printAuthorByID(offset);
            primaryIndexFile.close();
        }
        catch (const std::exception& e) {
            cerr << "Error converting line " << ": " << e.what() << endl;
            // Handle or skip this line as needed
        }
    }
    // Select all from Books where Author ID= number;
    else if(query.find("Select all from Books where Author ID=") != string::npos) {
        string temp = query.substr(query.find('=') + 1);
        try {
            int ID = stoi(temp);
            fstream secondaryIndexFile("SecondaryIndexBook.txt", ios::in | ios::binary);
            vector<int> ISBNs = getBookByAuthorID(ID, secondaryIndexFile);
            printBookByAuthorID(ISBNs);
            secondaryIndexFile.close();
        }
        catch (const std::exception& e) {
            cerr << "Error converting line " << ": " << e.what() << endl;
            // Handle or skip this line as needed
        }
    }
    // Select Author from Authors where Author Name= name;
    else if(query.find("Select Author from Authors where Author Name=") != string::npos) {
        string name = query.substr(query.find('=') + 2);
        // remove the last ";"
        name = name.substr(0, name.size() - 1);
        fstream secondaryIndexFile("SecondaryIndexAuthor.txt", ios::in | ios::binary);
        vector<int> IDs = getAuthorByName(name, secondaryIndexFile);
        printAuthorByName(IDs);
        secondaryIndexFile.close();
    }
    else {
        cout << "Invalid query!" << endl;
    }
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

    }
}
/**
 * Function to format a number into a two-byte string.
 * For example, if number is 1, it will be formatted as "01".
 *
 * @param number The number to be formatted.
 * @return The formatted string representing the number.
 */
string formatTwoBytes(int number) {
    ostringstream oss;
    oss << setw(2) << setfill('0') << number;
    return oss.str();
}

/**
 * Function to add a new author record to the "Author.txt" file.
 *
 * @param author The Author object containing author information.
 */
void addAuthor(Author author) {

    // Opening the file in read/write mode
    fstream file("Author.txt", ios::in | ios::out);

    string header;
    string line;

    // Reading the first two lines from the file
    for (int i = 0; i < 2; ++i) {
        if (i == 0)
            file >> header; // Reading header
        else
            file >> line; // Reading the second line
    }

    file.close(); // Closing the file after reading

    // Calculating the size of the record to be added
    int recordSize = strlen(author.authorID) + strlen(author.authorName) + strlen(author.address) + 3;
    string recSize = formatTwoBytes(recordSize); // Formatting the size as a two-byte string

    // Reopening the file in read/write/binary mode
    file.open("Author.txt", ios::out | ios::in | ios::binary);

    // Checking if there are no deleted records
    if (header == "-1") {
        // No deleted records found, so appending the new record to the end of the file
        file.seekp(0, ios::end);

        insertAuthorPrimary(author.authorID, file.tellp()); // -> Add to primary index file
        insertAuthorName(author.authorName, author.authorID); // -> Add to secondary file

        file << recSize << author.authorID << '|' << author.authorName << '|' << author.address << '|';
        file.close();

    } else {
        int currentOffset = stoi(header);
        while (true) {
            // Loop to search for space in the file to add the new record
            stringstream data(line);
            data.seekg(currentOffset + 1); // Skip the '#'

            string prevOffset;
            char ch;
            while (data.get(ch) && ch != '|')
                prevOffset += ch;

            string size;
            while (data.get(ch) && ch != '|')
                size += ch;

            // If there is enough space at the current position to insert the new record
            if (stoi(size) == (recordSize + 2)) {
                file.seekp(0);
                file << prevOffset;

                file.seekp(currentOffset + header.length() + 2);

                insertAuthorPrimary(author.authorID, file.tellp()); // -> Add to primary index file
                insertAuthorName(author.authorName, author.authorID); // -> Add to secondary file

                file << recSize << author.authorID << '|' << author.authorName << '|' << author.address << '|';

                file.close();
                return;
            } else if (stoi(size) > (recordSize + 2)) {
                // If the available space is larger than needed, insert the new record and fill the remaining space with '#'
                file.seekp(0);
                file << prevOffset;

                file.seekp(currentOffset + header.length() + 2);

                insertAuthorPrimary(author.authorID, file.tellp()); // -> Add to primary index file
                insertAuthorName(author.authorName, author.authorID); // -> Add to secondary file

                file << recSize << author.authorID << '|' << author.authorName << '|' << author.address << '|';

                string rest = string(stoi(size) - recordSize - 2, '#');
                file << rest;

                file.close();
                return;
            } else {
                currentOffset = stoi(prevOffset);
            }
            if (stoi(prevOffset) == -1) {
                // If no suitable space is found, append the new record to the end of the file
                file.seekp(0, ios::end);

                insertAuthorPrimary(author.authorID, file.tellp()); // -> Add to primary index file
                insertAuthorName(author.authorName, author.authorID); // -> Add to secondary file

                file << recSize << author.authorID << '|' << author.authorName << '|' << author.address << '|';

                file.close();
                return;
            }
        }
    }
}

/**
 * Function to add a new book record to the "Book.txt" file.
 *
 * @param author The Book object containing book information.
 */
void addBook(Book book) {

    // Opening the file in read/write mode
    fstream file("Book.txt", ios::in | ios::out);

    string header;
    string line;

    // Reading the first two lines from the file
    for (int i = 0; i < 2; ++i) {
        if (i == 0)
            file >> header; // Reading header
        else
            file >> line; // Reading the second line
    }

    file.close(); // Closing the file after reading

    // Calculating the size of the record to be added
    int recordSize = strlen(book.ISBN) + strlen(book.bookTitle) + strlen(book.authorID) + 3;
    string recSize = formatTwoBytes(recordSize); // Formatting the size as a two-byte string

    // Reopening the file in read/write/binary mode
    file.open("Book.txt", ios::out | ios::in | ios::binary);

    // Checking if there are no deleted records
    if (header == "-1") {
        // No deleted records found, so appending the new record to the end of the file
        file.seekp(0, ios::end);

        insertBookPrimary(book.ISBN, file.tellp()); // -> Add to primary index file
        insertAuthorID(book.authorID, book.ISBN); // -> Add to secondary file

        file << recSize << book.ISBN << '|' << book.bookTitle << '|' << book.authorID << '|';
        file.close();

    } else {
        int currentOffset = stoi(header);
        while (true) {
            // Loop to search for space in the file to add the new record
            stringstream data(line);
            data.seekg(currentOffset + 1); // Skip the '#'

            string prevOffset;
            char ch;
            while (data.get(ch) && ch != '|')
                prevOffset += ch;

            string size;
            while (data.get(ch) && ch != '|')
                size += ch;

            // If there is enough space at the current position to insert the new record
            if (stoi(size) == (recordSize + 2)) {
                file.seekp(0);
                file << prevOffset;

                file.seekp(currentOffset + header.length() + 2);

                insertBookPrimary(book.ISBN, file.tellp()); // -> Add to primary index file
                insertAuthorID(book.authorID, book.ISBN); // -> Add to secondary file

                file << recSize << book.ISBN << '|' << book.bookTitle << '|' << book.authorID << '|';

                file.close();
                return;
            } else if (stoi(size) > (recordSize + 2)) {
                // If the available space is larger than needed, insert the new record and fill the remaining space with '#'
                file.seekp(0);
                file << prevOffset;

                file.seekp(currentOffset + header.length() + 2);

                insertBookPrimary(book.ISBN, file.tellp()); // -> Add to primary index file
                insertAuthorID(book.authorID, book.ISBN); // -> Add to secondary file

                file << recSize << book.ISBN << '|' << book.bookTitle << '|' << book.authorID << '|';

                string rest = string(stoi(size) - recordSize - 2, '#');
                file << rest;

                file.close();
                return;
            } else {
                currentOffset = stoi(prevOffset);
            }
            if (stoi(prevOffset) == -1) {
                // If no suitable space is found, append the new record to the end of the file
                file.seekp(0, ios::end);

                insertBookPrimary(book.ISBN, file.tellp()); // -> Add to primary index file
                insertAuthorID(book.authorID, book.ISBN); // -> Add to secondary file

                file << recSize << book.ISBN << '|' << book.bookTitle << '|' << book.authorID << '|';

                file.close();
                return;
            }
        }
    }
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

void deleteAuthorID(char authorID[]) {
    vector<pair<string, string>> data;

    ifstream secFile("SecondaryIndexBook.txt");
    ifstream llFile("LLBook.txt");

    string ID, isbn;
    int secPointer, llPointer, x;

    while (secFile >> ID >> secPointer) {
        llFile.clear();
        llFile.seekg(0, ios::beg);

        while (llFile >> llPointer >> isbn >> x) {
            if (secPointer == llPointer && ID != authorID) {
                data.push_back(make_pair(ID, isbn));

                while (x != -1) {
                    llFile >> llPointer >> isbn >> x;
                    data.push_back(make_pair(ID, isbn));
                }
                break;
            }
        }
    }

    secFile.close();
    llFile.close();

    // Update llFile1 directly using the data vector
    ofstream llFile1("LLBook.txt", ios::trunc);

    for (int i = 0; i < data.size(); ++i) {
        if (i < data.size() - 1 && data[i].first == data[i + 1].first)
            llFile1 << i << ' ' << data[i].second << ' ' << i + 1 << '\n';
        else
            llFile1 << i << ' ' << data[i].second << ' ' << -1 << '\n';
    }

    llFile1.close();

    ofstream secFile1("SecondaryIndexBook.txt", ios::trunc);

    for (int i = 0; i < data.size(); ++i) {
        if (i == 0 || data[i].first != data[i - 1].first)
            secFile1 << data[i].first << ' ' << i << '\n';
    }

    secFile1.close();

//    for (int i = 0; i < data.size(); ++i) {
//        cout << data[i].first << ' ' << data[i].second << '\n';
//    }
}

void deleteAuthorName(char name[]) {
    vector<pair<string, string>> data;

    ifstream secFile("SecondaryIndexAuthor.txt");
    ifstream llFile("LLAuthor.txt");

    string ID, isbn;
    int secPointer, llPointer, x;

    while (secFile >> ID >> secPointer) {
        llFile.clear();
        llFile.seekg(0, ios::beg);

        while (llFile >> llPointer >> isbn >> x) {
            if (secPointer == llPointer && ID != name) {
                data.push_back(make_pair(ID, isbn));

                while (x != -1) {
                    llFile >> llPointer >> isbn >> x;
                    data.push_back(make_pair(ID, isbn));
                }
                break;
            }
        }
    }

    secFile.close();
    llFile.close();

    // Update llFile1 directly using the data vector
    ofstream llFile1("LLAuthor.txt", ios::trunc);

    for (int i = 0; i < data.size(); ++i) {
        if (i < data.size() - 1 && data[i].first == data[i + 1].first)
            llFile1 << i << ' ' << data[i].second << ' ' << i + 1 << '\n';
        else
            llFile1 << i << ' ' << data[i].second << ' ' << -1 << '\n';
    }

    llFile1.close();

    ofstream secFile1("SecondaryIndexAuthor.txt", ios::trunc);

    for (int i = 0; i < data.size(); ++i) {
        if (i == 0 || data[i].first != data[i - 1].first)
            secFile1 << data[i].first << ' ' << i << '\n';
    }

    secFile1.close();

    for (int i = 0; i < data.size(); ++i) {
        cout << data[i].first << ' ' << data[i].second << '\n';
    }
}