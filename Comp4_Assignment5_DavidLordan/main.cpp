/* File:   main.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on October 11th, 2014, 5:20 PM, modified on Nov 7 at 11:20 AM to 
 * add additional documentation. 
 * 
 * This program reads an XML file, then tokenizes and parses each line. As 
 * tags are opened, element objects are created and their pointers are pushed 
 * to a vector acting as a stack. During this process, a tree of elements is created
 * and stored in memory. As the tags are closed, the element pointers
 * are removed from the stack and the objects deleted. Depending on the choice of the user,
 * as the program processes each line, the entire line, the current state of the XML document,
 * tag name and content or attributes when applicable are all displayed, as well as
 * any changes to the stack. 
 * 
 * After the tree has finished building and is stored in memory, the contents of the 
 * tree are displayed recursively. This includes displaying elements, their attributes,
 * content and their child attributes. 
 * 
 * This file has been updated to output the tree structure as a JSON file. This is
 * done by recursively going through the tree and surrounding the tree elements in such
 * a way that it will be consistent with the JSON syntax. 
 * 
 * This tree is also output as an HTML file. This is not part of the assignment but was
 * suggested as an extra challenge by Prof. Heines. 
 */

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Element.h"

using namespace std;

/**
 * Vector which acts as a stack, storing pointers to elements. This is used as the
 * element tree is being built to display the building process. 
 */
vector<Element*> elementVect;

/**
 * This is an element which is used as a placeholder for the current element
 * being processed in the tree. 
 */
Element* currentElement = new Element;

/**
 * A place holder "empty" element that is used to determine if the current element
 * has been changd. This is used in the tree building and display process. 
 * Important note: This is NOT the root element of the XML file or the tree. 
 */
Element* emptyRoot = currentElement;

/** A boolean value that stores whether or not the user wishes to see the tree being built.*/
bool showProcessing = false;

/** 
 * Enumeration for each of the possible states of XML document. These will be 
 *used extensively in the program to determine the context of a particular 
 *line of text. These states are determined by the parse function below. 
 * This was made by Prof. Jesse Heines of UML for the 91.204 Computing IV course
 * and is taken from that course's lecture notes. 
 */
enum ParserState {
    UNKNOWN, STARTING_DOCUMENT, DIRECTIVE,
    ELEMENT_OPENING_TAG, ELEMENT_CONTENT, ELEMENT_NAME_AND_CONTENT,
    ELEMENT_CLOSING_TAG, SELF_CLOSING_TAG,
    STARTING_COMMENT, IN_COMMENT, ENDING_COMMENT, ONE_LINE_COMMENT,
    ERROR
};

/**
 * This function is used during debugging to display the parser state. This 
 * function was created by Prof. Jesse Heines of UML for the 91.204 Computing IV course
 * and is taken from that course's lecture notes. 
 * @param ps the parser state
 */
void ShowState(ParserState ps) {
    cout << "ParserState = ";
    switch (ps) {
        case UNKNOWN: cout << "UNKNOWN";
            break;
        case STARTING_DOCUMENT: cout << "STARTING_DOCUMENT";
            break;
        case DIRECTIVE: cout << "DIRECTIVE";
            break;
        case ELEMENT_OPENING_TAG: cout << "ELEMENT_OPENING_TAG";
            break;
        case ELEMENT_CONTENT: cout << "ELEMENT_CONTENT";
            break;
        case ELEMENT_NAME_AND_CONTENT: cout << "ELEMENT_CONTENT";
            break;
        case ELEMENT_CLOSING_TAG: cout << "ELEMENT_CLOSING_TAG";
            break;
        case SELF_CLOSING_TAG: cout << "SELF_CLOSING_TAG";
            break;
        case STARTING_COMMENT: cout << "STARTING_COMMENT";
            break;
        case IN_COMMENT: cout << "IN_COMMENT";
            break;
        case ENDING_COMMENT: cout << "ENDING_COMMENT";
            break;
        case ONE_LINE_COMMENT: cout << "ONE_LINE_COMMENT";
            break;
        case ERROR: cout << "ERROR";
            break;
        default: cout << "UNKNOWN";
            break;
    }
    cout << endl;
}

