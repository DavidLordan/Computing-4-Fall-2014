/* File:   outputOptions.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on December 6th, 2014, 1:25 PM, modified on Dec 7th at 1:33 PM to 
 * add additional documentation. 
 * 
 * This is the implementation file for various output functions for the DOM tree,
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


/**Includes basic libraries and the Xerces 'LocalFileFormatTarget' class for the 
 export function*/
#include <string.h>
#include <stdlib.h>

#include "outputOptions.h"
#include <xercesc/framework/LocalFileFormatTarget.hpp>

/* Includes the boost regular expressions library*/
#include <boost/regex.hpp>

XERCES_CPP_NAMESPACE_USE
        using namespace std;

/**
 * Function to export the DOM structure to an external XML file. This implementation
 * is virtually identical to the 'print' function which user the serializer.
 * @param doc - The DOMDocument that is to be exported.
 * @param fileName - The XML file name specified by the user
 */
void exportTree(DOMDocument* doc, string fileName) {
    //Ensures that a file name was entered by the user
    if (fileName.empty()) {
        cout << "No file name was given. Enter a file name for the tree." << endl;
        getline(cin, fileName);
        cin.ignore(-1);
    }
    // Gets the doc's implementation so that it may be used to create the serializer
    DOMImplementation * impl = doc->getImplementation();
    // Creates a DOM serializer which is used to call the 'write' function. 
    DOMLSSerializer *theSerializer = ((DOMImplementationLS*) impl)->createLSSerializer();

    // Formats the serializer's output. 
    DOMLSOutput *theOutputDesc = ((DOMImplementationLS*) impl)->createLSOutput();

    //Creates a file path in the 'dist' folder from the file name
    fileName = "dist/" + fileName + ".xml";
    const char* xfileName = fileName.c_str();
    XMLCh * filePath = XMLString::transcode(xfileName);

    // Uses the file path to create a serializer output destination
    XMLFormatTarget *myFormTarget = new LocalFileFormatTarget(XMLString::transcode(filePath));
    theOutputDesc->setByteStream(myFormTarget);

    //Assigns the serializer's print parameters. Allows for the the DOM structure
    // to be printed in an easy to read XML format. 
    DOMConfiguration* serializerConfig = theSerializer->getDomConfig();
    serializerConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    serializerConfig->setParameter(XMLUni::fgDOMXMLDeclaration, true);

    // Function to print the DOM structure.
    theSerializer->write(doc, theOutputDesc);

    // Frees the memory allocated by the serializer and output DOM. 
    theOutputDesc->release();
    theSerializer->release();
    myFormTarget->flush();
    delete myFormTarget;

    // Informs the user that the tree has been saved
    cout << "Exported the DOM tree to '" << fileName << "'. " << endl;

}

/**
 * Function to display all of the basic commands, thier uses and syntax
 */
void showHelp() {

    cout << "'help': Gives a list of all commands and their uses." << endl << endl;

    cout << "'add' {element|attribute} parent_node {child_node|attribute_name} "
            "{content|value} : Adds an element or attribute to the specified "
            "parent node. Use the parent node 'null' to create a root." << endl << endl;

    cout << "'remove' {element|attribute} parent_node {child_node|attribute_name}: "
            "Removes the specified child node or attribute belonging to the specified"
            " parent node. Use the parent node 'null' to remove the entire tree." << endl << endl;

    cout << "'rename' {element|attribute} target_node {new_name|target_attribute}"
            " {new_content|new_attribute}: Renames a specified node (or attribute "
            "belonging to that node). If renaming an attribute the user will be "
            "prompted for the new attribute value." << endl << endl;

    cout << "'print' {list} {subtree-root}: Displays the current tree in an XML format or as a simple"
            " list. Enter an optional existing node name to display a subtree from that node."
            " Default format is XML." << endl << endl;

    cout << "'export' {file_name}: Export the tree as an external XML file. " << endl << endl;

    cout << "'quit': Ends the program." << endl << endl;
}

/**
 * Function to print the tree in an XML format. If an element is provided, the
 * subtree from that element will be printed.
 * @param doc - The document whos structure is to be printed
 * @param subRoot - The element whose subtree is to be printed, default is the root
 */
void printTree(DOMDocument* doc, string subRoot) {

    // Gets the doc's implementation so that it may be used to create the serializer
    DOMImplementation * impl = doc->getImplementation();

    // Creates a DOM serializer which is used to call the 'write' function. 
    DOMLSSerializer *theSerializer = ((DOMImplementationLS*) impl)->createLSSerializer();

    // Formats the serializer's output. 
    DOMLSOutput *theOutputDesc = ((DOMImplementationLS*) impl)->createLSOutput();
    XMLFormatTarget *myFormTarget = new StdOutFormatTarget();
    theOutputDesc->setByteStream(myFormTarget);

    //Assigns the serializer's print parameters. Allows for the the DOM structure
    // to be printed in an easy to read XML format. 
    DOMConfiguration* serializerConfig = theSerializer->getDomConfig();
    serializerConfig->setParameter(XMLUni::fgDOMWRTFormatPrettyPrint, true);
    serializerConfig->setParameter(XMLUni::fgDOMXMLDeclaration, false);

    //Checks if the tree is to be printed from the root
    if (subRoot.empty()) {
        // Function to print the DOM structure.
        theSerializer->write(doc, theOutputDesc);

    } else {

        // Confirms that the subRoot is an existing element
        const char* xInput = subRoot.c_str();
        XMLCh* xSubRoot = XMLString::transcode(xInput);
        DOMNodeList * nodeList = doc->getElementsByTagName(xSubRoot);

        if (nodeList->getLength() != 0) {

            // If the subRoot exists its subtree is printed
            DOMNode * subTreeRootNode = nodeList->item(0);
            theSerializer->write(subTreeRootNode, theOutputDesc);
            cout << endl;

        } else {
            //If the specified element does not exist, the user is informed
            cout << "No matching node named '" << subRoot << "' found." << endl;
        }
    }

    // Frees the memory allocated by the serializer and output DOM. 
    theOutputDesc->release();
    theSerializer->release();
    delete myFormTarget;
}

