#pragma once

#include <string>
#include <functional>

using std::string;

struct UILabel
{
  float x;
  float y;
  string Text;
  float FontSize = 16.0f;
};

struct UIInputBox
{
  float x;
  float y;
  float Width;
  float Height;
  int Limit;
  UILabel Label;
};

struct UIButton
{
  float x;
  float y;
  float Width;
  float Height;
  UILabel Label;
  //lambda Action
};