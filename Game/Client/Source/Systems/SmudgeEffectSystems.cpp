#include "Systems/SmudgeEffectSystems.hpp"

void SmudgeBallSystem(entt::registry& Scene, std::map<string, SpriteAsset>& Assets)
{
  auto View = Scene.view<Position, SmudgeBall, Sprite, TeamId, Speed>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Smb = View.get<SmudgeBall>(Entity);
    auto& Spr = View.get<Sprite>(Entity);
    auto& Spd = View.get<Speed>(Entity);
    auto& Tid = View.get<TeamId>(Entity);

    Spr.OffsetY = Smb.Height;

    Spd.MaxSpeed *= 0.9f;
    Smb.Height -= 0.7f;

    if (Smb.Height < -1.0f)
    {
      //CreatePrefabSmudgeSmall(Scene, Pos.x, Pos.y, Tid.Team);
      Scene.destroy(Entity);
    }
  }
}