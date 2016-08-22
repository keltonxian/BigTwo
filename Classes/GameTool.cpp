//
//  GameTool.cpp
//  BigTwo
//
//  Created by xiankelton on 8/14/16.
//
//

#include "GameTool.h"


Scale9Sprite* GameTool::addTextBg(Node *parent, Size realSize, Point position, Point anchorPoint, int zorder)
{
    return KUtil::addScale9Sprite(parent, KUtil::getPath(F_FRAME, "frame_text_bg.png"), Rect(0, 0, 62, 56), Rect(30, 25, 2, 6), realSize, position, anchorPoint, zorder);
}

Label* GameTool::addLabelOutlineDefault(Node *parent, const std::string& text, int fontSize, Point position, Color4B color, Color4B outlineColor, int outlineSize, Point anchorPoint, int zorder, const Size& dimensions/* = Size::ZERO*/, TextHAlignment hAlignment/* = TextHAlignment::LEFT*/, TextVAlignment vAlignment/* = TextVAlignment::BOTTOM*/)
{
    Label *label = KUtil::addLabelConfig(parent, text, KUtil::getPath(F_FONT, "luoliti.ttf"), fontSize, position, color, anchorPoint, zorder, dimensions, hAlignment, vAlignment);
    label->enableOutline(outlineColor, outlineSize);
    return label;
}

MenuItemSprite* GameTool::addBtn2(Vector<MenuItem *> *itemArray, const std::string& text, int fontSize, Point position, Point anchorPoint, const ccMenuCallback& callback)
{
    Sprite *s1 = KUtil::createSprite(KUtil::getPath(F_BTN, "btn_2.png"));
    Sprite *s2 = KUtil::createSprite(KUtil::getPath(F_BTN, "btn_2.png"));
    s2->setScale(0.9);
    auto size = s1->getContentSize();
    auto btn = KUtil::addItemSpriteBySprite(itemArray, s1, s2, position, anchorPoint, callback);
    GameTool::addLabelOutlineDefault(btn, text, fontSize, Point(size.width/2, size.height/2), Color4B(255, 255, 255, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_CENTER_CENTER, 1);
    return btn;
}



