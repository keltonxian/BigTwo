//
//  SceneBigTwo.cpp
//  BigTwo
//
//  Created by keltonxian on 2/6/15.
//
//

#include "SceneBigTwo.h"

#include "MainScene.h"
#include "GameTool.h"
#include "Ai.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#import "KUtilIOS.h"
#endif

#define SIDE_DOWN      1
#define SIDE_RIGHT     2
#define SIDE_UP        3
#define SIDE_LEFT      4
#define POKER_ZORDER_NORMAL   100
#define Y_DOWN_CARD_UNPRESS   140
#define Y_DOWN_CARD_PRESS     200
#define Y_PLAY_CARD           400
#define BTN_NULL       0
#define BTN_READY      1
#define BTN_PLAY       2
#define TAG_PLAYER_TALK   101
#define STATE_WAIT       0
#define STATE_DEAL       1
#define STATE_PLAY       2
#define STATE_RESULT     3
#define SCALE_POKER_TABLE 0.7

#pragma mark init ---
Scene* SceneBigTwo::createScene(int type)
{
    auto scene = Scene::create();
    auto layer = SceneBigTwo::create(type);
//    layer->setType(type);
    scene->addChild(layer, 1, 0);
    return scene;
}

SceneBigTwo* SceneBigTwo::create(int type)
{
    SceneBigTwo *sbt = new SceneBigTwo();
    if (sbt && sbt->init())
    {
        sbt->autorelease();
        sbt->setGameType(type);
        sbt->initPlayer();
        sbt->initTable();
        return sbt;
    }
    else
    {
        delete sbt;
        sbt = NULL;
        return NULL;
    }
}

void SceneBigTwo::setGameType(int type)
{
    _gameType = type;
}

