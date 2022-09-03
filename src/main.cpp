#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <chrono>
#include "date.h"
#include <wex.h>
#include "cStarterGUI.h"

class cJob {
    public:
    cJob( const std::string& line );
    private:
    std::string myName;
    std::chrono::time_point <std::chrono::system_clock> myStart;
    std::chrono::time_point <std::chrono::system_clock> myStop;
};

cJob::cJob( const std::string& line )
{
    typedef std::chrono::duration<int,std::milli> milli_t;

    myName = line.substr(2, 1);
    std::tm tm = {};
    std::stringstream ss(line.substr(5, 8));
    std::cout << ss.str() << "\n";
    ss >> std::get_time(&tm, "%H:%M:%S");
    std::cout << tm.tm_hour << " hours and " << tm.tm_min << " minutes\n";
    myStart = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    std::cout << date::format("%m/%d/%Y %I:%M:%S %p\n", myStart ) << "\n";
    
    //myStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::from_time_t(std::mktime(&tm)));
    std::stringstream ss2(line.substr(19, 8));
    std::cout << ss2.str() << "\n";
    ss >> std::get_time(&tm, "%H:%M:%S");
    myStop = std::chrono::system_clock::from_time_t(std::mktime(&tm));
   std::cout << date::format("%m/%d/%Y %I:%M:%S %p\n", myStop ) << "\n";

    //milli_t d = std::chrono::duration_cast<std::chrono::seconds>( myStop - myStart );

    // std::cout << std::chrono::system_clock::to_time_t(myStart).tm_hour << " | " << std::chrono::system_clock::to_time_t( myStop ) << "\n";
    // std::cout << ( myStop - myStart ).count() << "\n";
}

void read()
{
    std::ifstream f("input.txt");
    if (!f.is_open())
    {
        throw std::runtime_error(
            "Cannot open input");
    }
    std::string line;
    std::string jobname;
    while (getline(f, line))
    {
        std::cout << line << "\n";
        cJob job( line );

        std::istringstream iss(line);
        jobname = line.substr(2, 1);

        std::tm tm = {};
        std::stringstream ss(line.substr(5, 12));
        ss >> std::get_time(&tm, "%H:%M:%S");
        auto start = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        std::stringstream ss2(line.substr(19, 12));
        auto stop = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }
}
main()
{
    std::string sbaseday("2022-09-02 ");
    std::chrono::hours timezone(5);
    
    std::stringstream ss(sbaseday + "09:45:30");

    std::tm tm = {};
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    auto myStart = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    myStart -= timezone;
    std::cout << date::format("%I:%M:%S\n", myStart ) << "\n";

    std::chrono::seconds secs;
    ss >> date::parse("%H:%M:%S", secs);
    std::cout << "hours past midnight " << secs.count() / (60.0*60.0) << "\n";

    //read();
    return 0;
}
