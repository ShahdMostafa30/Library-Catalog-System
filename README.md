# Library Catalog System
## Overview
In this project, a simple library catalog system is implemented using text files manipulation in C++.
### Concepts used:
1. Logical and physical files manipulation.
2. Primary and secondary indexes.
3. Multilevel indexes.
4. Internal fragmentation for memory usage.

## Menu and Features 
### 1. Add new author
Allows administrators to add a new author record.
Checks the AVAIL LIST for available records before adding a new one.
Updates indexes accordingly.
### 2. Add New Book
Allows administrators to add a new book record.
Checks the AVAIL LIST for available records before adding a new one.
Updates indexes accordingly.
### 3. Update Author Name with Author ID
Allows administrators to update non-key fields of existing records.
Ensures updates do not exceed allocated size.
Updates indexes accordingly.
### 4. Update Book Title with ISBN
### 5. Delete Book with (ISBN)
Marks a record with a (*) marker instead of physically deleting it.
Updates AVAIL LIST and indexes accordingly.
### 6. Delete Author (Author ID)
### 7. Print Author Data with (Author ID)
### 8. Print Book Data with (ISBN)
### 9. Write Query
o Select all from Authors where Author ID=’xxx’; // this query will use primary index to get the
results.

o Select all from Books where Author ID=’xxx’; // this query will use secondary index to get the
results.

o Select Author Name from Authors where Author ID=’xxx’; // this query will use secondary
index to get the results.

### 10. Exit


## Project Files Structure
| File | Content | Structure |
| -------- | -------- | -------- |
| main.cpp | code | C++ code |
| Author.txt | Author data records in sequential order separated with a pipe | Author ID, Author Name, Address |
| Book.txt | Books data records in sequential order separated with a pipe | ISBN Book, Title, Author ID |
| PrimaryIndexAuthor.txt | Primary index file for author using Author ID and byte offset separated with a single space | Author ID, offset |
| PrimaryIndexAuthor.txt | Primary index file for author using Author ID and byte offset separated with a single space | Author ID, offset |
| PrimaryIndexBook.txt | Primary index file for book using Book ISBN and byte offset separated with a single space | Book ISBN, offset |
| SecondaryIndexAuthor.txt | Secondary index file for author using Author Name and a pointer to a record in a linked list separated with a single space | Name, Pointer |
| SecondaryIndexBook.txt | Secondary index file for book using Author ID and a pointer to a record in a linked list separated with a single space | Author ID, Pointer |
| LLAuthor.txt | Linked list file for Author using a hash value from the author secondary index file, Author ID, and a pointer to the author record in the author data file separated with a single space | #, Author ID, Pointer(byte offset) |
| LLBook.txt | Linked list file for Book using a hash value from the author secondary index file, Book ISBN, and a pointer to the book record in the author data file separated with a single space | #, Book ISBN, Pointer(byte offset) |

## Dependencies
No files or dependecies required, just make sure you run C++ 17 and include C++ external libraries.

## Contributing 
Contributions to this project are welcomed. Feel free to fork the repository and create pull requests for improvements or additional features.

## Contributers
- [Hend Ahmad](https://github.com/LifelongLearner-HEND)
- [Shahd Mostafa](https://github.com/ShahdMostafa30)
- [Shahd Osama](https://github.com/shahdosama10)
- [Maryam Osama](https://github.com/maryamosama33)
