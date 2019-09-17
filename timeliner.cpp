#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// TODO main: use terminal arguments for input and output
#define INPUT_FILE "events.json"
#define OUTPUT_FILE "timeline.json"
#define OUT     // useful output reminder



json ReadJson(std::string);
void WriteJson(std::string, json);
void PrintJson(json);

void InitializeTimeline(json &);;
void Timeliner(json &, json &);
int GroupByTransID(json &, int &);
int FindTransIDIndex(json &, int);
int FindInsertIndex(json &, std::string);
void MoveBuyInfo(json &, int, json &, int);
void MoveProductInfo(json &, int, json &, int, int &);

// Entry point of the application
int main(){

    json Events;
    json Timeline;
    json &EventsRef = Events;
    json &TimelineRef = Timeline;

    Events = ReadJson(INPUT_FILE);
    std::cout << "main: Events before Timeliner call:\n";
    PrintJson(Events);
    Timeliner(EventsRef,TimelineRef);
    std::cout << "main: timeline after Timeliner call:\n";
    PrintJson(Timeline);
    std::cout << "main: Events after timeliner call:\n";
    PrintJson(Events);
    WriteJson(OUTPUT_FILE, Timeline);

    return 0;
}



// Prints a Json object in a nice format
void PrintJson(json JsonData){
    std::cout << std::setw(4) << JsonData << std::endl;
}

// Reads a Json file turns it into a json variable
// TODO json reader: file opening/ existence checks
json ReadJson(std::string FileName){
    json JsonData = NULL;
    std::ifstream EventsFile(FileName, std::ifstream::binary);
    EventsFile >> JsonData;

    return JsonData;
}

// Writes a Json file from a a json variable
//TODO json writer: file opening checks
void WriteJson(std::string FileName,json JsonData){
    std::ofstream TimelineFile;
    TimelineFile.open(OUTPUT_FILE);
    TimelineFile << JsonData;
    TimelineFile.close();

    return;
}


// 
void Timeliner(json &BuyData, json &Timeline){
    { // begin scope of variables; they will be "decomissioned" before the end of this recursion
        int BuyEventIdx = -1;       // will hold the index of the buy event in the transaction_id group
        int &BuyEventIdxRef = BuyEventIdx;
        int FinalEventIndex = 0;    // will hold the index of the last event in the transaction_id group
        int InsertIndex = -1;       // will hold the index where insertion in the timeline should be made
        int ProductIndex = 0;       // will iterate through product events in the transaction_id group
        int &ProductIndexRef = ProductIndex;

        FinalEventIndex = GroupByTransID(BuyData, OUT BuyEventIdx);
        InsertIndex = FindInsertIndex(Timeline,BuyData["events"][BuyEventIdx]["timestamp"].get<std::string>());
        if(Timeline["timeline"].size()>0){
            json nothing = nullptr;
            Timeline["timeline"].insert(Timeline["timeline"].begin()+InsertIndex,nothing); // insert a new entry in timeline
        }

        MoveBuyInfo(BuyData,BuyEventIdx,Timeline,InsertIndex);
        for(int r = FinalEventIndex-1; r >= 0; r--){
            // remember the buy event was removed in the last step;
            MoveProductInfo(BuyData,r,Timeline,InsertIndex,ProductIndexRef);
        }
    } // end of variables scope
    
    // recursion: if BuyData is not empty yet, call Timeliner again
    if(BuyData["events"].size()>0){
        Timeliner(BuyData,Timeline);
    }
    return;
}

// Finds the last event of a group with the same transaction_id; also returns the buy event
// of that group via reference
// FIXME the code is assuming that events of the same transaction are neighbors;
int GroupByTransID(json &BuyData, OUT int& BuyEventIdx){
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
    } while ( EventIdx < BuyData["events"].size()-1 && strcmp( TransID.c_str(),TestID.c_str() )==0);    
    
    // reduce EventIdx if the do-while block was not stopped by the end of BuyData being reached
    if(EventIdx < BuyData["events"].size()-1 ){
        EventIdx--;
    }
    return EventIdx;
}

// Search which key is the transaction ID in "custom_data"
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
    return r;
}

// Finds the index to insert information in the timeline; this way it is built to be
// ordered by timestamp (most recent first)
int FindInsertIndex(json &Timeline, std::string Timestamp){
    int i = 0;
    std::string str = "";
    if(Timeline["timeline"].size() == 0){ return 0;}
    str = Timeline["timeline"][i]["timestamp"].get<std::string>();
    while ( strcmp(Timestamp.c_str(),str.c_str()) < 0  ){
        i++;
        str = Timeline["timeline"][i]["timestamp"].get<std::string>();
    }

    return i;
}

// Moves information from the buy event to the timeline (timestamp, revenue, store_name and trans._id)
void MoveBuyInfo(json &BuyData, int BuyEventIdx, json &Timeline, int InsertIndex){ 
    std::string key = "";
    // add timestamp and revenue to Timeline
    Timeline["timeline"][InsertIndex]["timestamp"] = BuyData["events"][BuyEventIdx]["timestamp"];
    Timeline["timeline"][InsertIndex]["revenue"] = BuyData["events"][BuyEventIdx]["revenue"];
    // add custom data to Timeline (cycle through custom data), store_name and trans._id
    for(int r=0; r<2; r++){
        key = BuyData["events"][BuyEventIdx]["custom_data"][r]["key"];
        Timeline["timeline"][InsertIndex][key] = BuyData["events"][BuyEventIdx]["custom_data"][r]["value"];
    }
    // remove that info from BuyData (perhaps just delete the buy element?)
    BuyData["events"].erase(BuyEventIdx);
    
    return;
}

// Moves information from product events to the "products" field in the timeline
void MoveProductInfo(json &BuyData, int EventIdx, json &Timeline, int InsertIndex, int &ProductIndex){ 
    std::string key = "";
    int tid_flag = 0;
    // go through custom data of r-th event
    for(int s=0; s < BuyData["events"][EventIdx]["custom_data"].size() ; s++){
        // ignore transaction_id
        key = BuyData["events"][EventIdx]["custom_data"][s]["key"].get<std::string>();
        tid_flag = strcmp("transaction_id",key.c_str());
        if(tid_flag!=0){
            // add key and value (product_price or product_name) to Timeline
            Timeline["timeline"][InsertIndex]["products"][ProductIndex][key] = BuyData["events"][EventIdx]["custom_data"][s]["value"];
        }                  
    }
    ProductIndex++;
    // remove event from BuyData
    BuyData["events"].erase(EventIdx);
    return;
}