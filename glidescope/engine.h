#ifndef GLIDESCOPE_ENGINE_H
#define GLIDESCOPE_ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui.h>
#include <implot.h>
#include <imgui_internal.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <tinyfiledialogs/tinyfiledialogs.h>
#include <glog/logging.h>
#include <map>

#include "vrptw.h"
#include "node.h"
#include "fleet.h"
#include "request.h"
#include "solution.h"
#include "loaders/vrptw_loader.h"
#include "loaders/efvrptw_loader.h"
#include "loaders/soln_loader.h"

#include "view.h"
#include "state.h"
#include "utils.h"
#include "graphics.h"

#define ASSERT(_e, ...)               \
    if (!(_e)) {                      \
        fprintf(stderr, __VA_ARGS__); \
        exit(1);                      \
    }

class GEngine {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    ImGuiIO* io;
    TTF_Font* font;
    std::shared_ptr<GlobalState> state;
    std::vector<GView*> views;
    int width, height;
    bool will_exit;
    bool performance_window = true;
    bool demo_window = false;
    bool grid_on = true;
    bool setup_route_on = false;
    int grid_step = 5;
    int color_mode = 3;
    ImVec2 viewport_pos, viewport_size;
    std::map<int, ImVec2> node_positions;
    ImDrawList* viewport_draw_list;
    ImVec2 toScreenCoords(float x, float y, std::pair<ImVec2, ImVec2> bounds);
    void loop();
    void processEvent(SDL_Event& event);
    void handleViewportClick(uint8_t button, int x, int y);
    void recalculateViewport();
    void setupUI();
    void renderUI();
    void teardown();
    float getScale() const;
    void setColorMode(int mode);
public:
    GEngine(int width, int height);
    void registerView(GView* view);
    void start();
    void reset(char* path = nullptr);
    ~GEngine();
};

#endif // GLIDESCOPE_ENGINE_H