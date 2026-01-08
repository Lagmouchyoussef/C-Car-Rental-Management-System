#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "Exercise1.c++"
using namespace std;

// === CLASSES FOR OPTIONS ===

// Class for Insurance option
class InsuranceOption {
protected:
    bool insuranceActive;
    double dailyInsuranceCost;

public:
    InsuranceOption() : insuranceActive(false), dailyInsuranceCost(10.0) {}

    InsuranceOption(bool active, double cost)
        : insuranceActive(active), dailyInsuranceCost(cost) {}

    void activateInsurance() { insuranceActive = true; }
    void deactivateInsurance() { insuranceActive = false; }
    bool isInsured() const { return insuranceActive; }

    double getDailyInsuranceCost() const { return dailyInsuranceCost; }
    void setDailyInsuranceCost(double cost) { dailyInsuranceCost = cost; }

    double insuranceCost(int numDays) const {
        return insuranceActive ? (dailyInsuranceCost * numDays) : 0.0;
    }

    string toStringInsurance() const {
        return insuranceActive ?
               "(Insurance: " + to_string(dailyInsuranceCost) + "€/day)" :
               "(No insurance)";
    }
};

// Class for GPS option
class GPSOption {
protected:
    bool gpsActive;
    double dailyGPSCost;

public:
    GPSOption() : gpsActive(false), dailyGPSCost(5.0) {}

    GPSOption(bool active, double cost)
        : gpsActive(active), dailyGPSCost(cost) {}

    void activateGPS() { gpsActive = true; }
    void deactivateGPS() { gpsActive = false; }
    bool hasGPS() const { return gpsActive; }

    double getDailyGPSCost() const { return dailyGPSCost; }
    void setDailyGPSCost(double cost) { dailyGPSCost = cost; }

    double gpsCost(int numDays) const {
        return gpsActive ? (dailyGPSCost * numDays) : 0.0;
    }

    string toStringGPS() const {
        return gpsActive ?
               "(GPS: " + to_string(dailyGPSCost) + "€/day)" :
               "(No GPS)";
    }
};

// === OPTIONED CAR CLASS ===

class OptionedCar : public Car, public InsuranceOption, public GPSOption {
public:
    // Constructors
    OptionedCar()
        : Car(), InsuranceOption(), GPSOption() {}

    OptionedCar(const string& licensePlate, double dailyPrice, int numSeats,
                 bool insurance = false, bool gps = false,
                 double insuranceCost = 10.0, double gpsCost = 5.0)
        : Car(licensePlate, dailyPrice, numSeats),
          InsuranceOption(insurance, insuranceCost),
          GPSOption(gps, gpsCost) {}

    OptionedCar(const OptionedCar& oc)
        : Car(oc),
          InsuranceOption(oc),
          GPSOption(oc) {}

    // Assignment operator
    OptionedCar& operator=(const OptionedCar& oc) {
        if (this != &oc) {
            Car::operator=(oc);
            InsuranceOption::operator=(oc);
            GPSOption::operator=(oc);
        }
        return *this;
    }

    // Redefinition of totalCost with options
    double totalCost(int numDays) const override {
        // Base cost of the car with tax
        double baseCost = Car::totalCost(numDays);

        // Addition of option costs (without tax)
        double optionsCost = insuranceCost(numDays) + gpsCost(numDays);

        return baseCost + optionsCost;
    }

    // Detailed cost
    void displayDetails(int numDays) const {
        double baseRentalCost = rentalCost(numDays);
        double taxCost = baseRentalCost * getTax();
        double insurance = insuranceCost(numDays);
        double gps = gpsCost(numDays);
        double total = totalCost(numDays);

        cout << fixed << setprecision(2);
        cout << "Details for " << numDays << " days:" << endl;
        cout << "  Base rental: " << baseRentalCost << " €" << endl;
        cout << "  Tax (" << (getTax() * 100) << "%): " << taxCost << " €" << endl;
        if (isInsured()) {
            cout << "  Insurance: " << insurance << " €" << endl;
        }
        if (hasGPS()) {
            cout << "  GPS: " << gps << " €" << endl;
        }
        cout << "  TOTAL: " << total << " €" << endl;
    }

