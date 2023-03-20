#include <Systems/RendererSystems.hpp>

Color IndexToRGB[] = {PlayerColorPink, PlayerColorGreen, PlayerColorRed,
  PlayerColorCyan, PlayerColorLime, PlayerColorOrange, WHITE, {0, 0, 0, 100}};

// Todo delete unwanted systems
void SpriteRendererSystem(entt::registry& Scene, Texture2D& Tex,
  std::map<string, SpriteAsset>& Assets)
{
  // Sort the order based on depth so the rendering order is correct
  Scene.sort<Sprite>([&Scene](const entt::entity& A, const entt::entity& B){
    auto& ASpr = Scene.get<Sprite>(A);
    auto& BSpr = Scene.get<Sprite>(B);

    if (BSpr.Depth > ASpr.Depth) return true;
    
    if(BSpr.Depth == ASpr.Depth)
      if ((uint32_t)B < (uint32_t)A) return true;

    return false;
  });

  auto View = Scene.view<Position, Sprite>().use<Sprite>();
  for (auto Entity : View)
  {
    auto& Pos = View.get<Position>(Entity);
    auto& Spr = View.get<Sprite>(Entity);

    DrawTextureRec(Tex, {Assets[Spr.Asset].x, Assets[Spr.Asset].y,
      Assets[Spr.Asset].Width, Assets[Spr.Asset].Height},
      {Pos.x - Spr.OffsetX, Pos.y - Spr.OffsetY},
      IndexToRGB[Spr.Color]);
  }
}

void CursorRenderingSystem(Texture2D& Tex, std::map<string, SpriteAsset>& Assets)
{
  DrawTextureRec(Tex, {Assets["Cursor"].x, Assets["Cursor"].y,
    Assets["Cursor"].Width, Assets["Cursor"].Height},
    {(float)GetMouseX() / 4 - 5, (float)GetMouseY() / 4 - 5}, WHITE);
}