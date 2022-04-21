//============================================================================
// Name: ProjectTwo.cpp
// Author: Kerrian Offermann
// Copyright/Credit: Copyright © 2017 SNHU CODE; StackOverflow; zyBooks; Copyright (c) 2017 Romain Sylvian
// Description: CS 300 - Project Two
//============================================================================


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "CSVparser.hpp" /* Credit: SNHU CODE and Romain Sylvain */
#include "CSVparser.cpp" /* Credit: SNHU CODE and Romain Sylvain */

using namespace std;



double strToDouble(string str, char ch);


struct Course { // Structure for courses
    string courseNumber; // Initialize course number
    string courseName; // Initialize course name
    string preNumber; // Initialize prerequisite course number
    string preName; // Initialize prerequisite course name
    vector<string> prereqs; // Initialize prerequisite vector

    Course() {
        courseNumber = "";
        courseName = "";
        preNumber = "";
        preName = "";
        prereqs;
    }
};

struct Node { // Internal structure for course tree
    Course course;

    Node* right;
    Node* left;

    Node() {
        right = left = nullptr;
    }

    Node(Course course) : Node() {
        this->course = course;
    }

};



class CourseTree {

private: // Members for course tree
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public: // Methods for course tree
    CourseTree();
    virtual ~CourseTree();
    void InOrder();
    Node* getRoot();
    void Insert(Course course);
    void Remove(string courseNumber);
    Course Search(string courseNumber);
    void printCourses(Course course);
};


CourseTree::CourseTree() { // Constructor for the course tree
    root = nullptr; // Root is set to null before being loaded
}


CourseTree::~CourseTree() { // Deconstructor for the course tree

}


void CourseTree::InOrder() {
    this->inOrder(root); // The inOrder function is called starting at the root
}




void CourseTree::Insert(Course course) {
    if (this->root == nullptr) { // If the root is null 
        this->root = new Node(course); // Then set root to a new node
    }
    else {
        this->addNode(root, course); // Else add it to the tree with the root left alone
    }
}



Course CourseTree::Search(string courseNumber) {
    Node* current = root; // Set current node to the root

    while (current != nullptr) { // While the current node is not null 
        if (current->course.courseNumber.compare(courseNumber) == 0) { // If the course number is not the same as the current node
            cout << current->course.courseNumber << ", " << current->course.courseName << endl; // Print the course number and course name
            cout << "Prerequisites: "; // Print "Prerequisites"
            if (current->course.prereqs.size() == 0) { // If the current prequisite node is not there
                cout << "" << endl; // Print nothing since nothing is there
            }
            else { // Otherwise
                for (int i = 0; i < current->course.prereqs.size(); ++i) { // For all prerequisite courses
                    if (i != (current->course.prereqs.size() - 1)) { // If there is a pre. course present
                        cout << current->course.prereqs.at(i) << ", "; // Then separate the course with a comma and print
                    }
                    else {
                        cout << current->course.prereqs.at(i) << endl; // Otherwise print the current prequisite course
                    }
                }
            }

            return current->course; // Return with whatever the current node is
        }

        if (courseNumber.compare(current->course.courseNumber) < 0) { // If the course number is less than the current node
            current = current->left; // Move the current node to the left
        }

        else {
            current = current->right; // Otherwise move node to the right
        }
    }

    Course course;
    return course; // Return the course
}


void CourseTree::addNode(Node* node, Course course) {
    if (node->course.courseNumber.compare(course.courseNumber) > 0) { // If the course number is greater in comparison
        if (node->left == nullptr) { // And if the left node is null
            node->left = new Node(course); // Then turn that current node into the left node
        }

        else {
            this->addNode(node->left, course); // Otherwise add a node to the left
        }
    }
    else { // If the course number is not greater
        if (node->right == nullptr) { // And if the node to the right is null
            node->right = new Node(course); // Then create a new right node
        }
        else {
            this->addNode(node->right, course); // Otherwise, add a node to the right of the current node
        }
    }
}

void CourseTree::inOrder(Node* node) {
    if (node != nullptr) { // If the node is null
        inOrder(node->left); // Move to the left node
        cout << node->course.courseNumber << ", " << node->course.courseName << endl; // Print the course number and course name
        inOrder(node->right); // Then move to the right
    }
}





void loadfile(string csvPath, CourseTree* courses) {
    cout << "CSV file loading... " << csvPath << endl;

    csv::Parser file = csv::Parser(csvPath); // Initialize parser and path


    try { // Set up try-catch to find errors whilst reading files
        for (unsigned int i = 0; i < file.rowCount(); i++) { // For each row in the CSV file

            Course course; // Call tree structure and start adding different courses and prereq. courses
            course.courseNumber = file[i][1];
            course.courseName = file[i][0];
            course.preNumber = file[i][8];
            course.preName = file[i][4];

            courses->Insert(course); // Add courses to the tree
        }
    }
    catch (csv::Error& e) { // Catch errors
        std::cerr << e.what() << std::endl; // Print what the error is
    }
}

void printCourses(Course course) {
    cout << course.courseNumber << ": " << course.courseName << " | " << course.preNumber << " | "
        << course.preName << endl;
    return;
}

int main(int argc, char* argv[]) {

    string userInput; // For when the user types in a course to search
    string csvPath = "courses.csv"; // Load the csv file as a string called "csvPath"
    CourseTree* tree{}; // Define the structure that will be loading the courses
    Course course; // Call the structure


    int choice = 0; // Initialize choice as 0

    while (choice != 9) { // While the choice entered is not "9"

        switch (choice) { // The switch is the choice that is entered
            cout << "\nWelcome to the course planner" << endl;
            cout << "Please select an option from the menu\n" << endl;
            cout << "1. Load Data Structure." << endl;
            cout << "2. Print Course List." << endl;
            cout << "3. Print Course." << endl;
            cout << "9. Exit" << endl;

            cin >> choice; // Allows user to enter a choice


        case 1: // Case 1 = Option 1
            loadfile(csvPath, tree); // Load the csvPath into the tree
            break; // Break the loop and return to the menu

        case 2: // Case 2 = Option 2
            cout << "Here is a sample schedule:" << endl;
            printCourses(course);
            tree->InOrder(); // Print all of the courses in order from the root and then in left to right order
            break; // Break the loop and return to the main menu

        case 3: // Case 3 = Option 3

            cout << "What course would you like information about?" << endl;
            getline(cin, userInput); // Get the entire line of the user's input
            tree->Search(userInput); // Search the tree for the user's input
            break; // Break and return to main menu

        case 9:
            cout << "Thank you for using the course planner. Goodbye!" << endl; // Print farewell message when user exits the menu
            break;

        }


    }
    return 0;

}