#include<iostream>
#include<cstring>
#include<vector>

using namespace std;

class Network;

// Classes definition
class InkCartridge
{
    private:
        float level;
    public:
        //Constructor
        InkCartridge(float level);

        // Getters
        float getLevel();
};

enum class PaperSize { None, A4, A3, Letter };

std::ostream& operator<<(std::ostream& os, PaperSize size);

class Printer
{
    private:
        char* name;
        int trayCapacity;
        int paperQty;
        PaperSize currentSize;
        float inkLevel;
        Network* connectedNetwork;
    public:
        // Contructor
        Printer(const char* name, int trayCapacity);

        // Methods
        void loadPaper(int count, PaperSize size);
        void loadInk(InkCartridge &ink);
        void connect(Network &n);
        const char* print(const char* message, PaperSize size);

        // Getters
        char* getName();
        int getPaperQty();
        float getInkLevel();
};

class Computer
{
    private:
        char* name;
        Network* connectedNetwork;
    public:
        // Contructor
        Computer(const char* name);

        // Methods
        void connect(Network &n);
        void print(const char* message, PaperSize size);

        // Getters
        char* getName();
};

class Network
{
    private:
        char* name;
        vector<Computer*> computers;
        vector<Printer*> printers;

    public:
        // Constructor
        Network (const char* name);

        // Methods
        void displayConnections();
        void addComputer(Computer *c);
        void addPrinter(Printer *p);
        bool print(const char* message, PaperSize size);

        // Getters
        char* getName();
};

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

        cout << endl;

        // We print "hello world" from the laptop
        laptop.print("Hello world", PaperSize::A4);
        cout << endl;

        // We attempt several printings from the desktop
        desktop.print("Testing", PaperSize::A4);
        cout << endl;
        desktop.print("Important memo", PaperSize::A4);
        cout << endl;
        desktop.print("Lecture notes", PaperSize::A4);

        cout << endl;

        // We reload the Epson printer
        InkCartridge ink3(30);
        epson.loadInk(ink3);
        epson.loadPaper(10, PaperSize::A3);

        cout << endl;

        // We try to print our important message again
        desktop.print("Lecture notes", PaperSize::A4);

        cout << endl;

        // We reload with the correct paper size
        epson.loadPaper(30, PaperSize::A4);

        cout << endl;

        // We reload with the correct paper qty
        epson.loadPaper(5, PaperSize::A4);

        cout << endl;

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


std::ostream& operator<<(std::ostream& os, PaperSize size) {
    switch(size) {
        case (PaperSize::A4): os << "A4"; break;
        case (PaperSize::A3): os << "A3"; break;
        case (PaperSize::Letter): os << "Letter"; break;
        default: os << "None";
    }
    return os;
}


// ===== Implementation: Network =====
// Constructor
Network::Network (const char* name) {
    this->name = new (nothrow) char[strlen(name) + 1];
    strcpy(this->name, name);
}

// Methods
void Network::displayConnections() {
    std::cout << name << " is connected to: " << std::endl;
    std::cout << "    " << printers.size() << " printers: " << std::endl;
    for(Printer* p : printers) {
        std::cout << "        " << p->getName() << std::endl;
    }
        std::cout << "    " << printers.size() << " printers: " << std::endl;
    for(Computer* c : computers) {
        std::cout << "        " << c->getName() << std::endl;
    }
}

void Network::addComputer(Computer *c) {
    computers.push_back(c);
}
void Network::addPrinter(Printer *p) {
    printers.push_back(p);
}

bool Network::print(const char* message, PaperSize size) {
    std::cout << name << " received request to print \""
              << message << "\"" << std::endl;
    for(Printer* p : printers) {
        const char* printerResponse = p->print(message, size);
        if(strcmp(printerResponse, "OK")) {
            std::cout << name << " cannot print \"" << message << "\" using "
                      << p->getName() << " " << printerResponse << std::endl;
        } else {
            std::cout << name << " successfully printed \"" << message
                      << "\" using " << p->getName() << std::endl;
            return true;
        }
    }
    std::cout << name << " cannot print message" << std::endl;
    return false;
}

// Getters
char* Network::getName() { return name; };


// Implementation: InkCartridge
//Constructor
InkCartridge::InkCartridge(float level) {
    this->level = level;
}

// Getters
float InkCartridge::getLevel() { return level; }


// Implementation: Printer
// Constructor
Printer::Printer(const char* name, int trayCapacity) {
    this->name = new (nothrow) char[strlen(name) + 1];
    strcpy(this->name, name);
    this->trayCapacity = trayCapacity;
    paperQty = 0;
    currentSize = PaperSize::None;
    inkLevel = 0.0;
}

// Methods
void Printer::loadPaper(int count, PaperSize size) {
    std::cout << "Loading " << count << " " << size << " sheets into "
              << name << " tray" << std::endl;

    if((size != currentSize && count > trayCapacity) ||
       (size == currentSize && paperQty + count > trayCapacity)) {
            std::cout << "Cannot load " << count << " " << size
                    << " sheets into tray, not enough space" << std::endl;
            return;
    }

    if(paperQty != 0) {
        std::cout << "Removing " << paperQty << " " << currentSize
                  << " sheets from tray" << std::endl;
        paperQty = 0;
    }

    currentSize = size;
    paperQty = paperQty + count;
    std::cout << "Loaded " << count << " " << size
              << " sheets into tray (now " << paperQty << ")" << std::endl;
}

void Printer::loadInk(InkCartridge &ink) {
    std::cout << "Loading new ink cartridge into " << name
              << " (" << ink.getLevel() << " units)" << std::endl;
    inkLevel = ink.getLevel();
}
void Printer::connect(Network &n) {
    n.addPrinter(this);
    connectedNetwork = &n;
    std::cout << name << " connects to " << n.getName() << std::endl;
}

const char* Printer::print(const char* message, PaperSize size) {
    if(paperQty < 1) {
        return "(not enough paper)";
    }
    if(size != currentSize) {
        return "(wrong paper size)";
    }
    if(inkLevel < strlen(message)) {
        return "(not enough ink)";
    }

    paperQty--;
    inkLevel -= strlen(message);
    std::cout << "--- " << message << " --- from " << name << std::endl;
    std::cout << "Used 1 " << size << " sheets from tray (now "
              << paperQty << ")" << std::endl;
    std::cout << "Used " << strlen(message) << " units of ink from cartridge (now "
              << inkLevel << ")" << std::endl;

    return "OK";
}

// Getters
char* Printer::getName() { return name; }
int Printer::getPaperQty() { return paperQty; }
float Printer::getInkLevel() { return inkLevel; }


// Implementation: Computer
// Contructor
Computer::Computer(const char* name) {
    this->name = new (nothrow) char[strlen(name) + 1];
    strcpy(this->name, name);
}

// Methods
void Computer::connect(Network &n) {
    n.addComputer(this);
    connectedNetwork = &n;
    std::cout << name << " connects to " << n.getName() << std::endl;
}

void Computer::print(const char* message, PaperSize size) {
    std::cout << name << " wants to print \"" << message << "\"" << std::endl;
    if(!connectedNetwork->print(message, size)) {
        std::cout << name << " cannot print message" << std::endl;
    }
}

// Getters
char* Computer::getName() { return name; }