bool SceneBigTwo::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    isCanTouch = true;
    Poker::getPack(&pack);
    setupPoker();
    sidePlayerX = 150;
    sidePlayerY = FULL_HEIGHT - 200;
    upPlayerX = HALF_WIDTH+100;
    upPlayerY = FULL_HEIGHT-120;
    downPlayerX = 90;
    downPlayerY = Y_PLAY_CARD-100;
    auto bg = KUtil::addSprite(this, KUtil::getPath(F_BG, "bg_3.png"), Point(HALF_WIDTH, 0), ANCHOR_CENTER_DOWN, 0);
    bg->setScale(KUtil::getScale(FULL_WIDTH, FULL_HEIGHT, bg->getContentSize(), true));
    
    auto table = KUtil::addSprite(this, KUtil::getPath(F_BG, "bg_4.png"), Point(HALF_WIDTH, 0), ANCHOR_CENTER_DOWN, 0);
    table->setScale(KUtil::getScale(FULL_WIDTH, FULL_HEIGHT, bg->getContentSize(), true));
    
    KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_coin.png"), Point(sidePlayerX-60, sidePlayerY-85), ANCHOR_LEFT_UP, 0);
    KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_coin.png"), Point(FULL_WIDTH-sidePlayerX-60, sidePlayerY-85), ANCHOR_LEFT_UP, 0);
    KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_coin.png"), Point(downPlayerX+65, downPlayerY+70), ANCHOR_LEFT_UP, 0);
    KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_coin.png"), Point(upPlayerX+65, upPlayerY+70), ANCHOR_LEFT_UP, 0);
    coinLeft = GameTool::addLabelOutlineDefault(this, "0", 30, Point(sidePlayerX-15, sidePlayerY-82), Color4B(255, 255, 0, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_UP, 10);
    coinRight = GameTool::addLabelOutlineDefault(this, "0", 30, Point(FULL_WIDTH-sidePlayerX-15, sidePlayerY-82), Color4B(255, 255, 0, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_UP, 10);
    coinDown = GameTool::addLabelOutlineDefault(this, "0", 30, Point(downPlayerX+105, downPlayerY+73), Color4B(255, 255, 0, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_UP, 10);
    coinUp = GameTool::addLabelOutlineDefault(this, "0", 30, Point(upPlayerX+105, upPlayerY+73), Color4B(255, 255, 0, 255), Color4B(0, 0, 0, 255), 2, ANCHOR_LEFT_UP, 10);
    
    coverLeft = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_cover.png"), Point(sidePlayerX-60, sidePlayerY-130), ANCHOR_LEFT_UP, 30);
    coverRight = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_cover.png"), Point(FULL_WIDTH-sidePlayerX-60, sidePlayerY-130), ANCHOR_LEFT_UP, 30);
    coverUp = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "icon_cover.png"), Point(upPlayerX+68, upPlayerY+10), ANCHOR_LEFT_UP, 30);
    
    countLeft = KUtil::addLabelBmf(this, "0", KUtil::getPath(F_FONT, "font_num_1.fnt"), Point(sidePlayerX-20, sidePlayerY-130+5), ANCHOR_LEFT_UP, 30);
    countLeft->setScale(2);
    
    countRight = KUtil::addLabelBmf(this, "0", KUtil::getPath(F_FONT, "font_num_1.fnt"), Point(FULL_WIDTH-sidePlayerX-20, sidePlayerY-130+5), ANCHOR_LEFT_UP, 30);
    countRight->setScale(2);
    
    countUp = KUtil::addLabelBmf(this, "0", KUtil::getPath(F_FONT, "font_num_1.fnt"), Point(upPlayerX+68+40, upPlayerY+10+5), ANCHOR_LEFT_UP, 30);
    countUp->setScale(2);
    
    int btnFontSize = 32;
    int btnY = Y_DOWN_CARD_UNPRESS+150;
    int btnOffsetX = 200;
    Vector<MenuItem *> itemArray;
    
    btnReady = GameTool::addBtn2(&itemArray, "准备", btnFontSize, Point(HALF_WIDTH, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(SceneBigTwo::callbackReady, this));
    btnReady->setVisible(false);
    
    btnPlay = GameTool::addBtn2(&itemArray, "出牌", btnFontSize, Point(HALF_WIDTH+btnOffsetX/2*3, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(SceneBigTwo::callbackPlay, this));
    btnPlay->setVisible(false);
    
    btnPass = GameTool::addBtn2(&itemArray, "不出", btnFontSize, Point(HALF_WIDTH-btnOffsetX/2*3, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(SceneBigTwo::callbackPass, this));
    btnPass->setVisible(false);
    
    btnTip = GameTool::addBtn2(&itemArray, "提示", btnFontSize, Point(HALF_WIDTH, btnY), ANCHOR_CENTER_DOWN, CC_CALLBACK_1(SceneBigTwo::callbackTip, this));
    btnTip->setVisible(false);
    
    KUtil::addItemSpriteSimple(&itemArray, KUtil::getPath(F_BTN, "btn_back.png"), Point(30, FULL_HEIGHT-10), ANCHOR_LEFT_UP, CC_CALLBACK_1(SceneBigTwo::callbackBack, this));
    
    KUtil::addMenu(this, &itemArray, 150);
    
    labelClock = KUtil::addLabelBmf(this, "0", KUtil::getPath(F_FONT, "font_num_1.fnt"), Point(FULL_WIDTH-10, FULL_HEIGHT-10), ANCHOR_RIGHT_UP, 10);
    labelClock->setScale(1.2);
    updateClock(0);
    schedule(schedule_selector(SceneBigTwo::updateClock), 1);
    
    showCount(false);
    
    return true;
}

SceneBigTwo::SceneBigTwo():lastTouchPoker(NULL)
{
    
}

SceneBigTwo::~SceneBigTwo()
{
    
}

void SceneBigTwo::onEnter()
{
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(SceneBigTwo::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(SceneBigTwo::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(SceneBigTwo::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void SceneBigTwo::onExit()
{
    Layer::onExit();
}

bool SceneBigTwo::onTouchBegan(Touch* touch, Event* event)
{
    if (false == isCanTouch) {
        return true;
    }
    checkTouchPoker(touch);
    return true;
}

void SceneBigTwo::onTouchMoved(Touch* touch, Event* event)
{
    if (false == isCanTouch) {
        return;
    }
    checkTouchPoker(touch);
}

void SceneBigTwo::onTouchEnded(Touch* touch, Event* event)
{
    if (false == isCanTouch) {
        return;
    }
    lastTouchPoker = NULL;
}

#pragma mark ---
#pragma mark setup ---
void SceneBigTwo::setupPoker()
{
    ssize_t total = pack.size();
    auto defaultPos = Point(HALF_WIDTH, HALF_HEIGHT);
    
    for (int i = 0; i < total; i++) {
        auto poker = pack.at(i);
        poker->setPosition(defaultPos);
        poker->setScale(SCALE_X);
        poker->setAnchorPoint(ANCHOR_CENTER_CENTER);
        poker->setTag((int)total-i);
        poker->setVisible(false);
        this->addChild(poker, POKER_ZORDER_NORMAL+1);
    }
}

void SceneBigTwo::addPlayer(Player *player)
{
    playerList.pushBack(player);
    Point position;
    if (player->getIcon() < 1 || player->getIcon() > 5) {
        CCLOG("ERROR DouDiZhu::addPlayer iconType icon(%d)", player->getIcon());
        return;
    }
    char iconType[20] = {0};
    memset(iconType, 0, 20);
    sprintf(iconType, "icon_%d.png", player->getIcon());
    switch (player->getSide()) {
        case SIDE_DOWN:
        {
            position = Point(downPlayerX, downPlayerY);
        }
            break;
        case SIDE_LEFT:
        {
            position = Point(sidePlayerX, sidePlayerY);
        }
            break;
        case SIDE_UP:
        {
            position = Point(upPlayerX, upPlayerY);
        }
            break;
        case SIDE_RIGHT:
        {
            position = Point(FULL_WIDTH-sidePlayerX, sidePlayerY);
        }
            break;
            
        default:
            CCLOG("ERROR DouDiZhu::addPlayer iconPos side(%d)", player->getSide());
            return;
    }
    auto frame = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "frame_icon.png"), position, ANCHOR_CENTER_CENTER, 10);
    auto icon = KUtil::addSprite(frame, KUtil::getPath(F_ICON, iconType), Point(61, 72), ANCHOR_CENTER_CENTER, -1);
    icon->setScale(0.95);
    GameTool::addLabelOutlineDefault(frame, player->getName(), 30, Point(63, 146), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_CENTER_DOWN, 1);
    player->setSprite(frame);
}

Player* SceneBigTwo::loadPlayerById(int pid, int side)
{
    auto path = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    ValueMap saveData = FileUtils::getInstance()->getValueMapFromFile(path);
    if (0 == saveData.size()) {
        return Player::createPlayer(pid, side, "路人", 1, (side==SIDE_DOWN)?false:true, 0);
    }
    char key[20] = {0};
    memset(key, 0, 20);
    sprintf(key, "%d", pid);
    ValueMap::iterator iter = saveData.find(key);
    if (iter == saveData.end()) {
        return Player::createPlayer(pid, side, "路人", 1, (side==SIDE_DOWN)?false:true, 0);
    }
    std::string s = saveData.at(key).asString();
    // 名字 ID 积分赛积分 冲百赛积分 冲百赛胜场
    char name[100] = {0};
    memset(name, 0, 100);
    int icon = 1;
    int point1 = 0;
    int point2 = 0;
    int point3 = 0;
    int point4 = 0;
    int point5 = 0;
    int point6 = 0;
    int point7 = 0;
    int point8 = 0;
    int ret = sscanf(s.c_str(), "%s %d %d %d %d %d %d %d %d %d", name, &icon, &point1, &point2, &point3, &point4, &point5, &point6, &point7, &point8);
    if (10 != ret) {
        return Player::createPlayer(pid, side, "路人", 1, (side==SIDE_DOWN)?false:true, 0);
    }
    if (_gameType == BIGTWO_TYPE_POINT) {
        CCLOG("side[%d] point[%d]", side, point1);
        auto player = Player::createPlayer(pid, side, name, icon, (side==SIDE_DOWN)?false:true, point1);
        player->setPointVar(point1, point2, point3, point4, point5, point6, point7, point8);
        return player;
    }
    if (_gameType == BIGTWO_TYPE_100) {
        auto player = Player::createPlayer(pid, side, name, icon, (side==SIDE_DOWN)?false:true, point2);
        player->setPointVar(point1, point2, point3, point4, point5, point6, point7, point8);
        return player;
    }
    return Player::createPlayer(pid, side, name, icon, (side==SIDE_DOWN)?false:true, 0);
}

void SceneBigTwo::initPlayer()
{
    addPlayer(loadPlayerById(10001, SIDE_DOWN));
    addPlayer(loadPlayerById(10002, SIDE_RIGHT));
    addPlayer(loadPlayerById(10003, SIDE_LEFT));
    addPlayer(loadPlayerById(10004, SIDE_UP));
}

Player* SceneBigTwo::getPlayer(int side)
{
    ssize_t total = playerList.size();
    for (int i = 0; i < total; i++) {
        auto player = playerList.at(i);
        if (player->getSide() == side) {
            return player;
        }
    }
    return NULL;
}

void SceneBigTwo::initTable()
{
    Poker::randomPack(&pack);
    auto defaultPos = Point(HALF_WIDTH, HALF_HEIGHT);
    for (int i = 0; i < pack.size(); i++) {
        auto poker = pack.at(i);
        poker->setPosition(defaultPos);
        poker->setScale(1);
        poker->setAnchorPoint(ANCHOR_CENTER_CENTER);
        poker->setTag((int)pack.size()-i);
        poker->setVisible(false);
    }
    updatePlayerCoin();
    _state = STATE_WAIT;
    winSide = -1;
    roundCount = 0;
    firstHand = SIDE_LEFT;//randomFirstHand();
    currentSide = firstHand;
    logic();
}

void SceneBigTwo::cleanTable()
{
    showCount(false);
    removeAllTalk();
    getPlayer(SIDE_DOWN)->cleanPokerList();
    getPlayer(SIDE_RIGHT)->cleanPokerList();
    getPlayer(SIDE_UP)->cleanPokerList();
    getPlayer(SIDE_LEFT)->cleanPokerList();
}

void SceneBigTwo::restart()
{
    cleanTable();
    initTable();
}

void SceneBigTwo::updatePlayerCoin()
{
    char numStr[50] = {0};
    memset(numStr, 0, 50);
    sprintf(numStr, "%d", getPlayer(SIDE_LEFT)->getPoint());
    coinLeft->setString(numStr);
    memset(numStr, 0, 50);
    sprintf(numStr, "%d", getPlayer(SIDE_RIGHT)->getPoint());
    coinRight->setString(numStr);
    memset(numStr, 0, 50);
    sprintf(numStr, "%d", getPlayer(SIDE_DOWN)->getPoint());
    coinDown->setString(numStr);
    memset(numStr, 0, 50);
    sprintf(numStr, "%d", getPlayer(SIDE_UP)->getPoint());
    coinUp->setString(numStr);
}

void SceneBigTwo::dealPoker()
{
    isCanTouch = false;
    Player *player = getPlayer(SIDE_DOWN);
    ssize_t total = pack.size();
    auto size = pack.at(0)->getSizeScale();
    float delay = 0;
    int index = (int)total-1;
    for (int i = index; i >= 13; i--, index--) {
        auto poker = pack.at(i);
        delay = moveSideAnimation(poker, delay, i%3);
    }
    for (int i = index; i >= 0; i--, index--) {
        auto poker = pack.at(i);
        player->insertPoker(poker);
    }
    player->sortPokerList();
    auto list = *(player->getPokerList());
    int len = player->getPokerCount();
    float hwidth = (FULL_WIDTH-size.width) / (len-1);
    if (hwidth > size.width/3*2) {
        hwidth = size.width/3*2;
    }
    float startx = HALF_WIDTH - (hwidth*(len-1)+size.width)/2;
    float speed = 250;
    float timeWaitTouch = 0;
    for (int i = 0; i < len; i++) {
        auto poker = list.at(i);
        poker->setVisible(true);
        poker->setTag(i);
        this->reorderChild(poker, i);
        poker->setPosition(Point(HALF_WIDTH, Y_DOWN_CARD_UNPRESS));
        Point pos = Point(startx + size.width/2 + i * hwidth, Y_DOWN_CARD_UNPRESS);
        auto time = abs(pos.x-HALF_WIDTH) / speed;
        if (time > timeWaitTouch) {
            timeWaitTouch = time;
        }
        this->moveAnimation(poker, pos, (9-abs(9-i))/10, time);
        if (0 == i) {
            delayCallLogic(poker, timeWaitTouch);
        }
    }
    
    scheduleOnce(schedule_selector(SceneBigTwo::callbackEnableTouch), timeWaitTouch);
}

float SceneBigTwo::moveSideAnimation(Sprite *sprite, float delay, int sideFlag)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    if (0 == sideFlag) {
        action = CallFuncN::create(CC_CALLBACK_1(SceneBigTwo::callbackMoveRightSide, this));
    } else if (1 == sideFlag) {
        action = CallFuncN::create(CC_CALLBACK_1(SceneBigTwo::callbackMoveUpSide, this));
    } else {
        action = CallFuncN::create(CC_CALLBACK_1(SceneBigTwo::callbackMoveLeftSide, this));
    }
    list.pushBack(action);
    action = Sequence::create(list);
    sprite->runAction(action);
    
    return delay + 0.05;
}

void SceneBigTwo::callbackMoveRightSide(Node* pSender)
{
    Player *player = getPlayer(SIDE_RIGHT);
    Poker *poker = (Poker *)pSender;
    poker->setVisible(false); // for sortPokerSprite
    poker->setPosition(player->getSprite()->getPosition());
    player->insertPoker(poker);
    updatePlayerPokerCount(player, countRight);
}

void SceneBigTwo::callbackMoveUpSide(Node* pSender)
{
    Player *player = getPlayer(SIDE_UP);
    Poker *poker = (Poker *)pSender;
    poker->setVisible(false); // for sortPokerSprite
    poker->setPosition(player->getSprite()->getPosition());
    player->insertPoker(poker);
    updatePlayerPokerCount(player, countUp);
}

void SceneBigTwo::callbackMoveLeftSide(Node* pSender)
{
    Player *player = getPlayer(SIDE_LEFT);
    Poker *poker = (Poker *)pSender;
    poker->setVisible(false); // for sortPokerSprite
    poker->setPosition(player->getSprite()->getPosition());
    player->insertPoker(poker);
    updatePlayerPokerCount(player, countLeft);
}

void SceneBigTwo::updatePlayerPokerCount(Player *player, Label *label)
{
    char count[5];
    memset(count, 0, 5);
    sprintf(count, "%d", player->getPokerCount());
    label->setString(count);
}

float SceneBigTwo::moveAnimation(Sprite *sprite, Point targetPos, float delay, float time)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(SceneBigTwo::callbackMove, this));
    list.pushBack(action);
    action = MoveTo::create(time, targetPos);
    list.pushBack(action);
    action = Sequence::create(list);
    sprite->runAction(action);
    
    return delay + time;
}

void SceneBigTwo::callbackMove(Node* pSender)
{
    int tag = pSender->getTag();
    this->reorderChild(pSender, POKER_ZORDER_NORMAL+tag);
}

void SceneBigTwo::showCount(bool isShow)
{
    coverLeft->setVisible(isShow);
    coverRight->setVisible(isShow);
    coverUp->setVisible(isShow);
    countLeft->setVisible(isShow);
    countRight->setVisible(isShow);
    countUp->setVisible(isShow);
}
#pragma mark ---
#pragma mark logic ---
void SceneBigTwo::logic()
{
    roundCount++;
    int state = _state;
    if (roundCount > 4 && _state != STATE_PLAY) {
        state = nextState();
        if (state == STATE_DEAL) {
            roundCount = 0;
            lastCardType = 0;
            lastNum = 0;
            lastFace = 0;
            lastPlayListSize = 0;
            lastPlaySide = -1;
            playList.clear();
            sideDownPlayedList.clear();
            sideLeftPlayedList.clear();
            sideRightPlayedList.clear();
            sideUpPlayedList.clear();
            showCount(true);
            dealPoker();
            // make roundCount > 4, then recall logic will go state_play
            roundCount = 5;
            firstHand = -99;
//            firstHand = getFirstHand();
//            currentSide = firstHand;
//            currentSide = (firstHand==SIDE_DOWN)?SIDE_LEFT:(firstHand-1);
            return;
        }
        if (_state == STATE_PLAY) {
            removeAllTalk();
        }
    }
    if (_state == STATE_PLAY) {
        if (true == checkGameOver()) {
            isCanTouch = false;
            this->scheduleOnce(schedule_selector(SceneBigTwo::showResult), 0.2);
            return;
        }
    }
    Player *player;
    if (state == STATE_PLAY && -99 == firstHand) {
        firstHand = getFirstHand();
        currentSide = firstHand;
        player = getPlayer(currentSide);
    } else {
        player = nextPlayer();
    }
    if (state == STATE_PLAY) {
        removeTalk(player->getSide());
        if (-1 != lastPlaySide && lastPlaySide == player->getSide()) {
            lastCardType = 0;
            lastNum = 0;
            lastFace = 0;
            lastPlayListSize = 0;
        }
    }
    if (false == player->isAi()) {
        if (state == STATE_WAIT) {
            showBtn(BTN_READY);
        } else if (state == STATE_PLAY) {
            hidePlayedPoker(sideDownPlayedList);
            showBtn(BTN_PLAY);
        }
        return;
    }
    showBtn(BTN_NULL);
    if (state == STATE_WAIT) {
        showTalk(player, "准备!!");
        return;
    }
    
    if (state == STATE_PLAY) {
        aiPlay(player);
        return;
    }
}

void SceneBigTwo::callbackLogic(Node* pSender)
{
    logic();
}

float SceneBigTwo::delayCallLogic(Sprite *sprite, float delay)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(SceneBigTwo::callbackLogic, this));
    list.pushBack(action);
    action = Sequence::create(list);
    sprite->runAction(action);
    
    return delay;
}

