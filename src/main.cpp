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
    void DisplayText();

    std::string myName;
    std::chrono::seconds myStart;
    std::chrono::seconds myStop;
    std::chrono::duration<int> myDuration;
};

cJob::cJob( const std::string& line )
{
    myName = line.substr(2, 1);
    std::stringstream ss(line.substr(5, 8));
    ss >> date::parse("%H:%M:%S", myStart);
    
     std::stringstream ss2(line.substr(19, 8));
     ss2 >> date::parse("%H:%M:%S", myStop);

    myDuration = myStop - myStart;

     std::cout << "Duration secs " << myDuration.count() << "\n";
}

void cJob::DisplayText()
{
    std::cout << myStart.count() << " to " << myStop.count() << ", ";
}

class cSolution
{
public:
    void read();

    std::vector< std::vector< cJob > > myJob;

    void addJob( const cJob& job );

    void DisplayText();
};

void cSolution::DisplayText()
{
    for( auto& vjob : myJob )
    {
        std::cout << vjob[0].myName << ": ";
        for ( auto& job : vjob )
        {
            job.DisplayText();
        }
        std::cout << "\n";
    }
}

 void cSolution::addJob( const cJob& job )
 {
    for( auto& vjob : myJob )
    {
        if( job.myName == vjob[0].myName )
            {
                vjob.push_back( job );
                return;
            }
    }
    std::vector< cJob > vjob;
    vjob.push_back( job );
    myJob.push_back( vjob );
 }

void cSolution::read()
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
        addJob( job );
    }
}
main()
{
    cSolution S;
    S.read();
    S.DisplayText();
    return 0;
}
