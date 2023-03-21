#include "Systems/UISystems.hpp"

bool Vector2InRectangle(Vector2 Vec, Rectangle Rec)
{
  return (
    Rec.x <= Vec.x &&
    Vec.x <= (Rec.x + Rec.width) &&
    Rec.y <= Vec.y &&
    Vec.y <= (Rec.y + Rec.height)
  );
}

Vector2 Vector2AddRectangle(Vector2 Vec, Rectangle Rec)
{
  return (Vector2){Vec.x + Rec.x, Vec.y + Rec.y};
}

void UIRenderLabelSystem(entt::registry& Scene, std::map<std::string, Font>& Assets)
{
  auto View = Scene.view<UILabel>();
  for (auto Entity : View)
  {
    auto& Lab = View.get<UILabel>(Entity);
    
    // Render text
    DrawTextEx(Assets["m5x7"], Lab.Text.c_str(), Lab.Pos,
      Lab.FontSize, 1.0f, Lab.FontColor);
  }
}

void UIInputBoxSystem(entt::registry& Scene, std::map<std::string, Font>& Assets)
{
  auto View = Scene.view<UIInputBox>();
  for (auto Entity : View)
  {
    auto& Inp = View.get<UIInputBox>(Entity);
    auto& Lab = Inp.Label;
    
    // Input box logic
    bool bIsHovered = Vector2InRectangle(GetMousePosition(), Inp.Rect);

    if (Inp._DelayTime > 0) Inp._DelayTime--;

    if (Inp._CursorPulse > 0) Inp._CursorPulse--;
    else Inp._CursorPulse = 60;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && bIsHovered)
      if (!Inp._bIsActive) Inp._bIsActive = true;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !bIsHovered)
      if (Inp._bIsActive) Inp._bIsActive = false;

    if (Inp._bIsActive)
    {
      int Key = GetCharPressed();

      // Type text into label
      while (Key > 0)
      {
        if (!IsKeyDown(KEY_LEFT_CONTROL) || !IsKeyDown(KEY_RIGHT_CONTROL))
        {
          if (Lab.Text.length() != Inp.Limit) Lab.Text.push_back((char)Key);
          Key = GetCharPressed();
        }
      }

      // Copy and Paste
      if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_C))
        SetClipboardText(Lab.Text.c_str());
      
      if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_V))
        Lab.Text = GetClipboardText();

      // Delete
      if (IsKeyPressed(KEY_BACKSPACE) && Lab.Text.length() > 0)
      {
        Lab.Text.pop_back();
        Inp._DelayTime = 18;
      }

      if (IsKeyDown(KEY_BACKSPACE) && Lab.Text.length() > 0 && Inp._DelayTime == 0)
      {
        Lab.Text.pop_back();
        Inp._DelayTime = 2;
      }
    }

    // Render background
    DrawRectangleLinesEx(Inp.Rect, 4.0f, UIColorBlack);

    // Render innertext
    std::string StringToDraw = Lab.Text;
    if (Inp._CursorPulse > 30 && Inp._bIsActive || Inp._DelayTime > 0)
      StringToDraw.push_back('|');

    if (Lab.Text.length() == 0 && !Inp._bIsActive) StringToDraw = Inp.PlaceholderTxt;

    DrawTextEx(Assets["m5x7"], StringToDraw.c_str(),
      Vector2AddRectangle(Lab.Pos, Inp.Rect), Lab.FontSize, 1.0f,
      (Lab.Text.length() == 0 && !Inp._bIsActive) ? UIColorGray : Lab.FontColor);
  }
}

void UIRenderButtonSystem(entt::registry& Scene, std::map<std::string, Font>& Assets)
{
  auto View = Scene.view<UIButton>();
  for (auto Entity : View)
  {
    auto& Btn = View.get<UIButton>(Entity);
    auto& Lab = Btn.Label;

    // Button logic
    bool bIsHovered = Vector2InRectangle(GetMousePosition(), Btn.Rect);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && bIsHovered)
      Btn.Action();
      
    // Render background
    DrawRectangleLinesEx(Btn.Rect, 4.0f, UIColorBlack);
    
    // Render text
    DrawTextEx(Assets["m5x7"], Lab.Text.c_str(), Vector2AddRectangle(Lab.Pos, Btn.Rect),
      Lab.FontSize, 1.0f, Lab.FontColor);
  }
}