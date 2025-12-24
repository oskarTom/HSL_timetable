#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

//TODO: Add error message for when can't connect to API

using namespace std;


void loadConfig()
{
    std::ifstream config_stream{"hsltt.conf"};
}

cpr::Response requestStops()
{
    string query = R"(
    {
        stops(name: "Maunula") {
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
        cpr::Header{{"digitransit-subscription-key", API_KEY}},
        cpr::Body{request_body.dump()}
    );
    return r;
}

cpr::Response sendRequest()
{
    string query = R"(
        {
        stop(id: "HSL:1282104") {
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
        cpr::Header{{"digitransit-subscription-key", API_KEY}},
        cpr::Body{request_body.dump()}
    );
    return r;
}

void printRoutes(nlohmann::json routes)
{
    for (auto route: routes) {
        std::cout << std::setw(2) << std::setfill(' ') 
                    << (int)route["scheduledDeparture"]/60/60 ;
        std::cout <<":"<< std::setw(2) << std::setfill('0')
                    << (int)route["scheduledDeparture"]%3600/60 ;
        std::cout << "\t" << (string)route["trip"]["route"]["shortName"] << std::endl;
    } 
}

void printRegionalRoutes(nlohmann::json stops, string name)
{
    std::cout << std::endl;
    for (auto stop: stops) {
        if ( string(stop["name"]).compare(name) ) continue;
        nlohmann::json routes = stop["stoptimesWithoutPatterns"];
        for (auto route: routes)  {
        std::cout << std::setw(2) << std::setfill(' ') 
                    << ((int)route["scheduledDeparture"]/60/60)%24 ;
        std::cout <<":"<< std::setw(2) << std::setfill('0')
                    << (int)route["scheduledDeparture"]%3600/60 ;
        std::cout << "\t" << std::setw(4) << std::setfill(' ') << (string)route["trip"]["route"]["shortName"] ;
        std::cout << " " << (string)route["headsign"] << std::endl;
        } 
        std::cout << std::endl;
    } 
}

int main(int argc, char *argv[])
{
    cpr::Response r = requestStops();

    if (r.status_code == 200) {
        nlohmann::json json_response = nlohmann::json::parse(r.text);
        // nlohmann::json routes = json_response["data"]["stop"]["stoptimesWithoutPatterns"];
        nlohmann::json routes = json_response["data"]["stops"];
        // printRoutes(routes);
        printRegionalRoutes(routes, "Maunula");
        // cout << "Response:\n" << json_response.dump(2) << endl;
    } else {
        cerr << "Failed! Status code: " << r.status_code << endl;
    }

    return 0;
}

