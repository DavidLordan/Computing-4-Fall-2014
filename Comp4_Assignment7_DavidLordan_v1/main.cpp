/* File:   main.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on December 3rd, 2014, 12:20 PM, modified on Dec 7th at 10:03 AM to 
 * add additional documentation. 
 * 
 * This program is to demonstrate use of regular expressions to parse command line
 * user input along with using the Xerces library to create a DOM structure. 
 * This is an expansion of the previous assignment which was to create a 'hard-coded'
 * DOM structure in memory and to display it to the user. This assignment goes further
 * by building the tree dynamically from the user's requests as well as giving the
 * user various options for tree manipulation. All basic commands are parsed with
 * regular expressions to extract keywords and variable reflecting the meaning of the user's input. 
 * 
 * There are seven basic commands:
 * 
 * Help: Displays a list of all basic commands, their use, and proper syntax. 
 * 
 * Add: Allows the user to create and add an element or attribute to a specific parent along with optional content.
 * 
 * Remove: Allows the user to remove an element or attribute from a specific parent.
 * 
 * Rename: Allows the user to rename an element or attribute after it has been created. 
 * 
 * Print: Allows the user to display the entire DOM structure, or a sub-tree from a specific element.
 *        By default this uses the 'DOMSerializer' to print an XML structure, but by using the print
 *        command along with 'list' the tree is displayed in a custom format using the 'TreeWalker' class.
 * 
 * Export: Allows the user to export the entire DOM structure as an XML file with a specific name.
 *         The file is stored in the project's distribution folder.
 * 
 * Quit: Exits the program. 
 */

/* Includes the necessary Xerces libraries and Xerces namespace. */
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <iostream>
#include "xercesc/dom/DOMTreeWalker.hpp"

#include <string.h>
#include <stdlib.h>

/*Inludes custom source files for various program functions.*/
#include "outputOptions.h"
#include "treeManipulation.h"

/* Includes the boost regular expressions library*/
#include <boost/regex.hpp>

XERCES_CPP_NAMESPACE_USE
        using namespace std;

/**
 * A function that takes user input, uses regular expressions to parse it, and extracts keywords. 
 * @param input - The input string taken from the command line. 
 * @param command - The basic command keyword to be updated. 
 * @param selector - The selector keyword which specifies how a basic command is to be executed. 
 * @param parentName - A string used to search for nodes.
 * @param childOrAtt - A string used to create or rename nodes. 
 * @param contentOrVal - A string also used to create or rename nodes.  
 */
void parseInput(string input, string &command, string &selector, string &parentName, string &childOrAtt, string &contentOrVal) {

    //Creates a regEx used to check for basic commands. 
    const boost::regex reBasic("(print|export|add|remove|help|rename|quit).*");

    // Used to continue parsing if the basic command is valid. 
    bool valid = true;

    boost::smatch what;

    // RegEx search for basic command.
    boost::regex_search(input, what, reBasic);
    command = what[1];

    // Parses a print command for the 'list' format keyword and for element names to use as a subroot. 
    if (command == "print") {
        boost::regex rePrintType("print\\s(list).*");
        boost::regex_search(input, what, rePrintType);
        selector = what[1];

        if (selector.empty()) {
            boost::regex reSubtreeRoot("print\\s(\\w*)");
            boost::regex_search(input, what, reSubtreeRoot);
            parentName = what[1];
        } else {
            boost::regex reSubtreeRoot("print\\slist\\s(\\w*)");
            boost::regex_search(input, what, reSubtreeRoot);
            parentName = what[1];
        }
    }

    // Parses export command to extract a selector which will be used as a file name. 
    if (command == "export") {
        boost::regex reExportFile("export\\s(\\w*).*");
        boost::regex_search(input, what, reExportFile);
        selector = what[1];
    }


    //Parses add, remove, and rename commands and extracts the necessary variables for each,
    // such as parent name, element or attribute selector, child or attribute name,
    // and content or value.
    if (command == "add" || command == "remove" || command == "rename") {

        boost::regex reElement(".*(add|remove|rename)\\selement.*");
        boost::regex reAttribute(".*(add|remove|rename)\\sattribute.*");

        if (boost::regex_match(input, reElement)) {
            selector = "element";
        } else if (boost::regex_match(input, reAttribute)) {

            selector = "attribute";
        } else {
            valid = false;
        }

        if (valid) {
            boost::regex reParentNode(".*" + selector + "\\s(\\w*).*");
            if (boost::regex_search(input, what, reParentNode)) {
                parentName = what[1];
            }

            boost::regex reChildOrAtt(".*" + parentName + "\\s(\\w*).*");
            if (boost::regex_search(input, what, reChildOrAtt)) {
                childOrAtt = what[1];
            }

            boost::regex reContentOrVal(".*" + childOrAtt + "\\s(\\w*).*");
            if (boost::regex_search(input, what, reContentOrVal)) {
                contentOrVal = what[1];
            }

        } else {
            // Informs the user if an invalid selector has been attempted. 
            cout << "Invalid selector. Must be 'element' or 'attribute'. " << endl;
            command = "invalid";
        }
    }
}