/**
 * This function parses a line read from the XML file, extracts the appropriate
 * content and returns the determined state. The function assumes that the 'trim'
 * function has already been applied to the line of text, removing any leading 
 * white space and/or tabs. Further assumptions are as follows:
 * (1) The XML file is well-formed.
 * (2) There is at most one complete element per line.
 * (3) All element opening tags start on new lines.
 * (4) If there is an element closing tag, it is the last thing to appear on that line.
 * (5) If there is no element closing tag on the same line as an element opening tag,
 * the line has no content.
 * (6) Attributes and their values are ignored. 
 * Much of this logic of this function is based on lecture notes of Prof. Jesse Heines of UML for 
 * the 91.204 Computing IV course. 
 * @param currentLine   - Most recent line read from XML file by 'openFile' function
 * @param currentState  - The current state of the parser as determined by the last line parsed.
 * @param content       - Used to store extracted content, if applicable. Passed by ref.
 * @param tagName       - Used to store extracted tag name, if applicable. Passed by ref.
 * @return              - The new parser state as determined by the algorithm in the function. 
 */
ParserState parse(string currentLine, ParserState currentState, string &content, string &tagName) {

    //The function first determines if '>' is located in the current line, if not, the line must 
    //be a starting comment, in comment, or possibly unknown. 
    if (currentLine.find('>') == string::npos) {

        //The function then determines if the last line started a comment or 
        // was in a comment, if so, the current line is still in a comment. 
        if (currentState == STARTING_COMMENT || currentState == IN_COMMENT) {
            return IN_COMMENT;
        }

        //Checks to see if the line starts with '<!--' indicating a starting comment
        if (currentLine[1] == '!' && currentLine[2] == '-' && currentLine[3] == '-') {
            return STARTING_COMMENT;
        }// If we do not have a '>' and the line is not a comment, parser returns
            // unknown state. 
        else {
            return UNKNOWN;
        }
    }//The following tests are for when a '>' is found.


        //Checks if '>' is really '-->', implying an ending comment.
    else if (currentLine[0] != '<'
            && currentLine[currentLine.find('>') - 1] == '-'
            && currentLine[currentLine.find('>') - 2] == '-') {

        return ENDING_COMMENT;

        // Continues, knowing that the '>' is not part of a closing comment.
    } else {

        //The followings tests are if the first character of the line is '<'
        if (currentLine[0] == '<') {

            //Checks if '<' is really '<?', implying a directive. The directive
            //is extracted and DIRECTIVE returned. 
            if (currentLine[1] == '?') {
                content = currentLine.substr(2, currentLine.rfind('?') - 2);

                return DIRECTIVE;

                //Checks for one line comment, uses the 'content' variable, which
                //Is passed by reference, to store the extracted comment.
            } else if (currentLine[1] == '!' && currentLine [2] == '-' && currentLine[3] == '-') {
                content = currentLine.substr(4, currentLine.rfind('-') - 5);

                return ONE_LINE_COMMENT;

                //Checks for closing tag, extracts tag name.
            } else if (currentLine[1] == '/') {
                tagName = currentLine.substr(2, currentLine.find('>') - 2);
                return ELEMENT_CLOSING_TAG;

                //Eliminating all other possibilities, the following now treats
                // the current line as either an opening tag, or a complete element.
                // Either way, the tag name is extracted and stored in tagName.
            } else {

                //Extracting of tag name.
                tagName = currentLine.substr(1, currentLine.find('>') - 1);
                tagName = tagName.substr(0, tagName.find(' '));

                //Checks for complete element, extracts the content
                if (currentLine[currentLine.rfind('<') + 1] == '/') {
                    content = currentLine.substr(currentLine.find('>') + 1, currentLine.find('/'));
                    content = content.substr(0, content.find('/') - 1);
                    return ELEMENT_NAME_AND_CONTENT;
                }// Checks for self-closing tag extracts content.
                else if (currentLine[currentLine.rfind('>') - 1] == '/') {
                    tagName = tagName.substr(0, tagName.find('/'));
                    content = ("{EMPTY}");
                    return SELF_CLOSING_TAG;
                }// By elimination the line must be an opening tag, whos tag name
                    // has already been extracted. 
                else {

                    return ELEMENT_OPENING_TAG;
                }
            }
        }// If for any reason the current line did not pass any of the above tests, an
            //unknown state is returned. 
        else {
            return UNKNOWN;
        }
    }
}

