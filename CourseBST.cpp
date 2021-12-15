
// By: Paul Kenaga
// CS 300 Project 2
// 12/10/21

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

// Structure for Course objects
struct Course {
    string courseNumber;
    string courseName;
    string prerequisite; // Holds prerequisites as a string
    vector<string> prerequisites; // Holds prerequisites as a vector

    // Default Constructor with initialized variables
    Course() {
        courseNumber = "";
        courseName = "";
        prerequisite = "";
        prerequisites = {};
    }
};

// Internal structure for BST node
struct Node {
    Course course;
    struct Node* leftChild;
    struct Node* rightChild;

    // Default constructor for new node
    Node() {

        // Initialize variables
        leftChild = nullptr;
        rightChild = nullptr;
    }

    // Call new node constructor and assign course to new node
    Node(Course course) : Node() {
        this->course = course;
    }
};

// BST class definition 
class BinarySearchTree {

    // Declare private data members and methods
private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseNumber);

    // Declare public constructor and methods
public:
    BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseNumber);
    Course Search(string courseNumber);
};

// Default constructor for BST
BinarySearchTree::BinarySearchTree() {

    // Initialize root equal to nullptr
    root = nullptr;
}

// Call to traverse BST 
void BinarySearchTree::InOrder() {

    // Call inOrder fuction and pass root 
    this->inOrder(root);
}

// Prints courses in aplhanumeric order
void BinarySearchTree::inOrder(Node* node) {

    // Traverse tree while node has data
    if (node != nullptr) {

        // Traverse left subtree
        inOrder(node->leftChild);

        // Output course number and course name
        cout << node->course.courseNumber << ": " << node->course.courseName << endl;

        // Traverse right subtree 
        inOrder(node->rightChild);
    }
}

// Adds new node to BST based on course number
void BinarySearchTree::addNode(Node* node, Course course) {

    // If parameter node is larger than new node then,
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {

        // If left child is empty, set new node to left child
        if (node->leftChild == nullptr) {
            node->leftChild = new Node(course);
        }

        // Else set node to left child and to recurse down tree
        else {
            addNode(node->leftChild, course);
        }
    }

    else {

        // If right child is empty, set new node to right child
        if (node->rightChild == nullptr) {
            node->rightChild = new Node(course);
        }

        // Else set node to right child and to recurse down tree
        else {
            addNode(node->rightChild, course);
        }
    }

}

// Adds root to BST if empty or calls recursive method to add new node to BST
void BinarySearchTree::Insert(Course course) {

    // If root is empty, assign root to new course object
    if (root == nullptr) {
        root = new Node(course);
    }

    // Call addNode, passing root and course as parameters
    else {
        addNode(root, course);
    }
}

// Searches for course in BST based on course number passed as parameter
Course BinarySearchTree::Search(string courseNumber) {
    Course course;

    // Set current node equal to root
    Node* currentNode = root;

    // While currentnode is not equal to nullptr
    while (currentNode != nullptr) {

        // If course number matches current node course number, return course
        if (currentNode->course.courseNumber.compare(courseNumber) == 0) {
            return currentNode->course;
        }

        // If course number is smaller than current node's then traverse left subtree
        if (courseNumber.compare(currentNode->course.courseNumber) < 0) {
            currentNode = currentNode->leftChild;
        }

        // If course nuber is bigger than current node's then traverse right subtree
        else {
            currentNode = currentNode->rightChild;
        }
    }

    // Return empty course if no matching course found
    return course;
}

// Copies data from CSV file to new course object for BST passed in parameter
int loadCourses(string csvPath, BinarySearchTree* bst) {
    // Initialize input file stream
    ifstream dataFile;
    string row;

    // Open the file
    dataFile.open(csvPath, ios::in);

    // Display error if file does not open, return to menu
    if (!dataFile.is_open()) {
        cout << endl << "Error: Please check CSV file '" << csvPath << "' and try again." << endl << endl;

        // Return 0 so that menu option 2 & 3 may not be selected
        return 0;
    }

    // Get data each row of CSV file and create new course object 
    while (getline(dataFile, row)) {

        // Variables to hold csv data from each row
        string courseNumber;
        string courseName;
        string prerequisite;

        // Initialize new course object
        Course course;

        // Initialize string stream with CSV row
        stringstream ss(row);

        // Save first string as course number (first column)  
        getline(ss, courseNumber, ',');
        course.courseNumber = courseNumber;

        // Save second string as course number (second column)  
        getline(ss, courseName, ',');
        course.courseName = courseName;

        // Save remaining data on row as prerequisite
        getline(ss, prerequisite);
        course.prerequisite = prerequisite;

        // If the course has prerequisites, add it to the course prerequisite vector
        // This helps seperate prerequisites as seperate variables of the same type instead of one string
        if (prerequisite != ",") {

            // Initialize another string stream with prerequisite
            stringstream BB(prerequisite);

            // Add to the back of the vector
            while (BB.good()) {
                getline(BB, prerequisite, ',');
                course.prerequisites.push_back(prerequisite);
            }
        }

        // Call method to insert new course object into BST
        bst->Insert(course);

    }

    // Confirm data structure created and file opened/closed
    cout << endl << "Data Structure created from \"" << csvPath << "\"." << endl;
    dataFile.close();
    cout << endl << csvPath << " successfully opened and closed." << endl << endl;

    // Return 1 so that menu option 2 & 3 may be selected
    return 1;

}

