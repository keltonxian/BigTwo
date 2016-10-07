//
//  MainScene.cpp
//  KillTime
//
//  Created by keltonxian on 7/11/14.
//
//

#include "MainScene.h"
#include "SceneBigTwo.h"
#include "SceneRule.h"
#include "GameTool.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <platform/android/jni/JniHelper.h>
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#import "KUtilIOS.h"
#endif

Scene* MainScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto bg = KUtil::addSprite(this, KUtil::getPath(F_BG, "bg_3.png"), Point(HALF_WIDTH, 0), ANCHOR_CENTER_DOWN, 0);
    bg->setScale(KUtil::getScale(FULL_WIDTH, FULL_HEIGHT, bg->getContentSize(), true));
    
    Vector<MenuItem *> itemArray;
    
    GameTool::addBtn2(&itemArray, "规则", 32, Point(FULL_WIDTH-5, FULL_HEIGHT-5), ANCHOR_RIGHT_UP, CC_CALLBACK_1(MainScene::callbackRule, this));
    
    KUtil::addMenu(this, &itemArray, 30);
    
    _normalMatchScore = 0;
    _hundredMatchScore = 0;
    
    initPlayerData();
    initMyData();
    
    listGameKind.pushBack(GameKind::create(BIGTWO_TYPE_POINT, "普通赛", "积分", _normalMatchScore));
    listGameKind.pushBack(GameKind::create(BIGTWO_TYPE_100, "冲百赛", "胜利场数", _hundredMatchScore));
    
    int tableViewX = 50;
    int tableViewY = 200;
    cellHeight = FULL_HEIGHT - tableViewY * 2;
    cellWidth = 250 * 2;
    TableView* tableView = TableView::create(this, Size(FULL_WIDTH-50*2, cellHeight));
    tableView->setDirection(ScrollView::Direction::HORIZONTAL);
    tableView->setPosition(Point(tableViewX, tableViewY));
    tableView->setDelegate(this);
    this->addChild(tableView, 100);
    tableView->reloadData();
    
    return true;
}

void MainScene::addPlayer(const char *altas, int iconIndex, int objIndex)
{
    bool isPlayerExist = KUtilIOS::checkEntityExistByIndex("Player", objIndex);
    if (true == isPlayerExist) {
        return;
    }
    KUtilIOS::saveStringValueByKey("Player", "atlas", altas, objIndex);
    KUtilIOS::saveIntValueByKey("Player", "icon", iconIndex, objIndex);
    KUtilIOS::saveIntValueByKey("Player", "normal_match_score", 0, objIndex);
    KUtilIOS::saveIntValueByKey("Player", "normal_match_win_count", 0, objIndex);
    KUtilIOS::saveIntValueByKey("Player", "normal_match_count", 0, objIndex);
    KUtilIOS::saveIntValueByKey("Player", "hundred_match_score", 0, objIndex);
    KUtilIOS::saveIntValueByKey("Player", "hundred_match_win_count", 0, objIndex);
    KUtilIOS::saveIntValueByKey("Player", "hundred_match_count", 0, objIndex);
}

void MainScene::initPlayerData()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    addPlayer("潜力股", 0, 0);
    addPlayer("婕婕姐", 1, 1);
    addPlayer("冬瓜哥", 2, 2);
    addPlayer("店小二", 3, 3);
#else
    auto p = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    ValueMap sd = FileUtils::getInstance()->getValueMapFromFile(p);
    CCLOG("initPlayerData data size[%zd]", sd.size());
    if (sd.size() >= 4) {
        return;
    }
    ValueMap saveData;
    // format
    // 名字 ID 积分赛积分
    saveData.insert(make_pair("10001", Value("潜力股 1 0 0 0 0 0 0 0 0")));
    saveData.insert(make_pair("10002", Value("婕婕姐 2 0 0 0 0 0 0 0 0")));
    saveData.insert(make_pair("10003", Value("冬瓜哥 3 0 0 0 0 0 0 0 0")));
    saveData.insert(make_pair("10004", Value("店小二 4 0 0 0 0 0 0 0 0")));
    
    auto path = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    FileUtils::getInstance()->writeToFile(saveData, path);
#endif
}