/**
 * Trim leading and trailing white space (spaces and tabs) from the string
 * passed as an argument and return the trimmed string.
 * NOTE: This function was written by Prof. Jesse Heines of UMass Lowell 
 * for the Fall 2014 91.204-Computing IV class. 
 * @param str string to trim
 * @return  trimmed string
 */
string trim(string str) {
    string::iterator it = str.begin();
    while (*it == ' ' || *it == '\t') {
        str.erase(str.begin());
    }
    it = str.end() - 1;
    while (*it == ' ' || *it == '\t') {
        str.erase(str.end() - 1);
    }
    return str;
}

/**
 * This function iterates through the elementVect, which is used as a stack,
 * displaying the current stack content. This is called whenever the stack content is changed.
 * @param elementVect   -   The vector which stores element pointers, used to implement a stack.
 */
void showStack(vector<Element*> elementVect) {
    cout << "*** The stack now contains: ";

    if (!elementVect.empty()) {
        for (vector<Element*>::iterator it = elementVect.begin(); it != elementVect.end(); ++it) {
            cout << (*it)->getTagName();

            // Simply adds a period at the end of the list, otherwise the element
            // names are separated with a comma. 
            if (it == elementVect.end() - 1)
                cout << ".";
            else
                cout << ", ";
        }
        cout << endl;
    } else {
        cout << "{EMPTY}" << endl;
    }
}

/**
 * Function to process each line, building the element tree. If the user has chosen so,
 * the tree building itself will be displayed.
 * @param currentLine - The current line of an XML file which is to be processed.
 * @param currentState - The current parser state of the XML file. Used to give context to a line.
 * @param lineNumber - The line number on which the current line was found. 
 * @param content - The content within an element that was extracted from the 'parse' function.
 * @param tagName - The tag name that was extracted from an XML opening tag by the 'parse' function.
 */
