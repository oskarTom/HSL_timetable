#include <cpr/cpr.h>
#include <cstdlib>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <stdlib.h>

//TODO: Add error message for when can't connect to API

std::string _api_key;

void loadConfig()
{
    std::ifstream config_stream{std::string(std::getenv("HOME"))+"/.config/hsltt/hsltt.conf"};
    if (!config_stream)
    {
        throw std::runtime_error("Could not find a config file!");
    }

    for (std::string line; std::getline(config_stream, line); ) {
        std::istringstream iss(line);
        std::string id, eq, val;

        if (!(iss >> id)) {
            throw std::runtime_error("Failed to read the config file!");
        } else if (id[0] == '#') {
            continue;
        } else if (!(iss >> eq >> val || eq != "=" || iss.get() != EOF )) {
            throw std::runtime_error("Failed to read a line from config file");
        } else if (id == "api-key") {
            _api_key = val;
        }

    }
}

cpr::Response requestStops()
{
    std::string query = R"(
    {
        stops(name: "Kamppi") {
            gtfsId
            name
            stoptimesWithoutPatterns {
                scheduledArrival
                realtimeArrival
                arrivalDelay
                scheduledDeparture
                realtimeDeparture
                departureDelay
                realtime
                realtimeState
                serviceDay
                headsign
                trip {
                    route {
                        shortName
                        longName
                        mode
                    }
                }
            }
        }
    }
    )";

    nlohmann::json request_body = {
        {"query", query}
    };

    cpr::Response r = cpr::Post(
        cpr::Url{"https://api.digitransit.fi/routing/v2/hsl/gtfs/v1"},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Header{{"digitransit-subscription-key", _api_key}},
        cpr::Body{request_body.dump()}
    );
    return r;
}

void printRoutes(nlohmann::json stops, std::string name)
{
    std::cout << std::endl;
    for (auto stop: stops) {
        // if ( std::string(stop["name"]).compare(name) ) continue;
        nlohmann::json routes = stop["stoptimesWithoutPatterns"];
        for (auto route: routes)  {
            std::string color_code = "0";
            if ( route["trip"]["route"]["mode"] == "SUBWAY" ) color_code = "33";
            else if ( route["trip"]["route"]["mode"] == "BUS" ) color_code = "34";
            else if ( route["trip"]["route"]["mode"] == "RAIL" ) color_code = "35";
            else if ( route["trip"]["route"]["mode"] == "TRAM" ) color_code = "36";
            // else std::cout << route["trip"]["route"]["mode"] ;
            std::cout << std::setw(2) << std::setfill(' ') << ((int)route["scheduledDeparture"]/60/60)%24 ;
            std::cout <<":"<< std::setw(2) << std::setfill('0') << (int)route["scheduledDeparture"]%3600/60 ;
            std::cout << "\033[1;" << color_code << "m";
            std::cout << "\t" << std::setw(4) << std::setfill(' ') << (std::string)route["trip"]["route"]["shortName"] ;
            std::cout << " " << (std::string)route["headsign"] << "\033[m" << std::endl;
        } 
        std::cout << std::endl;
    } 
}

int main(int argc, char *argv[])
{
    try {
        loadConfig();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    cpr::Response r = requestStops();

    if (r.status_code == 200) {
        nlohmann::json json_response = nlohmann::json::parse(r.text);
        nlohmann::json routes = json_response["data"]["stops"];
        printRoutes(routes, "Maunula");
    } else {
        std::cerr << "Failed! Status code: " << r.status_code << std::endl;
    }

    return 0;
}

