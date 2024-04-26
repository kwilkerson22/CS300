//============================================================================
// Name        : HashTable.cpp
// Author      : Kenneth Wilkerson
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // resizing tableSize to initialize node structure
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // creating local tableSize variable
    this->tableSize = size;

    // resize nodes size
    nodes.resize(tableSize);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // loop through every node in the hash table
    for (int i = 0; i < nodes.size(); i++) {
        Node* current = nodes[i].next;
        while (current != nullptr) { // if the current element is in use
            Node* temp = current; // store value in temp variable
            current = current->next; // move current to next value
            delete temp; // delete stored value in temp
        }
    }
    
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // variable for calculating hash value
    int hashValue = key % tableSize;

    // return hash value
    return hashValue;

}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // find hash value for incoming bid
    unsigned int key = hash(atoi(bid.bidId.c_str()));

    // new node for bid insert
    Node* newNode = new Node(bid, key);

    // pull node from index marked by key
    Node* current = &nodes[key];

    // if current key is unused
    if (current->key == UINT_MAX) {
        nodes[key] = *newNode; // store node at key value 
    }

    // otherwise find the next open node
    else {
        while (current->next != nullptr) {
            current = current->next; // set current to next value
        }
    }
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // for each element in the list
    for (int i = 0; i < nodes.size(); ++i) {
        Node* current = &nodes[i]; // retrieve the value at the index i

        // while the current element is not NULL or assigned a default value
        while (current != nullptr) {
            if (current->key != UINT_MAX) {
                cout << "Key " << i << ": "; // print key number
                cout << current->bid.bidId << " | "; // print bid id 
                cout << current->bid.title << " | "; // print bid title
                cout << current->bid.amount << " | "; // print bid amount
                cout << current->bid.fund << " | " << endl;
            }
            current = current->next; // move current to the next node
        }
    }
    

}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // find hash value for bid id to be removed
    unsigned int key = hash(atoi(bidId.c_str()));

    // retrieve node associated with the hash value
    Node* current = &nodes[key];

    Node* prev = nullptr; // node to shift value after delete

    // when current is NOT null and the bid id matches the bid entered to be removed
    while (current != nullptr && current->bid.bidId != bidId) {
        prev = current; // store current in prev
        current = current->next; // move current to next node
    }

    // if current is NOT null
    if (current != nullptr) {
        if (prev == nullptr) { // if storage variable is null
            nodes[key] = *current->next; // store the next node in the hash value element of current
        }
        else {
            prev->next = current->next; // move the next node into storage node
        }
        delete current; // delete node from hash table
}
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // find hash value for bid id to be searched
    unsigned int key = hash(atoi(bidId.c_str()));

    Node* current = &nodes[key]; // retrive node at key

    // while the current node is NOT null
    while (current != nullptr) {
        if (current->bid.bidId == bidId) { // if the current bid id matches the bid id searched
            return current->bid; // return the current bid
        }
        current = current->next; // move current to the next node for next iteration
    }
    // return bid if no entry is found for the key
    return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    cout << file.rowCount() << " bids read" << endl;
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;
    bidTable = new HashTable();
    
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
