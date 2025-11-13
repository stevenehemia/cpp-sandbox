#include<string>
#include<vector>
#include<iostream>

using namespace std;

class Debtor {
    protected:
        string name;
        string address;
        vector<Contract*> contracts;
        vector<Loan*> loans;
        vector<Car> ownedCars;
    public:
        // Constructor
        Debtor(string n, string ad):
            name(n), address(ad) {}
        
        // Pure virtual methods
        virtual void addContract() = 0;
        virtual double disposableIncome() const = 0;

        // Concrete methods
        double totalLoans() const {
            double sum = 0.0;
            for(Loan* l : loans) {
                sum = sum + l->getRemainingAmount();
            }
            return sum;
        }
        virtual void applyLoan(Bank& b, Car& c) {
            if (b.grantLoan(disposableIncome(), c)) {
                sendAcceptance();
                Loan newLoan(this, &b, c.price());
                loans.push_back(&newLoan);
                ownedCars.push_back(c);
            } else {
                cout << "Not eligible for loan";
            }
        }
        void sendAcceptance() {
            cout << "Loan accepted";
        }
};

class Person: public Debtor {
    private:
        int age;
    public:
        // Constructor
        Person(string n, string ad, int ag):
            Debtor (n, ad), age(ag) {}
        
        // Getters
        string getName() const { return name; }
        string getAddress() const { return address; }
        int getAge() const { return age; }

        // Methods
        void addContract(Contract* c) {
            if(contracts.size() >= 3) {
                cout << "Contract limit exceeded.";
            } else {
                contracts.push_back(c);
            }
        }

        double totalSalary() const {
            double sum = 0.0;
            for (Contract* c : contracts) {
                sum = sum + c->getSalary();
            }
            return sum;
        }

        double priceOfCars() const {
            double sum = 0.0;
            for (Car c : ownedCars) {
                sum = sum + c.price();
            }
        }

        double disposableIncome() const override {
            return (totalSalary() / age - 0.1 * priceOfCars() - 0.2 * totalLoans());
        }
};

class Company: public Debtor {
    private:
        double turnover;
    public:
        // Constructor
        Company(string n, string ad, double t):
            Debtor (n, ad), turnover(t) {}
        
        // Getters
        string getName() const { return name; }
        string getAddress() const { return address; }
        double getTurnover() const { return turnover; }

        // Methods
        void addContract(Contract* c) {
            contracts.push_back(c);
        }

        double salaryCost() const {
            double sum = 0.0;
            for(Contract* c : contracts) {
                sum = sum + c->getSalary();
            }
            return sum;
        }

        double disposableIncome() const override {
            return (turnover - salaryCost() - 0.2 * totalLoans());
        }
};

class Bank {
    private:
        string name;
        string address;
        double interest;
        vector<Car> ownedCars;
    public:
        // Constructor
        Bank(string n, string ad, double i):
            name(n), address(ad), interest(i) {}

        // Getters
        string getName() const { return name; }
        string getAddress() const { return address; }
        double getInterestRate() const { return interest; }

        // Methods
        bool grantLoan(double di, Car c) {
            if(di > c.price() * interest) {
                return true;
            } else {
                return false;
            }
        }

};

class Contract {
    private:
        Company* employer;
        Person* employee;
        double salary;
    public:
        // Contructor
        Contract(Company* c, Person* p, double s)
            : employer(c), employee(p), salary(s) {}
        
        // Getters
        Company* getEmployer() { return employer; }
        Person* getEmployee() { return employee; }
        double getSalary() { return salary; }
};

class Car {
    private:
        string numberPlate;
        double horsepower;
        double mileage;
    public:
        // Constructor
        Car(string n, double hp, double m) {
            numberPlate = n;
            horsepower = hp;
            mileage = m;
        }

        // Getters
        string getNumberPlate() const { return numberPlate; }
        double getHorsepower() const { return horsepower; }
        double getMileage() const { return mileage; }

        double price() {
            return (horsepower * 10000 / mileage);
        }
};

class Loan {
    private:
        Debtor* borrower;
        Bank* lender;
        double loanAmount;
        double remainingAmount;
    public:
        // Constructor
        Loan(Debtor* b, Bank* l, double la)
            : borrower(b), lender(l), loanAmount(la), remainingAmount(la) {}
        
        // Getters
        Debtor* getBorrower() const { return borrower; }
        Bank* getLender() const { return lender; }
        double getLoanAmount() const { return loanAmount; }
        double getRemainingAmount() const { return remainingAmount; }
};