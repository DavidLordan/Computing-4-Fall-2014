/* File:   main.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on September 12, 2014, 12:29 PM, modified on Sept 19th at 3:15 PM
 * This program reads an XML file, processes it and displays each tag name
 * in alphabetical order along with the line on which the first occurrence of a
 * particular tag was found, along with the number of times the tag was found. 
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
 * This function alphabetizes the element names and removes duplicates found
 * in the passed vector. This function also keeps track of and updates the
 * number of occurrences of a particular element name. 
 * This was previously split up as two separate functions, one to alphabetize
 * and one to remove the duplicates and updates occurrences. However, the remove
 * duplicates function only worked properly if the elements had already been 
 * alphabetized. I decided to merge them to reduce interdependence and avoid
 * possible problems with using these functions in the future.  
 * @param vect - Element pointer vector. 
 * @return New vector of element-pointers with unique, alphabetized element 
 * names.
 */
vector <Element*> alphabetize_remDuplicates(vector<Element*> vect) {

    string str1;
    string str2;

    // This loop was the original alphabetize function.
    for (int i = 0; i < vect.size() - 1; i++) {

        str1 = vect[i]->getStrElementName();
        str2 = vect[i + 1]->getStrElementName();

        if (str1.compare(str2) > 0) {
            swap(vect[i], vect[i + 1]);
            i = -1; // If any adjacent elements are
            // found to NOT be in alphabetical
            // order, the loop resets.
        }
    }

    // This loop was was the original "remove duplicates" function.
    for (int i = 0; i < vect.size() - 1; i++) {

        str1 = vect[i]->getStrElementName();
        str2 = vect[i + 1]->getStrElementName();

        if (str1.compare(str2) == 0) {

            vect.erase(vect.begin() + i + 1);
            vect[i]->setAppearances(vect[i]->getAppearances() + 1);
            i = -1;
        }
    }

    return vect; // Returns a new vector with the element names 
    // alphabetized and the duplicates removed. 
}

/**
 * This function reads the file passed to it as a parameter and populates a
 * vector of element-object pointers. Each object contains an element name 
 * found on a particular line of the passed file, as well as the line number. 
 * @param strPath - The name of the file to be read.
 * @param elementVect - A vector of element-object pointers. Passed by ref.
 * @return Status flag EXIT_SUCCESS.
 */
int openfile(string strPath, vector<Element*> &elementVect) {

    //Creates an object of the class ifstream and uses it to open the passed
    // file name. 
    ifstream infile;
    infile.open(strPath);

    //Initializes a string to store the current line being processed and 
    //initializes a line number counter. 
    string currentLine;
    int lineNumber = 0;

    //The entire top line of the passed file is removed and stored in the
    //variable "currentLine".
    getline(infile, currentLine);

    //This loop scans through and processes the passed file name line by line
    // until the last line has been reached. 
    while (!currentLine.empty()) {
        //Passes the current line the trim function, removing any white space
        // and/or tabs in the line. 
        currentLine = trim(currentLine);

        //Checks to see if current line is anything other than an opening tag.
        //XML closing tags, comments and processing instructions are ignored. 
        if (currentLine[1] != '?'
                && currentLine[1] != '!'
                && currentLine[1] != '/') {

            //Creates a new object of the element class, passes the current,
            // trimmed line along with the line number to the Element class
            // constructor. A pointer to the new object is added to the vector. 
            Element* ElementPtr = new Element(currentLine, lineNumber + 1);
            elementVect.push_back(ElementPtr);
        }

        //Increments the line number, gets the next line of the file.
        lineNumber++;
        getline(infile, currentLine);

    }
    //Closes the input file.
    infile.close();
    return EXIT_SUCCESS;
}

/**
 * Displays the element names along with the line that the FIRST OCCURENCE 
 * was found on, along with the number of occurrences.
 * @param elementVect - The vector of Element object pointers whose content is
 * displayed.
 */
void displayElements(vector<Element*> elementVect) {

    for (int i = 0; i < elementVect.size(); i++) {
        cout << "Element \"" << elementVect[i]->getStrElementName()
                << "\" was first found on line "
                << elementVect[i]->getLineNo() << ". Number of appearances: "
                << elementVect[i]->getAppearances() << endl;
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

int main(int argc, char** argv) {

    //Initializes a vector to store Element object pointers.
    vector<Element*> elementVect;


    //Calls the open file function and passes the file to open along with the
    // vector to store Element object pointers.
    openfile("musicFile.xml", elementVect);

    //Alphabetizes and removes the duplicates of element names with elementVect.
    //This was originally two separate functions, but as one was dependent on 
    //the other, they were merged.
    elementVect = alphabetize_remDuplicates(elementVect);

    //Displays the elements names along with their line numbers and number of
    //occurrences. Each object is then deleted from the vector.
    displayElements(elementVect);
    clearVectorContents(elementVect);
    return 0;
}
