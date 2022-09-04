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
#include "plot2d.h"
#include "cStarterGUI.h"

class cJob
{
public:
    cJob(const std::string &line);
    void DisplayText();

    std::string myName;
    std::chrono::seconds myStart;
    std::chrono::seconds myStop;
    std::chrono::duration<int> myDuration;
};

cJob::cJob(const std::string &line)
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

struct sPlotData
{
    std::vector<std::vector<double>> trace;
    int min;
    int max;
    double scale;
    double timeStartHours;
};

class cSolution
{
public:
    void read();

    std::vector<std::vector<cJob>> myJob;

    void addJob(const cJob &job);

    void DisplayText();

    sPlotData plotData();

private:
    void range(int &min, int &max);
};

sPlotData cSolution::plotData()
{
    sPlotData ret;
    range(ret.min, ret.max);
    int delta = (ret.max - ret.min) / 100;
    ret.timeStartHours = ret.min / ( 60. * 60. );
    ret.scale = delta / ( 60. * 60. );
    double onValue = 30;
    for (auto &vjob : myJob)
    {
        std::vector<double> vd;

        for (
            int t = ret.min;
            t < ret.max + delta;
            t += delta)
        {
            double v = 0;
            for (auto &job : vjob)
            {
                if (job.myStart.count() <= t && t <= job.myStop.count())
                {
                    v = onValue;
                    break;
                }
            }

            vd.push_back(v);
        }
        std::cout << "\n";
        ret.trace.push_back(vd);
        onValue -= 10;
    }
    return ret;
}

void cSolution::range(int &min, int &max)
{
    min = 24 * 60 * 60;
    max = 0;
    for (auto &vjob : myJob)
    {
        for (auto &job : vjob)
        {
            if (job.myStart.count() < min)
                min = job.myStart.count();
            if (job.myStop.count() > max)
                max = job.myStop.count();
        }
    }
}

void cSolution::DisplayText()
{
    for (auto &vjob : myJob)
    {
        std::cout << vjob[0].myName << ": ";
        for (auto &job : vjob)
        {
            job.DisplayText();
        }
        std::cout << "\n";
    }
}

void cSolution::addJob(const cJob &job)
{
    for (auto &vjob : myJob)
    {
        if (job.myName == vjob[0].myName)
        {
            vjob.push_back(job);
            return;
        }
    }
    std::vector<cJob> vjob;
    vjob.push_back(job);
    myJob.push_back(vjob);
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
        cJob job(line);
        addJob(job);
    }
}
class cGUI : public cStarterGUI
{
public:
    cGUI()
        : cStarterGUI(
              "Starter",
              {50, 50, 1000, 500}),
          myPlot(wex::maker::make<wex::plot::plot>(fm))
    {
        myPlot.move({300, 30, 500, 600});

        cSolution S;
        S.read();
        S.DisplayText();
        auto pd = S.plotData();

        myPlot.XValues(
            pd.timeStartHours,
            pd.scale );
        wex::plot::trace &t1 = myPlot.AddStaticTrace();
        wex::plot::trace &t2 = myPlot.AddStaticTrace();
        wex::plot::trace &t3 = myPlot.AddStaticTrace();
        t1.thick(6);
        t2.thick(6);
        t3.thick(6);
        t1.color(0x0000FF);
        t2.color(0xFF0000);

        t1.set(pd.trace[0]);
        t2.set(pd.trace[1]);
        t3.set(pd.trace[2]);

        show();
        run();
    }

private:
    wex::plot::plot &myPlot;
};
main()
{

    cGUI theGUI;

    return 0;
}
