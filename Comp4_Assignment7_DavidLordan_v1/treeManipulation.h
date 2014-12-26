/* File:   outputOptions.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on December 7th, 2014, 5:23 PM, modified on Dec 7th at 6:22 PM to 
 * add additional documentation. 
 * 
 * This is the header file for various program functions that 
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

#ifndef TREEMANIPULATION_H
#define	TREEMANIPULATION_H

/* Includes the necessary Xerces libraries and standard libraries. */
#include <xercesc/dom/DOM.hpp>
#include <iostream>
#include <string.h>
#include <stdlib.h>

/* Uses the Xerces and std namespace.*/
XERCES_CPP_NAMESPACE_USE
        using namespace std;

/**
 * A function to create and add the DOM structure's root element. Checks if the root
 * already exists and will not allow the creation of additional roots.  
 * @param doc - The DOMDocument to which the root will be added.
 * @param rootName - The user's name for the root element. 
 * @param rootContent - Option content that the user may add to the root. 
 */
void addRoot(DOMDocument* &doc, string rootName, string rootContent);

/**
 * A function that allows the user to rename an existing element or attribute. 
 * @param doc - The DOMDocument which is to be manipulated. 
 * @param selector - Allows the user to specify whether an element or attribute is being renamed. 
 * @param oldNameOrParent - The element to be renamed, or the parent of the attribute to be renamed. 
 * @param newNameOrAtt - The user provided new name of an element, or the name of the attribute to rename.
 * @param newContentOrNewAtt - Optional new content to be added, or the new name of the specified attribute.
 */
void renameNode(DOMDocument* &doc, string selector, string oldNameOrParent, string newNameOrAtt, string newContentOrNewAtt);

/**
 * A function to create and add a new node to the DOM structure. 
 * @param doc - The DOM document to which the new node will be added. 
 * @param addType - A selector for specifying whether an element or attribute is to be added. 
 * @param parentName - The parent to which the new node will be attached. 
 * @param childOrAtt - The name of the new node. 
 * @param contentOrVal - The content or value of the new node. 
 */
void addNode(DOMDocument* &doc, string addType, string parentName, string childOrAtt, string contentOrVal);

/**
 * Function to remove a user specified element or attribute from the DOM structure. 
 * @param doc - The DOMDocument which is being manipulated.
 * @param removeType - A selector for whether or not a element or attribute is being removed. 
 * @param parentName - The parent of the node to be removed.
 * @param childOrAtt - The element or attribute name which is to be removed. 
 */
void removeNode(DOMDocument * &doc, string removeType, string parentName, string childOrAtt);

#endif	/* TREEMANIPULATION_H */

