#include <Systems/RendererSystems.hpp>

Color IndexToRGB[6] = {PlayerColorPink, PlayerColorGreen, PlayerColorRed,
  PlayerColorCyan, PlayerColorLime, PlayerColorOrange};

// Todo rework sprite system to work with depth and delete unwanted systems
void SpriteRendererSystem(entt::registry& Scene, Texture2D& Tex,
  std::map<string, SpriteAsset>& Assets)
{
  auto View = Scene.view<Position, Sprite>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Spr = View.get<Sprite>(Entity);

    DrawTextureRec(Tex, {Assets[Spr.Asset].x, Assets[Spr.Asset].y,
      Assets[Spr.Asset].Width, Assets[Spr.Asset].Height},
      {Pos.x - Spr.OffsetX, Pos.y - Spr.OffsetY},
      WHITE);
  }
}

void SmudgeRendererSystem(entt::registry& Scene, Texture2D& Tex,
  std::map<string, SpriteAsset>& Assets)
{
  auto View = Scene.view<Position, Smudge>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Spr = View.get<Smudge>(Entity);

    DrawTextureRec(Tex, {Assets[Spr.Asset].x, Assets[Spr.Asset].y,
      Assets[Spr.Asset].Width, Assets[Spr.Asset].Height},
      {Pos.x - Assets[Spr.Asset].OffsetX, Pos.y - Assets[Spr.Asset].OffsetY},
      IndexToRGB[Spr.Color]);
  }
}

void ShadowRendererSystem(entt::registry& Scene, Texture2D& Tex,
  std::map<string, SpriteAsset>& Assets)
{
  auto View = Scene.view<Position, Shadow>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Sha = View.get<Shadow>(Entity);

    DrawTextureRec(Tex, {Assets[Sha.Asset].x, Assets[Sha.Asset].y,
      Assets[Sha.Asset].Width, Assets[Sha.Asset].Height},
      {Pos.x - Assets[Sha.Asset].OffsetX, Pos.y - Assets[Sha.Asset].OffsetY},
      {0, 0, 0, 100});
  }
}

void BackgroundRendererSystem(entt::registry& Scene, Texture2D& Tex,
  std::map<string, SpriteAsset>& Assets)
{
  auto View = Scene.view<TileTag, Position>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);

    DrawTextureRec(Tex, {Assets["FloorTile"].x, Assets["FloorTile"].y,
      Assets["FloorTile"].Width, Assets["FloorTile"].Height},
      {Pos.x, Pos.y}, WHITE);
  }
}

void ColliderDebugRendererSystem(entt::registry& Scene)
{
  auto View = Scene.view<Position, Collider>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Col = View.get<Collider>(Entity);

    DrawCircleV({Pos.x, Pos.y}, Col.Diameter / 2.0f, {85, 170, 250, 191});
  }
}

void CursorRenderingSystem(Texture2D& Tex, std::map<string, SpriteAsset>& Assets)
{
  DrawTextureRec(Tex, {Assets["Cursor"].x, Assets["Cursor"].y,
    Assets["Cursor"].Width, Assets["Cursor"].Height},
    {(float)GetMouseX() / 4 - 5, (float)GetMouseY() / 4 - 5}, WHITE);
}