void proccesLine(string currentLine, ParserState &currentState, int lineNumber, string content, string tagName) {


    // Depending upon the current state, an appropriate output is displayed, 
    // and the proper actions are taken with the stack. 
    switch (currentState) {

        case STARTING_COMMENT:
        {
            if (showProcessing) {
                cout << lineNumber << ": " << currentLine << endl;
                cout << "*** Comment started";
            }
            break;
        }
        case IN_COMMENT:
        {
            if (showProcessing) {
                cout << lineNumber << ": " << currentLine << endl;
                cout << "*** Comment line";
            }
            break;
        }

        case ENDING_COMMENT:
        {
            if (showProcessing) {
                cout << lineNumber << ": " << currentLine << endl;
                cout << "*** Ending comment";
            }
            break;
        }
        case ONE_LINE_COMMENT:
        {
            if (showProcessing) {
                cout << lineNumber << ": " << content << endl;
                cout << "*** One line comment";
            }
            break;
        }

        case DIRECTIVE:
        {
            if (showProcessing) {
                cout << lineNumber << " : " << currentLine << endl;
                cout << "*** Directive = " << content << endl;
            }
            break;
        }

        case ELEMENT_OPENING_TAG:
            // If the current line is determined to be an opening tag, a new element
            // object is created and its pointer pushed to the stack. The stack contents
            // are then listed to confirm the change. 
        {

            Element* ElementPtr = new Element(tagName, lineNumber, "Empty", currentLine);

            // The new element is then listed a child of the current element.
            currentElement->addChild(ElementPtr);

            // The current element is then set as the parent to the new element. 
            ElementPtr->setParent(currentElement);

            // The new element is added to the element stack. 
            elementVect.push_back(ElementPtr);

            // Tree building process is displayed to the user. 
            if (showProcessing) {
                cout << lineNumber << " : " << currentLine << endl;
                cout << "*** Element Opened = " << tagName << endl;

                if (currentElement != emptyRoot) {
                    cout << "*** Creating new child: " << ElementPtr->getTagName()
                            << ", for parent: " << currentElement->getTagName() << endl;
                } else {
                    cout << "*** No parent found, creating root element: " << ElementPtr->getTagName() << endl;
                }

                if (!ElementPtr->vecAttribute.empty()) {
                    cout << "*** Attributes found: " << endl;
                    ElementPtr->displayAttributes();
                }

                showStack(elementVect);

            }
            // Sets the level of the new element to be one level higher than its parent. 
            ElementPtr->setLevel(ElementPtr->getParent()->getLevel() + 1);

            // Sets the current element as the new element, building a new branch of the tree. 
            currentElement = ElementPtr;
            break;
        }

        case ELEMENT_NAME_AND_CONTENT:
        {

            // Creates a new element and creates the parent/child relationship with the current element.
            Element* ElementPtr = new Element(tagName, lineNumber, content, currentLine);
            currentElement->addChild(ElementPtr);
            ElementPtr->setParent(currentElement);

            if (showProcessing) {
                cout << lineNumber << " : " << currentLine << endl;
                cout << "*** Complete element found:" << endl;
                cout << "*** Element Name = " << tagName << endl;
                cout << "*** Element Content = " << content << endl;
                cout << "*** Creating new child: " << ElementPtr->getTagName()
                        << ", for parent: " << currentElement->getTagName() << endl;

                if (!ElementPtr->vecAttribute.empty()) {
                    cout << "*** Attributes found: " << endl;
                    ElementPtr->displayAttributes();
                }
                cout << "*** Stack unchanged" << endl;

            }
            // Sets the new element's level to be one higher than its parent. 
            ElementPtr->setLevel(ElementPtr->getParent()->getLevel() + 1);
            break;
        }

        case ELEMENT_CLOSING_TAG:

            // When a closing tag is found, if it does not match the top tag name
            // on the stack then there is a problem with the XML. The current state
            // is set to ERROR, which will end the program due to the condition of
            // the while loop in the 'openFile' function. 
        {
            if (showProcessing) {
                cout << lineNumber << " : " << currentLine << endl;
            }

            if (tagName != elementVect[elementVect.size() - 1]->getTagName()) {
                cout << "ERROR!!! - Closing tag on line " << lineNumber
                        << " does not match last opened tag, '"
                        << elementVect[elementVect.size() - 1]->getTagName() << "' ." << endl;
                ;
                cout << "XML is not well formed. Ending program." << endl;
                currentState = ERROR;
            }// If the closing tag does match the top element on the stack, the element
                // is popped and the updated state of the stack is displayed to confirm
                // the change. 
            else {

                // Sets the current element to be it's parent, moving up the tree. 
                currentElement = currentElement->getParent();

                elementVect.pop_back();

                if (showProcessing) {
                    cout << "*** Element closed = " << tagName << endl;
                    showStack(elementVect);
                }
            }

            break;
        }

        case SELF_CLOSING_TAG:
        {

            // Creates a new element and sets up the parent/child relationship with the current element.
            Element* ElementPtr = new Element(tagName, lineNumber, "Empty", currentLine);
            currentElement->addChild(ElementPtr);
            ElementPtr->setParent(currentElement);

            if (showProcessing) {
                cout << lineNumber << " : " << currentLine << endl;
                cout << "*** Self-closing element found: " << endl;
                cout << "*** Element Name = " << tagName << endl;
                cout << "*** Element Content = " << content << endl;
                cout << "*** Creating new child: " << ElementPtr->getTagName()
                        << ", for parent: " << currentElement->getTagName() << endl;

                if (!ElementPtr->vecAttribute.empty()) {
                    cout << "*** Attributes found: " << endl;
                    ElementPtr->displayAttributes();
                }
                cout << "*** Stack unchanged." << endl;
            }

            // Sets the new element's level to be one level higher that it's parent. 
            ElementPtr->setLevel(ElementPtr->getParent()->getLevel() + 1);
            break;
        }

        case UNKNOWN:
        {
            cout << currentLine << endl;
            cout << "Parser state unknown!" << endl;
            cout << "Last line checked was line number " << lineNumber << "." << endl;
            cout << "Entire line: " << currentLine << endl;
            cout << "XML is not well formed. Ending program." << endl;
            break;
        }

        default:
        {
            cout << currentLine << endl;
            cout << "ERROR!!!" << endl;
            cout << "XML is not well formed. Ending program." << endl;
            break;
        }
    }

    if (showProcessing) {
        cout << endl << endl;
    }
}

