/* File:   outputOptions.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on December 7th, 2014, 5:23 PM, modified on Dec 7th at 6:22 PM to 
 * add additional documentation. 
 * 
 * This is the implementation file for various program functions that 
 * manipulate the DOM structure. These functions allow the user to add a root,
 * add an ordinary node, rename an existing node, or remove a node.
 * 
 * This program is to demonstrate use of regular expressions to parse command line
 * user input along with using the Xerces library to create a DOM structure. 
 * This is an expansion of the previous assignment which was to create a 'hard-coded'
 * DOM structure in memory and to display it to the user. This assignment goes further
 * by building the tree dynamically from the user's requests as well as giving the
 * user various options for tree manipulation. All basic commands are parsed with
 * regular expressions to extract keywords and variable reflecting the meaning of the user's input.
 */

/**Includes basic libraries and the Xerces 'LocalFileFormatTarget' class for the 
 export function*/
#include <string.h>
#include <stdlib.h>
#include "treeManipulation.h"

/* Includes the boost regular expressions library*/
#include <boost/regex.hpp>

XERCES_CPP_NAMESPACE_USE
        using namespace std;

/**
 * Function to simplify use of the XMLString::transcode function.
 * @param input - A string which will be converted to an XMLCh*
 * @return - An XMLCh* which can be used to properly create DOMNodes. 
 */
XMLCh* X(string input) {

    const char* xInput = input.c_str();
    return XMLString::transcode(xInput);
}

/**
 * A function to create and add the DOM structure's root element. Checks if the root
 * already exists and will not allow the creation of additional roots.  
 * @param doc - The DOMDocument to which the root will be added.
 * @param rootName - The user's name for the root element. 
 * @param rootContent - Option content that the user may add to the root. 
 */
void addRoot(DOMDocument* &doc, string rootName, string rootContent) {

    //Checks if there is already a root.
    if (!doc->getFirstChild()) {

        // Replaces the current doc with a new doc with a specified root. 
        doc = doc->getImplementation()->createDocument(0, X(rootName), 0);

        //Adds optional content.
        DOMText* txtNode = doc->createTextNode(X(rootContent));
        doc->getFirstChild()->appendChild(txtNode);

        //Informs the user that a  root has been created. 
        cout << "Created root '" << rootName << "'";

        if (rootContent.empty()) {
            cout << "." << endl;
        } else {
            cout << " with content '" << rootContent << "'." << endl;
        }
    } else {
        // Informs the user that there is already a root and more roots cannot be added. 
        cout << "Root already exists! Do not use 'null' as parent of new node." << endl;
    }
}

/**
 * A function that allows the user to rename an existing element or attribute. 
 * @param doc - The DOMDocument which is to be manipulated. 
 * @param selector - Allows the user to specify whether an element or attribute is being renamed. 
 * @param oldNameOrParent - The element to be renamed, or the parent of the attribute to be renamed. 
 * @param newNameOrAtt - The user provided new name of an element, or the name of the attribute to rename.
 * @param newContentOrNewAtt - Optional new content to be added, or the new name of the specified attribute.
 */
void renameNode(DOMDocument* &doc, string selector, string oldNameOrParent, string newNameOrAtt, string newContentOrNewAtt) {

    // Creates a list of nodes under the specified name.
    DOMNodeList * nodeList = doc->getElementsByTagName(X(oldNameOrParent));

    // Confirms that the specified node exists. 
    if (nodeList->getLength() != 0) {
        DOMNode * oldNode = nodeList->item(0);

        if (selector == "element") {

            // Renames the node to the new name specified by the user. 
            doc->renameNode(oldNode, 0, X(newNameOrAtt));

            // Checks if the user wants to add new content, if not, the old content is preserved.
            if (newContentOrNewAtt.empty()) {
                const XMLCh* xNewContentOrVal = oldNode->getTextContent();
                oldNode->setTextContent(xNewContentOrVal);
            } else {
                // Updates the element content. 
                oldNode->setTextContent(X(newContentOrNewAtt));
            }


            // Informs the user of the updates. 
            cout << "Renamed the element '" << oldNameOrParent << "' to '" << newNameOrAtt << "'";
            if (newContentOrNewAtt.empty()) {
                cout << "." << endl;
            } else {
                cout << " with the new content '" << newContentOrNewAtt << "'." << endl;
            }


        } else if (selector == "attribute") {
            // Confirms that the parent element has attributes. 
            if (!oldNode->hasAttributes()) {
                cout << "The element '" << oldNameOrParent << "' does not have any attributes." << endl;
            } else {

                // Confirms that parent element has the specified attribute. 
                DOMNamedNodeMap * attributes = oldNode->getAttributes();
                if (DOMNode * attNode = attributes->getNamedItem(X(newNameOrAtt))) {

                    // Renames the old attribute and prompts the user for the new attribute value. 
                    doc->renameNode(attNode, 0, X(newContentOrNewAtt));
                    cout << "Please enter a value for the new attribute: " << endl;
                    string input;
                    getline(cin, input);
                    cin.ignore(-1);
                    attNode->setTextContent(X(input));

                    // Informs the user of the updates to the tree.
                    cout << "Attribute '" << newNameOrAtt << "' belonging to '" << oldNameOrParent
                            << "' renamed to '" << newContentOrNewAtt << "' with the new value '"
                            << input << "'." << endl;

                } else {
                    // Informs the user that the specified parent does not have a matching attribute.
                    cout << "The element '" << oldNameOrParent << "' does not have an attribute named '" << newNameOrAtt << "'. " << endl;
                }
            }
        }
    } else {
        // Informs the user that the tree has no matching element. 
        cout << "No matching element named '" << oldNameOrParent << "' found." << endl;
    }
}