int SceneBigTwo::getFirstHand()
{
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        auto plist = player->getPokerList();
        for (int j = 0; j < plist->size(); j++) {
            auto poker = plist->at(j);
            if (poker->getNum() == POKER_NUM::THREE && poker->getFace() == POKER_FACE::DIAMOND) {
                return player->getSide();
            }
        }
    }
    return SIDE_DOWN;
}

bool SceneBigTwo::checkGameOver()
{
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        auto len = (*player->getPokerList()).size();
        if (0 == len) {
            winSide = player->getSide();
            return true;
        }
    }
    return false;
}

void SceneBigTwo::showResult(float delta)
{
    if (_gameType == BIGTWO_TYPE_POINT) {
        int point = 0;
        int winSide = 0;
        for (int i = 0; i < playerList.size(); i++) {
            auto player = playerList.at(i);
            auto list = player->getPokerList();
            size_t size = list->size();
            if (0 == size) {
                winSide = player->getSide();
                continue;
            }
            player->addPoint(0);
            //余牌数小于8张，每张1分
            //余牌数8，9张，每张2分
            //余牌数10~12张，每张3分
            //余牌数13张，每张4分
            if (size < 8) {
                point += size * 1;
            } else if (size < 10) {
                point += size * 2;
            } else if (size < 13) {
                point += size * 3;
            } else if (size == 13) {
                point += size * 4;
            }
        }
        auto player = getPlayer(winSide);
        player->addPoint(point);
        auto layer = ResultLayer::create();
        layer->setData(point, winSide, playerList, CC_CALLBACK_1(SceneBigTwo::callbackResult, this));
        KUtil::addLayer(layer, 20, 0);
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        if (winSide == SIDE_DOWN) {
            KUtilIOS::insertRank(IOS_KEY_RANK_POINT, getPlayer(SIDE_DOWN)->getPoint());
        }
#endif
    } else if (_gameType == BIGTWO_TYPE_100) {
        int winSide = 0;
        int sideGet100 = -1;
        for (int i = 0; i < playerList.size(); i++) {
            int point = 0;
            auto player = playerList.at(i);
            auto list = player->getPokerList();
            size_t size = list->size();
            if (0 == size) {
                player->addPoint(0);
                winSide = player->getSide();
                continue;
            }
            //余牌数小于8张，每张1分
            //余牌数8，9张，每张2分
            //余牌数10~12张，每张3分
            //余牌数13张，每张4分
            if (size < 8) {
                point += size * 1;
            } else if (size < 10) {
                point += size * 2;
            } else if (size < 13) {
                point += size * 3;
            } else if (size == 13) {
                point += size * 4;
            }
            player->addPoint(point);
            if (player->getPoint() >= 100) {
                sideGet100 = player->getSide();
            }
        }
        int endWinSide = -1;
        int pMark = -1;
        if (-1 != sideGet100) {
            for (int i = 0; i < playerList.size(); i++) {
                auto player = playerList.at(i);
                if (-1 == pMark || player->getPoint() < pMark) {
                    pMark = player->getPoint();
                    endWinSide = player->getSide();
                }
                player->setPoint(0);
                if (player->getSide() == sideGet100) {
                    continue;
                }
                player->point3 += 1;
            }
        }
        auto layer = ResultLayer::create();
        layer->setDataHundred(sideGet100, endWinSide, winSide, playerList, CC_CALLBACK_1(SceneBigTwo::callbackResult, this));
        KUtil::addLayer(layer, 20, 0);
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        if (-1 != sideGet100 && winSide == SIDE_DOWN) {
            KUtilIOS::insertRank(IOS_KEY_RANK_HUNDRED, getPlayer(SIDE_DOWN)->getHundredWinCount()+1);
        }
#endif
    }
    saveGame();
}

