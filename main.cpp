
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




int main() {


    Author author;
    cout<<"Name: \n";
    cin>>author.authorName;
    cout<<"ID: \n";
    cin>>author.authorID;
    cout<<"Address: \n";
    cin>>author.address;

    addAuthor(author);


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