    // Redefinition of toString
    string toString() const override {
        string result = "OptionedCar[ID: " + to_string(getId()) +
                       ", License: " + getLicensePlate() +
                       ", Price/day: " + to_string(getDailyPrice()) +
                       ", Seats: " + to_string(getNumSeats()) + "] ";
        result += toStringInsurance() + " " + toStringGPS();
        return result;
    }

    // Redefinition of clone
    Vehicle* clone() const override {
        return new OptionedCar(*this);
    }
};

// === TEST PROGRAM FOR EXERCISE 2 ===

void testExercise2() {
    cout << "\n\n=== EXERCISE 2: Cars with options ===" << endl << endl;

    // 1. Creation of different cars with options
    cout << "1. Creating cars with different options:" << endl;
    cout << "-----------------------------------------" << endl;

    // Car without options
    OptionedCar car1("AA-111-BB", 50.0, 5);

    // Car with insurance only
    OptionedCar car2("CC-222-DD", 60.0, 7, true, false);

    // Car with GPS only
    OptionedCar car3("EE-333-FF", 55.0, 4, false, true);

    // Car with insurance and GPS
    OptionedCar car4("GG-444-HH", 70.0, 5, true, true, 12.0, 7.0);

    // Display of cars
    cout << car1 << endl;
    cout << car2 << endl;
    cout << car3 << endl;
    cout << car4 << endl;

    // 2. Test of rental cost with options
    cout << "\n\n2. Calculating rental cost (5 days):" << endl;
    cout << "-------------------------------------" << endl;

    int days = 5;

    cout << "\nCar 1 - Without options:" << endl;
    car1.displayDetails(days);

    cout << "\nCar 2 - With insurance:" << endl;
    car2.displayDetails(days);

    cout << "\nCar 3 - With GPS:" << endl;
    car3.displayDetails(days);

    cout << "\nCar 4 - With insurance and GPS:" << endl;
    car4.displayDetails(days);

    // 3. Polymorphism test
    cout << "\n\n3. Polymorphism test:" << endl;
    cout << "----------------------" << endl;

    // Creation of a vector of pointers to Vehicle
    vector<Vehicle*> vehicles;

    // Filling the vector with different types of vehicles
    vehicles.push_back(new Car("II-555-JJ", 45.0, 5));
    vehicles.push_back(new Utility("KK-666-LL", 85.0, 18.0));
    vehicles.push_back(new OptionedCar("MM-777-NN", 65.0, 5, true, false));
    vehicles.push_back(new OptionedCar("OO-888-PP", 75.0, 7, false, true));
    vehicles.push_back(new OptionedCar("QQ-999-RR", 80.0, 5, true, true));
    vehicles.push_back(new Utility("SS-000-TT", 90.0, 25.0));

    // Display and calculation of costs via polymorphism
    cout << "Rental cost for 3 days (polymorphism):" << endl;
    cout << fixed << setprecision(2);
    for (auto p : vehicles) {
        cout << "\n" << *p;
        cout << "\nTotal cost (3 days): " << p->totalCost(3) << " €" << endl;
    }

    // 4. Test with the agency
    cout << "\n\n4. Integration with the agency:" << endl;
    cout << "-------------------------------" << endl;

    Agency optionsAgency("Agency_Options", 10);

    // Adding vehicles to the agency
    for (auto p : vehicles) {
        optionsAgency.addVehicle(p);
    }

    cout << optionsAgency;
    cout << "\nTotal to pay for 4 days: " << totalToPay(optionsAgency, 4) << " €" << endl;

    // Memory cleanup
    for (auto p : vehicles) {
        delete p;
    }
}

// Main program complete
int main() {
    // Execution of Exercise 1
    mainExercise1();

    // Execution of Exercise 2
    testExercise2();

    cout << "\n\n=== END OF PROGRAM ===" << endl;
    return 0;
}