void SceneBigTwo::callbackResult(Ref* pSender)
{
    restart();
    isCanTouch = true;
}

Player* SceneBigTwo::getNextPlayer(Player *player)
{
    //nextPlayer();
    /**
     #define SIDE_DOWN      1
     #define SIDE_RIGHT     2
     #define SIDE_UP        3
     #define SIDE_LEFT      4
     */
    int cSide = player->getSide();
    int begin = SIDE_DOWN;
    int end = SIDE_LEFT;
    cSide = (++cSide)>end?begin:cSide;
    Player *nextPlayer = getPlayer(cSide);
    return nextPlayer;
}

void SceneBigTwo::aiPlay(Player *player)
{
    if (player->getSide() == SIDE_LEFT) {
        hidePlayedPoker(sideLeftPlayedList);
    } else if (player->getSide() == SIDE_UP) {
        hidePlayedPoker(sideUpPlayedList);
    } else { // SIDE_RIGHT
        hidePlayedPoker(sideRightPlayedList);
    }
//    CCLOG("AiPlay start----------");
    Vector<Poker *> tipPokerList;
    Vector<Poker *> pokerList = *(player->getPokerList());
    //---- get poker to play
    CardType *cardType = CardType::create(lastCardType, lastNum, lastFace);
    int ret = Ai::choosePoker(pokerList, pack, cardType, lastPlayListSize, 0, &tipPokerList, getNextPlayer(player)->getPokerCount());
    size_t size = tipPokerList.size();
    if (0 != ret || 0 == size) {
        showTalk(player, "不出!!");
        return;
    }
    std::sort( std::begin(tipPokerList), std::end(tipPokerList), Player::PokerListComparisonLess );
    //----
    //---- get played poker type
    CardType *ct = Ai::validCardsAndGetCardType(CardType::create(lastCardType, lastNum, lastFace), lastPlayListSize, tipPokerList);
    if (ct->isIllegeal() || ct->isNone()) {
        showTalk(player, "不出!!");
        return;
    }
//    CardType *ct = Ai::getCardType(tipPokerList);
    lastCardType = ct->getCardType();
    lastNum = ct->getNum();
    lastFace = ct->getFace();
    lastPlayListSize = (int)tipPokerList.size();
//    CCLOG("AiPlay end----------");
    //----
    auto width = 0;
    if (size > 0) {
        width = tipPokerList.at(0)->getSizeScale().width;
    }
    float gapy = width / 3;
    //
    float offsetx = 170;
    float offsety = -80;
    for (int i = 0; i < size; i++) {
        Poker *poker = tipPokerList.at(i);
        poker->setVisible(true);
        poker->setScale(SCALE_POKER_TABLE);
        player->removePoker(poker);
        Point position;
        if (player->getSide() == SIDE_LEFT) {
            sideLeftPlayedList.pushBack(poker);
            updatePlayerPokerCount(player, countLeft);
            position = Point(sidePlayerX + offsetx + gapy*i, sidePlayerY+offsety);
        } else if (player->getSide() == SIDE_UP) {
            sideUpPlayedList.pushBack(poker);
            updatePlayerPokerCount(player, countUp);
            position = Point(upPlayerX - offsetx - gapy*(size-1-i), upPlayerY-80);
        } else { // SIDE_RIGHT
            sideRightPlayedList.pushBack(poker);
            updatePlayerPokerCount(player, countRight);
            position = Point(FULL_WIDTH - sidePlayerX - offsetx - gapy*(size-1-i), sidePlayerY+offsety);
        }
        poker->setPosition(position);
    }
    sortPlaySprite(tipPokerList);
    sortPokerSprite(player, 0.5, true);
    lastPlaySide = player->getSide();
}

