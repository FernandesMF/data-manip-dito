#include "nlohmann/json.hpp"

//TODO take these out later
#include <iostream>
#include <iomanip>
// std::cout << std::setw(4) << Events << std::endl;

#pragma once

using json = nlohmann::json;

json Timeliner(json &);
void GroupByTransID(json &);

void Timeliner(json &BuyData){

    json TimelineUnord = NULL;
    json Timeline = NULL;

    // get group of events with the same trans. id as the first
    // transfer it/them to Timeline (insert at the right place, remove from BuyData)
    // repeat until BuyData is empty


    GroupByTransID(BuyData);

    // order timeline

    Timeline = BuyData;      // FIXME this is just dummy code;

    return;
}

void GroupByTransID(json &Data){


    return;
}