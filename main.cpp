#include <iostream>
#include <iomanip>
#include <fstream>
#include "Timeliner.h"

#define events_file "events.json"

json ReadJson(std::string);

int main(){

    json Events = NULL;

    Events = ReadJson(events_file);

    std::cout << std::setw(4) << Events<< std::endl;

    // transform teh events into a timeline
    // write timeline to external file
    // print timeline to screen?

    return 0;
}

// read events file, store in json variable Events
json ReadJson(std::string FileName){
    json Events = NULL;
    std::ifstream EventsFile(FileName, std::ifstream::binary);
    EventsFile >> Events;

    return Events;
}