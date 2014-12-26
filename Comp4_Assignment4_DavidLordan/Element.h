/* File:   Element.h
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on October 11th, 2014, 4:46 PM, modified on Oct 23rd at 4:43 PM to
 * add additional documentation. 
 * 
 * This is the header file for the Element class, which is
 * used when reading an XML file. Each object of the class can store
 * a tag's name, what line it was found on, the element content, the element's 
 * attributes, its level within an element tree, children and a reference to it's parent. 
 * Much of the design for this class was taken from the lecture notes of Prof.
 * Jesse Heines of UMass Lowell for the Fall 2014 91.204-Computing IV class.
 */


#ifndef ELEMENT_H
#define	ELEMENT_H

#include <string>
#include <iostream>
#include <vector>
#include "Attribute.h"

using namespace std;

class Element {
public:

    /**
     * Default constructor. Sets strTagName and strContent to be an 
     * empty string, sets lineNumber to 0.
     */
    Element();

    /**
     * Copy constructor
     * Creates a new Element object setting each element variable to match the
     * object it is copied from.
     * @param orig  -   Original element object which is copied. 
     */
    Element(const Element& orig);

    /**
     * Standard destructor.
     */
    virtual ~Element();

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
    Element(string strElementName, int nLineNo, string content, string currentLine);

    /**
     * Standard getter for the line number that a element is found on in an XML file.
     * Used throughout the program for output and organizes the the building of the tree.
     * @return lineNumber - The line in an XML file on which an element was found. 
     */
    int getLineNo() const;

    /**
     * Standard getter for the element's tag name, used throughout the program
     * for output and for building the tree. 
     * @return strTagName - The string stored as the elements tag name. 
     */
    string getTagName() const;

    /**
     * Standard getter for the content of an element. If the element has no content
     * this will simply return "Empty". Used when displaying the tree structure. 
     * @return strContent - The content of the current element. Defaults to "Empty".
     */
    string getStrContent() const;

    /**
     * Standard setter for the element's content. Called when a new element is created.
     * @param content - A string which is to be stored as the element's content.
     */
    void setStrContent(string content);

    /**
     * Standard setter for the element's tag name. Called when a new element is created.
     * @param tagName - A string which is to be stored as the element's tag name.
     */
    void setTagName(string tagName);

    /**
     * Function used to output the element's attribute names and values. This is used during the 
     * tree building process and after the tree has been built. During tree building, 
     * this function is called before the element's level has been set. The function
     * uses this default level to determine if the tree is currently being built. 
     * This behavior is not reliable and should be updated in future versions.
     */
    void displayAttributes();

    /**
     * Function to store pointers to children of the element. Pointers are pushed
     * on to the element's 'vecChildren' vector and the number of children is incremented. 
     * This is called in the program just after a new element is created. In future versions
     * this should be called in the element's constructor. 
     * @param newChild - A pointer to an element that is the child of the element object.
     */
    void addChild(Element* newChild);

    /**
     * Function to store a pointer to the current element's parent. This is called
     * in the program just after a new element is created. In future versions
     * this should be called in the element's constructor. 
     * @param Parent - A pointer to an element that is the parent of the element object. 
     */
    void setParent(Element* Parent);

    /**
     * Function that returns a pointer to the current element's parent. This is used
     * for creating the element tree as well as setting the element object's level. 
     * @return parent - A pointer to the element object's parent. 
     */
    Element* getParent();

    /**
     * Returns the number of children that an element object contains. This is called
     * as the element tree is being displayed. 
     * @return noOfChildren - An integer which is the number of children that the element object has. 
     */
    int getNoOfChildren();

    /**
     * A function that returns the level on which an element was found. This is called
     * when new element objects are being created to retrieve the level of their parent.
     * This is also called as the element tree is being displayed.
     * @return level - The level within the element tree in which the element object is located. 
     */
    int getLevel();

    /**
     * Sets an element objects level as the element tree is being created. 
     * In future versions this should be called within the element constructor. 
     * @param newLevel - The level in which an element is found within the element tree.
     *                   This parameter is always passed as the element object's parent's level + 1.
     */
    void setLevel(int newLevel);

    /**
     * A simple boolean function that determines if the element object contains
     * attributes or not. This is done by checking if the 'vecAttribute' vector is empty.
     * This is called as the tree is being displayed. 
     * @return - Boolean value referencing the presence or lack of attributes for the element. 
     */
    bool hasAttributes();

    /** A vector which is used to store pointers to attribute objects that belong to the
     current element object. */
    vector<Attribute*> vecAttribute;

    /** A vector which is used to store pointers to other element objects that are
     children of the current element object.*/
    vector<Element*> vecChildren;

private:
    /** Name of an element extracted from an opening XML tag. */
    string strTagName;

    /** The line number of an XML document in which the element or it's opening tag was found.*/
    int lineNumber;

    /** The content of an element extracted from the XML file.*/
    string strContent;

    /** The number of children that belong to the element. Used when displaying the tree.*/
    int noOfChildren = 0;

    /** A pointer to the element that is the parent of the current element object. */
    Element* parent;

    /** The level within the element tree structure in which the element is located. */
    int level;

    /**
     * Function that scans an entire XML line to find attribute names
     * and their values. When attributes are found, a new attribute object is created
     * and pushed onto the current element's attribute vector, "vecAttribute". Also,
     * the function's algorithm is executed twice, searching for double quotes and single
     * quotes respectively. 
     * @param currentLine - A complete XML line which in which attributes are searched for. 
     */
    void findAttributes(string currentLine);
};

#endif	/* ELEMENT_H */

