#pragma once

#include <map>
#include <string>

#include "raylib.h"
#include "entt/entity/registry.hpp"

#include "Math.hpp"
#include "Components.hpp"

using std::string;

void AudioPlayerSystem(entt::registry& Scene, std::map<string, Sound> Assets);