/**
 * This function opens and reads the passed text file, line by line. As each line
 * is taken from the input file, leading white space is removed, the current state
 * is determined by calling the 'parse' function, and the line is processed.
 * The 'processLine' function will also display relevant information about the 
 * current line. As each line is processed, an element tree is created.
 * @param strPath - The name of the file to be read.
 * @return Status flag - If the current state after reading the file is not 'ERROR', returns EXIT_SUCCESS. 
 */
int openfile(string strPath) {

    // Variable to store the current state of the document, which will be used
    // to parse and process each line. Initialized to STARTING_DOCUMENT. 
    ParserState currentState = STARTING_DOCUMENT;

    //Creates an object of the class ifstream and uses it to open the passed
    // file name. 
    ifstream infile;
    infile.open(strPath);

    //Initializes a string to store the current line being processed and 
    //initializes a line number counter. 
    string currentLine;
    int lineNumber = 1;

    //The entire top line of the passed file is removed and stored in the
    //variable "currentLine".
    getline(infile, currentLine);

    // These strings are used to store content and tag names extracted by the
    // 'currentState' function, which are then passed into the 'proccessLine'
    // function, and then in turn passed to the Element constructor. 
    string content;
    string tagName;

    cout << "Building tree for file: " << strPath << "..." << endl << endl;

    // This loop scans through and processes the passed file name line by line
    // until the last line has been reached. This will also terminate if the
    // current state is ERROR or UNKNOWN, which indicates that a passed XML file is not
    // well-formed. 
    while (!currentLine.empty() && currentState != ERROR && currentState != UNKNOWN) {

        //Passes the current line the trim function, removing any white space
        // and/or tabs in the line. 
        currentLine = trim(currentLine);

        // The current state is determined by the state returned from the previous
        // line and the text on the current line. 'Content' and 'tagName' are
        // passed to store extracted content and tag names. 
        currentState = parse(currentLine, currentState, content, tagName);

        // Processes the line depending upon the current state, displays relevant
        // information and updates the element pointer stack, 'elementVect' .
        proccesLine(currentLine, currentState, lineNumber, content, tagName);

        //Increments the line number, gets the next line of the file.
        lineNumber++;
        getline(infile, currentLine);
    }
    //Closes the input file.
    infile.close();

    // If after reading the XML file the current state is "ERROR", the function
    // returns the appropriate exit status. 
    if (currentState == ERROR || currentState == UNKNOWN) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }

}

/**
 * As each object within the pointer had memory allocated on the heap, they 
 * must each be deleted "manually". 
 * @param vect - The vector of Element objects, each of which is to be deleted.
 */
void clearVectorContents(vector <Element*> &vect) {

    for (vector<Element*>::iterator it = vect.begin(); it != vect.end(); ++it) {
        delete *it;
    }
    vect.clear();
}

/**
 * Function that is called recursively to output the element tree, which is already stored
 * in it's entirety in memory. 
 * @param currentElement - The current element whose information is to be displayed. 
 */
