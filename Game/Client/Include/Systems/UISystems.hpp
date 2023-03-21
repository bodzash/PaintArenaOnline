#pragma once

#include <map>
#include <string>

#include "raylib.h"
#include "raymath.h"
#include "entt/entity/registry.hpp"

#include "UIComponents.hpp"

#define UIColorBlack (Color){31, 24, 51, 255}
#define UIColorGray (Color){104, 113, 122, 255}
#define UIColorGreen (Color){48, 156, 99, 255}

void UIRenderLabelSystem(entt::registry& Scene, std::map<std::string, Font>& Assets);
void UIInputBoxSystem(entt::registry& Scene, std::map<std::string, Font>& Assets);
void UIRenderButtonSystem(entt::registry& Scene, std::map<std::string, Font>& Assets);