void MainScene::initMyData()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    int winMatchCount = 0;
    int matchCount = 0;
    int countWin1 = KUtilIOS::getIntValueByKey("Player", "normal_match_win_count", 0, 0);
    int countWin2 = KUtilIOS::getIntValueByKey("Player", "hundred_match_win_count", 0, 0);
    winMatchCount = countWin1 + countWin2;
    int count1 = KUtilIOS::getIntValueByKey("Player", "normal_match_count", 0, 0);
    int count2 = KUtilIOS::getIntValueByKey("Player", "hundred_match_count", 0, 0);
    matchCount = count1 + count2;
    std::string atlas = KUtilIOS::getStringValueByKey("Player", "atlas", "小赌神", 0);
    int iconIndex = KUtilIOS::getIntValueByKey("Player", "icon", 0, 0);
    int normalMatchScore = KUtilIOS::getIntValueByKey("Player", "normal_match_score", 0, 0);
    int hundredMatchScore = KUtilIOS::getIntValueByKey("Player", "hundred_match_score", 0, 0);
#else
    int winMatchCount = 0;
    int loseMatchCount = 0;
    auto path = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    ValueMap data = FileUtils::getInstance()->getValueMapFromFile(path);
    ValueMap::iterator iter = data.find("WinCount");
    if (iter != data.end()) {
        winMatchCount = data.at("WinCount").asInt();
    }
    iter = data.find("LoseCount");
    if (iter != data.end()) {
        loseMatchCount = data.at("LoseCount").asInt();
    }
    int matchCount = winMatchCount + loseMatchCount;
    
    // 名字 ID 积分赛积分 冲百赛积分 冲百赛胜场
    char atlas[100] = {0};
    memset(atlas, 0, 100);
    int iconIndex = 1;
    int normalMatchScore = 0;
    int hundredMatchScore = 0;
    int point3 = 0;
    int point4 = 0;
    int point5 = 0;
    int point6 = 0;
    int point7 = 0;
    int point8 = 0;
    
    iter = data.find("10001");
    if (iter != data.end()) {
        std::string s = data.at("10001").asString();
        CCLOG("v:[%s]", s.c_str());
        int ret = sscanf(s.c_str(), "%s %d %d %d %d %d %d %d %d %d", atlas, &iconIndex, &normalMatchScore, &hundredMatchScore, &point3, &point4, &point5, &point6, &point7, &point8);
        if (10 != ret) {
            sprintf(atlas, "me");
            iconIndex = 1;
            normalMatchScore = 0;
            hundredMatchScore = 0;
            point3 = 0;
            point4 = 0;
            point5 = 0;
            point6 = 0;
            point7 = 0;
            point8 = 0;
        }
    }
#endif
    
    GameTool::addTextBg(this, Size(FULL_WIDTH, 100), Point(0, 0), ANCHOR_LEFT_DOWN, 10);
    int x = 50;
    int y = 30;
    char iconType[20] = {0};
    memset(iconType, 0, 20);
    sprintf(iconType, "icon_%d.png", iconIndex);
    auto frame = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "frame_icon.png"), Point(x, y), ANCHOR_LEFT_DOWN, 20);
    auto iconSprite = KUtil::addSprite(frame, KUtil::getPath(F_ICON, iconType), Point(61, 72), ANCHOR_CENTER_CENTER, -1);
    iconSprite->setScale(0.95);
    x += 140;
    GameTool::addLabelOutlineDefault(this, atlas, 30, Point(x, y), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 20);
    
    x = FULL_WIDTH - 800;
    GameTool::addLabelOutlineDefault(this, "统计:", 35, Point(x, y), Color4B(200, 150, 200, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_DOWN, 20);
    
    x += 110;
    char str[500] = {0};
    memset(str, 0, 500);
    sprintf(str, "%d(胜)/%d(总)", winMatchCount, matchCount);
    GameTool::addLabelOutlineDefault(this, str, 35, Point(x, y), Color4B(200, 150, 0, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_DOWN, 20);
    
    _normalMatchScore = normalMatchScore;
    _hundredMatchScore = hundredMatchScore;
}

void MainScene::onEnter()
{
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto keyPadListener = EventListenerKeyboard::create();
    keyPadListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
    keyPadListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyPadListener, this);
}

void MainScene::onExit()
{
    Layer::onExit();
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    //    CCLOG("onKeyPressed");
    //    CCLOG("onKeyPressed keyCode[%d]", keyCode);
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (EventKeyboard::KeyCode::KEY_BACK == keyCode) {
        JniMethodInfo t;
        
        if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "exitGame", "()V"))
        {
            t.env->CallStaticVoidMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
    }
