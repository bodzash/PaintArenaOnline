#pragma once

#include <string>
#include <functional>

#include "raylib.h"

using std::string;

#define UIColorBlack (Color){31, 24, 51, 255}
#define UIColorGreen (Color){48, 156, 99, 255}

struct UILabel
{
  Vector2 Pos;
  string Text;
  float FontSize = 16.0f;
  Color FontColor = UIColorBlack;
};

struct UIInputBox
{
  Rectangle Rect;
  int Limit;
  UILabel Label;
  string PlaceholderTxt;
  bool _bIsActive;
  int _DelayTime = 0;
  int _CursorPulse = 60;
};

struct UIButton
{
  Rectangle Rect;
  UILabel Label;
  std::function<void()> Action;
};