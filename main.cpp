#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

class City {

    //  Represent all city details, name , country , history, mayorName, mayorAddress, population
    //  , year population recorded.
    public:
        //  Attributes
        std::string name, country, history, mayorName, mayorAddress;
        int population, recordYear;
        double latitude, longitude;

        //  Constructor and Destructor
        //  Main constructor
        City (std::string cityName, std::string cityCountry, int pop, int year, double lat, double lon
            , std::string mayor, std::string address, std::string hist)
            //  Initialise and make variable names more intuitive
            : name(cityName), country(cityCountry), population(pop),
            recordYear(year), latitude(lat), longitude(lon),
            mayorName(mayor), mayorAddress(address), history(hist) {}

        //  Methods

        void display() const {
             //  Used to print all details of the city.
             std::cout << "City Name: " << name << "\n"
                  << "Country: " << country << "\n"
                  << "History: " << history << "\n"
                  << "Population: " << population << "\n"
                  << "Population Recorded in: " << recordYear << "\n"
                  << "Mayor Name: " << mayorName << "\n"
                  << "Mayor Address: " << mayorAddress << "\n"
                  << "Coordinates: (" << latitude << ", " << longitude << ")\n";
        }
    //  Update city variables by string int or double, likely make shorter if I have time.
    void update(const std::string& field, const std::string& value) {
            if (field == "name") name = value;
            else if (field == "country") country = value;
            else if (field == "history") history = value;
            else if (field == "mayorName") mayorName = value;
            else if (field == "mayorAddress") mayorAddress = value;
            else std::cerr << "Invalid field name.\n";
        }
    void update(const std::string& field, int value) {
            if (field == "population") population = value;
            else if (field == "recordYear") recordYear = value;
            else std::cerr << "Invalid field name.\n";
        }

    void update(const std::string& field, double value) {
            if (field == "latitude") latitude = value;
            else if (field == "longitude") longitude = value;
            else std::cerr << "Invalid field name.\n";
        }

};

//  Class for displacement formula and angular distance
//  cos d = sin(phi1)*sin(phi2) + cos(phi1)*cos(phi2)*cos(L1 - L2)
//  (6371*pi*d) / 180 = s (km)

class DistanceCalculator {
    public:
    static constexpr double EARTH_RADIUS_KM = 6371.0;
    //  Method to calculate the displacement between cities
    static double calculateDistance(const City& city1, const City& city2) {

        // Convert lat and lon to rads from deg
        const double lat1 = city1.latitude * M_PI / 180.0;
        const double lon1 = city1.longitude * M_PI / 180.0;
        const double lat2 = city2.latitude * M_PI / 180.0;
        const double lon2 = city2.longitude * M_PI / 180.0;

        //  find cos(D) angular distance sin(phi1)*sin(phi2) + cos(phi1)*cos(phi2)*cos(L1 - L2)
        double cosD = sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lon1 - lon2);

        //  cosD validation range = [-1, 1]
        //  Note clamp restrains value to -1 to 1 so that trig works and no domain error.
        cosD = std::clamp(cosD, -1.0, 1.0);

        //  Use acos to find d
        const double d = acos(cosD);

        //  Convert angular distance to linear distance.
        const double distance = d * EARTH_RADIUS_KM;

        return distance;

    }
};

//  Class to manage the file cities data is stored in.
//  File Format :
//  name,country,population,recordYear,latitude,longitude,mayorName,mayorAddress,history
class FileManager {
public:
    static std::vector<City> loadData(const std::string& fileName) {
        std::vector<City> cities;   //  Store Loaded cities instances
        std::ifstream file(fileName);

        //  Validate that the file opened or not
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file.\n";
            return cities;
        }
        //  While loop to read files with cities data
        std::string line;
        while (std::getline(file, line)) {

            std::istringstream stream(line);

            std::string name, country, history, mayorName, mayorAddress;
            int population, recordYear;
            double latitude, longitude;

            //  Read the data from a single line
            std::getline(stream, name, ',');
            std::getline(stream, country, ',');
            stream >> population;
            stream.ignore(); // Skip comma
            stream >> recordYear;
            stream.ignore(); // Skip comma
            stream >> latitude;
            stream.ignore(); // Skip comma
            stream >> longitude;
            stream.ignore(); // Skip comma
            std::getline(stream, mayorName, ',');
            std::getline(stream, mayorAddress, ',');
            std::getline(stream, history);

            cities.emplace_back(name, country, population, recordYear, latitude, longitude, mayorName, mayorAddress, history);
        }

