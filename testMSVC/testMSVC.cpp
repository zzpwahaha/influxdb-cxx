// testMSVC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <InfluxDB.h>
#include <InfluxDBFactory.h>
#include <ctime>
#include <string>

int main()
{
    // Available over HTTP only
    //auto influxdb = influxdb::InfluxDBFactory::Get("http://admin:Twizzler@6.1.1.77:8086?db=home");
    /// Pass an IFQL to get list of points
    //std::vector<influxdb::Point> points = influxdb->query("SELECT \"Cold Shield\" from ColdEdge order by time desc limit 1");
    //std::vector<influxdb::Point> points = influxdb->query("SELECT \"Cold finger\" from ColdEdge order by time desc limit 1");
    //std::vector<influxdb::Point> points = influxdb->query("SELECT \"HC-4 outlet\" from \"6_month\".\"Stinger\" order by time desc limit 1");

    auto influxdb = influxdb::InfluxDBFactory::Get("http://grafana:Twizzler@6.1.1.93:8086?db=monitoring");
    std::vector<influxdb::Point> points = influxdb->query("SELECT \"pressure\" from \"B240_main_chamber\" order by time desc limit 1");
    std::time_t t = std::chrono::system_clock::to_time_t(points[0].getTimestamp());
    std::chrono::time_point<std::chrono::system_clock> tt = points[0].getTimestamp();
    auto a = std::chrono::duration_cast<std::chrono::seconds>(tt.time_since_epoch()).count();
    char tmBuff[50];
    ctime_s(tmBuff, sizeof(tmBuff), &t);
    std::string ss(tmBuff);
    std::string temperature = points[0].getFields();
    temperature = temperature.substr(temperature.find("=") + 1, temperature.size());
    std::cout << ss << temperature;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
