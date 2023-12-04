#include<bits/stdc++.h>
#include <fstream>
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
long long getAuthorByID(int id, fstream &indexFile); // search for the author by id and print the author record
long long getBookByISBN(int isbn, fstream &indexFile); // search for the book by isbn and print the book record
void updateAuthorNameByID(int id, string newName, fstream &indexFile); // search for the author by id and update the name
void updateBookTitleByISBN(int isbn, string newTitle, fstream &indexFile); // search for the book by isbn and update the title

int main() {
    return 0;
}
//----------------------------------------------------------------------------------------------------------------------
// A function that takes the authorID as a parameter and binary search for the authorID in the PrimaryIndexAuthor file
// and get the offset of the author record then seek to the offset in the Author file and read the author record
long long getAuthorByID(int id, fstream &indexFile){
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
    return offset;
}
// A function that takes the ISBN as a parameter and binary search for the ISBN in the PrimaryIndexBook file
long long getBookByISBN(int isbn, fstream &indexFile) {
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
    // store records into a map to sort them
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
    return offset;
}

//----------------------------------------------------------------------------------------------------------------------
void updateAuthorNameByID(int id, string newName, fstream &indexFile) {
    // search for the author by id and print the author record
    long long offset = getAuthorByID(id, indexFile);

    if (offset != -1) {//author is found

        ifstream author("Author.txt", ios::binary);

        // seek to the offset
        author.seekg(offset, ios::beg);

        // read the length of the author record
        char length[3];
        length[2] = '\0';
        author.read(length, 2);//read 2 bytes

        // convert the length to integer
        int len = stoi(length);

        // read about len bytes from the current position
        char *record = new char[len + 1];
        record[len] = '\0';//
        author.read(record, len);
        author.close();

        // parse the record
        stringstream ss(record);
        string ID, name, address;
        getline(ss, ID, '|');
        getline(ss, name, '|');
        getline(ss, address, '|');

        // printing old record
        cout<<ID<<"|"<<name<<"|"<<address<<"|"<<endl;

        // update code

        fstream author2("Author.txt", ios::in | ios::out);

        if(newName.length() <= 30) {      //new name is valid

            if (name.length() == newName.length()) {    //new name has the same length as the old one

                author2.seekp(offset + ID.size() + 3, ios::beg);  //seek to the name field
                author2 << newName; // update the name
                author2.close();
                cout << "Author name is updated successfully!" << endl;
                cout<<ID<<"|"<<name<<"|"<<address<<"|"<<endl;

            } else if (name.length() > newName.length()) { //new name is shorter than the old one

                author2.seekp(offset + ID.size() + 3, ios::beg); //seek to the name field
                author2 << newName;// update the name

                //fill the rest of the field with '-'
                for (int i = 0; i < name.length() - newName.length(); ++i) {
                    author2 << "-";
                }

                author2.close();
                cout << "Author name is updated successfully!" << endl;
                cout<<ID<<"|"<<name;

                //print the rest of the field-> ex: shahd--
                for (int i = 0; i < name.length() - newName.length(); ++i) {
                    cout<<"-";
                }
                cout<<"|"<<address<<"|"<<endl;

            } else {            //name.length()<newName.length()

                cout << "\"We use internal fragmentation\": new name is longer than the old one!" << endl;
              // cout << "Do you want to continue? (y/n)" << endl;
            }
        }else { //new name is too long (not valid)

            cout << "Author name is too long!" << endl;
         //   cout << "Do you want to continue? (y/n)" << endl;
        }
    }
    else { //author is not found

        cout << "Author is NOT found!" << endl;
       // cout << "Do you want to continue? (y/n)" << endl;
    }
}

void updateBookTitleByISBN(int isbn, string newTitle, fstream &indexFile) {
    // search for the book by isbn and print the book record
    long long offset = getBookByISBN(isbn, indexFile);

    if (offset != -1) {// book is found

        ifstream book("Book.txt", ios::binary);

        // seek to the offset
        book.seekg(offset, ios::beg);

        // read the length of the book record
        char length[3];
        length[2] = '\0';
        book.read(length, 2);

        // convert the length to integer
        int len = stoi(length);
        char *record = new char[len + 1]; // read about len bytes from the current position
        record[len] = '\0';
        book.read(record, len);
        book.close();

        // parse the record
        stringstream ss(record);
        string ISBN, title, authorID;
        getline(ss, ISBN, '|');
        getline(ss, title, '|');
        getline(ss, authorID, '|');

        // printing old record
        cout <<ISBN<<"|"<<title<<"|"<<authorID<<"|" << endl;

        // update code

        fstream book2("Book.txt", ios::in | ios::out);

        if(newTitle.length() <= 30) { //new title is valid

            if (title.length() == newTitle.length()) {//new title has the same length as the old one

                book2.seekp(offset + ISBN.size() + 3, ios::beg);//seek to the title field
                book2 << newTitle;
                book2.close();
                cout << "Book title is updated successfully!" << endl;
                cout <<ISBN<<"|"<<newTitle<<"|"<<authorID<<"|" << endl;

            } else if (title.length() > newTitle.length()) {//new title is shorter than the old one

                book2.seekp(offset + ISBN.size() + 3, ios::beg);//seek to the title field
                book2 << newTitle;

                for (int i = 0; i < title.length() - newTitle.length(); ++i) {//fill the rest of the field with '-'
                    book2 << "-";
                }
                book2.close();
                cout << "Book title is updated successfully!" << endl;
                cout <<ISBN<<"|"<<newTitle;

                for(int i = 0; i < title.length()-newTitle.length(); i++)//print the rest of the field-> ex: Book--
                    cout<<"-";
                cout<<"|"<<authorID<<"|" << endl;

            } else { //title.length()<newTitle.length()
                cout << "\"We use internal fragmentation\": new title is longer than the old one!" << endl;
                  //   cout << "Do you want to continue? (y/n)" << endl;
            }
        }
        else { //new title is too long (not valid)
            cout << "Book title is too long!" << endl;
            //   cout << "Do you want to continue? (y/n)" << endl;
        }
    }
    else { //book is not found
        cout << "Book is NOT found!" << endl;
        // cout << "Do you want to continue? (y/n)" << endl;
    }
}




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
