/* File:   Element.h
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on September 13, 2014, 4:29 PM, modified on Oct 1st at 8:22 PM to
 * add additional documentation. 
 * 
 * This is the header file for the Element class, which is
 * used when reading an XML file. Each object of the class can store
 * a tag's name, what line it was found on, and the element content. 
 * Much of the design for this class was taken from the lecture notes of Prof.
 * Jesse Heines of UMass Lowell for the Fall 2014 91.204-Computing IV class.
 */


#ifndef ELEMENT_H
#define	ELEMENT_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Element {
public:

    //Default constructor
    //Sets the strTagName and strContent to be an empty string, sets lineNumber and 
    //numOfAppearances to 0.
    Element();

    /**
     * Copy constructor
     * Creates a new Element object setting each element variable to match the
     * object it is copied from.
     * @param orig  -   Original element object which is copied. 
     */
    Element(const Element& orig);

    //Destructor
    virtual ~Element();

    /**
     * Custom constructor.
     * This constructor will create a new element object, storing the element tag
     * name, content, and the line number on which it was found. 
     * @param tagName   -   The name of the tag, which is stored in strTagName
     * @param nLineNo   -   The line number on which the element was found.
     * @param content   -   The element content, stored in strContent.
     */
    Element(string strElementName, int nLineNo, string content);


    //The remaining functions are standard getters for each of the class's 
    //private variables.
    int getLineNo() const;

    string getTagName() const;

    string getStrContent() const;

    void setStrContent(string content);

    void setTagName(string tagName);



private:
    //Private variables which store the particular name of a tag, the line on
    //which it was found and the tag content.
    string strTagName;

    int lineNumber;

    string strContent;

};

#endif	/* ELEMENT_H */

