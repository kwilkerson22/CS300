//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Kenneth Wilkerson
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

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

// Internal structure for tree node
struct Node {
    Bid bid;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Bid aBid) :
            Node() {
        bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);
    Node* removeNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // setting root equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // calling inOrder function and passing root node
    inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    // calling postOrder function and passing root node
    BinarySearchTree::postOrder(root);

}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    // calling preOrder function and passing root node
    preOrder(root);

}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // if root equals nullptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node(bid); 
    }
    // if root is not equal to nullptr
    else {
        addNode(root, bid); // add node to root
    }
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    // remove node root associated with bidId
    root = removeNode(root, bidId);
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // current node begins at the root
    Node* current = root;

    // loop through nodes that are not null
    while (current != nullptr) {
        // if bidId matches current node
        if (bidId == current->bid.bidId) {
            // return current node bid value
            return current->bid;
        }
        // else if bidId is less than the current node
        else if (bidId < current->bid.bidId) {
            // move current to the left
            current = current->left;
        }
        // else bidId is greater than the current node
        else {
            // move current to the right
            current = current->right;
        }

    }
    Bid bid;
    return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    // if bidId is less than bid id in tree 
    if (bid.bidId < node->bid.bidId) {
        // move to the left node
        if (node->left == nullptr) { // if left node is empty
            node->left = new Node(bid); // add new bid to left node
        }
        else {
            addNode(node->left, bid); // recurse down the left node
        }
    }
    else {
        // if right node is null
        if (node->right == nullptr) {
            // add new bid to right node
            node->right = new Node(bid);
        }
        else {
            addNode(node->right, bid); // recurse down the right node
        }
    }
    }

Node* BinarySearchTree::removeNode(Node* node, string bidId) {
    // if node is equal to nullptr
    if (node == nullptr) {
        // return null value
        return nullptr;
    }

    // if bidId searched is less than the bidId of node
    if (bidId < node->bid.bidId) {
        // move to left nodes
        node->left = removeNode(node->left, bidId);
    }
    // if bidId searched is greater than the bidId of node
    else if (bidId > node->bid.bidId) {
        // move to right node nodes
        node->right = removeNode(node->right, bidId);
    }
    else {
        // if both node sides are null
        if (node->left == nullptr && node->right == nullptr) {
            delete node; // delete the node
            return nullptr; // return null value
        }
        // if the left side is null
        else if (node->left == nullptr) {
            Node* temp = node->right; // move right node into storage variable
            delete node; // delete the node
            return temp; // return storage variable value
        }
        // if the right side is null
        else if (node->right == nullptr) {
            Node* temp = node->left; // move left node into storage variable
            delete node; // delete the node
            return temp; // return storage variable value
        }
        // if both sides are NOT null
        else {
            Node* temp = node->right; // copy right side into storage variable
            while (temp->left != nullptr) { // find the smallest value in the right subtree
                temp = temp->left; // copy smallest value in right subtree into storage variable
            }
            // node inherits storage variable
            node->bid = temp->bid;
            // remove right node
            node->right = removeNode(node->right, temp->bid.bidId);
        }
    }
    // return node
    return node;
}

void BinarySearchTree::inOrder(Node* node) {
    // if node is not equal to nullptr
    if (node != nullptr) {

        // inOrder left
        inOrder(node->left);

        // print bidID
        cout << node->bid.bidId << " | ";
        // print title
        cout << node->bid.title << " | ";
        // print amount
        cout << node->bid.amount << " | ";
        // print fund
        cout << node->bid.fund << " | " << endl;

        // inOrder right
        inOrder(node->right);

    }
}
void BinarySearchTree::postOrder(Node* node) {
    // if node is not equal to nullptr
    if (node != nullptr) {
        //postOrder left
        postOrder(node->left);

        // postOrder right
        postOrder(node->right);

        // print bid id
        cout << node->bid.bidId << " | ";
        // print bid title
        cout << node->bid.title << " | ";
        // print amount
        cout << node->bid.amount << " | ";
        // print fund
        cout << node->bid.fund << " | " << endl;

    }
}

void BinarySearchTree::preOrder(Node* node) {
    // if node is not equal to null ptr
    if (node != nullptr) {
        // print bid id
        cout << node->bid.bidId << " | ";
        // print bid title
        cout << node->bid.title << " | ";
        // print bid amount
        cout << node->bid.amount << " | ";
        // print bid fund
        cout << node->bid.fund << " | " << endl;

        // preOrder left
        preOrder(node->left);

        // preOrder right
        preOrder(node->right);
    }      
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
void loadBids(string csvPath, BinarySearchTree* bst) {
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
            bst->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    cout << file.rowCount() << " bids read." << endl;
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

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Bid bid;

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
            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            // display bids in order
            bst->InOrder();
            break;

        case 3:
            // Initialize a timer variable for searching for a bid
            ticks = clock();

            // search for bid matching bidKey and store result
            bid = bst->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            // if matching bid found
            if (!bid.bidId.empty()) {
                // display bid
                displayBid(bid);
                // if not
            } else {
                // print bid not found
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }
            
            // print time in ticks and seconds
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            // remove bid that matches bidKey
            bst->Remove(bidKey);
            break;
        }
    }
    // exit message
    cout << "Good bye." << endl;

	return 0;
}
