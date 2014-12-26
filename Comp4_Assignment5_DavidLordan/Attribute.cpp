/* 
 * File:   Attributes.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on October 11th, 2014, 5:20 PM, modified on Oct 23rd at 4:41 PM to 
 * add additional documentation. 
 * 
 * This is a basic implementation file for the 'Attributes' class. Objects of this class store
 * an attribute name and value of an XML element. The class also has standard setter and 
 * getter methods to set or retrieve those variables. 
 */

#include "Attribute.h"

/**
 * Default attribute constructor. Sets the attribute name and value to empty strings. 
 */
Attribute::Attribute() {
    setAttName("");
    setAttValue("");
}

/**
 * Standard copy constructor supplied by Net-Beans. 
 * @param orig - Another attribute object who's variables are used for the new object. 
 */
Attribute::Attribute(const Attribute& orig) {

}

/**
 * Basic object destructor supplied by Net-Beans. 
 */
Attribute::~Attribute() {
}

/**
 * Custom constructor. Sets the attribute name and value to the passed parameters.
 * Called whenever a new attribute object is created. 
 * @param newName - The name extracted from an XML element opening tag. 
 * @param newValue - The value extracted from an XML element opening tag. 
 */
Attribute::Attribute(string newName, string newValue) {
    name = newName;
    value = newValue;
}

/**
 * Standard getter for the attribute name. Used when displaying the element tree. 
 * @return 
 */
string Attribute::getAttName() {
    return name;
}

/**
 * Standard getter for the attribute value. Used when displaying the element tree.
 * @return value - The attribute object's value. 
 */
string Attribute::getAttValue() {
    return value;
}

/**
 * Standard setter for the attribute name. Called in the custom attribute constructor.
 * @param newName - Extracted attribute name from an XML element opening tag. 
 */
void Attribute::setAttName(string newName) {
    name = newName;
}

/**
 * Standard setter for the attribute value. Called in the custom attribute constructor. 
 * @param newValue - Extracted attribute value from an XML element opening tag. 
 */
void Attribute::setAttValue(string newValue) {
    value = newValue;
}