/**
 * Function to display a DOM element's attributes. 
 * @param attributes - a DOM map which contains a list of attribute names and values. 
 * @param level - The level on which an attribute's parent is located. Used for output
 *                formatting. 
 */
void displayAttributes(DOMNamedNodeMap* attributes, int level) {

    // Tag to hold attribute names and values which then sent to 'transcode'.
    const XMLCh * tag = NULL;

    // For loop which displays each attribute found on a particular element. 
    for (XMLSize_t j = 0; j < attributes->getLength(); j++) {
        DOMNode * nextAtt = attributes->item(j);

        DOMAttr* att = dynamic_cast<DOMAttr*> (nextAtt);
        tag = att->getName();

        // Output formatting. 
        for (int i = 0; i <= (level - 1); i++) {
            cout << "   ";
        }

        cout << "Attribute " << (j + 1) << ": ";
        cout << XMLString::transcode(tag);
        tag = att->getValue();
        cout << " = " << XMLString::transcode(tag) << endl;

    }
}

/**
 * Function that implements the DOMTreeWalkers which is used to traverse the
 * DOM structure. Creates the tree then scans through each node, checks its type,
 * and depending on the results outputs the content in a particular format. The 
 * user may provide an element name whose subtree will be printed. By default the
 * entire tree is displayed
 * @param doc - The DOM document containing the DOM structure. 
 * @param subRoot - The element whose subtree is to be displayed
 */
void treeWalker(DOMDocument* doc, string subRoot) {
    // Boolean used to start the tree-walker
    bool valid = true;
    // Variable used to keep track of the current element's level in the tree. 
    int level = 0;

    //DOMElement whose subtree is to be printed. May be either an ordinary element
    // or the actual root.
    DOMElement* rootElem = NULL;

    // Checks if there is a subRoot provided, if not, the actual root is used
    if (subRoot.empty()) {
        rootElem = doc->getDocumentElement();
    } else {

        // Confirms that the element provided exists in the tree
        const char* xInput = subRoot.c_str();
        XMLCh* xSubRoot = XMLString::transcode(xInput);
        DOMNodeList * nodeList = doc->getElementsByTagName(xSubRoot);

        if (nodeList->getLength() != 0) {
            // Assigns the rootElem variable to the provided element
            DOMNode * subTreeRootNode = nodeList->item(0);
            rootElem = dynamic_cast<DOMElement*> (subTreeRootNode);

        } else {
            // If no matching element is found, the user is informed and the 
            // tree-walker will not start
            cout << "No matching node named '" << subRoot << "' found." << endl;
            valid = false;
        }
    }

    //If the sub-tree has a valid root, the tree-walker will start
    if (valid) {
        cout << endl;

        //  Uses root element and uses it to create the tree walker object. The
        //  walker's filter is set to show all nodes. 
        DOMTreeWalker *walker = doc->createTreeWalker(rootElem, DOMNodeFilter::SHOW_ALL, NULL, true);

        // Placeholder variable used to store tag names and content. Passed to transcode. 
        const XMLCh * tag = NULL;
        //Grabs the walker root to begin traversing the tree. 
        DOMNode * node = walker->getRoot();
        // Placholder node for an element that is being analyzed.
        DOMElement* elemNode = NULL;

        // Loop which traverses the entire DOM tree and prints its content. 
        do {

            node = walker->getCurrentNode();

            switch (node->getNodeType()) {

                    // If the current node is an element node, the node is cast as such
                    // and its name printed. If the element contains any attributes they
                    // are also displayed.
                case DOMNode::ELEMENT_NODE:

                    for (int i = 0; i < level; i++) {
                        cout << "   ";
                    }

                    // Checks if the current node is the root. If not, the node 
                    // is cast as a DOMElement
                    if (node != rootElem) {
                        elemNode = dynamic_cast<DOMElement*> (node);
                    } else {
                        elemNode = rootElem;
                    }

                    // Prints the current element name
                    tag = elemNode->getTagName();
                    cout << XMLString::transcode(tag) << ": ";
                    if (elemNode->getFirstChild()->getNodeType() != 3) {
                        cout << endl;
                    }

                    break;

                    // If the current node is found to be a text node containing element
                    // content, its text is displayed. 
                case DOMNode::TEXT_NODE:

                    tag = node->getTextContent();
                    cout << XMLString::transcode(tag) << endl;

                    // After the content is displayed, checks if the content's parent
                    // has attributes. If so, they are displayed.
                    if (node->getParentNode()->hasAttributes()) {
                        DOMNamedNodeMap* attributes = node->getParentNode()->getAttributes();
                        displayAttributes(attributes, level);
                    }

                    break;

                default:
                    cout << "ERROR!" << endl;
                    break;
            }

            // Goes the first child of the current node and increments the level. 
            if (walker->getCurrentNode()->hasChildNodes()) {
                walker->firstChild();
                level++;

                //Algorithm that can be used to traverse the tree, going from sibling to sibling
                // before moving back the parent.
            } else while (walker->nextSibling() == NULL && walker->getCurrentNode() != rootElem) {

                    walker->parentNode();
                    level--;
                }

            // When the above algorithm returns to the root, the loop ends. 
        } while (walker->getCurrentNode() != rootElem);
        cout << endl;
    }
}
