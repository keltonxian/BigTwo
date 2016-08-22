//
//  SceneRule.cpp
//  BigTwo
//
//  Created by keltonxian on 3/23/15.
//
//

#include "SceneRule.h"
#include "GameTool.h"
#include "MainScene.h"

#define QAKIND_POINT_MATCH       1
#define QAKIND_HUNDRED_MATCH     2
#define QAKIND_BIGTWO_RULE       3
#define DETAIL_POINT_MATCH     "积分赛规则:\n每局拥有方块3的先出。\n每局最快出完手牌的一方为胜者，其余为败者。\n根据各位败者手上剩余牌来决定胜者所获积分。\n积分标注:\n余牌数小于8张，每张1分。\n余牌数8，9张，每张2分。\n余牌数10~12张，每张3分。\n余牌数13张，每张4分。"
#define DETAIL_HUNDRED_MATCH   "冲百赛规则:\n每局拥有方块3的先出。\n每局最快出完手牌的一方为胜者，其余为败者。\n败者根据自己手上剩余的牌，来决定所累加的分数，当任一方累加分数达到100时，则其为最终失败者，此时比赛结束，累计分数最低的一方为胜者。\n积分标注:\n余牌数小于8张，每张1分。\n余牌数8，9张，每张2分。\n余牌数10~12张，每张3分。\n余牌数13张，每张4分。"
#define DETAIL_BIGTWO_RULE     "锄大地规则:\n先按数字分大小:  2>A>K>Q>J>10>9>8>7>6>5>4>3\n数字相同的按花色分大小:  黑桃>红心>梅花>方块\n出牌牌型种类:\n1. 单张\n2. 一对(数字相同)\n3. 顺子(按数字顺序从小到大连续5张，不需相同花色)\n4. 花(5张相同花色的牌)\n5. 夫佬(3张数字相同牌 加 一对)\n6. 金刚(4张数字相同牌 加 单张)\n7. 同花顺(按数字顺序从小到大连续5张，并且要同一花色)\n上手所出牌型为单张或一对时，只能跟着出同一牌型，并要大于上手的牌。\n上手出剩余牌型时，除了出同一牌型并牌面大于上手，还能根据牌型大小来出: 同花顺>金刚>夫佬>花>顺。\n最先出完手上牌的一方为胜。"

Scene* SceneRule::createScene()
{
    auto scene = Scene::create();
    auto layer = SceneRule::create();
    scene->addChild(layer);
    return scene;
}

bool SceneRule::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto bg = KUtil::addSprite(this, KUtil::getPath(F_BG, "bg_3.png"), Point(HALF_WIDTH, 0), ANCHOR_CENTER_DOWN, 0);
    bg->setScale(KUtil::getScale(FULL_WIDTH, FULL_HEIGHT, bg->getContentSize(), true));
    
    Size size = Size(FULL_WIDTH-20, FULL_HEIGHT-160-10);
    auto tbg = GameTool::addTextBg(this, size, Point(10, 160), ANCHOR_LEFT_DOWN, 10);
    
    detailLabel = KUtil::addLabelConfig(tbg, DETAIL_POINT_MATCH, KUtil::getPath(F_FONT, "luoliti.ttf"), 30, Point(10, 10), Color4B(0, 0, 0, 255), ANCHOR_LEFT_DOWN, 10, Size(size.width-20, size.height-20), TextHAlignment::LEFT, TextVAlignment::TOP);
    
    Vector<MenuItem *> itemArray;
    
    GameTool::addBtn2(&itemArray, "返回", 32, Point(FULL_WIDTH-5, FULL_HEIGHT-5), ANCHOR_RIGHT_UP, CC_CALLBACK_1(SceneRule::callbackBack, this));
    
    KUtil::addMenu(this, &itemArray, 30);
    
    listQAKind.pushBack(QAKind::create(QAKIND_POINT_MATCH, "普通赛规则"));
    listQAKind.pushBack(QAKind::create(QAKIND_HUNDRED_MATCH, "冲百赛规则"));
    listQAKind.pushBack(QAKind::create(QAKIND_BIGTWO_RULE, "锄大地规则"));
    
    cellHeight = 160;
    cellWidth = 250;
    TableView* tableView = TableView::create(this, Size(FULL_WIDTH, cellHeight));
    tableView->setDirection(ScrollView::Direction::HORIZONTAL);
    tableView->setPosition(Point(0, 0));
    tableView->setDelegate(this);
    this->addChild(tableView, 100);
    tableView->reloadData();
    
    return true;
}

void SceneRule::onEnter()
{
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(SceneRule::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(SceneRule::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(SceneRule::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void SceneRule::onExit()
{
    Layer::onExit();
}

bool SceneRule::onTouchBegan(Touch* touch, Event* event)
{
    //    CCLOG("onTouchBegan");
    return true;
}

void SceneRule::onTouchMoved(Touch* touch, Event* event)
{
    
}

void SceneRule::onTouchEnded(Touch* touch, Event* event)
{
    
}

void SceneRule::callbackBack(cocos2d::Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionSlideInB::create(0.4, MainScene::createScene()));
}

void SceneRule::callbackBtn(cocos2d::Ref* pSender)
{
    int idx = ((MenuItem *)pSender)->getTag();
    QAKind *qk = listQAKind.at(idx);
    int kind = qk->getType();
    std::string detail;
    if (kind == QAKIND_POINT_MATCH) {
        detail = DETAIL_POINT_MATCH;
    } else if (kind == QAKIND_HUNDRED_MATCH) {
        detail = DETAIL_HUNDRED_MATCH;
    } else if (kind == QAKIND_BIGTWO_RULE) {
        detail = DETAIL_BIGTWO_RULE;
    }  else {
        return;
    }
    detailLabel->setString(detail);
}

void SceneRule::scrollViewDidScroll(ScrollView* view)
{
    
}

void SceneRule::scrollViewDidZoom(ScrollView* view)
{
    
}

void SceneRule::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

Size SceneRule::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(cellWidth, cellHeight);
}

TableViewCell* SceneRule::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    if (cell) {
        cell->removeAllChildrenWithCleanup(true);
        
    }
    
    cell = new TableViewCell();
    cell->autorelease();
    QAKind *gk = listQAKind.at(idx);
    std::string title = gk->getTitle();
    Vector<MenuItem *> itemArray;
    
    auto item = GameTool::addBtn2(&itemArray, title, 30, Point(cellWidth/2, cellHeight/2), ANCHOR_CENTER_CENTER, CC_CALLBACK_1(SceneRule::callbackBtn, this));
    item->setTag(idx);
    
    KUtil::addMenu(cell, &itemArray, 30);
    return cell;
}

ssize_t SceneRule::numberOfCellsInTableView(TableView *table)
{
    return listQAKind.size();
}

#pragma mark QAKind ---

QAKind* QAKind::create(int type, const std::string& title)
{
    QAKind *gk = new QAKind();
    if (gk)
    {
        gk->autorelease();
        gk->_type = type;
        gk->_title = title;
        return gk;
    }
    CC_SAFE_DELETE(gk);
    return NULL;
}

QAKind::QAKind()
{
    
}

QAKind::~QAKind()
{
    
}

int QAKind::getType()
{
    return _type;
}

std::string QAKind::getTitle()
{
    return _title;
}




