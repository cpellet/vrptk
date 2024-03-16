#ifndef GLIDESCOPE_GRAPHICS_H
#define GLIDESCOPE_GRAPHICS_H

#include <SDL2/SDL.h>
#include <Eigen/Dense>
#include <imgui.h>
#include <map>

int SDL_RenderFilledCircle(SDL_Renderer* renderer, int x, int y, int radius);
std::pair<ImVec2, ImVec2> getBounds(const Eigen::MatrixXd& nodes);
int closestNode(const std::map<int, ImVec2>& node_positions, int x, int y);

#endif // GLIDESCOPE_GRAPHICS_H