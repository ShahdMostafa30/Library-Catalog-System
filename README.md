# Library Catalog System
## Overview
In this project, a simple library catalog system is implemented using text files manipulation in C++.
### Concepts used:
1. Logical and physical files manipulation.
2. Primary and secondary indexes.
3. Multilevel indexes.
4. Internal fragmentation for memory usage.

## Project File Structures
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






   