void displayTree(Element* currentElement) {

    // Begins tree output. 
    if (currentElement != emptyRoot) {

        for (int i = 1; i < currentElement->getLevel(); i++) {
            cout << ".";
        }
        cout << "Element \"" << currentElement->getTagName()
                << "\" was found at line " << currentElement->getLineNo()
                << ", level " << currentElement->getLevel();
        if (currentElement->getStrContent() == "Empty")
            cout << ", with no content";
        else
            cout << ", containing \"" << currentElement->getStrContent() << '"';
        cout << " and " << currentElement->getNoOfChildren();
        if (currentElement->getNoOfChildren() == 1)
            cout << " child";
        else
            cout << " children";

        if (currentElement->hasAttributes()) {

            cout << ", along with " << currentElement->vecAttribute.size();

            if (currentElement->vecAttribute.size() == 1)
                cout << " attribute: " << endl;
            else
                cout << " attributes: " << endl;

            currentElement->displayAttributes();
        } else
            cout << "." << endl;

        cout << endl;
    }// If the current element is the emptyRoot, which should be the parameter the first time this
        // function is called, then the user is told that the element tree has started to be displayed. 
    else {
        //Displays a line of '*' to separate the display of the tree building process
        for (int i = 0; i < 40; i++) {
            cout << "*";
        }
        cout << endl << endl << "Now showing the element tree:" << endl << endl;
    }

    // Checks if the current element has children, if so, the program iterates through
    // each child to calling this function for each. 
    if (!currentElement->vecChildren.empty()) {

        for (vector<Element*>::iterator it = currentElement->vecChildren.begin();
                it != currentElement->vecChildren.end(); ++it) {

            displayTree(*it);

        }
    }
}

/**
 * Recursively traverses the tree, delete elements that whose memory was allocated dynamically. 
 * @param currentElement - The element that is to be deleted after all it's children are deleted.
 */
void deleteTree(Element* currentElement) {

    if (!currentElement->vecChildren.empty()) {

        for (vector<Element*>::iterator it = currentElement->vecChildren.begin();
                it != currentElement->vecChildren.end(); ++it) {

            deleteTree(*it);
        }
    }
    delete currentElement;
    currentElement->vecChildren.clear();

}

/**
 * Function to as the user if they wish to have the tree building processed displayed for them.
 * If so, the boolean 'showProcessing' is set to true. 
 */
void askDisplay() {
    int input = 0;

    while (input != 1 && input != 2 && cin) {
        cout << "Would you like to see the tree as it is being built?" << endl;
        cout << "Enter 1 for yes or 2 for no:" << endl;
        cin>>input;

        switch (input) {
            case 1:
            {
                showProcessing = true;
                break;
            }
            case 2:
            {
                showProcessing = false;
                break;
            }
            default:
            {
                cout << "Invalid entry, please try again." << endl;
                cin.clear();
                cin.ignore(256, '\n');
                break;
            }
        }
    }
}

/**
 * Function to ask the user for an output format. Currently the only optional output
 * format is as an HTML file. I considered making the JSON output optional, but since
 * that is the purpose of the current assignment, the JSON file will be created no matter what. 
 * @return - A boolean value that will be checked before creating the HTML file of the tree. 
 */
bool askOutputFormat() {
    int input = 0;

    while (input != 1 && input != 2 && cin) {
        cout << "\nWould you like to also create an HTML file of the element tree?" << endl;
        cout << "Enter 1 for yes or 2 for no:" << endl;
        cin>>input;

        switch (input) {
            case 1:
            {
                return true;
                break;
            }
            case 2:
            {
                return false;
                break;
            }
            default:
            {
                cout << "Invalid entry, please try again." << endl;
                cin.clear();
                cin.ignore(256, '\n');
                break;
            }
        }
    }
    return false;
}

/**
 * Function to add spaces to an output file corresponding with the element's level.
 * This is simply to make the JSON file more readable and does not affect the file's
 * validity. 
 * @param currentElement - The current element who's level is be checked.
 * @param os - The output stream of a JSON file. Passed by reference. 
 */
void addSpaces(Element* currentElement, ofstream &os) {
    for (int i = 0; i < currentElement->getLevel(); i++) {
        os << " ";
    }
}

