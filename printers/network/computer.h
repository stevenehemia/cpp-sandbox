#ifndef COMPUTER_H
#define COMPUTER_H


#include <iostream>
#include "network.h"
#include "printer.h"


/* Forward class declaration */
class Network;
enum class PaperSize;


/* ============================ Computer class ============================ */

class Computer 
{	
    private:
	/* Attributes */
	char *name;
	Network *connectedNetwork;

    public:
        /**
	 * Contructor for a Computer object
	 * @param p1 Name of the computer
	 * @return A Computer object
	 */
	Computer(const char *name);	
	
	/**
	 * Connect A Computer object to a Network object
	 * @param p1 The Network to be connected to
	 * @return None
	 * @note The computer's connectedNetwork variable will be
	 * assigned with the connected network's name
	 */
	void connect(Network &n);

	/**
	 * Send a print request to the connected Network
	 * @param p1 Text to be printed
	 * @param p2 Size of the paper the text will be printed on
	 * @return None
	 * @note The Computer will print an error message out to the console
	 * if the request was unsuccessful
	 */
	void print(const char *message, PaperSize size) const;
		
	/**
	 * Getter for a Computer's name
	 * @return the Computer's name
	 */
	char* getName() const;
};


#endif
