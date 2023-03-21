#include "Systems/UISystems.hpp"

void UIRenderLabelSystem(entt::registry& Scene, std::map<std::string, Font>& Assets)
{
  auto View = Scene.view<UILabel>();
  for (auto Entity : View)
  {
    auto& Lab = View.get<UILabel>(Entity);
    
    DrawTextEx(Assets["m5x7"], Lab.Text.c_str(), {Lab.x, Lab.y},
      Lab.FontSize, 1.0f, BLACK);
  }
}