/**
 * This is an experimental function to output the element tree as an HTML file. 
 * This was suggested as an extra challenge for the assignment by Prof. Heines 
 * and is not part of the assignment's requirements. This is virtually identical
 * to the algorithm in the 'displayTree' function.
 * @param currentElement - The current element whose data is to be written to the HTML file. 
 * @param os - ofstream object that is sent recursively to the function. Passed by reference.
 */
void writeHTML(Element* currentElement, ofstream &os) {

    if (currentElement == emptyRoot) {
        cout << "Now outputting the element tree as an HTML file..." << endl;
        // If the current element is the root, standard opening code for an html file is written. 
        os << "<!DOCTYPE html>\n<html lang=\"en\">\n\n<head>\n <title></title>\n"
                " <meta charset=\"utf-8\">\n</head>\n<body>\n";

    }
    if (currentElement != emptyRoot) {
        os << "<div>";
        os << "<div>";
        for (int i = 1; i < currentElement->getLevel(); i++) {
            os << ".";
        }
        os << "Element \"" << currentElement->getTagName()
                << "\" was found at line " << currentElement->getLineNo()
                << ", level " << currentElement->getLevel();
        if (currentElement->getStrContent() == "Empty")
            os << ", with no content";
        else
            os << ", containing \"" << currentElement->getStrContent() << '"';
        os << " and " << currentElement->getNoOfChildren();
        if (currentElement->getNoOfChildren() == 1)
            os << " child";
        else
            os << " children";

        if (currentElement->hasAttributes()) {

            os << ", along with " << currentElement->vecAttribute.size();

            if (currentElement->vecAttribute.size() == 1)
                os << " attribute: ";
            else
                os << " attributes: ";

            for (vector<Attribute*>::iterator it = currentElement->vecAttribute.begin();
                    it != currentElement->vecAttribute.end(); it++) {
                os << "</div>";
                os << "<div>";
                for (int j = 1; j < currentElement->getLevel(); j++) {
                    os << ".";
                }

                os << "Attribute name: " << (*it)->getAttName();
                os << ", Attribute value: " << (*it)->getAttValue();
                os << "</div>";
            }

        } else {
            os << ".";
            os << "</div>";
        }

        os << "</div>" << endl;
    }

    // Checks if the current element has children, if so, the program iterates through
    // each child to calling this function for each. 
    if (!currentElement->vecChildren.empty()) {

        for (vector<Element*>::iterator it = currentElement->vecChildren.begin();
                it != currentElement->vecChildren.end(); ++it) {

            writeHTML(*it, os);
        }
    }

    // After the function has returned to the empty root, the ofstream is closed. 
    if (currentElement == emptyRoot) {
        os << "</body>";
        os.close();
        cout << "HTML output complete." << endl;
        cout << "File 'DJL_Assn05_Element_Tree.html' written to the folder dist." << endl;
    }
}

/**
 * Function to output the element tree as a JSON file. This a very similar
 * algorithm to the 'displayTree' and 'writeHTML' functions, though with changes
 * that allow for proper JSON syntax. The function is called recursively, traversing
 * the entire element tree and creating a matching JSON structure. 
 * @param currentElement - The element whose children and attributes are to be written to the JSON file.
 * @param os - The output stream to the JSON file. Passed by reference. 
 */