        file.close();
        return cities;
    }
    static void saveData(const std::vector<City>& cities, const std::string& fileName) {
        std::ofstream file(fileName);

        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file.\n";
            return;
        }

        for (const auto& city : cities) {
            file << city.name << ","
            << city.country << ","
            << city.population << ","
            << city.recordYear << ","
            << city.latitude << ","
            << city.longitude << ","
            << city.mayorName << ","
            << city.mayorAddress << ","
            << city.history << "\n";

        }
        file.close();
    }
};

//  Class for User Interface, this includes user input, output and command processing,
//  add, save, display, delete all or city.
//  name,country,population,recordYear,latitude,longitude,mayorName,mayorAddress,history

class UserInterface {
public:
    // Start the user interface loop
    static void start(std::vector<City>& cities) {
        std::string fileName ;
        std::string command;

        std::cout << "Welcome to the Cities of the World Program!\n";

        //  Check if the user wants to use a previously edited file
        std::cout << "Enter the filename to load data :";
        std::getline(std::cin, fileName);
        if (fileName.empty()) {
            std::cout << "Starting without a file . . ." << std::endl;
        } else {
            cities = FileManager::loadData(fileName);
        }

        std::cout << "Available commands: add, delete, search, update, display, distance, save, exit\n";

        while (true) {
            std::cout << "\nEnter a command: ";
            std::getline(std::cin, command);

            if (command == "add") {
                addCity(cities);
            } else if (command == "delete") {
                deleteCity(cities);
            } else if (command == "search") {
                searchCity(cities);
            } else if (command == "update") {
                updateCity(cities);
            } else if (command == "display") {
                displayCities(cities);
            } else if (command =="distance") {
                distance(cities);
            } else if (command == "save") {
                saveToFile(cities);
            } else if (command == "exit") {
                std::cout << "Exiting the program. Goodbye!\n";
                break;
            } else {
                std::cout << "Invalid command. Please try again.\n";
            }
        }
    }

private:
    // Add a new city
    static void addCity(std::vector<City>& cities) {
        std::string name, country, history, mayorName, mayorAddress;
        int population, recordYear;
        double latitude, longitude;

        std::cout << "Enter city name: ";
        std::getline(std::cin, name);

        std::cout << "Enter country: ";
        std::getline(std::cin, country);

        std::cout << "Enter population: ";
        std::cin >> population;
        std::cin.ignore(); // Clear newline character

        std::cout << "Enter record year: ";
        std::cin >> recordYear;
        std::cin.ignore();

        std::cout << "Enter latitude: ";
        std::cin >> latitude;
        std::cin.ignore();

        std::cout << "Enter longitude: ";
        std::cin >> longitude;
        std::cin.ignore();

        std::cout << "Enter mayor name: ";
        std::getline(std::cin, mayorName);

        std::cout << "Enter mayor address: ";
        std::getline(std::cin, mayorAddress);

        std::cout << "Enter short history: ";
        std::getline(std::cin, history);

        cities.emplace_back(name, country, population, recordYear, latitude, longitude, mayorName, mayorAddress, history);
        std::cout << "City '" << name << "' added successfully.\n";
    }
    //  Search for a particular city, could have been built in to display
    static void searchCity(std::vector<City>& cities) {

        std::cout << "Search for an  city by its name:"<< std::endl;
        std::string cityName;
        std::getline(std::cin, cityName);

        std::vector<City>::iterator it;
        if (cityNameIterator(cities, cityName, it)) {
            // If city is found, display its details
            it->display();
        } else {
            std::cout << "City '" << cityName << "' not found.\n";
        }
        }

    // Delete a city
    static void deleteCity(std::vector<City>& cities) {
        std::cout << "Enter the name of the city to delete: ";
        std::string cityName;
        std::getline(std::cin, cityName);

        std::vector<City>::iterator it;
        cityNameIterator( cities, cityName, it);

        if (it != cities.end()) {
            cities.erase(it);
            std::cout << "City '" << cityName << "' has been deleted successfully.\n";
        } else {
            std::cout << "Error: City '" << cityName << "' not found.\n";
        }
    }
    //  Helper to iterate through city names uses lambda function with capture clause,
    static bool cityNameIterator (std::vector<City>& cities, std::string &cityName, std::vector<City>::iterator& it) {
        it = std::find_if(cities.begin(), cities.end(),[&cityName](const City& city) {
            return city.name == cityName;   //  method of lambda func
        });
        return it != cities.end();
    }

