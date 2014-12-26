/* File:   main.cpp
 * Author: David Lordan, UMass Lowell Computer Science
 * david_lordan@student.uml.edu
 *
 * Created on November 15th, 2014, 505 PM, modified on Nov 26th at 6:33 PM to 
 * add additional documentation. 
 * 
 * This program is used to demonstrate the functionality and understanding
 * of the Xerces library. An XML structure is created using DOMNodes, modeling
 * my Computer Science degree program of study. The direct children of the root
 * each correspond to a regular Fall or Spring semester, plus an additional three
 * Summer semesters. As this works out to only 9 semesters the the assignment 
 * requires 8, I have added a speculative graduate level semester based on classes I 
 * may wish to take, and the professors currently listed as teaching them. 
 * 
 * 
 * Each semester then contains a list of classes. Each class contains the instructor,
 * the campus and the grade. Each class may or may not contain attributes as well. 
 * 
 * After the XML structure is created and stored in memory it is then printed
 * using the Xerces DOMSerializer and Xerces tree walker. The tree walker is 
 * used to traverse the DOMNodes and can be used to create a custom output style. 
 */

/* Includes the necessary Xerces libraries and Xerces namespace. */
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <iostream>
#include "xercesc/dom/DOMTreeWalker.hpp"

#include <string.h>
#include <stdlib.h>

XERCES_CPP_NAMESPACE_USE

        using namespace std;

/**
 * Function to simplify use of the XMLString::transcode function.
 * @param - A char* (string) to use as an argument for transcode.
 * @return - An XMLCh* which can be used to properly create DOMNodes. 
 */
XMLCh* X(const char* const input) {
    return XMLString::transcode(input);
}

/** An array of courses I have taken, am taking and intend to take. Used as
    element content. */
const char *courses[] = {"Physics_1", "Calculus_1", "Java_1", "Microeconomics", "Calculus_2",
    "Gen.Chem_1", "Composition1", "CAD_1", "Physics_2", "Physics_3", "Calculus_3", "Statics",
    "Material_Science", "Logic_Design", "Obj.Oriented.Prog", "Film_Studies",
    "Differential_Equations", "Gen.Chem_2", "Comp_2", "DiscreteStruct_1", "IntroToEthics",
    "Comp_4", "Assembly", "GUI_Prog_1", "DiscreteStruct_2", "Prob_and_Stats", "GUI_Prog_2",
    "Software_Engineering_1", "Org._of_Prog_Lang.", "Composition_for_CS", "Comp_Architecture",
    "World_Civ_1", "Intro_to_Itnl._Relations", "Intro_to_Sociology", "Found._of_CS", "Algorithms",
    "Software_Engineerings_2", "Operating_Systems", "Internet_WebSystems_1", "Data_Visualization",
    "Data_Mining"};

/** An array of professors for the courses listed above. Used as element content.*/
const char *professors[] = {"Prof. Jeong", "Dr. Stone", "Prof. Guan", "Prof. O'Brien",
    "Prof. Gibbs", "Prof. MacTaylor", "Prof. Burt", "Prof. Walsh", "Prof. Jeong",
    "Prof. Steigerwald", "Dr. Dakhama", "Prof. Jeong", "Prof. Jeong",
    "Prof. Steigewald", "Prof. Hassan", "Prof. Ronan", "Dr. Dakhama", "Prof. Day",
    "Prof. Lepsky", "Prof. Doerr", "Prof. White", "Prof. Heines", "Prof. Kim", "Prof. Heines",
    "Prof. Montenegro", "Prof. Graham-Eagle", "Prof. Heines", "Prof. Cao", "Prof. Martin",
    "Prof. Turgeon", "Prof. Moloney", "Prof. Kalivas", "Prof. Carter", "Prof. Sloan",
    "Prof. Daniels", "Prof. Daniels", "Prof. Cao", "Prof. Moloney", "Prof. Levkowitz",
    "Prof. Grinstein", "Prof. Chen"};

/** An array of the semesters in my program of study. Each of these will be used as
    a tag name for the direct children of the root. */
const char *semesters[] = {"Spring_13", "Summer_13", "Fall_13", "Spring_14", "Summer_14", "Fall_14",
    "Spring_15", "Summer_15", "Fall_15", "Spring_16"};

/** Array of the number of courses for each semester. Used as a for loop limit
    when populating the DOM structure. */
int courseLoad[] = {4, 4, 5, 5, 3, 5, 5, 3, 4, 3};

