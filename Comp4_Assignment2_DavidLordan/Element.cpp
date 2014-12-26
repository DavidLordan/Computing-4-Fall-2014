/* File:   Element.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on September 13, 2014, 4:29 PM, modified on Sept 19th at 8:42 PM
 * 
 * This is the implementation file for the Element class, which is
 * used when reading an XML file. Each object of the class can store
 * a tag's name, what line it was found on and its number of appearances.
 * It should be noted that the number of appearances at this time must be kept
 * track of via an outside function or method, but the setAppearances function
 * can be used to update this number.
 * Much of the design for this class was taken from the lecture notes of Prof.
 * Jesse Heines of UMass Lowell for the Fall 2014 91.204-Computing IV class.
 */
#include "Element.h"


//Default constructor
//Sets the strTagName to be an empty string, sets lineNumber and 
//numOfAppearances to 0

Element::Element() {
    strTagName = "";
    lineNumber = 0;
    numOfAppearances = 0;
}
//Copy constructor
//Creates a new Element object setting each element variable to match the
//object it is copied from

Element::Element(const Element& orig) {
    strTagName = orig.getStrElementName();
    lineNumber = orig.getLineNo();
    numOfAppearances = orig.getAppearances();
}

/**
 * Custom constructor
 * This constructor will tag a full opening tag and extract the tag name,
 * cutting out the opening and closing braces as well as any attributes,
 * leaving only the tag name. Also sets the line number to whatever number is
 * passed and sets  numOfAppearances to 1.
 * @param strElementName - The unedited tag to be used for strElementName
 * @param nLineNo - The number on which the tag was found
 */
Element::Element(string strElementName, int nLineNo) {

    //Creates a substring from the passed string up until the '>' character
    //is found, isolating the opening tag. A 2nd substring is created from the
    // 1st up to when any white space is found, removing any attributes which
    // may exist in the tag and leaving only the element name. 
    strElementName = strElementName.substr(1, strElementName.find('>') - 1);
    strElementName = strElementName.substr(0, strElementName.find(' '));
    strTagName = strElementName;

    lineNumber = nLineNo;

    numOfAppearances = 1;
}
//Destructor

Element::~Element() {
}


//The remaining functions are standard getters for each of the class's 
//private variables, as a well as a setter for numOfAppearances.

string Element::getStrElementName() const {
    return strTagName;
}

int Element::getLineNo() const {
    return lineNumber;
}

int Element::getAppearances() const {
    return numOfAppearances;
}

void Element::setAppearances(int num) {
    numOfAppearances = num;
}
