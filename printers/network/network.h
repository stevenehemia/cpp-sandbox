#ifndef NETWORK_H
#define NETWORK_H


#include <vector>

#include "computer.h"
#include "printer.h"


/* Forward class declaration */
class Computer;
class Printer;


/* ============================== Network class =========================== */

class Network
{
    private:
	/* Attributes */
	char *name;
	std::vector<Computer*> computers;
	std::vector<Printer*> printers;

    public:
	/**
	 * Constructor for a Network object
	 * @param p1 Name of the Network
	 * @return A Network object
	 */
	Network(const char *name);
	
	/**
	 * Add a Computer object to the list of connected computers
	 * @param p1 The Computer object to be connected
	 * @return None
	 * @note The computers list will be appended with the Computer
	 * object if new
	 */
	void addComputer(Computer *c);

	/**
	 * Add a Printer object to the list of connected printers
	 * @param p1 The Printer object to be connected
	 * @return None
	 * @note The printers list wil be appended with the Printer
	 * object if new
	 */
	void addPrinter(Printer *p);

	/**
	 * Find an available printer in all connected printers, then 
	 * send a print request to the printer
	 * @param p1 Text to be printed
	 * @param p2 Size of the paper to be printed on
	 * @return True if the Network can find available printer,
	 * false if there is no available printer
	 * @note The network will attempt to send the print request
	 * to all connected printers (from the first connected to last) 
	 * and stop when it find an available printer or has exhausted
	 * all connected printers
	 */
	bool print(const char *message, PaperSize size) const;

	/**
	 * Print the name of all connected devices out to the console
	 * @return None
	 */
	void displayConnections() const;
		
	/**
	 * Getter for the network's name
	 * @return Network's name
	 */
	char* getName() const;
};

#endif
