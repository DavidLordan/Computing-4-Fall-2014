/* File:   Element.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on September 13, 2014, 4:29 PM, modified on Oct 1st at 8:42 PM to
 * add additional documentation. 
 * 
 * This is the implementation file for the Element class, which is
 * used when reading an XML file. Each object of the class can store
 * a tag's name, what line it was found on, and the element content. 
 * Much of the design for this class was taken from the lecture notes of Prof.
 * Jesse Heines of UMass Lowell for the Fall 2014 91.204-Computing IV class.
 */
#include "Element.h"


//Default constructor
//Sets the strTagName and strContent to be an empty string, sets lineNumber and 
//numOfAppearances to 0.
Element::Element() {
    strTagName = "";
    lineNumber = 0;
    strContent = "";
}

/**
 * Copy constructor
 * Creates a new Element object setting each element variable to match the
 * object it is copied from.
 * @param orig  -   Original element object which is copied. 
 */
Element::Element(const Element& orig) {
    strTagName = orig.getTagName();
    lineNumber = orig.getLineNo();
    strContent = orig.getStrContent();
}

/**
 * Custom constructor.
 * This constructor will create a new element object, storing the element tag
 * name, content, and the line number on which it was found. 
 * @param tagName   -   The name of the tag, which is stored in strTagName
 * @param nLineNo   -   The line number on which the element was found.
 * @param content   -   The element content, stored in strContent.
 */
Element::Element(string tagName, int nLineNo, string content) {

    strContent = content;
    
    strTagName = tagName;

    lineNumber = nLineNo;
  
}

//Destructor
Element::~Element() {
}


//The remaining functions are standard getters for each of the class's 
//private variables.

string Element::getTagName() const {
    return strTagName;
}

int Element::getLineNo() const {
    return lineNumber;
}

string Element::getStrContent() const{
    return strContent;
}

void Element::setTagName(string tagName){
    strTagName = tagName;
}
    
void Element::setStrContent(string content){
    strContent = content;
    
 }