    // Update a city's details
    static void updateCity(std::vector<City>& cities) {
        std::cout << "Enter the name of the city to update: ";
        std::string cityName;
        std::getline(std::cin, cityName);

        std::vector<City>::iterator iter;
        cityNameIterator( cities, cityName, iter);

        if (iter != cities.end()) {
            std::string field;
            std::cout << "Enter the field to update (name, country, population, recordYear, latitude, longitude, mayorName, mayorAddress, history): ";
            std::getline(std::cin, field);

            if (field == "name" || field == "country" || field == "mayorName" || field == "mayorAddress" || field == "history") {
                std::cout << "Enter the new value: ";
                std::string value;
                std::getline(std::cin, value);
                iter->update(field, value);
            } else if (field == "population" || field == "recordYear") {
                std::cout << "Enter the new value: ";
                int value;
                std::cin >> value;
                std::cin.ignore();
                iter->update(field, value);
            } else if (field == "latitude" || field == "longitude") {
                std::cout << "Enter the new value: ";
                double value;
                std::cin >> value;
                std::cin.ignore();
                iter->update(field, value);
            } else {
                std::cout << "Invalid field name.\n";
            }
        } else {
            std::cout << "Error: City '" << cityName << "' not found.\n";
        }
    }

    // Display all cities
    // Display all cities or a specific field
    static void displayCities(const std::vector<City>& cities) {
        if (cities.empty()) {
            std::cout << "No cities to display.\n";
            return;
        }

        std::cout << "Enter the field to display (name, country, population, recordYear, latitude, longitude, mayorName, mayorAddress, history): [Enter Blank For ALL] ";
        std::string field;
        std::getline(std::cin, field);

        if (field.empty()) {
            // Display all details if no field is specified
            for (const auto& city : cities) {
                city.display();
                std::cout << "\n";
            }
        } else {
            // Display specific field for all cities
            for (const auto& city : cities) {
                if (field == "name") {
                    std::cout << "City Name: " << city.name << "\n";
                } else if (field == "country") {
                    std::cout << "Country: " << city.country << "\n";
                } else if (field == "population") {
                    std::cout << "Population: " << city.population << "\n";
                } else if (field == "recordYear") {
                    std::cout << "Record Year: " << city.recordYear << "\n";
                } else if (field == "latitude") {
                    std::cout << "Latitude: " << city.latitude << "\n";
                } else if (field == "longitude") {
                    std::cout << "Longitude: " << city.longitude << "\n";
                } else if (field == "mayorName") {
                    std::cout << "Mayor Name: " << city.mayorName << "\n";
                } else if (field == "mayorAddress") {
                    std::cout << "Mayor Address: " << city.mayorAddress << "\n";
                } else if (field == "history") {
                    std::cout << "History: " << city.history << "\n";
                } else {
                    std::cout << "Invalid field name. Please try again.\n";
                    return; // Exit early if the field is invalid
                }
            }
        }
    }

    static void distance(std::vector<City>& cities) {
        if (cities.empty()) {
            std::cout << "No cities to calculate the distance between.\n";
            return;
        }

        std::cout << "Enter the first city: ";
        std::string cityA;
        std::getline(std::cin, cityA);

        std::cout << "Enter the second city: ";
        std::string cityB;
        std::getline(std::cin, cityB);

        std::vector<City>::iterator iterA, iterB;

        // Find the first city
        if (!cityNameIterator(cities, cityA, iterA)) {
            std::cout << "City '" << cityA << "' not found.\n";
            return;
        }

        // Find the second city
        if (!cityNameIterator(cities, cityB, iterB)) {
            std::cout << "City '" << cityB << "' not found.\n";
            return;
        }

        // Calculate the distance
        double distance = DistanceCalculator::calculateDistance(*iterA, *iterB);
        std::cout << "The distance between " << cityA << " and " << cityB << " is " << distance << " kilometers.\n";
    }

    //  Save functionality
    static void saveToFile(const std::vector<City>& cities) {
        std::cout << "Enter the file name to save the data: ";
        std::string fileName;
        std::getline(std::cin, fileName);

        FileManager::saveData(cities, fileName);
        std::cout << "Data successfully saved to " << fileName << ".\n";
    }
};

int main() {
    std::vector<City> cities;
    UserInterface::start(cities);
    return 0;
}

