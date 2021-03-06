#pragma once

#include "UIHeader.h"
#include "ui/UICheckBox.h"
#include "STVisibleRect.h"
#include "Dialog.h"


Button * quickButton(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback);
ImageView * quickImageView(const string& name, Widget* root);
ImageView * quickImageView(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback);
CheckBox * quickCheckBox(const string& name, Widget* root, const CheckBox::ccCheckBoxCallback &callback);
Layout * quickLayout(int iTag, Widget* root, const Widget::ccWidgetTouchCallback &callback);
Layout * quickLayout(const string& name, Widget* root, const Widget::ccWidgetTouchCallback &callback);

void quickSetString(int iTag, Widget *itemRoot, const string &text);
void quickSetString(const string &itemName, Widget *itemRoot, const string &text);
void quickSetImage(int iTag, Widget *itemRoot, const string &filename);
void quickSetImage(const string &itemName, Widget *itemRoot, const string &filename);
void quickSetVisible(int iTag, Widget *itemRoot, bool bVisible);
void quickSetVisible(const string& name , Widget *itemRoot, bool bVisible);
void quickAddEvent(const string& eventName, const function<void(EventCustom*)>& callback, Node *node);
void quickSetButtonTitle(int iTag, Widget *itemRoot, const string &itemName);
void quickSetFontName(const string& name, Widget *itemRoot, const string& fontname);
void quickLogContentSize(Node *node);
void quickLogPosition(Node *node);

Widget* quickWidgetFromOtherRoot(const string& name, Widget *itemRoot, Node *parent);
void quickBackToOriginal(Node * node, Vec2 &original);

bool quickIfAInB(Widget * a, Widget * b);
void quickSetLocalZOrder(Widget * widget, int localZOrder);
bool quickIsOutsideOfScreen(Widget * widget);
Vec2 rotateWithDegree(Vec2 &point, float fDegree);
int cycleNum(int iCount, bool bAdd, int *idx);
int quickRandom(int min, int max);
float quickRandomFloat(float min, float max);
Armature * quickGetAnimation(int animationIndex, const string &name, const Vec2 &pos);
std::string getcurrTime();

