#include <iostream>
#include <iomanip>
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// TODO main: use terminal arguments for input and output
#define INPUT_FILE "events.json"
#define OUTPUT_FILE "timeline.json"



json ReadJson(std::string);
void WriteJson(std::string, json);
void PrintJson(json);

void InitializeTimeline(json &);;
void Timeliner(json &, json &);
int GroupByTransID(json &, int &);
int FindTransIDIndex(json &, int);
int FindInsertIndex(json &, std::string);



int main(){

    json Events;
    json Timeline;
    json &EventsRef = Events;
    json &TimelineRef = Timeline;

    Events = ReadJson(INPUT_FILE);
    PrintJson(Events);

    // FIXME remove these printing and prototypes
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
    //

    InitializeTimeline(TimelineRef);
    PrintJson(Timeline); //FIXME remove this print
    std::cout << "main timeline first item after initialization " << Timeline["timeline"] << std::endl; // FIXME remove this print
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



void InitializeTimeline(json &Timeline){
    Timeline["timeline"] = NULL;
    return;
}

void Timeliner(json &BuyData, json &Timeline){
    int BuyEventIdx = -1;
    int &BuyEventIdxRef = BuyEventIdx;
    int FinalEventIndex = 0;
    int InsertIndex = -1;

    // get group of events with the same trans. id as the first; get id of the buy event
    FinalEventIndex = GroupByTransID(BuyData, BuyEventIdx);
    std::cout << "timeliner FinalEventIndex " << FinalEventIndex << std::endl; // FIXME remove this print
    std::cout << "timeliner BuyEventIdx " << BuyEventIdx << std::endl; // FIXME remove this print

    // find index to insert events in timeline
    InsertIndex = FindInsertIndex(Timeline,BuyData["events"][BuyEventIdx]["timestamp"].get<std::string>());
    std::cout << "Timeliner FindInsertIndex " << InsertIndex << std::endl; // FIXME remove this print

    // transfer items from BuyData to Timeline
    //for(int r = 0; r <= FinalEventIndex; r++){}

    // repeat until BuyData is empty (call recursively)
        // delete IdIndexes and ...Ref?
        // call Timeliner again from if statement

    return;
}

// FIXME the code is assuming that events of the same transaction are neighbors;
int GroupByTransID(json &BuyData, int& BuyEventIdx){
    int EventIdx = 0;
    int IdIdx = -1;
    std::string TransID = "";
    std::string TestID = "";
    std::string EventValue = "";

    // find transaction index and id of the first item in BuyData
    IdIdx = FindTransIDIndex(BuyData,0);
    TransID = BuyData["events"][0]["custom_data"][IdIdx]["value"].get<std::string>();
    EventValue = BuyData["events"][0]["event"].get<std::string>();
    if(strcmp( "comprou",EventValue.c_str() )==0){
        BuyEventIdx = 0;
    }

    // keep checking transaction ids of the next events, until there is a different one
    do {
        EventIdx++;
        IdIdx = FindTransIDIndex(BuyData,EventIdx);
        TestID = BuyData["events"][EventIdx]["custom_data"][IdIdx]["value"].get<std::string>();
        EventValue = BuyData["events"][EventIdx]["event"].get<std::string>();
        if(strcmp( "comprou",EventValue.c_str() )==0){
            BuyEventIdx = EventIdx;
        }
    } while ( EventIdx < BuyData["events"].size() && strcmp( TransID.c_str(),TestID.c_str() )==0);    
    EventIdx--;

    return EventIdx;
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
    std::cout << "FindTransIDIndex event " << EventIdx << ", transaction id " << r << std::endl;    // FIXME remove this print
    return r;
}

int FindInsertIndex(json &Timeline, std::string Timestamp){
    int i = 0;
    std::string str = "";
    if(Timeline["timeline"] == 0){ return 0;}
    str = Timeline["timeline"][i]["timestamp"].get<std::string>();
    while ( strcmp(Timestamp.c_str(),str.c_str()) < 0  ){
        i++;
        str = Timeline["timeline"][i]["timestamp"].get<std::string>();
    }

    return i;
}