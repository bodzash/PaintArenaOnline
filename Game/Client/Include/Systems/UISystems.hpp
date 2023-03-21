#pragma once

#include <map>
#include <string>

#include "raylib.h"
#include "raymath.h"
#include "entt/entity/registry.hpp"

#include "UIComponents.hpp"

void UIRenderLabelSystem(entt::registry& Scene, std::map<std::string, Font>& Assets);
void UIInputBoxSystem(entt::registry& Scene, std::map<std::string, Font>& Assets);