void SceneBigTwo::showBtn(int btnType)
{
    switch (btnType) {
        case BTN_READY:
        {
            btnReady->setVisible(true);
            btnPlay->setVisible(false);
            btnPass->setVisible(false);
            btnTip->setVisible(false);
        }
            break;
        case BTN_PLAY:
        {
            btnReady->setVisible(false);
            btnPlay->setVisible(true);
            btnPass->setVisible(true);
            btnTip->setVisible(true);
        }
            break;
            
        default:
        {
            btnReady->setVisible(false);
            btnPlay->setVisible(false);
            btnPass->setVisible(false);
            btnTip->setVisible(false);
        }
            break;
    }
}

void SceneBigTwo::updateClock(float delta)
{
    struct timeval nowTimeval;
    gettimeofday(&nowTimeval, NULL);
    struct tm * tm;
    time_t time_sec ;
    time_sec = nowTimeval.tv_sec;
    tm = localtime(&time_sec);
    int nMinute = tm->tm_min;
    int nSecond = tm->tm_sec;
    int nHour = tm->tm_hour;
    char timeStr[50] = {0};
    memset(timeStr, 0, 50);
    sprintf(timeStr, "%02d:%02d:%02d", nHour, nMinute, nSecond);
    labelClock->setString(timeStr);
}

int SceneBigTwo::nextState()
{
    roundCount = 1;
    int begin = STATE_WAIT;
    int end = STATE_RESULT;
    _state = (++_state)>end?begin:_state;
    return _state;
}

void SceneBigTwo::checkTouchPoker(Touch* touch)
{
    Player *playerDown = getPlayer(SIDE_DOWN);
    auto list = *(playerDown->getPokerList());
    for (int i = (int)list.size()-1; i >= 0; i--) {
        auto poker = list.at(i);
        if (!poker->isInRect(touch)) {
            continue;
        }
        if (poker == lastTouchPoker) {
            break;
        }
        bool isPress = poker->isPress();
        pokerUp(poker, !isPress);
        lastTouchPoker = poker;
        break;
    }
}

void SceneBigTwo::pokerUp(Poker *poker, bool isSetUp)
{
    if (isSetUp == poker->isPress()) {
        return;
    }
    if (isSetUp) {
        poker->setPositionY(Y_DOWN_CARD_PRESS);
    } else {
        poker->setPositionY(Y_DOWN_CARD_UNPRESS);
    }
    updatePlayList(poker, isSetUp);
    poker->setPress(isSetUp);
}

