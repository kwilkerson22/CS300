//============================================================================
// Name        : ProjectTwo.cpp
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
struct Course {
    string courseNumber; // unique identifier
    string courseName; 
    string prereqOne;
    string prereqTwo;
    Course() { 
        //prereqOne = "none";
        //prereqTwo = "none";
    }
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Course aCourse) :
            Node() {
        course = aCourse;
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

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);
    Node* removeNode(Node* node, string courseNumber);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(Course course);
    void Remove(string courseNumber);
    Course Search(string courseNumber);
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
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equals nullptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node(course); 
    }
    // if root is not equal to nullptr
    else {
        addNode(root, course); // add node to root
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseNumber) {
    // remove node root associated with bidId
    root = removeNode(root, courseNumber);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseNumber) {
    // current node begins at the root
    Node* current = root;
    Course course;

    // loop through nodes that are not null
    while (current != nullptr) {
        // if courseNumber entered matches current node courseNumber
        if (courseNumber == current->course.courseNumber) {
            // return current node bid value
            return current->course;
        }
        // else if courseNumber entered is less than the current node courseNumber
        else if (courseNumber < current->course.courseNumber) {
            // move current to the left
            current = current->left;
        }
        // else courseNumber entered is greater than the current node courseNumber
        else {
            // move current to the right
            current = current->right;
        }

    }
    
    return course;
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param Course course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if courseNumber is less than  the node courseNumber in tree 
    if (course.courseNumber < node->course.courseNumber) {
        // move to the left node
        if (node->left == nullptr) { // if left node is empty
            node->left = new Node(course); // add new course to left node
        }
        else {
            addNode(node->left, course); // recurse down the left node
        }
    }
    else {
        // if right node is null
        if (node->right == nullptr) {
            // add new course to right node
            node->right = new Node(course);
        }
        else {
            addNode(node->right, course); // recurse down the right node
        }
    }
    }

Node* BinarySearchTree::removeNode(Node* node, string courseNumber) {
    // if node is equal to nullptr
    if (node == nullptr) {
        // return null value
        return nullptr;
    }

    // if courseNumber searched is less than the courseNumber of node
    if (courseNumber < node->course.courseNumber) {
        // move to left node
        node->left = removeNode(node->left, courseNumber);
    }
    // if courseNumber searched is greater than the courseNumber of node
    else if (courseNumber > node->course.courseNumber) {
        // move to right node
        node->right = removeNode(node->right, courseNumber);
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
            node->course = temp->course;
            // remove right node
            node->right = removeNode(node->right, temp->course.courseNumber);
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

        // print courseNumber
        cout << node->course.courseNumber << ", ";
        // print courseName
        cout << node->course.courseName << endl;
        // print first prerequisite
        //cout << node->course.prereqOne << " | ";
        // print second prerequisite
        //cout << node->course.prereqTwo << " | " << endl;

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

        // print courseNumber
        cout << node->course.courseNumber << " | ";
        // print courseName
        cout << node->course.courseName << " | ";
        // print first prerequisite
        cout << node->course.prereqOne << " | ";
        // print second prerequisite
        cout << node->course.prereqTwo << " | " << endl;

    }
}

void BinarySearchTree::preOrder(Node* node) {
    // if node is not equal to null ptr
    if (node != nullptr) {
        // print courseNumber
        cout << node->course.courseNumber << " | ";
        // print courseName
        cout << node->course.courseName << " | ";
        // print first prerequisite
        cout << node->course.prereqOne << " | ";
        // print second prerequisite
        cout << node->course.prereqTwo << " | " << endl;

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
 * Display the course information to the console (std::out)
 *
 * @param Course struct containing the course info
 */
void displayCourse(Course course) {
    // print the courseNumber and courseName for all courses
    cout << course.courseNumber << ", " << course.courseName << endl;
    

    // if there are no prerequisites found for course
    if (course.prereqOne.find_first_not_of(" ") == string::npos &&
        course.prereqTwo.find_first_not_of(" ") == string::npos) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        // if first prerequisite found, print information
        if (course.prereqOne.find_first_not_of(" ") != string::npos) {
            cout << "Prerequisites: " << course.prereqOne;
        }

        // if second prerequisite found, print information
        if (course.prereqTwo.find_first_not_of(" ") != string::npos) {
            cout << ", " << course.prereqTwo;
        }
        cout << endl; // add endline for formatting
    }
    return;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
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
            Course course;
            course.courseNumber = file[i][0];
            course.courseName = file[i][1];
            course.prereqOne = file[i][2];
            course.prereqTwo = file[i][3];

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(course);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    cout << file.rowCount() << " courses loaded." << endl; // number of courses loaded
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
    string csvPath ,courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "CS_300_Course_Information.csv";
        courseKey = "CSCI100";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;
    string courseSearch;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadCourses(csvPath, bst);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            // print out sample schedule message
            cout << "Here is a sample schedule:" << endl;

            // sort courses alphanumerically and print information
            bst->InOrder();
            break;

        case 3:
            // prompt user to enter course information to search
            cout << "What course do you want to know about? ";

            // variable for course to search
            cin >> courseKey;

            // convert lower case letters to upper case
            for (char& c : courseKey) {
                c = toupper(c);
            }

            // search for course in tree
            course = bst->Search(courseKey);


            if (!course.courseNumber.empty()) {
                // if course information is found display the course information
                displayCourse(course);
            }

            else {
                // message if course not found
                cout << "Course Number: " << courseSearch << " not found." << endl;
            }

            break;

            // input validation for incorret menu choices
        default:
            // no error message for exit option
            if (choice == 9) {
                break;
            }
            // only print error message for non menu values
            else {
                cout << choice << " is not a valid option." << endl;
                break;
            }
        }
    }
    // exit message
    cout << "Thank you for using the course planner!" << endl;

	return 0;
}