#endif
    //    CCLOG("onKeyReleased keyCode[%d]", keyCode);
}

bool MainScene::onTouchBegan(Touch* touch, Event* event)
{
    //    CCLOG("onTouchBegan");
    return true;
}

void MainScene::onTouchMoved(Touch* touch, Event* event)
{
    
}

void MainScene::onTouchEnded(Touch* touch, Event* event)
{
    
}

void MainScene::callbackRandomMatch(cocos2d::Ref* pSender)
{
    int idx = ((MenuItem *)pSender)->getTag();
    GameKind *gk = listGameKind.at(idx);
    Director::getInstance()->replaceScene(TransitionSlideInT::create(0.4, SceneBigTwo::createScene(gk->getType())));
}

void MainScene::callbackRule(cocos2d::Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionSlideInT::create(0.4, SceneRule::createScene()));
}

void MainScene::callbackRank(cocos2d::Ref* pSender)
{
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    int idx = ((MenuItem *)pSender)->getTag();
    GameKind *gk = listGameKind.at(idx);
    if (gk->getType() == BIGTWO_TYPE_POINT) {
        KUtilIOS::showRank(IOS_KEY_RANK_POINT);
        return;
    }
    if (gk->getType() == BIGTWO_TYPE_100) {
        KUtilIOS::showRank(IOS_KEY_RANK_HUNDRED);
        return;
    }
#endif
}


void MainScene::scrollViewDidScroll(ScrollView* view)
{
    
}

void MainScene::scrollViewDidZoom(ScrollView* view)
{
    
}

void MainScene::tableCellTouched(TableView* table, TableViewCell* cell)
{
    
}

Size MainScene::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size(cellWidth, cellHeight);
}

TableViewCell* MainScene::tableCellAtIndex(TableView *table, ssize_t idx)
{
    TableViewCell *cell = table->dequeueCell();
    if (cell) {
        cell->removeFromParentAndCleanup(true);
    }
    
    cell = new TableViewCell();
    cell->autorelease();
    Size size = Size(cellWidth-40, cellHeight-60);
    GameTool::addTextBg(cell, size, Point(cellWidth/2, cellHeight/2), ANCHOR_CENTER_CENTER, 10);
    GameKind *gk = listGameKind.at(idx);
    std::string title = gk->getTitle();
    GameTool::addLabelOutlineDefault(cell, title, 50, Point(40, size.height-15), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_UP, 20);
    char msg[200] = {0};
    memset(msg, 0, 200);
    sprintf(msg, "%s: %d", gk->getSubTitle1().c_str(), gk->getSubNum1());
    GameTool::addLabelOutlineDefault(cell, msg, 30, Point(40, size.height-100), Color4B(0, 100, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_UP, 20);
    Vector<MenuItem *> itemArray;
    
    auto item = GameTool::addBtn2(&itemArray, "进入", 32, Point(cellWidth/2-100, 40), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(MainScene::callbackRandomMatch, this));
    item->setTag(idx);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    item = GameTool::addBtn2(&itemArray, "排行榜", 32, Point(cellWidth/2+100, 40), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(MainScene::callbackRank, this));
    item->setTag(idx);
#endif
    
    KUtil::addMenu(cell, &itemArray, 30);
    return cell;
}

ssize_t MainScene::numberOfCellsInTableView(TableView *table)
{
    return listGameKind.size();
}

#pragma mark GameKind ---

GameKind* GameKind::create(int type, const std::string& title, const std::string& subTitle1, int subNum1)
{
    GameKind *gk = new GameKind();
    if (gk)
    {
        gk->autorelease();
        gk->_type = type;
        gk->_title = title;
        gk->_subTitle1 = subTitle1;
        gk->_subNum1 = subNum1;
        return gk;
    }
    CC_SAFE_DELETE(gk);
    return NULL;
}

GameKind::GameKind()
{
    
}

GameKind::~GameKind()
{
    
}

int GameKind::getType()
{
    return _type;
}

std::string GameKind::getTitle()
{
    return _title;
}

std::string GameKind::getSubTitle1()
{
    return _subTitle1;
}

int GameKind::getSubNum1()
{
    return _subNum1;
}



