/* File:   Element.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on October 11th, 2014, 4:46 PM, modified on Oct 23rd at 4:45 PM to
 * add additional documentation. 
 * 
 * This is the implementation file for the Element class, which is
 * used when reading an XML file. Each object of the class can store
 * a tag's name, what line it was found on, the element content, the element's 
 * attributes, its level within an element tree, children and a reference to it's parent. 
 * Much of the design for this class was taken from the lecture notes of Prof.
 * Jesse Heines of UMass Lowell for the Fall 2014 91.204-Computing IV class.
 */

#include "Element.h"

/**
 * Default constructor. Sets strTagName and strContent to be an 
 * empty string, sets lineNumber to 0.
 */
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
 * Custom constructor. Assigns the passed parameters to the newly created element's
 * member variables. Used in the program whenever a new element is created. Also
 * calls the 'findAttributes' function which will scan the line on which the element
 * was found for attributes and populate a vector of found attributes. Also sets the
 * to element's level to 0 as the 'setLevel' function is called after the new element
 * is created. This should be re-factored in future versions. 
 * @param tagName - A string which is used to store the element's tag name.
 * @param nLineNo - The line number of the XML file on which the element was found. 
 * @param content - A string which is used to store the element's content. 
 * @param currentLine - The entire line of an XML file on which the element was found,
 *                      used when 'findAttributes' is called. 
 */
Element::Element(string tagName, int nLineNo, string content, string currentLine) {

    strContent = content;

    strTagName = tagName;

    lineNumber = nLineNo;

    level = 0;

    findAttributes(currentLine);
}

/**
 * Standard destructor.
 */
Element::~Element() {
}

/**
 * Standard getter for the element's tag name, used throughout the program
 * for output and for building the tree. 
 * @return strTagName - The string stored as the elements tag name. 
 */
string Element::getTagName() const {
    return strTagName;
}

/**
 * Standard getter for the line number that a element is found on in an XML file.
 * Used throughout the program for output and organizes the the building of the tree.
 * @return lineNumber - The line in an XML file on which an element was found. 
 */
int Element::getLineNo() const {
    return lineNumber;
}

/**
 * Standard getter for the content of an element. If the element has no content
 * this will simply return "Empty". Used when displaying the tree structure. 
 * @return strContent - The content of the current element. Defaults to "Empty".
 */
string Element::getStrContent() const {
    return strContent;
}

/**
 * Standard setter for the element's tag name. Called when a new element is created.
 * @param tagName - A string which is to be stored as the element's tag name.
 */
void Element::setTagName(string tagName) {
    strTagName = tagName;
}

/**
 * Standard setter for the element's content. Called when a new element is created.
 * @param content - A string which is to be stored as the element's content.
 */
void Element::setStrContent(string content) {
    strContent = content;
}

/**
 * Function used to output the element's attribute names and values. This is used during the 
 * tree building process and after the tree has been built. During tree building, 
 * this function is called before the element's level has been set. The function
 * uses this default level to determine if the tree is currently being built. 
 * This behavior is not reliable and should be updated in future versions.
 */
void Element::displayAttributes() {

    if (!vecAttribute.empty()) {

        for (int i = 0; i < vecAttribute.size(); i++) {
            if (this->getLevel() != 0) {
                for (int j = 1; j < this->getLevel(); j++) {
                    cout << ".";
                }
            } else {
                cout << "*** ";
            }

            cout << "Attribute name: " << vecAttribute[i]->getAttName();
            cout << ", Attribute value: " << vecAttribute[i]->getAttValue() << endl;
        }
    }
}

/**
 * Function that scans an entire XML line to find attribute names
 * and their values. When attributes are found, a new attribute object is created
 * and pushed onto the current element's attribute vector, "vecAttribute". Also,
 * the function's algorithm is executed twice, searching for double quotes and single
 * quotes respectively. 
 * @param currentLine - A complete XML line which in which attributes are searched for. 
 */
void Element::findAttributes(string currentLine) {
    string name;
    string value;

    int searchStart = 0;
    int i = 0;

    while (currentLine.find("=\"", searchStart) != string::npos) {

        name = currentLine.substr(currentLine.find(' ', searchStart) + 1,
                currentLine.find('=', searchStart) -
                currentLine.find(' ', searchStart) - 1);

        int valStart = currentLine.find("=\"", searchStart) + 2;
        int valEnd = currentLine.find('"', valStart);

        value = currentLine.substr(valStart, valEnd - valStart);

        Attribute* AttributePtr = new Attribute(name, value);

        vecAttribute.push_back(AttributePtr);

        searchStart = currentLine.find("\" ", searchStart + 1);

        i++;
    }

    while (currentLine.find("=\'", searchStart) != string::npos) {

        name = currentLine.substr(currentLine.find(' ', searchStart) + 1,
                currentLine.find('=', searchStart) -
                currentLine.find(' ', searchStart) - 1);

        int valStart = currentLine.find("=\'", searchStart) + 2;
        int valEnd = currentLine.find('\'', valStart);

        value = currentLine.substr(valStart, valEnd - valStart);

        Attribute* AttributePtr = new Attribute(name, value);

        vecAttribute.push_back(AttributePtr);

        searchStart = currentLine.find("\' ", searchStart + 1);

        i++;
    }
}

/**
 * Function to store pointers to children of the element. Pointers are pushed
 * on to the element's 'vecChildren' vector and the number of children is incremented. 
 * This is called in the program just after a new element is created. In future versions
 * this should be called in the element's constructor. 
 * @param newChild - A pointer to an element that is the child of the element object.
 */
void Element::addChild(Element* newChild) {
    noOfChildren++;
    vecChildren.push_back(newChild);
}

/**
 * Function to store a pointer to the current element's parent. This is called
 * in the program just after a new element is created. In future versions
 * this should be called in the element's constructor. 
 * @param Parent - A pointer to an element that is the parent of the element object. 
 */
void Element::setParent(Element* Parent) {
    parent = Parent;
}

/**
 * Function that returns a pointer to the current element's parent. This is used
 * for creating the element tree as well as setting the element object's level. 
 * @return parent - A pointer to the element object's parent. 
 */
Element* Element::getParent() {
    return parent;
}

/**
 * Returns the number of children that an element object contains. This is called
 * as the element tree is being displayed. 
 * @return noOfChildren - An integer which is the number of children that the element object has. 
 */
int Element::getNoOfChildren() {
    return noOfChildren;
}

/**
 * A function that returns the level on which an element was found. This is called
 * when new element objects are being created to retrieve the level of their parent.
 * This is also called as the element tree is being displayed.
 * @return level - The level within the element tree in which the element object is located. 
 */
int Element::getLevel() {
    return level;
}

/**
 * Sets an element objects level as the element tree is being created. 
 * In future versions this should be called within the element constructor. 
 * @param newLevel - The level in which an element is found within the element tree.
 *                   This parameter is always passed as the element object's parent's level + 1.
 */
void Element::setLevel(int newLevel) {
    level = newLevel;
}

/**
 * A simple boolean function that determines if the element object contains
 * attributes or not. This is done by checking if the 'vecAttribute' vector is empty.
 * This is called as the tree is being displayed. 
 * @return - Boolean value referencing the presence or lack of attributes for the element. 
 */
bool Element::hasAttributes() {
    if (vecAttribute.empty())
        return false;
    else
        return true;
}