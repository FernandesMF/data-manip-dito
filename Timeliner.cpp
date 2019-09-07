//#include "Timeliner.h"

#include "nlohmann/json.hpp"

#pragma once

using json = nlohmann::json;

json Timeliner(json);

json Timeliner(json Events){

    json TimelineUnord = NULL;
    json Timeline = NULL;

    // group events by transaction if; one item per buy 
    // order timeline

    Timeline = Events;      // FIXME thi is just dummy code;

    return Timeline;
}