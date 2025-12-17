#include <iostream>
#include <cstring>
#include <vector>

#include "network.h"


/* Forward class declaration */
class Printer;
class Computer;


/* ============================= Network class ============================ */

/* Constructor for Network object */
Network::Network (const char* name)
{
    /* Assign Network's name */
    this->name = new (std::nothrow) char[strlen(name) + 1];
    strcpy(this->name, name);
}

/* Print all conncted devices out to the console */
void Network::displayConnections() const
{
    std::cout << name << " is connected to:" << std::endl;

    /* Print the name of all connected printers */
    std::cout << "    " << printers.size() << " printers:" << std::endl;
    for(Printer* p : printers) {
        std::cout << "        " << p->getName() << std::endl;
    }

    /* Print the name of all connected printers */
    std::cout << "    " << printers.size() << " computers:" << std::endl;
    for(Computer* c : computers) {
        std::cout << "        " << c->getName() << std::endl;
    }
}

/* Add a Computer object to the list of connected computers */
void Network::addComputer(Computer *c)
{
    /* Check for duplicates */
    for(Computer *computer : computers) {
        if(c == computer) {
	    /*Duplicate found, list will not be appended */
	    return;
	}
    }
    /* No duplicates, add the Computer object to the list */
    computers.push_back(c);    
}

/* Add a Printer object to the list of connected printers */
void Network::addPrinter(Printer *p)
{
    /* Check for duplicates */
    for(Printer *printer : printers) {
        if(p == printer) {
	    /* Duplicate found, list will not be appended */
	    return;
	}
    }

    /* No duplicates, add the Printer object to the list */
    printers.push_back(p);
}

/* Send a print request to connected printers */
bool Network::print(const char* message, PaperSize size) const
{
    std::cout << name << " received request to print \""
              << message << "\"" << std::endl;

    /* Iterate through all connected printers from the first printer */
    for(Printer *p : printers) {

	/* Send the print request to the printer and retrieve the response 
	 * from the printer */
        const char* printerResponse = p->print(message, size);

	/* Print error message if printer's response is not OK, print
	 * success message if response is OK */
        if(strcmp(printerResponse, "OK")) {
            std::cout << name << " cannot print \"" << message << "\" using "
                      << p->getName() << " " << printerResponse << std::endl;
        } else {
            std::cout << name << " successfully printed \"" << message
                      << "\" using " << p->getName() << std::endl;
            return true;
        }
    }

    /* No available printer to print, print error message to console */
    std::cout << name << " cannot print message" << std::endl;
    return false;
}

/* Getter for Network's name */
char* Network::getName() const { return name; };
