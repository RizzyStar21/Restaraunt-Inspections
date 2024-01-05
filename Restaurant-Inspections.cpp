/******************************************************************************
Author: Rizwan Mohamed 
Welcome to Resaurant Inspections
The code below displays the user some restaurant inspection data. 
It reads data from a file, processes the information, and provides 
various options for users to interact with and retrieve insights from the data.
The code organizes the restaurant data into objects, processes and analyzes the data,
and presents the results based on the user's choice in the menu. This program is 
designed for inspecting and analyzing restaurant inspection data to provide statistics and search capabilities.
*******************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// Class Date to track each individual part of the date 
class Date {
public:
    Date(int day, int month, int year) : day(day), month(month), year(year) {}

    int getDay() const {
        return day;
    }

    int getMonth() const {
        return month;
    }

    int getYear() const {
        return year;
    }

    bool isLeapYear() const {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            return true;
        }
        return false;
    }

    // Comparison operators for Date objects
    bool operator<(const Date& other) const {
        if (year < other.year) return true;
        if (year > other.year) return false;
        if (month < other.month) return true;
        if (month > other.month) return false;
        return day < other.day;
    }

    bool operator>(const Date& other) const {
        return other < *this;
    }

    bool operator==(const Date& other) const {
        return day == other.day && month == other.month && year == other.year;
    }

private:
    int day;
    int month;
    int year;
};



// Restaurant Class
// This class represents information about a restaurant's inspection, including its name, address, inspection date,
// risk rating, inspection result, and neighborhood. It provides methods to access and retrieve these details.

class Restaurant {
public:
    // Constructor to initialize restaurant details
    Restaurant(const string& name, const string& address, Date inspectionDate, char risk, const string& result, const string& neighborhood)
        : name(name), address(address), inspectionDate(inspectionDate), risk(risk), result(result), neighborhood(neighborhood) {}
	
    // Getter methods for accessing restaurant details
	const string& getName() const {
        return name;
    }

    const string& getAddress() const {
        return address;
    }

    const Date& getInspectionDate() const {
        return inspectionDate;
    }

    char getRisk() const {
        return risk;
    }

    const string& getResult() const {
        return result;
    }

    const string& getNeighborhood() const {
        return neighborhood;
    }

private:
    // Private member variables to store restaurant information
    string name;
    string address;
    Date inspectionDate;
    char risk;
    string result;
    string neighborhood;
};


// The code reads data from a user-specified file. The data contains information about restaurants, 
// including their names, addresses, inspection dates, risk levels, results, and neighborhoods.
vector<Restaurant> load_data(const string& datafile) {
    vector<Restaurant> restaurants;
    ifstream file(datafile);

    if (!file.is_open()) {
        cerr << "Failed to open the data file: " << datafile << endl;
        return restaurants; // Return an empty vector
    }

    string line;

    while (getline(file, line)) {
        // Declare inspectionDate outside of the if statement
        Date inspectionDate(0, 0, 0);

        // Split the line into individual pieces using ',' as the delimiter
        istringstream ss(line);
        string name, address, dateStr, riskStr, result, neighborhood;

        if (getline(ss, name, ',') &&
            getline(ss, address, ',') &&
            getline(ss, dateStr, ',') &&
            getline(ss, riskStr, ',') &&
            getline(ss, result, ',') &&
            getline(ss, neighborhood)) {

            // Parse dateStr
            int day, month, year;
            char delimiter;
            istringstream dateStream(dateStr);
            if (dateStream >> month >> delimiter >> day >> delimiter >> year && delimiter == '-') {
                inspectionDate = Date(day, month, year);
            } else {
                cerr << "Failed to parse date: " << dateStr << endl;
                continue; // Skip this entry
            }

            // Parse riskStr
            char risk = riskStr[0];

            // Create a Restaurant object and add it to the vector
            restaurants.push_back(Restaurant(name, address, inspectionDate, risk, result, neighborhood));
        } else {
            cerr << "Failed to parse line: " << line << endl;
        }
    }
    return restaurants;
}

// This function displays the overall inspection information, including the total number of 
// restaurants, the number that passed inspections, and the most recent passing inspection.
void displayOverAllInfo(const vector<Restaurant>& restaurants) {
    int totalRestaurants = restaurants.size();
    int passedRestaurants = 0;
    string mostRecentPassedRestaurantName;
    Date mostRecentPassedDate(1, 1, 2000);

    for (const Restaurant& restaurant : restaurants) {
        if (restaurant.getResult() == "Pass" || restaurant.getResult() == "Conditional") {
            passedRestaurants++;

            if (restaurant.getInspectionDate() > mostRecentPassedDate) {
                mostRecentPassedDate = restaurant.getInspectionDate();
                mostRecentPassedRestaurantName = restaurant.getName();
            }
        }
    }

    cout << "Number of restaurants: " << totalRestaurants << endl;
    cout << "Number that pass: " << passedRestaurants << endl;

    if (mostRecentPassedRestaurantName.empty()) {
        cout << "No recent passing inspections found." << endl;
    } else {
        cout << "Most recent passing inspection was of " << mostRecentPassedRestaurantName << " on " << 
		setfill('0') << setw(2) << mostRecentPassedDate.getMonth() << "-" << mostRecentPassedDate.getDay() << "-" << mostRecentPassedDate.getYear() << endl;
    }
}

// It calculates and displays the percentage of restaurants at different risk levels (High, Medium, Low).
void displayRiskPercentages(const vector<Restaurant>& restaurants) {
    // Initialize counters for each risk level.
    int highRiskCount = 0;
    int mediumRiskCount = 0;
    int lowRiskCount = 0;

    // Count the number of inspections in each risk category.
    for (const Restaurant& restaurant : restaurants) {
        char risk = restaurant.getRisk();
        if (risk == 'H') {
            highRiskCount++;
        } else if (risk == 'M') {
            mediumRiskCount++;
        } else if (risk == 'L') {
            lowRiskCount++;
        }
    }

    // Calculate the total number of inspections.
    int totalInspections = highRiskCount + mediumRiskCount + lowRiskCount;

    // Calculate risk percentages with one decimal place.
    double highRiskPercentage = static_cast<double>(highRiskCount) / totalInspections * 100.0;
    double mediumRiskPercentage = static_cast<double>(mediumRiskCount) / totalInspections * 100.0;
    double lowRiskPercentage = static_cast<double>(lowRiskCount) / totalInspections * 100.0;

    // Display the risk percentages.
    cout << "High Risk: " << fixed << setprecision(1) << highRiskPercentage << "%" << endl;
    cout << "Medium Risk: " << fixed << setprecision(1) << mediumRiskPercentage << "%" << endl;
    cout << "Low Risk: " << fixed << setprecision(1) << lowRiskPercentage << "%" << endl;
}

// This function presents statistics about passing, conditional passing, and failing inspections for each neighborhood
void displayPassingNumbersByNeighborhood(const vector<Restaurant>& restaurants) {
    vector<string> uniqueNeighborhoods;

    // Find unique neighborhoods
    for (const Restaurant& restaurant : restaurants) {
        const string& neighborhood = restaurant.getNeighborhood();
        if (find(uniqueNeighborhoods.begin(), uniqueNeighborhoods.end(), neighborhood) == uniqueNeighborhoods.end()) {
            uniqueNeighborhoods.push_back(neighborhood);
        }
    }

    // Display the statistics
    cout << "Neighborhood               Passed Cond. Pass     Failed" << endl;
    cout << "============               ====== ==========     ======" << endl;

    for (const string& neighborhood : uniqueNeighborhoods) {
        int passed = 0, condPassed = 0, failed = 0;

        for (const Restaurant& restaurant : restaurants) {
            if (restaurant.getNeighborhood() == neighborhood) {
                if (restaurant.getResult() == "Pass") {
                    passed++;
                } else if (restaurant.getResult() == "Conditional") {
                    condPassed++;
                } else if (restaurant.getResult() == "Fail") {
                    failed++;
                }
            }
        }

        cout << left << setw(30) << neighborhood << setw(11) << passed << setw(13)
        << condPassed << setw(9) << failed << endl;
    }
}

// Allows the user to search for a restaurant by name (case-insensitive). It then displays details of the matching restaurants.
void searchRestaurantByName(const vector<Restaurant>& restaurants) {
    // Prompt the user for a name to search for
    cout << "Enter restaurant to search for: ";
    string nameToSearch;
    cin.ignore();
    getline(cin, nameToSearch);

    // Display restaurants whose names contain the user's input (case-insensitive)
    bool found = false;

    for (const Restaurant& restaurant : restaurants) {
        string restaurantName = restaurant.getName();
        string nameToSearchLowerCase = nameToSearch;
        transform(restaurantName.begin(), restaurantName.end(), restaurantName.begin(), ::tolower);
        transform(nameToSearchLowerCase.begin(), nameToSearchLowerCase.end(), nameToSearchLowerCase.begin(), ::tolower);

        if (restaurantName.find(nameToSearchLowerCase) != string::npos) {
            found = true;
            cout << "Restaurant: " << restaurant.getName() << endl;
            cout << "Address: " << restaurant.getAddress() << endl;
            cout << "Inspection Date: " << setfill('0') << setw(2) << restaurant.getInspectionDate().getMonth() << "-"
            << setfill('0') << setw(2) <<  restaurant.getInspectionDate().getDay() << "-" << restaurant.getInspectionDate().getYear() << endl;
            cout << "Inspection Result: " << restaurant.getResult() << endl;
            cout << endl;
        }
    }

    if (!found) {
        cout << "No matching restaurants found." << endl;
    }
}


int main() {
	string datafile;
    cout << "Enter the data file to use: ";
    cin >> datafile;

    vector<Restaurant> restaurants = load_data(datafile);
    int choice;
    do {
        cout << "\nSelect a menu option:\n";
        cout << "   1. Display overall inspection information\n";
        cout << "   2. Display risk percentages\n";
        cout << "   3. Display passing numbers by neighborhood\n";
        cout << "   4. Search for restaurant by name\n";
        cout << "   5. Exit\n";
		cout << "Your choice: " << endl;
        cin >> choice;
		cout << endl;

        switch (choice) {
            case 1:
                displayOverAllInfo(restaurants);
                break;
            case 2:
                displayRiskPercentages(restaurants);
                break;
            case 3:
                displayPassingNumbersByNeighborhood(restaurants);
                break;
            case 4:
                searchRestaurantByName(restaurants);
                break;
            case 5:
                cout << "Exiting the program\n";
                break;
            default:
                cout << "Invalid choice. Please select a valid option.\n";
        }
    } while (choice != 5);

    return 0;
}