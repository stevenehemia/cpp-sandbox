#include <iostream>
#include <cstring>

#include "network.h"


/* Forward class declaration */
class Network;


/* ========================= InkCartridge class  ========================= */

/* Constructor for an InkCartridge object */
InkCartridge::InkCartridge(float level) {
    this->level = level;
}

/* Getter for InkCartridge's level */
float InkCartridge::getLevel() const { return level; }

/* Use ink to print a text */
void InkCartridge::useInk(int length) { level -= length; }


/* =========================== PaperSize class =========================== */

/* Overload << operator to output PaperSize object */
std::ostream& operator<<(std::ostream& os, PaperSize size) {
    switch(size) {
        case (PaperSize::A4): os << "A4"; break;
        case (PaperSize::A3): os << "A3"; break;
        case (PaperSize::Letter): os << "Letter"; break;
        default: os << "None";
    }
    return os;
}


/* =========================== PaperTray class =========================== */

/* Constructor for a PaperTray object */
PaperTray::PaperTray(int trayCapacity)
    : currentQty(0), currentSize(PaperSize::None)
{
    this->trayCapacity = trayCapacity;
    currentQty = 0;
    currentSize = PaperSize::None;
}

/* Getter for how much paper left in the paper tray */
int PaperTray::getCurrentQty() { return currentQty; };

/* Getter for what paper size is currently being hold */
PaperSize PaperTray::getCurrentSize() { return currentSize; };

/* Getter for tray capacity */
int PaperTray::getTrayCapacity() { return trayCapacity; };

/* Empty paper tray */
void PaperTray::removePaper()
{
    currentQty = 0;
    currentSize = PaperSize::None;
}

/* Load a paper tray with a specified paper size and qty */
void PaperTray::loadPaper(int count, PaperSize size)
{
    currentSize = size;
    currentQty = currentQty + count;
}

/* Use a sheet of paper */
void PaperTray::consumePaper() { currentQty--; }


/* ===========================  Printer class ============================ */

/* Constructor for a Printer object */
Printer::Printer(const char* name, int trayCapacity)
    : paperTray(trayCapacity), ink(nullptr)
{
    /* Assign Printer's name */
    this->name = new (std::nothrow) char[strlen(name) + 1];
    strcpy(this->name, name);
}

/* Load specified quantity and size of paper to a Printer */
void Printer::loadPaper(int count, PaperSize size)
{
    std::cout << "Loading " << count << " " << size << " sheets into "
              << name << " tray" << std::endl;

    /* Check paper tray's condition */
    if((size != paperTray.getCurrentSize()
       && count > paperTray.getTrayCapacity())
       || (size == paperTray.getCurrentSize()
       && paperTray.getCurrentQty() + count > paperTray.getTrayCapacity()))
    {
        /* Tray will be overloaded, print error error message to console */
	std::cout << "Cannot load " << count << " " << size
                  << " sheets into tray, not enough space" << std::endl;
        return;
    }

    /* Remove papers from paper tray if the papers to be loaded is not of
     * the same size */
    if(paperTray.getCurrentQty() != 0 && size != paperTray.getCurrentSize())
    {
        std::cout << "Removing " << paperTray.getCurrentQty() << " "
		  << paperTray.getCurrentSize() << " sheets from tray"
		  << std::endl;
        paperTray.removePaper();
    }

    /* Load paper tray with the specified paper size and qty */
    paperTray.loadPaper(count, size);
    std::cout << "Loaded " << count << " " << size
              << " sheets into tray (now " << paperTray.getCurrentQty()
	      << ")" << std::endl;
}

/* Load an InkCartridge to a Printer */
void Printer::loadInk(InkCartridge &ink)
{
    /* Print activity log to console */
    std::cout << "Loading new ink cartridge into " << name
              << " (" << ink.getLevel() << " units)" << std::endl;
   
    this->ink = &ink;
}

/* Connect a Printer to a Network */
void Printer::connect(Network &n)
{
    n.addPrinter(this);
    connectedNetwork = &n;

    /* Print activity log to console */
    std::cout << name << " connects to " << n.getName() << std::endl;
}

/* Print a text if resources (paper size, paper qty, ink level) are available
 * Return success/error message to print requestor */
const char* Printer::print(const char* message, PaperSize size)
{
    /* Check printer's resources to print the text */
    if(size != paperTray.getCurrentSize()) {
	/* Paper tray is not loaded with the correct paper size */
        return "(wrong paper size)";
    }
    if(paperTray.getCurrentQty() < 1) {
	if(ink->getLevel() < strlen(message)) {
	    /* Does not have enough ink and paper */
	    return "(not enough ink or paper)";
	} else {
	    /* Does not have enough paper only */
  	    return "(not enough paper)";
	}
    }
    if(ink->getLevel() < strlen(message)) {
	/* Does not have enough ink only */
        return "(not enough ink)";
    }

    /* Enough resource to print the text, proceed to print */
    /* Use a paper and ink to print the text */
    paperTray.consumePaper();
    ink->useInk(strlen(message));

    /* Print activity log to the console */
    std::cout << "--- " << message << " --- from " << name << std::endl;
    std::cout << "Used 1 " << size << " sheets from tray (now "
              << paperTray.getCurrentQty() << ")" << std::endl;
    std::cout << "Used " << strlen(message)
	      << " units of ink from cartridge (now "
              << ink->getLevel() << ")" << std::endl;

    /* Print successful, return a success message to the requestor */
    return "OK";
}

/* Getter for Printer's name */
char* Printer::getName() const { return name; }