/** An array of campuses in which the above courses took place. Used as tag content. */
const char *campus[] = {"Lynn/McGee", "Danvers/Berry", "Online", "Salem State North Campus",
    "UML North Campus"};

/**
 * Function to create the DOM structure. Scans through each member of the 'semesters'
 * array, creating a child node of the root for each. Each of the semester nodes are
 * then given children corresponding to each course in that semester. Each course
 * is then given a Instructor, Campus, and Grade child. Also, courses may or may 
 * not have attributes associated with them. Much of this function is based on
 * the Xerces DOMPrint sample program. 
 * @param impl - DOMImplementation used to create the DOM structure.
 * @return  - Returns a DOMDocument pointer containing the DOM structure.
 */
DOMDocument* createDOM(DOMImplementation *impl) {

    //Creates a DOMDocument from the DOMImplementaiton. Initializes the DOM
    // structure with a root node. 
    DOMDocument* doc = impl->createDocument(
            0, // root element namespace URI.
            X("CS_Degree"), // root element name
            0); // document type object (DTD).

    //Assigns the root element. 
    DOMElement* rootElem = doc->getDocumentElement();

    // Group of 3 DOMElment placeholder variables which each correspond to a 
    // different level of the DOM structure. 
    DOMElement* lvl1Elem;
    DOMElement* lvl2Elem;
    DOMElement* lvl3Elem;

    // Place holder variable for DOM content. 
    DOMText* txtNode;

    //Variables used to keep track of the next course and campus to use. 
    int classNum = 0;
    int campusNum = 0;

    // For loop which appends children to the root. Each iteration through 
    // the loop corresponds to a different semester. 
    for (int i = 0; i < 10; i++) {


        lvl1Elem = doc->createElement(X(semesters[i]));
        rootElem->appendChild(lvl1Elem);

        // For loop which appends children to a semester node. Each iteration
        // through the loop corresponds to a different course, along with that 
        // course's information.
        for (int j = classNum; j < (courseLoad[i] + classNum); j++) {

            lvl2Elem = doc->createElement(X(courses[j]));
            lvl1Elem->appendChild(lvl2Elem);

            lvl3Elem = doc->createElement(X("Instructor"));
            lvl2Elem->appendChild(lvl3Elem);

            txtNode = doc->createTextNode(X(professors[j]));
            lvl3Elem->appendChild(txtNode);

            lvl3Elem = doc->createElement(X("Campus"));
            lvl2Elem->appendChild(lvl3Elem);

            // When creating the last semester, three attributes are added to each
            // course. 
            if (i == 9) {
                lvl2Elem->setAttribute(X("class_Level"), X("Graduate"));
                lvl2Elem->setAttribute(X("info_reliability"), X("subject_to_change"));
                lvl2Elem->setAttribute(X("has_BachelorDeg"), X("true"));
            }

            // Add attributes to certain math courses. 
            if (strncmp(courses[j], "Differential_Equations", 15) == 0
                    || strncmp(courses[j], "Calculus", 8) == 0
                    || strncmp(courses[j], "Discrete", 8) == 0) {
                lvl2Elem->setAttribute(X("appliesToMathMinor"), X("true"));

            }

            // Switch statement that assigns a value to 'campusNum' depending on
            // the current semester, course or instructor. 
            switch (i) {
                case 0:
                    if (j == 0 || j == 1) {
                        campusNum = 0;
                    } else {
                        campusNum = 2;
                    }
                    break;
                case 1:
                    if (j == 4) {
                        campusNum = 1;
                    } else if (j == 5) {
                        campusNum = 3;
                    } else {
                        campusNum = 2;
                    }
                    break;
                case 2:
                case 3:
                    campusNum = 0;
                    break;
                case 4:
                    campusNum = 2;
                    if (strncmp(professors[j], "Prof. Lepsky", 13) == 0) {
                        campusNum = 4;
                    }
                    break;
                case 7:
                    campusNum = 2;
                    break;
                default:
                    campusNum = 4;
                    break;
            }

            // Uses 'campusNum' to add the appropriate campus name to the most
            // recently created element. 
            txtNode = doc->createTextNode(X(campus[campusNum]));
            lvl3Elem->appendChild(txtNode);

            lvl3Elem = doc->createElement(X("Grade"));
            lvl2Elem->appendChild(lvl3Elem);

            // Assigns grade values based on the semester. 
            if (i < 5) {
                txtNode = doc->createTextNode(X("A"));
                lvl3Elem->appendChild(txtNode);
            } else {
                txtNode = doc->createTextNode(X("N/A"));
                lvl3Elem->appendChild(txtNode);
            }
        }

        // Increments clasNum by the current semester's course load. 
        classNum += courseLoad[i];

    }

    // Returns the completed document. 
    return doc;
}

