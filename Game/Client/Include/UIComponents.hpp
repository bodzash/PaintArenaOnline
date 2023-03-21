#pragma once

#include <string>
#include <functional>

#include "raylib.h"

using std::string;

struct UILabel
{
  Vector2 Pos;
  string Text;
  Color FontColor = BLACK;
  float FontSize = 16.0f;
};

struct UIInputBox
{
  Rectangle Rect;
  int Limit;
  UILabel Label;
  bool _bIsActive;
  int _DelayTime = 0;
  int _CursorPulse = 60;
};

struct UIButton
{
  Rectangle Rect;
  UILabel Label;
  //lambda Action
};