/**
 * Function to prompt the user and accept input from the command line. 
 * @return - A string of input commands to be parsed by the 'parseInput' function. 
 */
string getUserInput() {
    //Variable used to control 'while' loop. 
    bool cont = true;
    string input = "";

    while (cont) {

        //User prompt
        cout << "Please enter a command, or enter 'help' to see a list of all commands." << endl;

        //Gets the entire command line input.
        getline(cin, input);
        cin.ignore(-1);

        //Checks that the first word of the input is a valid basic command. 
        const boost::regex reAdd("(add|print|quit|export|remove|rename|help).*");

        if (boost::regex_match(input, reAdd)) {
            cont = false;
        } else {
            cout << "Invalid input, please try again." << endl;
        }
    }

    return input;
}

/**
 * Function to reset variable strings. This is to avoid having the values from longer
 * commands carrying over to shorter commands. 
 * @param parentName - The parent variable used for tree manipulation.
 * @param childOrAtt - Element or attribute name variable also used for tree manipulation
 * @param contentOrVal - Element content or attribute value variable
 */
void clearVars(string &parentName, string &childOrAtt, string &contentOrVal) {
    //Resets all variables to be empty strings
    parentName = "";
    childOrAtt = "";
    contentOrVal = "";
}

/**
 * Core function of the program. Uses a loop to repeatedly ask the user for inputs
 * using the 'getUserInput' function. Then calls 'parseInput' to determine the meaning
 * of the user's commands. Calls the appropriate function to match the basic command
 * given and passes it the necessary variables. When the loop is ended the program
 * terminates. 
 */
void userTree() {

    /**Strings which are used to store parsed user input. The are passed to most program
     functions by reference and are essentially global variables, though only defined in
     this function for maintainability.
     */
    string parentName = ""; //Used to specify the parent of the node to be manipulated
    string childOrAtt = ""; //Used to specify the name of the node to be manipulated
    string contentOrVal = "";//Used to specify the content or value of the node to be manipulated
    string selector = "";//Used to specify whether the node to be manipulated is an element or attribute
    string inputString = "";//Used to store command lines taken from the user
    string command = "";//Used to store the basic command taken from the user input

    // Starts the XML platform utilities, allow for the use of several Xerces features. 
    XMLPlatformUtils::Initialize();

    // Creates a DOM implementation object. Taken from the Xeces DOMPrint sample program.
    XMLCh tempStr[3] = {chLatin_L, chLatin_S, chNull};
    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

    //Creates an empty document which will store the DOM structure.
    DOMDocument* doc = impl->createDocument();

    // Begins for loop which is the program cycle. 
    while (inputString != "quit") {

        // Gets the user input.
        inputString = getUserInput();

        if (command != "quit") {
            // Parses the user input and extracts various pieces of information, such as the
            // new command, selector, and node information.
            parseInput(inputString, command, selector, parentName, childOrAtt, contentOrVal);

            // Applies the appropriate function the appropriate command.
            if (command == "add") {
                // Checks if the user wishes to add a root element or basic element.
                if (parentName == "null") {
                    addRoot(doc, childOrAtt, contentOrVal);
                } else {
                    addNode(doc, selector, parentName, childOrAtt, contentOrVal);
                }
            }

            //Calls the removeNode function 
            if (command == "remove") {
                removeNode(doc, selector, parentName, childOrAtt);
            }

            // Checks if there is a root and thus a tree to print
            if (command == "print") {
                if (!doc->getFirstChild()) {
                    cout << "There is no tree to print. Must add a root first. " << endl;

                } else {
                    // Checks for selector type and calls the appropriate output format.
                    if (selector.empty()) {
                        printTree(doc, parentName);
                    } else if (selector == "list") {
                        treeWalker(doc, parentName);
                    } else {
                        cout << "Print format must be specified as 'xml' or 'list'." << endl;
                    }
                }
            }

            //Checks if there is a root and thus a tree to export
            if (command == "export") {
                if (!doc->getFirstChild()) {
                    cout << "There is no tree to export. Must add a root first. " << endl;
                } else {
                    exportTree(doc, selector);
                }
            }

            if (command == "rename") {
                renameNode(doc, selector, parentName, childOrAtt, contentOrVal);
            }

            if (command == "help") {
                showHelp();
            }
        }

        //Clears all variables on every loop cycle to avoid 'carry-over' from long commands to 
        // shorter ones
        clearVars(parentName, childOrAtt, contentOrVal);
    }

    // Closes the XML platform utilities and informs the user that the program is quitting
    XMLPlatformUtils::Terminate();
    cout << "Quitting Program." << endl;
}

/**
 * Standard C++ main function. 
 * @param argc - Number of command line arguments. 
 * @param argv - Array of pointers to command line arguments. 
 * @return - Returns error code if the program does not end properly. 
 */
int main(int argC, char* argV[]) {
    //User tree function which is the program cycle
    userTree();

    return 0;
}
