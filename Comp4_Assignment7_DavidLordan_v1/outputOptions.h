/* File:   outputOptions.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on December 6th, 2014, 1:25 PM, modified on Dec 7th at 1:33 PM to 
 * add additional documentation. 
 * 
 * This is the header file for various output functions for the DOM tree,
 * such as printing with the serializer, printing with the tree-walker, exporting
 * to an XML file and the 'help' command.
 * 
 * This program is to demonstrate use of regular expressions to parse command line
 * user input along with using the Xerces library to create a DOM structure. 
 * This is an expansion of the previous assignment which was to create a 'hard-coded'
 * DOM structure in memory and to display it to the user. This assignment goes further
 * by building the tree dynamically from the user's requests as well as giving the
 * user various options for tree manipulation. All basic commands are parsed with
 * regular expressions to extract keywords and variable reflecting the meaning of the user's input.
 */


#ifndef OUTPUTOPTIONS_H
#define	OUTPUTOPTIONS_H

/* Includes the necessary Xerces libraries and standard libraries. */
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <iostream>
#include "xercesc/dom/DOMTreeWalker.hpp"

#include <string.h>
#include <stdlib.h>


/* Uses the Xerces and std namespace.*/
XERCES_CPP_NAMESPACE_USE
        using namespace std;

/**
 * Function to export the DOM structure to an external XML file. This implementation
 * is virtually identical to the 'print' function which user the serializer.
 * @param doc - The DOMDocument that is to be exported.
 * @param fileName - The XML file name specified by the user
 */
void exportTree(DOMDocument* doc, string fileName);

/**
 * Function to print the tree in an XML format. If an element is provided, the
 * subtree from that element will be printed.
 * @param doc - The document whos structure is to be printed
 * @param subRoot - The element whose subtree is to be printed, default is the root
 */
void printTree(DOMDocument* doc, string parentName);

/**
 * Function to display all of the basic commands, thier uses and syntax
 */
void showHelp();

/**
 * Function that implements the DOMTreeWalkers which is used to traverse the
 * DOM structure. Creates the tree then scans through each node, checks its type,
 * and depending on the results outputs the content in a particular format. The 
 * user may provide an element name whose subtree will be printed. By default the
 * entire tree is displayed
 * @param doc - The DOM document containing the DOM structure. 
 * @param subRoot - The element whose subtree is to be displayed
 */
void treeWalker(DOMDocument* doc, string subRoot);

#endif	/* OUTPUTOPTIONS_H */

