#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
using namespace std;

// Abstract Vehicle class
class Vehicle {
protected:
    int id;
    string licensePlate;
    double dailyPrice;
    static int nextId;
    static double tax;
    
public:
    // Constructors
    Vehicle() : id(++nextId), licensePlate(""), dailyPrice(0.0) {}
    
    Vehicle(const string& licensePlate, double dailyPrice) 
        : id(++nextId), licensePlate(licensePlate), dailyPrice(dailyPrice) {}
    
    Vehicle(const Vehicle& v) 
        : id(++nextId), licensePlate(v.licensePlate), dailyPrice(v.dailyPrice) {}
    
    // Virtual destructor
    virtual ~Vehicle() {}
    
    // Assignment operator
    Vehicle& operator=(const Vehicle& v) {
        if (this != &v) {
            licensePlate = v.licensePlate;
            dailyPrice = v.dailyPrice;
        }
        return *this;
    }
    
    // Getters/Setters for the static tax
    static void setTax(double t) { tax = t; }
    static double getTax() { return tax; }
    
    // Pure virtual method to make the class abstract
    virtual double totalCost(int numDays) const = 0;
    
    // Method to calculate the rental cost without tax
    virtual double rentalCost(int numDays) const {
        return dailyPrice * numDays;
    }
    
    // Virtual method for cloning
    virtual Vehicle* clone() const = 0;
    
    // toString method for display
    virtual string toString() const = 0;
    
    // Getters
    int getId() const { return id; }
    string getLicensePlate() const { return licensePlate; }
    double getDailyPrice() const { return dailyPrice; }
    
    // Overload of the << operator
    friend ostream& operator<<(ostream& os, const Vehicle& v);
};

// Initialization of static members
int Vehicle::nextId = 0;
double Vehicle::tax = 0.2; // Default 20% tax

// Overload of the << operator for Vehicle
ostream& operator<<(ostream& os, const Vehicle& v) {
    os << v.toString();
    return os;
}

// Car class
class Car : public Vehicle {
protected:
    int numSeats;
    
public:
    // Constructors
    Car() : Vehicle(), numSeats(5) {}
    
    Car(const string& licensePlate, double dailyPrice, int numSeats) 
        : Vehicle(licensePlate, dailyPrice), numSeats(numSeats) {}
    
    Car(const Car& v) 
        : Vehicle(v), numSeats(v.numSeats) {}
    
    // Assignment operator
    Car& operator=(const Car& v) {
        if (this != &v) {
            Vehicle::operator=(v);
            numSeats = v.numSeats;
        }
        return *this;
    }
    
    // Redefinition of totalCost
    double totalCost(int numDays) const override {
        double cost = rentalCost(numDays);
        return cost + (cost * getTax());
    }
    
    // Redefinition of toString
    string toString() const override {
        return "Car[ID: " + to_string(id) + 
               ", License: " + licensePlate + 
               ", Price/day: " + to_string(dailyPrice) + 
               ", Seats: " + to_string(numSeats) + "]";
    }
    
    // Redefinition of clone
    Vehicle* clone() const override {
        return new Car(*this);
    }
    
    // Getter
    int getNumSeats() const { return numSeats; }
};

// Utility class
class Utility : public Vehicle {
protected:
    double volumeCubicMeters;
    
public:
    // Constructors
    Utility() : Vehicle(), volumeCubicMeters(0.0) {}
    
    Utility(const string& licensePlate, double dailyPrice, double volumeCubicMeters) 
        : Vehicle(licensePlate, dailyPrice), volumeCubicMeters(volumeCubicMeters) {}
    
    Utility(const Utility& u) 
        : Vehicle(u), volumeCubicMeters(u.volumeCubicMeters) {}
    
    // Assignment operator
    Utility& operator=(const Utility& u) {
        if (this != &u) {
            Vehicle::operator=(u);
            volumeCubicMeters = u.volumeCubicMeters;
        }
        return *this;
    }
    
    // Supplement for utility
    double utilitySupplement() const {
        return volumeCubicMeters * 5.0; // 5€ per m3
    }
    
    // Redefinition of rentalCost
    double rentalCost(int numDays) const override {
        return (dailyPrice * numDays) + (utilitySupplement() * numDays);
    }
    
    // Redefinition of totalCost
    double totalCost(int numDays) const override {
        double cost = rentalCost(numDays);
        return cost + (cost * getTax());
    }
    
    // Redefinition of toString
    string toString() const override {
        return "Utility[ID: " + to_string(id) + 
               ", License: " + licensePlate + 
               ", Price/day: " + to_string(dailyPrice) + 
               ", Volume: " + to_string(volumeCubicMeters) + " m3]";
    }
    
    // Redefinition of clone
    Vehicle* clone() const override {
        return new Utility(*this);
    }
    
    // Getter
    double getVolumeCubicMeters() const { return volumeCubicMeters; }
};

// Agency class
class Agency {
private:
    string name;
    int capacity;
    vector<Vehicle*> fleet;
    
public:
    // Constructor
    Agency(const string& name, int capacity) : name(name), capacity(capacity) {}
    
    // Deep copy constructor
    Agency(const Agency& a) : name(a.name), capacity(a.capacity), fleet(a.fleet.size()) {
        for (size_t i = 0; i < a.fleet.size(); ++i) {
            fleet[i] = a.fleet[i]->clone();
        }
    }
    
