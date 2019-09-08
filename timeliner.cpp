#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// TODO main: use terminal arguments for input and output
#define INPUT_FILE "events.json"
#define OUTPUT_FILE "timeline.json"



json ReadJson(std::string);
void WriteJson(std::string, json);
void PrintJson(json);

void Timeliner(json &, json &);
void GroupByTransID(json &, std::vector<int> &);
int FindTransIDIndex(json &, int);



int main(){

    json Events = NULL;
    json Timeline = NULL;
    json &EventsRef = Events;
    json &TimelineRef = Timeline;

    Events = ReadJson(INPUT_FILE);
    PrintJson(Events);

    ///
    std::cout << "-------------------------" << std::endl;
    PrintJson(Events["events"][0]);
    PrintJson(Events["events"][0]["custom_data"][1]["key"]);
    std::cout << "is transaction_id: ";
    std::cout << strcmp(Events["events"][0]["custom_data"][1]["key"].get<std::string>().c_str(),"transaction_id");
    std::cout << std::endl;
    std::cout << "custom data length: " << Events["events"][0]["custom_data"].size() << std::endl;
    PrintJson(Events["events"][0]["custom_data"][1]["value"]);
    PrintJson(Events["events"][0]["timestamp"]);
    std::cout << "-------------------------" << std::endl;
    ///
    
    Timeliner(EventsRef,TimelineRef);
    //PrintJson(Timeline);
    WriteJson(OUTPUT_FILE, Timeline);

    return 0;
}



void PrintJson(json JsonData){
    std::cout << std::setw(4) << JsonData << std::endl;
}

// TODO json reader: file opening/ existence checks
json ReadJson(std::string FileName){
    json JsonData = NULL;
    std::ifstream EventsFile(FileName, std::ifstream::binary);
    EventsFile >> JsonData;

    return JsonData;
}

//TODO json writer: file opening checks
void WriteJson(std::string FileName,json JsonData){
    std::ofstream TimelineFile;
    TimelineFile.open(OUTPUT_FILE);
    TimelineFile << JsonData;
    TimelineFile.close();

    return;
}



void Timeliner(json &BuyData, json &Timeline){

    std::vector<int> IdIndexes;
    std::vector<int> &IdIndexesRef = IdIndexes;

    // get group of events with the same trans. id as the first
    GroupByTransID(BuyData, IdIndexesRef);

    // transfer it/them to Timeline (insert at the right place, remove from BuyData)
    // repeat until BuyData is empty (call recursively)
        // delete IdIndexes and ...Ref?
        // call Timeliner again from if statement

    Timeline = BuyData;      // FIXME this is just dummy code;

    return;
}

// ill assume that events of the same transaction are all neighbors...
void GroupByTransID(json &BuyData,std::vector<int> &IdIndexes){
    int EventIdx = 0;
    int i1 = 0;

    // find transaction index of the first item in BuyData
    i1 = FindTransIDIndex(BuyData,EventIdx);

    // keep checking transaction ids of the next events, until there is a different one
    // store indexes in IdIndexes

    return;
}

int FindTransIDIndex(json &BuyData, int EventIdx){
    int r = -1;
    int R = BuyData["events"][EventIdx]["custom_data"].size();
    std::string str;
    bool bStillLooking = true;
    while(r<R && bStillLooking){
        r++;
        str = BuyData["events"][EventIdx]["custom_data"][r]["key"].get<std::string>();
        if(strcmp("transaction_id",str.c_str())==0){
            bStillLooking = false;
        }
    }
    std::cout << "event " << EventIdx << ", transaction id " << r << std::endl;
    return r;
}