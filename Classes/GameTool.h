//
//  GameTool.h
//  BigTwo
//
//  Created by xiankelton on 8/14/16.
//
//

#ifndef GameTool_h
#define GameTool_h

#include "GameHeader.h"

class GameTool {
public:
    static Scale9Sprite* addTextBg(Node *parent, Size realSize, Point position, Point anchorPoint, int zorder);
    static Label* addLabelOutlineDefault(Node *parent, const std::string& text, const int fontSize, Point position, Color4B color, Color4B outlineColor, int outlineSize, Point anchorPoint, int zorder, const Size& dimensions = Size::ZERO, TextHAlignment hAlignment = TextHAlignment::LEFT, TextVAlignment vAlignment = TextVAlignment::BOTTOM);
    static MenuItemSprite* addBtn2(Vector<MenuItem *> *itemArray, const std::string& text, int fontSize, Point position, Point anchorPoint, const ccMenuCallback& callback);
};

#endif /* GameTool_h */