    // Destructor
    ~Agency() {
        for (auto p : fleet) {
            delete p;
        }
    }
    
    // Assignment operator
    Agency& operator=(const Agency& a) {
        if (this != &a) {
            for (auto p : fleet) delete p;
            fleet.clear();
            name = a.name;
            capacity = a.capacity;
            fleet.resize(a.fleet.size());
            for (size_t i = 0; i < a.fleet.size(); ++i) {
                fleet[i] = a.fleet[i]->clone();
            }
        }
        return *this;
    }
    
    // Method to add a vehicle
    bool addVehicle(Vehicle* v) {
        if (fleet.size() < static_cast<size_t>(capacity) && v != nullptr) {
            fleet.push_back(v->clone());
            return true;
        }
        return false;
    }
    
    // + operator to merge two agencies
    Agency operator+(const Agency& a) const {
        string newName = name + "_" + a.name;
        int newCapacity = capacity + a.capacity;
        Agency newAgency(newName, newCapacity);
        for (auto p : fleet) {
            newAgency.addVehicle(p);
        }
        for (auto p : a.fleet) {
            newAgency.addVehicle(p);
        }
        return newAgency;
    }
    
    // toString method
    string toString() const {
        string result = "Agency: " + name + "\n";
        result += "Capacity: " + to_string(capacity) + "\n";
        result += "Vehicles (" + to_string(fleet.size()) + "):\n";
        for (auto p : fleet) {
            result += "  " + p->toString() + "\n";
        }
        return result;
    }
    
    // Getters
    string getName() const { return name; }
    int getCapacity() const { return capacity; }
    int getVehicleCount() const { return fleet.size(); }
    
    // Friend function to calculate the total to pay
    friend double totalToPay(const Agency& a, int numDays);
    
    // Overload of the << operator
    friend ostream& operator<<(ostream& os, const Agency& a);
};

// Friend function to calculate the total to pay
double totalToPay(const Agency& a, int numDays) {
    double total = 0.0;
    for (auto p : a.fleet) {
        total += p->totalCost(numDays);
    }
    return total;
}

// Overload of the << operator for Agency
ostream& operator<<(ostream& os, const Agency& a) {
    os << a.toString();
    return os;
}

// Main program to test Exercise 1
void mainExercise1() {
    cout << "=== EXERCISE 1: Car Rental Management ===" << endl << endl;
    
    // 1. Testing Vehicle, Car and Utility classes
    cout << "1. Testing Vehicle, Car and Utility classes:" << endl;
    cout << "----------------------------------------------------" << endl;
    
    // Tax modification
    Vehicle::setTax(0.18); // 18% tax
    
    // Vehicle creation
    Car v1("AB-123-CD", 50.0, 5);
    Car v2("EF-456-GH", 65.0, 7);
    Utility u1("IJ-789-KL", 80.0, 15.5);
    Utility u2("MN-012-OP", 95.0, 22.0);
    
    // Vehicle display (polymorphism)
    cout << "Created vehicles:" << endl;
    cout << v1 << endl;
    cout << v2 << endl;
    cout << u1 << endl;
    cout << u2 << endl;
    
    // Rental cost test
    int testDays = 3;
    cout << "\nRental cost for " << testDays << " days:" << endl;
    cout << fixed << setprecision(2);
    cout << "Car " << v1.getLicensePlate() << ": " << v1.totalCost(testDays) << " €" << endl;
    cout << "Car " << v2.getLicensePlate() << ": " << v2.totalCost(testDays) << " €" << endl;
    cout << "Utility " << u1.getLicensePlate() << ": " << u1.totalCost(testDays) << " €" << endl;
    cout << "Utility " << u2.getLicensePlate() << ": " << u2.totalCost(testDays) << " €" << endl;
    
    // 2. Testing the Agency class
    cout << "\n\n2. Testing the Agency class:" << endl;
    cout << "----------------------------" << endl;
    
    // Agency creation
    Agency agency1("Agency_Center", 5);
    
    // Adding vehicles
    agency1.addVehicle(&v1);
    agency1.addVehicle(&u1);
    agency1.addVehicle(&v2);
    
    cout << "Agency 1 created:" << endl;
    cout << agency1;
    
    // Deep copy test
    Agency agency2 = agency1; // Copy constructor
    agency2 = agency1; // Assignment operator
    
    // Modification of the copied agency
    agency2.addVehicle(&u2);
    
    cout << "\nAgency 2 (modified copy):" << endl;
    cout << agency2;
    
    // Creation of a third agency
    Agency agency3("Agency_North", 3);
    Car v3("QR-345-ST", 55.0, 4);
    agency3.addVehicle(&v3);
    
    cout << "\nAgency 3:" << endl;
    cout << agency3;
    
    // Agency merger
    Agency mergedAgency = agency1 + agency3;
    cout << "\nMerged agency (agency1 + agency3):" << endl;
    cout << mergedAgency;
    
    // Test of the friend function totalToPay
    int rentalDays = 7;
    cout << "\n3. Calculation of the total to pay for " << rentalDays << " days:" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << "Agency 1: " << totalToPay(agency1, rentalDays) << " €" << endl;
    cout << "Agency 2: " << totalToPay(agency2, rentalDays) << " €" << endl;
    cout << "Agency 3: " << totalToPay(agency3, rentalDays) << " €" << endl;
    cout << "Merged agency: " << totalToPay(mergedAgency, rentalDays) << " €" << endl;
}
