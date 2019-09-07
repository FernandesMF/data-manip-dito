#include <iostream>
#include <iomanip>
#include <fstream>
#include "Timeliner.cpp"

#define INPUT_FILE "events.json"
#define OUTPUT_FILE "timeline.json"

json ReadJson(std::string);
void WriteJson(std::string,json);
void PrintJson(json);

int main(){

    json Events = NULL;
    json Timeline = NULL;

    Events = ReadJson(INPUT_FILE);
    PrintJson(Events);
    Timeline = Timeliner(Events);       // TODO make dummy timeline function
    PrintJson(Timeline);
    WriteJson(OUTPUT_FILE, Timeline);

    return 0;
}

json ReadJson(std::string FileName){
    json JsonData = NULL;
    std::ifstream EventsFile(FileName, std::ifstream::binary);
    EventsFile >> JsonData;

    return JsonData;
}

void WriteJson(std::string FileName,json JsonData){
    //std::ofstream TimelineFile(FileName, std::ofstream::binary);
    //JsonData >> TimelineFile;
    std::ofstream TimelineFile;
    TimelineFile.open(OUTPUT_FILE);
    TimelineFile << JsonData;
    TimelineFile.close();

    return;
}

void PrintJson(json JsonData){
    std::cout << std::setw(4) << JsonData << std::endl;        // nice print
}