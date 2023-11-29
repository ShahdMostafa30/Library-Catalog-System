
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
vector<Author> loadAuthorData();
bool compareByName(const Author& a, const Author& b);
void insertAuthorNameSecondary();
vector<Book> loadBookData();
bool compareByAuthorID(const Book& a, const Book& b);
void insertAuthorIDSecondary();




int main() {
//  insertAuthorID();

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
 * Loads author data from a file.
 * Reads three fields for each Author: ID, Name, Address
 * Returns a vector of Author structs.
 */
vector<Author> loadAuthorData() {
    vector<Author> authors;

    ifstream file("Author.txt");

    string line;

    getline(file, line); // Read the entire line

    istringstream ss(line);
    string field;

    while (getline(ss, field, '|')) {
        Author author;

        // Read three fields for an Author
        strcpy(author.authorID, field.c_str());
        getline(ss, field, '|');
        strcpy(author.authorName, field.c_str());
        getline(ss, field, '|');
        strcpy(author.address, field.c_str());

        // Remove first two characters
        string idString = author.authorID;
        idString = idString.substr(2);
        strcpy(author.authorID, idString.c_str());

        // Add the Author to the vector
        authors.push_back(author);
    }

    file.close();
    return authors;
}
/**
 * Comparison function to sort Authors by name.
 *
 * @param a The first Author struct to compare
 * @param b The second Author struct to compare
 * @return True if a's name is lexicographically less than b's name, false otherwise
 */
bool compareByName(const Author& a, const Author& b) {
    return strcmp(a.authorName, b.authorName) < 0;
}
/**
 * Loads data from file and sorts it by name.
 * Writes sorted data to LLAuthor.txt and SecondaryIndexAuthor.txt.
 */
void insertAuthorNameSecondary() {

    //load data from file and sort this by name
    vector<Author> data = loadAuthorData();

    sort(data.begin(), data.end(), compareByName);


    ofstream llFile("LLAuthor.txt",ios::trunc);
    for (int i = 0; i < data.size(); ++i) {

        llFile << i <<' '<< data[i].authorID;

        // Check if next author has the same name
        if (i + 1 < data.size() && strcmp(data[i + 1].authorName , data[i].authorName)==0) {
            llFile << ' ' << i + 1 << '\n';
        } else {
            llFile << ' ' << -1 << '\n';
        }
    }

    llFile.close();

    ofstream secFile("SecondaryIndexAuthor.txt");

    secFile << data[0].authorName << ' ' << 0 << '\n';

    for (int i = 1; i < data.size(); ++i) {
        if(strcmp(data[i].authorName,data[i-1].authorName) == 0)
            continue;
        else
            secFile  << data[i].authorName <<' '<< i << '\n';
    }

}
/**
 * Loads book data from a file.
 * Reads three fields for each Book: ISBN, BookTitle, AuthorID
 * Returns a vector of Book structs.
 */
vector<Book> loadBookData() {
    vector<Book> authors;
    ifstream file("Book.txt");
    string line;

    getline(file, line); // Read the entire line

    istringstream ss(line);
    string field;

    while (getline(ss, field, '|')) {
         Book book;

        // Read three fields for a Book
        strcpy(book.ISBN, field.c_str());
        getline(ss, field, '|');
        strcpy(book.bookTitle, field.c_str());
        getline(ss, field, '|');
        strcpy(book.authorID, field.c_str());

        // Remove first two characters
        string idString = book.ISBN;
        idString = idString.substr(2);
        strcpy(book.ISBN, idString.c_str());

        authors.push_back(book);
    }
    file.close();
    return authors;
}
/**
 * Comparison function to sort Books by author ID.
 *
 * @param a The first Book struct to compare
 * @param b The second Book struct to compare
 * @return True if a's author ID is lexicographically less than b's author ID, false otherwise
 */
bool compareByAuthorID(const Book& a, const Book& b) {
    return strcmp(a.authorID, b.authorID) < 0;
}
/**
 * Loads data from file and sorts it by author ID.
 * Writes sorted data to LLBook.txt and SecondaryIndexBook.txt.
 */
void insertAuthorIDSecondary() {

    //load data from file and sort this by authorID
    vector<Book> data = loadBookData();

    sort(data.begin(),data.end(), compareByAuthorID);


    ofstream llFile("LLBook.txt",ios::trunc);
    for (int i = 0; i < data.size(); ++i) {

        llFile << i <<' '<< data[i].ISBN;

        // Check if next book has the same authorID
        if (i + 1 < data.size() && strcmp(data[i + 1].authorID , data[i].authorID)==0) {
            llFile << ' ' << i + 1 << '\n';
        } else {
            llFile << ' ' << -1 << '\n';
        }
    }

    llFile.close();
    ofstream secFile("SecondaryIndexBook.txt");
    secFile << data[0].authorID << ' ' << 0 <<'\n';
    for (int i = 1; i < data.size(); ++i) {
        if(strcmp(data[i].authorID,data[i-1].authorID) == 0)
            continue;
        else
            secFile << data[i].authorID << ' '<< i <<'\n';
    }
}

//void addAuthor(){
//
//}