void writeJSON(Element* currentElement, ofstream &os) {


    if (currentElement == emptyRoot) {
        cout << "Now outputting the element tree as a JSON file..." << endl;
        os << "{\n";
    } else { //Checks if the current element is a parent. If so, the output is 
        //formatted to get ready to display its attributes and children. 
        if (!currentElement->vecChildren.empty()) {
            addSpaces(currentElement, os);
            os << "\"" << currentElement->getTagName() << "\":{\n";

            for (vector<Attribute*>::iterator it = currentElement->vecAttribute.begin();
                    it != currentElement->vecAttribute.end(); it++) {
                addSpaces(currentElement, os);
                os << "\"" << (*it)->getAttName() << "\":";
                os << "\"" << (*it)->getAttValue() << "\",\n";
            }

            // Makes sure the element is NOT self-closing and is a leaf. Outputs the
            // element's contents. 
        } else if (currentElement->getStrContent() != "Empty") {
            addSpaces(currentElement, os);
            os << "\"" << currentElement->getTagName() << "\":";


            /* Several musicbrainz XML files have double quotes in the element
            // content, (12" Vinyl for example), the following algorithm adds a 
              forward slash before any quotes found within a tag's content. */
            if (currentElement->getStrContent().find('\"') != string::npos) {
                string content = currentElement->getStrContent();
                for (int i = 0; i < content.size(); i++) {
                    if (content[i] == '\"') {
                        content = content.substr(0, i) + '\\' + content.substr(i, content.size());
                        i++;
                        currentElement->setStrContent(content);
                    }
                }
            }

            os << "\"" << currentElement->getStrContent() << "\"";

            if (currentElement->getParent()->vecChildren.back() == currentElement) {
                os << "\n";
            } else {
                os << ",\n";
            }
        }
        // Checks if the current element is a self-closing. If there are attributes, they are 
        // printed as sub-objects. Otherwise, the element value is "Empty".
        if (currentElement->getStrContent() == "Empty" && currentElement->vecChildren.empty()) {
            addSpaces(currentElement, os);
            os << "\"" << currentElement->getTagName() << "\"";

            if (currentElement->hasAttributes()) {
                os << ":{\n";
                for (vector<Attribute*>::iterator it = currentElement->vecAttribute.begin();
                        it != currentElement->vecAttribute.end(); it++) {

                    addSpaces(currentElement, os);
                    os << "\"" << (*it)->getAttName() << "\":";
                    os << "\"" << (*it)->getAttValue() << "\"";

                    if (it != currentElement->vecAttribute.end() - 1) {
                        os << ",\n";
                    } else {
                        os << "\n";
                        addSpaces(currentElement, os);
                        os << "},\n";
                    }
                }
            } else {
                os << ":\"Empty\",\n";
            }
        }
    }
    // Calls the 'writeJSON' function for each of the current element's children. 
    for (vector<Element*>::iterator it = currentElement->vecChildren.begin();
            it != currentElement->vecChildren.end(); ++it) {
        writeJSON(*it, os);
    }

    // After their children as run through the 'writeJSON' function, the braces for each 
    // parent are closed. 
    if (currentElement->getStrContent() == "Empty" && !currentElement->vecChildren.empty()) {
        if (currentElement->getParent()->vecChildren.back() == currentElement) {
            addSpaces(currentElement, os);
            os << "}\n";
        } else {
            addSpaces(currentElement, os);
            os << "},\n";
        }
    }

    // Closes the entire JSON file. 
    if (currentElement == emptyRoot) {
        os << "}\n";
        cout << "JSON output complete." << endl;
        cout << "File 'DJL_Assn05_Element_Tree.json' written to the folder dist." << endl;
    }

}

/**
 * Standard C++ main function. 
 * @param argc - Number of command line arguments. 
 * @param argv - Array of pointers to command line arguments. 
 * @return - Returns error code if the program does not end properly. 
 */
int main(int argc, char** argv) {

    // Asks the user if they would like for the tree building process to be displayed. 
    askDisplay();

    /**
     * If the 'openFile' function exits successfully, the element tree is displayed,
     * converted to a JSON structure and exported, exported as an HTML file, then
     * deleted recursively. 
     */
    if (openfile("Assignment5_musicFile.xml") == EXIT_SUCCESS) {

        displayTree(currentElement);

        //Converts the element tree to a JSON structure and writes it to an external file
        ofstream jsonStream;
        jsonStream.open("dist/DJL_Assn05_Element_Tree.json");
        writeJSON(currentElement, jsonStream);

        //Writes the element tree to an html file.

        if (askOutputFormat()) {
            ofstream htmlStream;
            htmlStream.open("dist/DJL_Assn05_Element_Tree.html");
            writeHTML(currentElement, htmlStream);
        }

        //Recursively deletes all element objects.
        deleteTree(currentElement);
    }

    // Clears any remaining contents of the 'elementVect' vector, deleting each
    // object that any pointers in the vector may point to. 
    clearVectorContents(elementVect);

    return 0;
}
