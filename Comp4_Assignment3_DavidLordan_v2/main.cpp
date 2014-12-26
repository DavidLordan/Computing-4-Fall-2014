/* File:   main.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on September 28, 2014, 7:40 PM, modified on Oct 1st at 8:05 PM to 
 * add additional documentation. 
 * This program reads an XML file, then tokenizes and parses each line. As 
 * tags are opened, element objects are created and their pointers are pushed 
 * to a vector acting as a stack. As the tags are closed, the element pointers
 * are removed from the stack and the objects deleted. Finally, as the program
 * processes each line, the entire line, the current state of the xml document,
 * tag name and content when applicable are all displayed, as well as
 * any changes to the stack. 
 */

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Element.h"

using namespace std;

//This is a vector that will store pointers element objects, which are created
//when opening tags are found (unless they are self-closing). Pointers will be
// popped from the vector as tags are closed. This vector is acting as a stack,
// where each tag closed should match the most recent tag opened.
vector<Element*> elementVect;

//Enumeration for each of the possible states of XML document. These will be 
//used extensively in the program to determine the context of a particular 
//line of text. These states are determined by the parse function below. 
// This was made by Prof. Jesse Heines of UML for the 91.204 Computing IV course
// and is taken from that course's lecture notes. 
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
    if (currentLine.find('>') == -1) {

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
                currentLine = currentLine.substr(4, currentLine.rfind('-') - 5);
                content = currentLine;
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

    while (str[0] == ' ' || str[0] == '\t') {
        str.erase(str.begin());
    }
    while (str[str.length() - 1] == ' ' || str[str.length() - 1] == '\t') {
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
        for (int i = 0; i <= elementVect.size() - 1; i++) {
            cout << elementVect[i]->getTagName();
            // Simply adds a period at the end of the list, otherwise the element
            // names are separated with a comma. 
            if (i == elementVect.size() - 1)
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
 * Function to process each line and display relevant information, such as the
 * line itself, the line number, the current state, extracted element information
 * if applicable and changes to the stack if applicable. The function also uses
 * the extracted information from the parse function to create element objects
 * when necessary, adding pointers to those objects to the stack, or removing them
 * when the matching closing tags are found. 
 * @param currentLine   -   The current line from the XML file being processed. 
 * @param currentState  -   The current state as determined by the parse function.
 * @param lineNumber    -   The line number of the current line. 
 * @param content       -   The extracted content of the current line.
 * @param tagName       -   The extracted tag name of the current line. 
 */
void proccesLine(string currentLine, ParserState &currentState, int lineNumber, string content, string tagName) {
    // Depending upon the current state, an appropriate output is displayed, 
    // and the proper actions are taken with the stack. 

    switch (currentState) {

        case STARTING_COMMENT:
        {
            cout << lineNumber << ": " << currentLine << endl;
            cout << "*** Comment started";
            break;
        }
        case IN_COMMENT:
        {
            cout << lineNumber << ": " << currentLine << endl;
            cout << "*** Comment line";
            break;
        }

        case ENDING_COMMENT:
        {
            cout << lineNumber << ": " << currentLine << endl;
            cout << "*** Ending comment";
            break;
        }
        case ONE_LINE_COMMENT:
        {
            cout << lineNumber << ": " << content << endl;
            cout << "*** One line comment";
            break;
        }

        case DIRECTIVE:
        {
            cout << lineNumber << " : " << currentLine << endl;
            cout << "*** Directive = " << content << endl;
            break;
        }

        case ELEMENT_OPENING_TAG:
            // If the current line is determined to be an opening tag, a new element
            // object is created and its pointer pushed to the stack. The stack contents
            // are they listed to confirm the change. 

        {
            cout << lineNumber << " : " << currentLine << endl;
            cout << "*** Element Opened = " << tagName << endl;
            Element* ElementPtr = new Element(tagName, lineNumber, content);
            elementVect.push_back(ElementPtr);
            showStack(elementVect);
            break;
        }

        case ELEMENT_NAME_AND_CONTENT:
        {

            cout << lineNumber << " : " << currentLine << endl;
            cout << "*** Complete element found:" << endl;
            cout << "*** Element Name = " << tagName << endl;
            cout << "*** Element Content = " << content << endl;
            cout << "*** Stack unchanged" << endl;
            break;
        }

        case ELEMENT_CLOSING_TAG:
            // When a closing tag is found, if it does not match the top tag name
            // on the stack then there is a problem with the XML. The current state
            // is set to ERROR, which will end the program due to the condition of
            // the while loop in the 'openFile' function. 
        {
            cout << lineNumber << " : " << currentLine << endl;

            if (tagName != elementVect[elementVect.size() - 1]->getTagName()) {
                cout << "ERROR!!! - Closing tag  on line " << lineNumber
                        << " does not match last opened tag, '"
                        << elementVect[elementVect.size() - 1]->getTagName() << "' ." << endl;
                ;
                cout << "XML is not well formed. Ending program." << endl;
                currentState = ERROR;
            }// If the closing tag does match the top element on the stack, the element
                // is popped and the updated state of the stack is displayed to confirm
                // the change. 
            else {
                cout << "*** Element closed = " << tagName << endl;
                delete elementVect[elementVect.size() - 1];
                elementVect.pop_back();
                showStack(elementVect);
            }

            break;
        }

        case SELF_CLOSING_TAG:
        {
            cout << lineNumber << " : " << currentLine << endl;
            cout << "*** Self-closing element found: " << endl;
            cout << "*** Element Name = " << tagName << endl;
            cout << "*** Element Content = " << content << endl;
            cout << "*** Stack unchanged." << endl;
            break;
        }

        case UNKNOWN:
        {
            cout << currentLine << endl;
            cout << "Parser state unknown" << endl;
            break;
        }

        default:
        {
            cout << currentLine << endl;
            cout << "ERROR!!!" << endl;
            break;
        }
    }

    cout << endl << endl;

}

/**
 * This function opens and reads the passed text file, line by line. As each line
 * is taken from the input file, leading white space is removed, the current state
 * is determined by calling the 'parse' function, and the line is processed.
 * The 'processLine' function will also display relevant information about the 
 * current line. 
 * @param strPath - The name of the file to be read.
 * @return Status flag EXIT_SUCCESS.
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

    //This loop scans through and processes the passed file name line by line
    // until the last line has been reached. This will also terminate if the
    // current state is ERROR, which indicates that a passed XML file is not
    // well-formed. 
    while (!currentLine.empty() && currentState != ERROR) {

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
    return EXIT_SUCCESS;
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

int main(int argc, char** argv) {

    //Calls the open file function and passes the file to open, which is then
    // analyzed and processed, with information about each line displayed.
    openfile("Assignment3_musicFile.xml");


    // Clears any remaining contents of the 'elementVect' vector, deleting each
    // object that any pointers in the vector may point to. 
    clearVectorContents(elementVect);

    return 0;
}
