#include "graphics.h"

int SDL_RenderFilledCircle(SDL_Renderer* renderer, int x, int y, int radius) {
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = 2 * radius;
    d = radius;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        } else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        } else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
    return status;
}

std::pair<ImVec2, ImVec2> getBounds(const Eigen::MatrixXd& nodes) {
    int min_x = nodes.col(1).minCoeff();
    int min_y = nodes.col(2).minCoeff();
    int max_x = nodes.col(1).maxCoeff();
    int max_y = nodes.col(2).maxCoeff();
    return std::make_pair(ImVec2(min_x, min_y), ImVec2(max_x, max_y));
}

int closestNode(const std::map<int, ImVec2>& node_positions, int x, int y) {
    int closest = -1;
    float min_dist = std::numeric_limits<float>::max();
    for (const auto& [id, pos] : node_positions) {
        float dist = (pos.x - x) * (pos.x - x) + (pos.y - y) * (pos.y - y);
        if (dist < min_dist) {
            min_dist = dist;
            closest = id;
        }
    }
    return min_dist < 100 ? closest : -1;
}