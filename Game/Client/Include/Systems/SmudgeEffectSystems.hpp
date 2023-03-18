#pragma once

#include <map>
#include <string>

#include "entt/entity/registry.hpp"

#include "Core.hpp"
#include "Components.hpp"

using std::string;

void SmudgeBallSystem(entt::registry& Scene, std::map<string, SpriteAsset>& Assets);