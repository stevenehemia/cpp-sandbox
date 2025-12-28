#ifndef PRINTER_H
#define PRINTER_H

#include "network.h"


/* Forward class declaration */
class Network;


/* ========================== InkCartridge class ========================== */

class InkCartridge
{	
    private:
	/* Attributes */
        float level;

    public:
        /**
	 * Constructor for an InkCartridge object
	 * @param p1 The ink level of the ink cartridge
	 * @return An InkCartridge object
	 */
	InkCartridge(float level);
	
	/**
	 * Getter for the ink level
	 * @return The level of the ink
	 */
	float getLevel() const;

	/**
	 * Use an InkCartridge to print a text
	 * @param p1 The text length to be printed
	 * @return None
	 * @note Cartridge's ink level will be deducted by the length
	 * of the text
	 */
	void useInk(int length);

};


/* =========================== PaperSize class ============================ */

enum class PaperSize { None, A4, A3, Letter };

/**
 * Helper function to output PaperSize class to ostream (overload <<)
 * @param p1 A reference to ostream object
 * @param p2 A PaperSize object
 * @return A reference to ostream object
 */
std::ostream& operator<<(std::ostream& os, PaperSize size);


/* ============================ PaperTray class =========================== */

class PaperTray
{    
    private:
	/* Attributes */
	int trayCapacity;
	int currentQty;
	PaperSize currentSize;

    public:
	/**
	 * Constructor for a PaperTray object
	 * @param p1 Capacity of the paper tray
	 * @return A PaperTray object
	 */
	PaperTray(int trayCapacity);
	
	/**
	 * Empty a PaperTray from papers
	 * @return None
	 * @note Set the currentQty to zero and currentSize to None
	 */
	void removePaper();

	/**
	 * Load a specified paper size and qty to a PaperTray
	 * @param p1 Quantity of the paper to be loaded
	 * @param p2 Size of the paper to be loaded
	 * @return None
	 * @note Add currentQty and change currentSize according to the
	 * passed arguments
	 */
	void loadPaper(int count, PaperSize size);

	/**
	 * Decrement paper quantity held inside a PaperTray by 1
	 * @return None
	 */
	void consumePaper();

	/**
	 * Getter for qty of papers held by a PaperTray
	 * @return Paper quantity currently held by the tray
	 */
	int getCurrentQty();

	/**
	 * Getter for the paper size currently held by a PaperTray
	 * @return Size of paper held by the tray
	 */
	PaperSize getCurrentSize();

	/**
	 * Getter for paper tray's capacity
	 * @return Tray's capacity
	 */
	int getTrayCapacity();
};


/* ============================ Printer class ============================= */

class Printer
{	
    private:
	// Attributes
	char *name;
	PaperTray paperTray;
	Network *connectedNetwork;
	InkCartridge *ink;
	
    public:
	/**
	 * Constructor for a Printer object
	 * @param p1 Name of the printer
	 * @param p2 Tray capacity of the printer
	 * @return A Printer object
	 */
	Printer(const char *name, int trayCapacity);
	
	/**
	 * Print a text on a specified paper size if the printer has 
	 * enough resources
	 * @param p1 A text to be printed
	 * @param p2 Size of the paper
	 * @return Error message if the printer does not have enough
	 * resources, "OK" if the printer can print the message
	 * @note Papers in printer's tray will be used by one regardless of
	 * text length and inkLevel will be deducted by the length of text
	 * (including whitespaces)
	 */
	const char* print(const char* message, PaperSize size);

	/**
	 * Load a specific paper size and qty to a Printer
	 * @param p1 Quantity of the paper
	 * @param p2 Size of the paper
	 * @return None, print an error/success message out to the console
	 * @note If the printer is loaded with the same paper size as its tray
	 * currently hold, an error will occur when the resulting quantity
	 * exceeds the tray capacity. If the printer is loaded with different
	 * paper size, the existing paper will be unloaded and an error will
	 * occur when the new paper quantity exceeds the tray capacity.
	 */
	void loadPaper(int count, PaperSize size);

	/**
	 * Load/reload an InkCartridge to a Printer
	 * @param p1 The ink cartridge to be loaded
	 * @return None
	 * @note Printer's ink cartridge can only be replaced, not possible
	 * to refill/top up
	 */
	void loadInk(InkCartridge &ink);

	/**
	 * Connect a Printer object to a Network object
	 * @param p1 The network to be connected to
	 * @return None
	 * @note Printer's connectedNetwork variable will be assigned with
	 * the network's name upon successful connection
	 */
	void connect(Network &n);

	/**
	 * Getter for printer's name
	 * @return Printer's name
	 */
	char* getName() const;
};

#endif
