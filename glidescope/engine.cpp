#include "engine.h"

GEngine::GEngine(int width, int height) {
    this->width = width;
    this->height = height;
    this->state = nullptr;
    this->window = nullptr;
    this->renderer = nullptr;
    this->font = nullptr;
    this->reset(NULL);
}

GEngine::~GEngine() {
    this->teardown();
}

void GEngine::start() {
    ASSERT(!SDL_Init(SDL_INIT_VIDEO), "SDL_Init failed: %s", SDL_GetError());
    ASSERT(!TTF_Init(), "TTF_Init failed: %s", TTF_GetError());
    this->window =
        SDL_CreateWindow("VRPTK Explorer", SDL_WINDOWPOS_CENTERED_DISPLAY(0),
            SDL_WINDOWPOS_CENTERED_DISPLAY(0), this->width,
            this->height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    ASSERT(this->window, "SDL_CreateWindow failed: %s", SDL_GetError());
    this->renderer = SDL_CreateRenderer(
        this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    ASSERT(this->renderer, "SDL_CreateRenderer failed: %s", SDL_GetError());
    const float scale_x{ this->getScale() };
    SDL_RenderSetScale(this->renderer, scale_x, scale_x);
    this->font = TTF_OpenFont("assets/Vdj.ttf", 15);
    ASSERT(this->font, "TTF_OpenFont failed: %s", TTF_GetError());
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    this->io = &ImGui::GetIO();
    this->io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard
        | ImGuiConfigFlags_DockingEnable
        | ImGuiConfigFlags_ViewportsEnable;
    const float font_size{ 12.0F * scale_x };
    this->io->FontDefault = this->io->Fonts->AddFontFromFileTTF(
        "assets/Vdj.ttf",
        font_size
    );
    this->io->FontGlobalScale = 1.0F / scale_x;
    ImGui_ImplSDL2_InitForSDLRenderer(this->window, this->renderer);
    ImGui_ImplSDLRenderer2_Init(this->renderer);
    LOG(INFO) << "Engine started successfully";
    this->loop();
    this->teardown();
}

float GEngine::getScale() const {
    int window_width, window_height;
    SDL_GetWindowSize(this->window, &window_width, &window_height);
    int render_width, render_height;
    SDL_GetRendererOutputSize(this->renderer, &render_width, &render_height);
    return static_cast<float>(render_width) / window_width;
}

void GEngine::loop() {
    uint64_t last_time = SDL_GetTicks64();
    uint32_t frameCount = 0;
    float frameTime = 0;
    while (!this->will_exit) {
        frameCount++;
        uint64_t frameStart = SDL_GetTicks64();
        uint64_t startPerf = SDL_GetPerformanceCounter();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&event)) {
            this->processEvent(event);
        }
        uint64_t frameEnd = SDL_GetTicks64();
        uint64_t endPerf = SDL_GetPerformanceCounter();
        uint64_t framePerf = endPerf - startPerf;
        frameTime = frameEnd - frameStart;
        last_time = frameStart;
        this->renderUI();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }
}

void GEngine::processEvent(SDL_Event& event) {
    ImGui_ImplSDL2_ProcessEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        this->will_exit = true;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_q) {
            this->will_exit = true;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        float x = event.button.x;
        float y = event.button.y;
        if (x > this->viewport_pos.x && x < this->viewport_pos.x + this->viewport_size.x &&
            y > this->viewport_pos.y && y < this->viewport_pos.y + this->viewport_size.y) {
            this->handleViewportClick(event.button.button, x, y);
        }
        break;
    }
}

void GEngine::handleViewportClick(uint8_t button, int x, int y) {
    Eigen::MatrixXd nodes = this->state->vrp->getNodes();
    if (button == SDL_BUTTON_LEFT) {
        this->state->selected_node = closestNode(this->node_positions, x, y);
    }
}