void SceneBigTwo::updatePlayList(Poker *poker, bool isAdd)
{
    if (true == isAdd) {
        playList.pushBack(poker);
        return;
    }
    int index = -1;
    for (int i = 0; i < playList.size(); i++) {
        auto p = playList.at(i);
        if (p->getPtype() == poker->getPtype()) {
            index = i;
            break;
        }
    }
    if (-1 == index) {
        return;
    }
    playList.erase(index);
}

void SceneBigTwo::resetPlayList()
{
    for (int i = 0; i < playList.size(); i++) {
        auto poker = playList.at(i);
        poker->setPositionY(Y_DOWN_CARD_UNPRESS);
        poker->setPress(false);
    }
    playList.clear();
}

Player* SceneBigTwo::nextPlayer()
{
    /**
     #define SIDE_DOWN      1
     #define SIDE_RIGHT     2
     #define SIDE_UP        3
     #define SIDE_LEFT      4
     */
    int begin = SIDE_DOWN;
    int end = SIDE_LEFT;
    currentSide = (++currentSide)>end?begin:currentSide;
    Player *player = getPlayer(currentSide);
    return player;
}

void SceneBigTwo::hidePlayedPoker(Vector<Poker *> playedList)
{
    for (int i = 0; i < playedList.size(); i++) {
        auto poker = playedList.at(i);
        poker->setVisible(false);
    }
}
#pragma mark ---
#pragma mark function ---
void SceneBigTwo::showTalk(Player *player, const char *talk)
{
    if (roundCount == 1) {
        removeAllTalk();
    }
    auto sprite = player->getSprite();
    Point position;
    Point anchorPoint;
    if (player->getSide()==SIDE_RIGHT) {
        position = Point(0, 73);
        anchorPoint = ANCHOR_RIGHT_CENTER;
    } else if (player->getSide()==SIDE_LEFT) {
        position = Point(126, 73);
        anchorPoint = ANCHOR_LEFT_CENTER;
    } else if (player->getSide()==SIDE_UP) {
        position = Point(0, 73);
        anchorPoint = ANCHOR_RIGHT_CENTER;
    } else {
        position = Point(63, 73);
        anchorPoint = ANCHOR_CENTER_DOWN;
    }
    auto label = GameTool::addLabelOutlineDefault(sprite, talk, 30, position, Color4B(0, 255, 0, 255), Color4B(100, 100, 100, 255), 2, anchorPoint, 5);
    label->setTag(TAG_PLAYER_TALK);
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = ScaleTo::create(0.1, 2);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(SceneBigTwo::callbackLogic, this));
    list.pushBack(action);
    action = Sequence::create(list);
    label->runAction(action);
}

void SceneBigTwo::removeTalk(int side)
{
    auto player = getPlayer(side);
    if (NULL == player) {
        return;
    }
    auto sprite = player->getSprite()->getChildByTag(TAG_PLAYER_TALK);
    if (nullptr == sprite) {
        return;
    }
    sprite->removeFromParentAndCleanup(true);
}

void SceneBigTwo::removeAllTalk()
{
    for (int i = SIDE_DOWN; i <= SIDE_LEFT; i++) {
        removeTalk(i);
    }
}

int SceneBigTwo::randomFirstHand()
{
    srand((int)time(NULL));
    ssize_t m = rand() % 100;
    if (0 <= m && m < 25) {
        return SIDE_LEFT;
    }
    if (25 <= m && m < 50) {
        return SIDE_RIGHT;
    }
    if (50 <= m && m < 75) {
        return SIDE_UP;
    }
    return SIDE_DOWN;
}

void SceneBigTwo::callbackReady(cocos2d::Ref* pSender)
{
    showTalk(getPlayer(SIDE_DOWN), "准备!!");
}

void SceneBigTwo::callbackPlay(cocos2d::Ref* pSender)
{
    if (0 == playList.size()) {
        showTip("请选择要出的牌");
        return;
    }
    
    std::sort( std::begin(playList), std::end(playList), Player::PokerListComparisonLess );
    //---- get played poker type
    CardType *cardType = Ai::validCardsAndGetCardType(CardType::create(lastCardType, lastNum, lastFace), lastPlayListSize, playList);
    if (cardType->isIllegeal() || cardType->isNone()) {
        showTip("出牌不合规则");
        return;
    }
    lastCardType = cardType->getCardType();
    lastNum = cardType->getNum();
    lastFace = cardType->getFace();
    lastPlayListSize = (int)playList.size();
    //----
    
    showBtn(BTN_NULL);
    
    size_t len = playList.size();
    auto width = 0;
    if (len > 0) {
        width = playList.at(0)->getSizeScale().width;
    }
    float gapy = width / 3;
    auto player = getPlayer(SIDE_DOWN);
    float startX = HALF_WIDTH - ((gapy * (len-1) + width))/2;
    for (int i = 0; i < playList.size(); i++) {
        auto poker = playList.at(i);
        //        CCLOG("play p[%d,%d]", poker->getFace(), poker->getNum());
        player->removePoker(poker);
        sideDownPlayedList.pushBack(poker);
        Point pos = Point(startX+width/2+gapy*i, Y_PLAY_CARD);
        moveScaleAnimation(poker, pos, SCALE_POKER_TABLE, 0, 0.2);
    }
    sortPlaySprite(playList);
    sortPokerSprite(player, 0, true);
    playList.clear();
    lastPlaySide = SIDE_DOWN;
}

void SceneBigTwo::callbackPass(cocos2d::Ref* pSender)
{
    if (-1 == lastPlaySide) {
        showTip("请出牌");
        return;
    }
    auto player = getPlayer(SIDE_DOWN);
    if (-1 != lastPlaySide && lastPlaySide == player->getSide()) {
        showTip("请出牌");
        return;
    }
    for (int i = 0; i < playList.size(); i++) {
        auto poker =playList.at(i);
        pokerUp(poker, false);
    }
    playList.clear();
    showTalk(player, "不出!!");
}

