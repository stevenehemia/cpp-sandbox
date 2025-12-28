#include <iostream>
#include <cstring>

#include "network.h"


/* Forward class declaration */
class Network;
enum class PaperSize;


/* ============================ Computer class =========================== */

/* Constructor for a Computer object */
Computer::Computer(const char* name)
{
    /* Assign Computer's name */
    this->name = new (std::nothrow) char[strlen(name) + 1];
    strcpy(this->name, name);
}

/* Connect a Computer object to a Network */
void Computer::connect(Network &n)
{
    n.addComputer(this);
    connectedNetwork = &n;

    /* Print activity log to console*/
    std::cout << name << " connects to " << n.getName() << std::endl;
}

/* Send a print request to a Network */
void Computer::print(const char* message, PaperSize size) const
{
    /* Print activity log to console */
    std::cout << std::endl;
    std::cout << name << " wants to print \"" << message << "\""
	      << std::endl;
    
    /* Make a print request to the connected network */
    if(!connectedNetwork->print(message, size)) {

    	/* Unable to print, print notification to console */
        std::cout << name << " cannot print message" << std::endl;
    }
}

/* Getter for Computer's name */
char* Computer::getName () const { return name; }

