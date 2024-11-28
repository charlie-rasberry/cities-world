#include <iostream>
#include <cmath>
#include <bits/stl_algo.h>

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

        //  Convert angular distance to linear distance, where 6371.0 is radius of Earth in km.
        double distance = d * 6371.0;

        return distance;

    }
};

int main() {
    //  Test Hardcoded City
    City myCity("New York", "USA", 8419600, 2021, 40.7128, -74.0060, "Eric Adams", "City Hall, NYC", "Founded in 1624.");
    City anCity("London", "UK", 8982000, 2021, 51.5074, -0.1278, "Sadiq Khan", "City Hall, London", "Founded by Romans.");
    std::cout << "My City:\n";
    myCity.display();
    std::cout << "\n";
    anCity.display();
    std::cout << "\n";

    double distance = DistanceCalculator::calculateDistance(myCity, anCity);
    std::cout << "Distance: " << distance << "\n";

    return 0;



}

