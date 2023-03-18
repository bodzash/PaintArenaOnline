#include "Systems/ShakeEffectSystems.hpp"

void ShakeSpriteSystem(entt::registry& Scene)
{
  auto View = Scene.view<Sprite, Shake>();
  for (auto Entity : View)
  {
    auto& Spr = View.get<Sprite>(Entity);
    auto& Shk = View.get<Shake>(Entity);

    if (Shk.Amount <= 0.0f)
    {
      Shk.Amount = 0.0f;
      Spr.OffsetX = Spr.OgOffsetX;
      Spr.OffsetY = Spr.OgOffsetY;
    }
    else
    {
      Spr.OffsetX = RandomRange(-Shk.Amount, Shk.Amount) + Spr.OgOffsetX;
      Spr.OffsetY = RandomRange(-Shk.Amount, Shk.Amount) + Spr.OgOffsetY;
      Shk.Amount -= 0.4f;
    }
  }
}