/**
 * Function that uses the Xerces serializer to display the tree in an XML format.
 * Much of this function is based on the Xerces DOMPrint sample program. 
 * @param doc - The DOMDocument that contains the DOM structure. 
 * @param impl - The DOMImplementation used to create the DOM structure. 
 */
void printTree(DOMDocument* doc, DOMImplementation *impl) {

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

    // Function to print the DOM structure.
    theSerializer->write(doc, theOutputDesc);

    // Frees the memory allocated by the serializer and output DOM. 
    theOutputDesc->release();
    theSerializer->release();

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
        cout << endl;
        DOMNode * nextAtt = attributes->item(j);

        DOMAttr* att = dynamic_cast<DOMAttr*> (nextAtt);
        tag = att->getName();

        // Output formatting. 
        for (int i = 0; i <= level; i++) {
            cout << "   ";
        }

        cout << "Attribute " << (j + 1) << ": ";
        cout << XMLString::transcode(tag);
        tag = att->getValue();
        cout << " = " << XMLString::transcode(tag);

    }
}

/**
 * Function that implements the DOMTreeWalkers which is used to traverse the
 * DOM structure. Creates the tree then scans through each node, checks its type,
 * and depending on the results outputs the content in a particular format. 
 * @param doc - The DOM document containing the DOM structure. 
 */
void treeWalker(DOMDocument* doc) {

    // Variable used to keep track of the current element's level in the tree. 
    int level = 0;
    cout << endl;

    //  Grabs the root element and uses it to create the tree walker object. The
    //  walker's filter is set to show all nodes. 
    DOMElement* rootElem = doc->getDocumentElement();
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

                elemNode = dynamic_cast<DOMElement*> (node);
                tag = elemNode->getTagName();
                cout << XMLString::transcode(tag) << ":";

                if (elemNode->hasAttributes()) {
                    DOMNamedNodeMap* attributes = elemNode->getAttributes();
                    displayAttributes(attributes, level);
                }

                if (elemNode->getFirstChild()->getNodeType() != 3) {
                    cout << endl;
                }

                break;


                // If the current node is found to be a text node containing element
                // content, its text is displayed. 
            case DOMNode::TEXT_NODE:

                tag = node->getTextContent();
                cout << XMLString::transcode(tag) << endl;
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
                if (walker->getCurrentNode()->getNodeType() == DOMNode::ELEMENT_NODE) {
                    cout << endl;
                }
                walker->parentNode();
                level--;
            }

        // When the above algorithm returns to the root, the loop ends. 
    } while (walker->getCurrentNode() != rootElem);
}

/**
 * Function to add a banner describing the output type. 
 * @param type - The output type which is be described by the banner. 
 */
void banner(string type) {

    for (int i = 0; i < 100; i++) {
        cout << "*";
    }
    cout << endl << "Output generated by the Xerces " << type << ":" << endl;
    for (int i = 0; i < 100; i++) {
        cout << "*";
    }

}

/**
 * Standard C++ main function. 
 * @param argc - Number of command line arguments. 
 * @param argv - Array of pointers to command line arguments. 
 * @return - Returns error code if the program does not end properly. 
 */
int main(int argC, char* argV[]) {

    // Starts the XML platform utilities, allow for the use of several Xerces features. 
    XMLPlatformUtils::Initialize();

    // Creates a DOM implementation object. Taken from the Xeces DOMPrint sample program.
    XMLCh tempStr[3] = {chLatin_L, chLatin_S, chNull};
    DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(tempStr);

    // Creates the DOM document to store the DOM structure. 
    DOMDocument* doc = createDOM(impl);

    // Creates a 'serializer' banner and calls the 'printTree' function, displaying
    // the tree with a Xerces serializer. 
    banner("serializer");
    printTree(doc, impl);


    // Creates a 'serializer' banner and calls the 'printTree' function, displaying
    // the tree with a Xerces tree walker object.
    banner("treeWalker");
    treeWalker(doc);

    // Closes the XML platform utilities.
    XMLPlatformUtils::Terminate();

    return 0;
}




