#ifndef GLIDESCOPE_VIEW_H
#define GLIDESCOPE_VIEW_H

#include <imgui.h>
#include <string>

#include "state.h"

class GView {
protected:
    char* name;
public:
    bool enabled;
    GView(const std::string name) : name(strdup(name.c_str())), enabled(true) {}
    ~GView();
    virtual void init(const ImGuiID* dockspace) {}
    virtual bool draw(GlobalState* state) {
        return enabled;
    }
    char* getName() const { return name; }
    
};

#endif // GLIDESCOPE_VIEW_H