void SceneBigTwo::callbackTip(cocos2d::Ref* pSender)
{
    Vector<Poker *> tipPokerList;
    Vector<Poker *> pokerList = *(getPlayer(SIDE_DOWN)->getPokerList());
    for (int i = 0; i < pokerList.size(); i++) {
        Poker *poker = pokerList.at(i);
        pokerUp(poker, false);
    }
    //---- get poker to play
    CardType *cardType = CardType::create(lastCardType, lastNum, lastFace);
    int ret = Ai::choosePoker(pokerList, pack, cardType, lastPlayListSize, 0, &tipPokerList, getNextPlayer(getPlayer(SIDE_DOWN))->getPokerCount());
    size_t size = tipPokerList.size();
    if (0 != ret || 0 == size) {
        callbackPass(nullptr);
        return;
    }
//    std::sort( std::begin(tipPokerList), std::end(tipPokerList), Player::PokerListComparisonLess );
//    for (int i = 0; i < tipPokerList.size(); i++) {
//        auto p = tipPokerList.at(i);
//        CCLOG("p num[%d] face[%d]", p->getNum(), p->getFace());
//    }
    //----
    //---- get played poker type
    CardType *ct = Ai::validCardsAndGetCardType(CardType::create(lastCardType, lastNum, lastFace), lastPlayListSize, tipPokerList);
    if (ct->isIllegeal() || ct->isNone()) {
        callbackPass(nullptr);
        return;
    }
    for (int i = 0; i < tipPokerList.size(); i++) {
        Poker *poker = tipPokerList.at(i);
        pokerUp(poker, true);
    }
}

void SceneBigTwo::callbackBack(cocos2d::Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionSlideInB::create(0.4, MainScene::createScene()));
}

void SceneBigTwo::callbackRemove(Node* pSender)
{
    pSender->removeFromParentAndCleanup(true);
}

void SceneBigTwo::showTip(const char *tip)
{
    auto size = Size(300, 150);
    auto bg = GameTool::addTextBg(this, size, Point(HALF_WIDTH, HALF_HEIGHT), ANCHOR_CENTER_CENTER, 100);
    GameTool::addLabelOutlineDefault(bg, tip, 30, Point(0, 0), Color4B(255, 255, 255, 255), Color4B(100, 100, 100, 255), 2, ANCHOR_LEFT_DOWN, 5, size, TextHAlignment::CENTER, TextVAlignment::CENTER);
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = MoveBy::create(0.5, Point(0, 50));
    list.pushBack(action);
    action = DelayTime::create(0.5);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(SceneBigTwo::callbackRemove, this));
    list.pushBack(action);
    action = Sequence::create(list);
    bg->runAction(action);
}

void SceneBigTwo::callbackEnableTouch(float delta)
{
    isCanTouch = true;
}

float SceneBigTwo::moveScaleAnimation(Sprite *sprite, Point targetPos, float scaleTo, float delay, float time)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    Vector<FiniteTimeAction *> listSpawn;
    action = MoveTo::create(time, targetPos);
    listSpawn.pushBack(action);
    action = ScaleTo::create(time, scaleTo);
    listSpawn.pushBack(action);
    action = Spawn::create(listSpawn);
    list.pushBack(action);
    action = Sequence::create(list);
    sprite->runAction(action);
    
    return delay + time;
}

void SceneBigTwo::sortPlaySprite(Vector<Poker *> list)
{
    for (int i = 0; i < list.size(); i++) {
        auto poker = list.at(i);
        this->reorderChild(poker, POKER_ZORDER_NORMAL+i);
    }
}

float SceneBigTwo::sortPokerSprite(Player *player, float delay, bool callLogic)
{
    player->sortPokerList();
    
    int len = player->getPokerCount();
    auto list = *(player->getPokerList());
    if (player->getSide() != SIDE_DOWN) {
        if (true == callLogic) {
            delayCallLogic(player->getSprite(), delay);
        }
        return delay;
    }
    Size size;
    if (list.size() > 0) {
        size = list.at(0)->getSizeScale();
    } else {
        size = pack.at(0)->getSizeScale();
    }
    float hwidth = (FULL_WIDTH-size.width) / (len-1);
    if (hwidth > size.width/3*2) {
        hwidth = size.width/3*2;
    }
    float startx = HALF_WIDTH - (hwidth*(len-1)+size.width)/2;
    for (int i = 0; i < len; i++) {
        auto poker = list.at(i);
        poker->setTag(i);
        Point pos = Point(startx + size.width/2 + i * hwidth, Y_DOWN_CARD_UNPRESS);
        this->moveAnimation(poker, pos, delay, 0.5);
    }
    if (true == callLogic) {
        delay += 1;
        delayCallLogic(player->getSprite(), delay);
    }
    return delay;
}

void SceneBigTwo::saveGame()
{
    int winCount = 0;
    int loseCount = 0;
    auto path = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    ValueMap loadData = FileUtils::getInstance()->getValueMapFromFile(path);
    ValueMap::iterator iter = loadData.find("WinCount");
    if (iter != loadData.end()) {
        winCount = loadData.at("WinCount").asInt();
    }
    iter = loadData.find("LoseCount");
    if (iter != loadData.end()) {
        loseCount = loadData.at("LoseCount").asInt();
    }
    
    ValueMap saveData;
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        int p1 = player->point1;
        int p2 = player->point2;
        int p3 = player->point3;
        int p4 = player->point4;
        int p5 = player->point5;
        int p6 = player->point6;
        int p7 = player->point7;
        int p8 = player->point8;
        
        char key[20] = {0};
        memset(key, 0, 20);
        sprintf(key, "%d", player->getPid());
        char v[600] = {0};
        memset(v, 0, 600);
        if (_gameType == BIGTWO_TYPE_POINT) {
            sprintf(v, "%s %d %d %d %d %d %d %d %d %d", player->getName().c_str(), player->getIcon(), player->getPoint(), p2, p3, p4, p5, p6, p7, p8);
        } else if (_gameType == BIGTWO_TYPE_100) {
            sprintf(v, "%s %d %d %d %d %d %d %d %d %d", player->getName().c_str(), player->getIcon(), p1, player->getPoint(), p3, p4, p5, p6, p7, p8);
        } else {
            sprintf(v, "%s %d %d %d %d %d %d %d %d %d", player->getName().c_str(), player->getIcon(), p1, p2, p3, p4, p5, p6, p7, p8);
        }
        Value value = Value(v);
        saveData.insert(make_pair(key, value));
    }
    
    if (winSide == SIDE_DOWN) {
        winCount++;
    } else {
        loseCount++;
    }
    saveData.insert(make_pair("WinCount", Value(winCount)));
    saveData.insert(make_pair("LoseCount", Value(loseCount)));
    
    //    auto path = FileUtils::getInstance()->getWritablePath() + SAVE_DATA_1;
    FileUtils::getInstance()->writeToFile(saveData, path);
}