// Main method
int  main() {
    string csvPath;
    string choice = "";

    /** Variable used to make sure that csv file is loaded before
     *  user selects menu option 2 or 3.
     */
    int i = 0;

    // Declare new BST
    BinarySearchTree* bst = nullptr;

    // Declare new course object
    Course course;

    cout << "Welcome to the course planner." << endl << endl;

    // Menu is displayed until user enters '9'
    while (choice != "9") {
        cout << "1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit." << endl << endl;
        cout << "What would you like to do? ";

        // Get user input
        cin >> choice;

        // If user enters 1,
        if (choice == "1") {

            // Call default BST constructor
            bst = new BinarySearchTree();

            // Promnpt user for csv file name with '.csv' extension
            cout << "Enter the \"FileName.csv\" that contains the course data : ";
            cin >> csvPath;

            // Call load course method passing file and BST as parameters
            i = loadCourses(csvPath, bst);

        }

        // If user enters 2,
        else if (choice == "2") {

            // Display error if data structure has not been loaded first
            if (i == 0) {

                cout << endl << "Error: You must Load Data Structure first." << endl << endl;

            }

            // Otherwise, display courses in aplhanumeric order by calling InOrder method
            else {

                cout << "Here is a sammple schedule:" << endl << endl;
                bst->InOrder();
                cout << endl << endl;

            }
        }

        // If user enters 3,
        else if (choice == "3") {

            // Display error if data structure has not been loaded first
            if (i == 0) {
                cout << endl << "Error: You must Load Data Structure first." << endl << endl;
            }

            // Otherwise prompt user for course number and search BST for match
            else {
                string input;
                cout << "What course do you want to know about? ";
                cin >> input;

                // Make sure that input is capitalized
                transform(input.begin(), input.end(), input.begin(), ::toupper);

                // Call search method
                course = bst->Search(input);

                // Display course information if found in BST
                if (!course.courseNumber.empty()) {
                    cout << endl;
                    cout << "Course Number: " << course.courseNumber << endl;
                    cout << "Course Name: " << course.courseName << endl;

                    // Get number of prerequisites
                    int number = course.prerequisites.size();

                    // Skip if course has no prerequisites
                    if (number == 0) {
                    }

                    // Otherwise if course has only one prerequisite, display it (no comma)
                    else if (course.prerequisites.at(number - 1) == "") {
                        cout << "Prerequisite: ";
                        for (int k = 0; k < number; k++) {
                            cout << course.prerequisites.at(k);
                        }
                    }

                    // Otherwise if course has more than one prerequisite, display them 
                    // seperated by comma except last one
                    else {
                        cout << "Prerequisites: ";
                        for (int k = 0; k < number; k++) {
                            if (k < number - 1) {
                                cout << course.prerequisites.at(k) << ", ";
                            }
                            else {
                                cout << course.prerequisites.at(k);
                            }
                        }
                    }

                    cout << endl << endl;
                }

                // Display error if course not found in BST
                else {
                    cout << endl << "Error: Course Number " << input << " not found." << endl << endl;
                }
            }
        }

        // If user enters 9, loop will break and program ends
        else if (choice == "9") {

            // Display thank you messsage
            cout << "Thank you for using the course planner!" << endl << endl;
        }

        // Display error message if user does not enter an option from the menu
        else {
            cout << endl << "Error: " << choice << " is not an option." << endl << endl;
            cin.ignore();
            cin.clear();
        }
    }

    // Display Goodbye
    cout << "Good bye." << endl;

    //Program ends
    return 0;
}