/**
 * A function to create and add a new node to the DOM structure. 
 * @param doc - The DOM document to which the new node will be added. 
 * @param addType - A selector for specifying whether an element or attribute is to be added. 
 * @param parentName - The parent to which the new node will be attached. 
 * @param childOrAtt - The name of the new node. 
 * @param contentOrVal - The content or value of the new node. 
 */
void addNode(DOMDocument* &doc, string addType, string parentName, string childOrAtt, string contentOrVal) {

    //Confirms that the specified parent exists. 
    DOMNodeList * nodeList = doc->getElementsByTagName(X(parentName));
    if (nodeList->getLength() != 0) {
        DOMNode * parentNode = nodeList->item(0);

        // Casts the matching node as a DOMElement so that it may be more easily manipulated. 
        DOMElement * parentElem = dynamic_cast<DOMElement*> (parentNode);


        if (addType == "element") {
            // Adds the new element to the specified parent. 
            DOMElement* childElem = doc->createElement(X(childOrAtt));
            parentElem->appendChild(childElem);

            DOMText* txtNode = doc->createTextNode(X(contentOrVal));
            childElem->appendChild(txtNode);


            // Informs the user of the updates to the tree.
            cout << "Added the element '" << childOrAtt << "' to parent '" << parentName << "'";
            if (contentOrVal.empty()) {
                cout << "." << endl;
            } else {
                cout << " with the new content '" << contentOrVal << "'." << endl;
            }


        } else if (addType == "attribute") {
            // Adds the new attribute to the specified parent. 
            parentElem->setAttribute(X(childOrAtt), X(contentOrVal));

            // Informs the user of the updates to the tree.
            cout << "Added the attribute '" << childOrAtt << "' to parent '" << parentName << "'";
            if (contentOrVal.empty()) {
                cout << "." << endl;
            } else {
                cout << " with the new value '" << contentOrVal << "'." << endl;
            }


        } else {
            //addType not element or attribute.
            cout << "ERROR" << endl;
        }

    } else {
        // Informs the user that no matching node was found. 
        cout << "No matching node named '" << parentName << "' found." << endl;
    }

    if (!doc->getFirstChild()) {
        // If the tree is empty, the user is told informed as such and instructed as to how to add a root. 
        cout << "No root has been created. Use the 'add element' command with the parent 'null' to create a root." << endl;
    }
}

/**
 * Function to remove a user specified element or attribute from the DOM structure. 
 * @param doc - The DOMDocument which is being manipulated.
 * @param removeType - A selector for whether or not a element or attribute is being removed. 
 * @param parentName - The parent of the node to be removed.
 * @param childOrAtt - The element or attribute name which is to be removed. 
 */
void removeNode(DOMDocument * &doc, string removeType, string parentName, string childOrAtt) {
    //Checks if the user is attempting to remove the root. 
    if (parentName == "null") {

        if (removeType == "element") {

            //Gets the root node
            DOMNode * node = doc->getFirstChild();
            DOMElement * rootNode = dynamic_cast<DOMElement*> (node);

            //Checks that the root node name matches the name given by the user.
            if (XMLString::transcode(rootNode->getTagName()) == childOrAtt) {

                //Removes the root and informs the user.
                doc->removeChild(rootNode);
                cout << "Removed root node '" << childOrAtt << "'. Tree is now empty." << endl;
            } else {
                // If the specified name does not match the root name, the user is informed
                // and no action is taken. 
                cout << "'" << childOrAtt << "' is not the name of the root." << endl;
            }

        } else {
            // If the selector is not 'element', no action is taken. 
            cout << "When using the remove command with the parent 'null, only "
                    "the 'element' selector may be used." << endl;
        }

    } else {
        //Checks that the specified parent exists
        DOMNodeList * nodeList = doc->getElementsByTagName(X(parentName));

        if (nodeList->getLength() != 0) {
            //Gets the parent node
            DOMNode * parentNode = nodeList->item(0);
            DOMElement * parentElem = dynamic_cast<DOMElement*> (parentNode);

            if (removeType == "element") {
                // Checks that the element to remove exists as a child of the parent. 
                nodeList = parentElem->getElementsByTagName(X(childOrAtt));
                if (nodeList->getLength() != 0) {

                    // Removes the specified node and informs the user.
                    DOMNode * childNode = nodeList->item(0);
                    parentElem->removeChild(childNode);

                    cout << "Child element '" << childOrAtt << "' belonging to '"
                            << parentName << "' has been removed." << endl;

                } else {
                    // If no matching child is found, the user is informed. 
                    cout << "No matching child named '" << childOrAtt << "' found for the parent '"
                            << parentName << "'. Please try again." << endl;
                }

            } else if (removeType == "attribute") {

                // Checks that the specified attribute exists and belongs to the specified parent
                DOMNamedNodeMap * nodeMap = parentElem->getAttributes();
                if (nodeMap->getNamedItem(X(childOrAtt))) {

                    // Removes the attribute and informs the user. 
                    parentElem->removeAttribute(X(childOrAtt));

                    cout << "The attribute '" << childOrAtt << "' belonging to parent '"
                            << parentName << "' has been removed." << endl;

                } else {
                    // If no matching attribute is found, the user is informed. 
                    cout << "The element '" << parentName << "' does not have an attribute named '" << childOrAtt << "' ." << endl;
                }

            } else {
                //addType not element or attribute
                cout << "ERROR" << endl;
            }

        } else {
            // If no matching parent name is found, the user is informed. 
            cout << "No matching node named '" << parentName << "' found. Please try again." << endl;
        }
    }
}