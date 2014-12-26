/* 
 * File:   Attributes.h
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on October 11th, 2014, 5:20 PM, modified on Oct 23rd at 4:33 PM to 
 * add additional documentation. 
 * 
 * This is a basic header file for the 'Attributes' class. Objects of this class store
 * an attribute name and value of an XML element. The class also has standard setter and 
 * getter methods to set or retrieve those variables. 
 */

#ifndef ATTRIBUTE_H
#define	ATTRIBUTE_H

#include <string>

using namespace std;

class Attribute {
public:

    /**
     * Default attribute constructor. Sets the attribute name and value to empty strings. 
     */
    Attribute();

    /**
     * Standard copy constructor supplied by Net-Beans. 
     * @param orig - Another attribute object who's variables are used for the new object. 
     */
    Attribute(const Attribute& orig);

    /**
     * Basic object destructor supplied by Net-Beans. 
     */
    virtual ~Attribute();

    /**
     * Custom constructor. Sets the attribute name and value to the passed parameters.
     * Called whenever a new attribute object is created. 
     * @param newName - The name extracted from an XML element opening tag. 
     * @param newValue - The value extracted from an XML element opening tag. 
     */
    Attribute(string newName, string newValue);

    /**
     * Standard setter for the attribute name. Called in the custom attribute constructor.
     * @param newName - Extracted attribute name from an XML element opening tag. 
     */
    void setAttName(string newName);

    /**
     * Standard setter for the attribute value. Called in the custom attribute constructor. 
     * @param newValue - Extracted attribute value from an XML element opening tag. 
     */
    void setAttValue(string newValue);

    /**
     * Standard getter for the attribute name. Used when displaying the element tree. 
     * @return 
     */
    string getAttName();

    /**
     * Standard getter for the attribute value. Used when displaying the element tree.
     * @return value - The attribute object's value. 
     */
    string getAttValue();

private:

    /** The attribute name that is extracted from the XML element opening tag.*/
    string name;

    /** The attribute value that is extracted from the XML element opening tag.*/
    string value;
};

#endif	/* ATTRIBUTE_H */