bool SceneBigTwo::loadGame()
{
    return false;
}

#pragma mark ---
#pragma mark ResultLayer ---
bool ResultLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    auto layerColor = LayerColor::create(Color4B(0, 0, 0, 200), FULL_WIDTH, FULL_HEIGHT);
    layerColor->setPosition(Point(0, 0));
    this->addChild(layerColor);
    
    Vector<MenuItem *> itemArray;
    
    GameTool::addBtn2(&itemArray, "继续", 32, Point(FULL_WIDTH-100, HALF_HEIGHT), ANCHOR_RIGHT_CENTER, CC_CALLBACK_1(ResultLayer::callbackNext, this));
    
    KUtil::addMenu(this, &itemArray, 30);
    
    return true;
}

void ResultLayer::setData(int coin, int winSide, Vector<Player *> playerList, const ccMenuCallback& callback)
{
    _callback = callback;
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        if (player->getIcon() < 1 || player->getIcon() > 5) {
            CCLOG("ERROR DouDiZhu::addPlayer iconType icon(%d)", player->getIcon());
            continue;
        }
        char iconType[20] = {0};
        memset(iconType, 0, 20);
        Point position = Point(100, 50+180*i);
        sprintf(iconType, "icon_%d.png", player->getIcon());
        auto frame = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "frame_icon.png"), position, ANCHOR_LEFT_DOWN, 10);
        auto icon = KUtil::addSprite(frame, KUtil::getPath(F_ICON, iconType), Point(61, 72), ANCHOR_CENTER_CENTER, -1);
        icon->setScale(0.95);
        GameTool::addLabelOutlineDefault(frame, player->getName(), 30, Point(150, 0), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 1);
        
        Color4B color;
        char winStr[50] = {0};
        memset(winStr, 0, 50);
        if (winSide == player->getSide()) {
            sprintf(winStr, "%s", "胜利!!!");
            color = Color4B(0, 255, 0, 255);
        } else {
            sprintf(winStr, "%s", "失败!!!");
            color = Color4B(200, 200, 200, 255);
        }
        auto win = GameTool::addLabelOutlineDefault(frame, winStr, 50, Point(360, 0), color, Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);
        win->setRotation(-20);
        
        auto iconCoin = KUtil::addSprite(frame, KUtil::getPath(F_FRAME, "icon_coin.png"), Point(460, 0), ANCHOR_LEFT_DOWN, 0);
        iconCoin->setScale(2);
        
        char numStr[50] = {0};
        memset(numStr, 0, 50);
        sprintf(numStr, "+%d", player->getPointChanged());
        color = Color4B(255, 255, 0, 255);
        GameTool::addLabelOutlineDefault(frame, numStr, 50, Point(540, 0), color, Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);
    }
}

void ResultLayer::setDataHundred(int side100, int endWinSide, int winSide, Vector<Player *> playerList, const ccMenuCallback& callback)
{
    _callback = callback;
    for (int i = 0; i < playerList.size(); i++) {
        auto player = playerList.at(i);
        if (player->getIcon() < 1 || player->getIcon() > 5) {
            CCLOG("ERROR DouDiZhu::addPlayer iconType icon(%d)", player->getIcon());
            continue;
        }
        char iconType[20] = {0};
        memset(iconType, 0, 20);
        Point position = Point(100, 50+180*i);
        sprintf(iconType, "icon_%d.png", player->getIcon());
        auto frame = KUtil::addSprite(this, KUtil::getPath(F_FRAME, "frame_icon.png"), position, ANCHOR_LEFT_DOWN, 10);
        auto icon = KUtil::addSprite(frame, KUtil::getPath(F_FRAME, iconType), Point(61, 72), ANCHOR_CENTER_CENTER, -1);
        icon->setScale(0.95);
        GameTool::addLabelOutlineDefault(frame, player->getName(), 30, Point(150, 0), Color4B(0, 0, 0, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 1);
        
        Color4B color;
        char winStr[50] = {0};
        memset(winStr, 0, 50);
        if (side100 == player->getSide()) {
            sprintf(winStr, "%s", "破百啦!!!");
            color = Color4B(250, 0, 0, 255);
        } else if (winSide == player->getSide()) {
            sprintf(winStr, "%s", "胜利!!!");
            color = Color4B(0, 255, 0, 255);
        } else {
            sprintf(winStr, "%s", "失败!!!");
            color = Color4B(200, 200, 200, 255);
        }
        auto win = GameTool::addLabelOutlineDefault(frame, winStr, 50, Point(360, 0), color, Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);
        win->setRotation(-20);
        
        auto iconCoin = KUtil::addSprite(frame, KUtil::getPath(F_FRAME, "icon_coin.png"), Point(460, 0), ANCHOR_LEFT_DOWN, 0);
        iconCoin->setScale(2);
        
        char numStr[50] = {0};
        memset(numStr, 0, 50);
        sprintf(numStr, "+%d", player->getPointChanged());
        color = Color4B(255, 255, 0, 255);
        GameTool::addLabelOutlineDefault(frame, numStr, 50, Point(540, 0), color, Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);
        
        if (endWinSide == player->getSide()) {
            char endWinStr[50] = {0};
            memset(endWinStr, 0, 50);
            sprintf(endWinStr, "%s", "最终胜利者!!!");
            GameTool::addLabelOutlineDefault(frame, endWinStr, 50, Point(640, 0), color = Color4B(188, 255, 33, 255), Color4B(255, 255, 255, 255), 2, ANCHOR_LEFT_DOWN, 10);
        }
    }
}

void ResultLayer::callbackNext(cocos2d::Ref* pSender)
{
    if (nullptr == _callback) {
        return;
    }
    _callback(nullptr);
    //    removeFromParentAndCleanup(true);
    KUtil::removeLayer(this);
}

ResultLayer::ResultLayer()
{
    
}

ResultLayer::~ResultLayer()
{
    
}

void ResultLayer::onEnter()
{
    Layer::onEnter();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(ResultLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ResultLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ResultLayer::onTouchEnded, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void ResultLayer::onExit()
{
    Layer::onExit();
}

bool ResultLayer::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void ResultLayer::onTouchMoved(Touch* touch, Event* event)
{
    
}

void ResultLayer::onTouchEnded(Touch* touch, Event* event)
{
    
}
#pragma mark ---



