#include <iostream>
#include <cmath>
#include <bits/stl_algo.h>
#include <vector>
#include <fstream>
#include <sstream>

class City {

    //  Represent all city details, name , country , history, mayorName, mayorAddress, population
    //  , year population recorded.
    public:
        //  Attributes
        std::string name, country, history, mayorName, mayorAddress;
        int population, recordYear;
        double latitude, longitude;

        //  Constructor and Destructor

        City ()
            //  Default to be overloaded
            : name (""), country (""), history (""), mayorName (""),
            mayorAddress(""), population(0), recordYear(0),
            latitude(0.0), longitude(0.0) {}

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
        double lat1 = city1.latitude * M_PI / 180.0;
        double lon1 = city1.longitude * M_PI / 180.0;
        double lat2 = city2.latitude * M_PI / 180.0;
        double lon2 = city2.longitude * M_PI / 180.0;

        //  find cos(D) angular distance sin(phi1)*sin(phi2) + cos(phi1)*cos(phi2)*cos(L1 - L2)
        double cosD = sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lon1 - lon2);

        //  cosD validation range = [-1, 1]
        //  Note clamp restrains value to -1 to 1 so that trig works and no domain error.
        cosD = std::clamp(cosD, -1.0, 1.0);

        //  Use acos to find d
        double d = acos(cosD);

        //  Convert angular distance to linear distance.
        double distance = d * EARTH_RADIUS_KM;

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

int main() {
    //  File Creation
    const std::string fileName = "cities.txt";

    // Load cities from file
    std::vector<City> cities = FileManager::loadData(fileName);

    //  Test Hardcoded City
    cities.emplace_back("New York", "USA", 8419600, 2021, 40.7128, -74.0060, "Eric Adams", "City Hall, NYC", "Founded in 1624.");
    cities.emplace_back("London", "UK", 8982000, 2021, 51.5074, -0.1278, "Sadiq Khan", "City Hall, London", "Founded by Romans.");

    // Display loaded cities
    std::cout << "Loaded Cities:\n";
    for (const auto& city : cities) {
        city.display();
        std::cout << "\n";
    }

    // Save updated cities back to file
    FileManager::saveData(cities, fileName);

    std::cout << "Data saved successfully to " << fileName << ".\n";

    return 0;



}

