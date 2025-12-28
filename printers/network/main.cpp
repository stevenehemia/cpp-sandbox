#include <iostream>

#include "network.h"
#include "printer.h"
#include "computer.h"

using namespace std;


int main() {

	cout << "=================================\n"
	     << "We start configuring our network:\n"
	     << "=================================\n"
		 << endl;

	// The main network comes online
	Network mainNetwork("DoC-net");

	// An Epson EP-101 printer is installed, with
	// a paper tray that can hold 10 sheets
	Printer epson("EP-101", 10);

	// We load the cartridge and A4 paper into
	// the Epson printer
	InkCartridge ink1(20);
	epson.loadInk(ink1);
	epson.loadPaper(10, PaperSize::A4);

	// An HP LaserJet printer is installed,
	// with a paper tray to hold 5 sheets
	Printer hp("LaserJet", 5);

	// We load up the HP printer
	InkCartridge ink2(15);
	hp.loadInk(ink2);
	hp.loadPaper(1, PaperSize::A4);

	// Two computers are powered on
	Computer laptop("Laptop");
	Computer desktop("Desktop");

	// The printers are connected to the network
	epson.connect(mainNetwork);
	hp.connect(mainNetwork);

	// The computers are connected to the network
	laptop.connect(mainNetwork);
	desktop.connect(mainNetwork);

	// We print "hello world" from the laptop
	laptop.print("Hello world", PaperSize::A4);

	// We attempt several printings from the desktop
	desktop.print("Testing", PaperSize::A4);
	desktop.print("Important memo", PaperSize::A4);
	desktop.print("Lecture notes", PaperSize::A4);

	cout << endl;

	// We reload the Epson printer
	InkCartridge ink3(30);
	epson.loadInk(ink3);
	epson.loadPaper(10, PaperSize::A3);

	// We try to print our important message again
	desktop.print("Lecture notes", PaperSize::A4);

	cout << endl;

	// We reload with the correct paper size
	epson.loadPaper(30, PaperSize::A4);

	cout << endl;

	// We reload with the correct paper qty
	epson.loadPaper(5, PaperSize::A4);

	// And we try to print once more
	desktop.print("Lecture notes", PaperSize::A4);

	cout << endl;

	// Finally, we ask the network to display its connections
	mainNetwork.displayConnections();

	cout << endl
	     << "=================================\n"
	     << "We finish configuring our network\n"
	     << "=================================\n";

	return 0;
}