void GEngine::renderUI() {
    static bool firstLoop = true;
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    this->node_positions.clear();
    ImGuiID dockspace_id = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
    if (firstLoop) {
        firstLoop = false;
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id);
        ImVec2 size = ImGui::GetMainViewport()->Size;
        size.y -= ImGui::GetFrameHeight();
        ImGui::DockBuilderSetNodeSize(dockspace_id, size);
        ImGui::DockBuilderSetNodePos(dockspace_id, ImVec2(0, ImGui::GetFrameHeight()));
        ImGuiID dock1 = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.8f, nullptr, &dockspace_id);
        ImGuiID dock2 = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.5f, nullptr, &dockspace_id);
        ImGuiID dock3 = ImGui::DockBuilderSplitNode(dock2, ImGuiDir_Down, 0.7f, nullptr, &dock2);
        ImGuiID dock4 = ImGui::DockBuilderSplitNode(dock1, ImGuiDir_Down, 0.2f, nullptr, &dock1);
        ImGui::DockBuilderDockWindow("Viewport", dock1);
        ImGui::DockBuilderDockWindow("Inspector", dock2);
        ImGui::DockBuilderDockWindow("Nodes", dock3);
        ImGui::DockBuilderDockWindow("Fleet", dock3);
        ImGui::DockBuilderDockWindow("Solution", dock4);
        ImGui::DockBuilderDockWindow("Schedule", dock4);
        ImGui::DockBuilderFinish(dockspace_id);
    }
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::BeginMenu("Open")) {
                if (ImGui::MenuItem("Dataset")) {
                    char const* lFilterPatterns[1] = { "*.xml" };
                    char const* lTheOpenFileName = tinyfd_openFileDialog(
                        "Open XML Dataset",
                        "",
                        1,
                        lFilterPatterns,
                        NULL,
                        0
                    );
                    if (lTheOpenFileName) {
                        this->reset(const_cast<char*>(lTheOpenFileName));
                    }
                }
                if (ImGui::MenuItem("Solution")) {
                    char const* lFilterPatterns[1] = { "*.txt" };
                    char const* lTheOpenFileName = tinyfd_openFileDialog(
                        "Open Solution",
                        "",
                        1,
                        lFilterPatterns,
                        NULL,
                        0
                    );
                    if (lTheOpenFileName) {
                        auto loader = new VRPTK::SolutionLoader();
                        this->state->solutions.push_back(loader->load(lTheOpenFileName));
                        LOG(INFO) << "Loaded " << lTheOpenFileName;
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Exit", "q")) {
                this->will_exit = true;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Solve")) {
            ImGui::SeparatorText("Third-party");
            if (ImGui::MenuItem("Ortools")) {
                LOG(INFO) << "Solving with Ortools";
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            for (auto view : this->views) {
                ImGui::MenuItem(view->getName(), NULL, &view->enabled);
            }
            ImGui::MenuItem("Performance", NULL, &this->performance_window);
            ImGui::MenuItem("ImGUI Demo", NULL, &this->demo_window);
            ImGui::Separator();
            if (ImGui::BeginMenu("Colors")) {
                if (ImGui::MenuItem("Dark")) {
                    ImGui::StyleColorsDark();
                }
                if (ImGui::MenuItem("Light")) {
                    ImGui::StyleColorsLight();
                }
                if (ImGui::MenuItem("Classic")) {
                    ImGui::StyleColorsClassic();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    Eigen::MatrixXd nodes = this->state->vrp->getNodes();
    for (auto view : this->views) {
        view->draw(this->state.get());
    }
    if (this->demo_window) {
        ImGui::ShowDemoWindow();
        //ImPlot::ShowDemoWindow();
    }
    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoDecoration);
    recalculateViewport();
    ImVec2 content_avail = ImGui::GetContentRegionAvail();
    ImGui::SetCursorPos(content_avail - ImVec2(30, 10));
    if (ImGui::Button("G", ImVec2(30, 30))) {
        this->grid_on = !this->grid_on;
    }
    ImGui::SetItemTooltip("Grid display");
    if (ImGui::BeginPopupContextItem()) {
        ImGui::SliderInt("Grid Step", &this->grid_step, 1, 50);
        ImGui::EndPopup();
    }
    ImGui::SetCursorPos(content_avail - ImVec2(60, 10));
    if (ImGui::Button("C", ImVec2(30, 30))) {
        this->color_mode = (this->color_mode + 1) % 3;
    }
    ImGui::SetItemTooltip("Color mode");
    if (ImGui::BeginPopupContextItem()) {
        const char* elem_name = color_mode == 0 ? "None" : color_mode == 1 ? "Type" : color_mode == 2 ? "Quantity" : "Id";
        ImGui::SliderInt("Scheme", &color_mode, 0, 3, elem_name);
        ImGui::EndPopup();
    }
    ImGui::SetCursorPos(content_avail - ImVec2(90, 10));
    if (ImGui::Button("S", ImVec2(30, 30))) {
        this->setup_route_on = !this->setup_route_on;
    }
    ImGui::SetItemTooltip("Display setup segment");
    ImGui::End();
    if (this->state->selected_node != -1) {
        ImVec2 window_pos = this->node_positions[this->state->selected_node];
        window_pos.x += 5;
        window_pos.y += 5;
        ImGui::SetNextWindowPos(window_pos);
        ImGui::Begin("Node Inspector", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
        ImGui::Text("Node %i", this->state->selected_node);
        ImGui::End();
    }
    if (this->performance_window) {
        ImGui::Begin("Performance", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking);
        float fps = ImGui::GetIO().Framerate;
        ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
        ImGui::End();
    }
    ImGui::Render();
}

ImVec2 GEngine::toScreenCoords(float x, float y, std::pair<ImVec2, ImVec2> bounds) {
    int padding = 10;
    int padding_y = 40;
    return ImVec2((x - bounds.first.x) * (viewport_size.x - padding) / (bounds.second.x - bounds.first.x) + viewport_pos.x + padding,
        (y - bounds.first.y) * (viewport_size.y - padding_y) / (bounds.second.y - bounds.first.y) + viewport_pos.y + padding_y);
}

void GEngine::recalculateViewport() {
    this->viewport_size = ImGui::GetContentRegionAvail();
    this->viewport_pos = ImGui::GetCursorScreenPos();
    Eigen::MatrixXd nodes = this->state->vrp->getNodes();
    std::map<int, VRPTK::Request> requests = this->state->vrp->getRequests();
    std::pair<ImVec2, ImVec2> bounds = getBounds(nodes);
    int padding = 10;
    int padding_y = 40;
    this->viewport_draw_list = ImGui::GetWindowDrawList();
    ImVec2 window_pos = ImGui::GetWindowPos();
    if (this->grid_on) {
        for (float x = bounds.first.x; x <= bounds.second.x; x += this->grid_step) {
            int screen_x = (x - bounds.first.x) * (viewport_size.x - padding) / (bounds.second.x - bounds.first.x) + window_pos.x + padding;
            this->viewport_draw_list->AddLine(ImVec2(screen_x, window_pos.y + padding_y), ImVec2(screen_x, window_pos.y + padding_y + viewport_size.y - padding), IM_COL32(255, 255, 255, 50));
        }
        for (float y = bounds.first.y; y <= bounds.second.y; y += this->grid_step) {
            int screen_y = (y - bounds.first.y) * (viewport_size.y - padding_y) / (bounds.second.y - bounds.first.y) + window_pos.y + padding_y;
            this->viewport_draw_list->AddLine(ImVec2(window_pos.x + padding, screen_y), ImVec2(window_pos.x + padding + viewport_size.x - padding, screen_y), IM_COL32(255, 255, 255, 50));
        }
    }
    for (int i = 0; i < nodes.rows(); i++) {
        int x = nodes(i, 1);
        int y = nodes(i, 2);
        ImVec2 screen_pos = this->toScreenCoords(x, y, bounds);
        this->node_positions[i] = ImVec2(screen_pos.x, screen_pos.y);
        bool selected = i == this->state->selected_node;
        ImU32 color = IM_COL32(255, 255, 255, 255);
        if (this->color_mode == 1) {
            ImColor c = ImColor::HSV((int)nodes(i, 3) / 7.0f, 0.6f, 0.6f);
            color = c.operator ImU32();
        } else if (this->color_mode == 2) {
            if (requests.find(i) != requests.end()) {
                double quantity = requests.at(i).getQuantity();
                ImColor c = ImColor::HSV(quantity / 100.0f, 0.6f, 0.6f);
                color = c.operator ImU32();
            }
        } else if (this->color_mode == 3) {
            ImColor c = ImColor::HSV(i / 7.0f, 0.6f, 0.6f);
            color = c.operator ImU32();
        }
        int size = selected ? 8 : 5;
        if (nodes(i, 3) == 0) {
            this->viewport_draw_list->AddTriangleFilled(ImVec2(screen_pos.x, screen_pos.y - size), ImVec2(screen_pos.x - size, screen_pos.y + size), ImVec2(screen_pos.x + size, screen_pos.y + size), color);
        } else if (nodes(i, 3) == 1) {
            this->viewport_draw_list->AddTriangleFilled(ImVec2(screen_pos.x, screen_pos.y + size), ImVec2(screen_pos.x - size, screen_pos.y - size), ImVec2(screen_pos.x + size, screen_pos.y - size), color);
        } else {
            this->viewport_draw_list->AddConvexPolyFilled(
                (ImVec2*)(new ImVec2[6]{
                    ImVec2(screen_pos.x, screen_pos.y),
                    ImVec2(screen_pos.x, screen_pos.y + (size * 2)),
                    ImVec2(screen_pos.x + (size * 1), screen_pos.y),
                    ImVec2(screen_pos.x, screen_pos.y),
                    ImVec2(screen_pos.x - (size * 1.5), screen_pos.y),
                    ImVec2(screen_pos.x, screen_pos.y - (size * 2.5)),
                    }), 6, color
                    );
        }
        // this->viewport_draw_list->AddCircleFilled(ImVec2(screen_x, screen_y), selected ? 5 : 3, color);
    }
    if (this->state->selected_solution != -1) {
        std::vector<std::vector<int> > routes = this->state->solutions[this->state->selected_solution]->getRoutes();
        int depot_x = nodes(0, 1);
        int depot_y = nodes(0, 2);
        ImVec2 depot_pos = this->toScreenCoords(depot_x, depot_y, bounds);
        for (int i = 0; i < routes.size(); i++) {
            std::vector<int> route = routes[i];
            ImVec4 color = (ImVec4)ImColor::HSV(i / 7.0f, 0.6f, 0.6f);
            if (this->setup_route_on) {
                int x2 = nodes(route[0], 1);
                int y2 = nodes(route[0], 2);
                ImVec2 screen_pos = this->toScreenCoords(x2, y2, bounds);
                this->viewport_draw_list->AddLine(ImVec2(depot_pos.x, depot_pos.y), ImVec2(screen_pos.x, screen_pos.y), ImU32(ImGui::ColorConvertFloat4ToU32(color)), 2);
            }
            for (int j = 0; j < route.size() - 1; j++) {
                int x1 = nodes(route[j], 1);
                int y1 = nodes(route[j], 2);
                int x2 = nodes(route[j + 1], 1);
                int y2 = nodes(route[j + 1], 2);
                ImVec2 screen_pos1 = this->toScreenCoords(x1, y1, bounds);
                ImVec2 screen_pos2 = this->toScreenCoords(x2, y2, bounds);
                this->viewport_draw_list->AddLine(ImVec2(screen_pos1.x, screen_pos1.y), ImVec2(screen_pos2.x, screen_pos2.y), ImU32(ImGui::ColorConvertFloat4ToU32(color)), 2);
            }
            if (this->setup_route_on) {
                int x1 = nodes(route[route.size() - 1], 1);
                int y1 = nodes(route[route.size() - 1], 2);
                ImVec2 screen_pos = this->toScreenCoords(x1, y1, bounds);
                this->viewport_draw_list->AddLine(ImVec2(depot_pos.x, depot_pos.y), ImVec2(screen_pos.x, screen_pos.y), ImU32(ImGui::ColorConvertFloat4ToU32(color)), 2);
            }
        }
    }
}

void GEngine::registerView(GView* view) {
    this->views.push_back(view);
}

void GEngine::teardown() {
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    TTF_CloseFont(this->font);
    TTF_Quit();
    SDL_Quit();
}

void GEngine::reset(char* path) {
    this->will_exit = false;
    this->state = std::shared_ptr<GlobalState>(new GlobalState());
    this->state->solutions = std::vector<VRPTK::Solution*>();
    this->state->selected_node = -1;
    this->state->selected_solution = -1;
    this->node_positions.clear();
    if (path != nullptr) {
        auto loader = new VRPTK::XMLLoader();
        this->state->vrp = loader->load(path);
        LOG(INFO) << "Loaded " << path;
    }
}