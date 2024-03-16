#include <glog/logging.h>

#include "engine.h"
#include "views/nodes.h"
#include "views/fleet.h"
#include "views/solution.h"
#include "views/inspector.h"
#include "views/schedule.h"

int main(int argc, char* argv[]){
    google::InitGoogleLogging(argv[0]);
    google::SetStderrLogging(google::GLOG_INFO);
    auto* engine = new GEngine(1280, 720);
    engine->registerView(new GNodesView("Nodes"));
    engine->registerView(new GFleetView("Fleet"));
    engine->registerView(new GSolutionView("Solution"));
    engine->registerView(new GScheduleView("Schedule"));
    engine->registerView(new GInspectorView("Inspector"));
    if (argc > 1) {
        engine->reset(argv[1]);
    } else {
        engine->reset();
    }
    engine->start();
    return 0;
}