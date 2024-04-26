//============================================================================
// Name        : LinkedList.cpp
// Author      : Kenneth Wilkerson
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

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
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid;
        Node *next;

        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size = 0;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    head = nullptr; // initialize head variable to null
    tail = nullptr; // initialize tail variable to null
    size = 0; //size tracker variable

}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // start at the head
    Node* current = head;

    // temp storage for copies
    Node* temp;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current; // hang on to current node and store in temp
        current = current->next; // make current the next node
        delete temp; // delete temp node
    }
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    //make a new node
    Node* newNode = new Node(bid);

    //if the head is null
    if (head == nullptr) {
        head = newNode; // new node becomes the head
        tail = newNode; // new node becomes the tail
    }
    // otherwise if the head is NOT null
    else {
        tail->next = newNode; // tail node is pointed in the direction of the new node
        tail = newNode; // tail becomes the new node
    }
    
    size += 1; // size count increases for each iteration
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    //make a new node
    Node* newNode = new Node(bid);

    // if the head is NOT null
    if (head == nullptr ) {
        head = newNode; // new node becomes the head
        tail = newNode; // new node becomes the tail
    }
    //otherwise if the head is NOT null
    else {
        //the new node points to the current head
        newNode->next = head;
        // the head becomes the new node
        head = newNode;
    }
    
    size += 1; // size count increases with each iteration
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    //start at the head of the list for printing
    Node* current = head;

    //loop for each existing node that is NOT null
    while (current != nullptr) {

        // print current bidID
        cout << current->bid.title << " | ";
        // print current amount
        cout << current->bid.amount << " | ";
        // print current fund
        cout << current->bid.fund << endl;

        //move to the next item in the list
        current = current->next;

    }
    
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
    // if head is NOT null AND the head bidId matches
      if (head != nullptr && head->bid.bidId == bidId) {
        // set temp variable equal to the value of head
        Node* copy = head;

        //make head point to the next node 
        head = head->next;

        // remove old head value
        delete copy;

        // decrease the value of size
        size -= 1;
        return;
    }
    

    // start at the head
    Node* current = head;

    // when the current node is NOT null and the next node is NOT null
    while (current != nullptr && current->next != nullptr) {
        // if the next bidId matches the bidId searched
        if (current->next->bid.bidId == bidId) {
            Node* copy = current->next; // store next node into copy
            current->next = current->next->next; // move an additional node over and store in next
            // if the copy and tail are equal, end of the list is reached
            if (copy == tail) {
                // tail node is the current node
                tail = current;
            }
            delete copy; // delete copy 
            size -= 1; // reduce size count
            return;
        }
        current = current->next; // move current to next node to continue iterating
       
    }
}
    

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
    // case for with the head is NOT null and the bidId matches the bidId searched
    if (head != nullptr && head->bid.bidId == bidId) {
        return head->bid;
    }

   
    // current starts as the head node
    Node* current = head;

    // loop as long as the current node is NOT null
    while (current != nullptr) {
        // if the current bidId matches the bidId searched
        if (current->bid.bidId == bidId) {
            return current->bid;
        }

        // move current to the next node to continue iterating
        current = current->next;
    }
    return Bid(); // return bid 
}



   


/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
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
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
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

    clock_t ticks;

    LinkedList bidList;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        // enter bid option
        case 1:
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);

            break;

        // load bid option
        case 2:
            ticks = clock();

            loadBids(csvPath, &bidList);

            cout << bidList.Size() << " bids read" << endl;

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;
        
        // display bids option
        case 3:
            bidList.PrintList();

            break;
        
        // search for bid option
        case 4:
            ticks = clock();

            bid = bidList.Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        // remove bid option
        case 5:
            bidList.Remove(bidKey);

            break;
        }
    }
    
    // choice 9 exits the program with a goodbye message
    cout << "Good bye." << endl;

    return 0;
}
