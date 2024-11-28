#include <iostream>
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

int main() {
    City defaultCity;
    std::cout << "Default City:\n";
    defaultCity.display();
    std::cout << "\n";

    //  Test Hardcoded City
    City myCity("New York", "USA", 8419600, 2021, 40.7128, -74.0060, "Eric Adams", "City Hall, NYC", "Founded in 1624.");
    std::cout << "My City:\n";
    myCity.display();
    std::cout << "\n";

    //  Test Update
    /*
    myCity.update("population", 8623000); // Updating population
    myCity.update("mayorName", "New Mayor"); // Updating mayor name

    std::cout << "Updated City:\n";
    myCity.display();
    */

    return 0;



}

