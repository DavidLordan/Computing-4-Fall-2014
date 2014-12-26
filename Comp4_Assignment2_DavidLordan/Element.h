/* File:   Element.h
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on September 13, 2014, 4:29 PM, modified on Sept 19th at 8:30 PM
 * 
 * This is a standard header file for the Element class, which is
 * used when reading an XML file. Each object of the class can store
 * a tag's name, what line it was found on and its number of appearances.
 * It should be noted that the number of appearances at this time must be kept
 * track of via an outside function or method, but the setAppearances function
 * can be used to update this number.
 *  Much of the design for this class was taken from the lecture notes of 
 * Jesse Heines of UMass Lowell for the Fall 2014 91.204-Computing IV class. 
 */

#ifndef ELEMENT_H
#define	ELEMENT_H

#include <string>
#include <iostream>

using namespace std;

class Element {
public:
    //Default constructor
    //Sets the strTagName to be an empty string, sets lineNumber and 
    //numOfAppearances to 0
    Element();
    //Copy constructor
    //Creates a new Element object setting each element variable to match the
    //object it is copied from
    Element(const Element& orig);
    //Destructor
    virtual ~Element();

    /**
     * Custom constructor
     * This constructor will tag a full opening tag and extract the tag name,
     * cutting out the opening and closing braces as well as any attributes,
     * leaving only the tag name. Also sets the line number to whatever int is
     * pass and sets  numOfAppearances to 1.
     * @param strElementName - The unedited tag to be used for strElementName
     * @param nLineNo - The number on which the tag was found
     */
    Element(string strElementName, int nLineNo);


    //The remaining functions are standard getters for each of the class's 
    //private variables, as a well as a setter for numOfAppearances.
    int getLineNo() const;

    string getStrElementName() const;

    int getAppearances() const;

    void setAppearances(int num);

private:
    //Private variables which store the particular name of a tag, the line on
    //which it was found and its number of appearances. 
    string strTagName;

    int lineNumber;

    int numOfAppearances;

};

#endif	/* ELEMENT_H */

