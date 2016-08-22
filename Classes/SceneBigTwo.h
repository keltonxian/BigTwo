//
//  SceneBigTwo.h
//  BigTwo
//
//  Created by keltonxian on 2/6/15.
//
//

#ifndef __BigTwo__SceneBigTwo__
#define __BigTwo__SceneBigTwo__

#include "GameHeader.h"

#include "Player.h"

#define BIGTWO_TYPE_POINT   1
#define BIGTWO_TYPE_100     2

class SceneBigTwo : public Layer {
public:
    static Scene* createScene(int type);
    virtual bool init() override;
//    CREATE_FUNC(SceneBigTwo);
    static SceneBigTwo* create(int type);
private:
    //init
    SceneBigTwo();
    virtual ~SceneBigTwo();
    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch* touch, Event* event) override;
    void onTouchMoved(Touch* touch, Event* event) override;
    void onTouchEnded(Touch* touch, Event* event) override;
    
    // var
    int _gameType;
    int _state;
    int winSide;
    int currentSide;
    int firstHand;
    bool isCanTouch;
    int sidePlayerX;
    int sidePlayerY;
    int upPlayerX;
    int upPlayerY;
    int downPlayerX;
    int downPlayerY;
    int roundCount;
    int lastCardType;
    int lastNum;
    int lastFace;
    int lastPlayListSize;
    int lastPlaySide;
    Vector<Poker *> pack;
    Vector<Player *> playerList;
    Vector<Poker *> playList;
    Vector<Poker *> sideDownPlayedList;
    Vector<Poker *> sideLeftPlayedList;
    Vector<Poker *> sideRightPlayedList;
    Vector<Poker *> sideUpPlayedList;
    Label *coinDown;
    Label *coinRight;
    Label *coinUp;
    Label *coinLeft;
    Label *countLeft;
    Label *countRight;
    Label *countUp;
    Sprite *coverLeft;
    Sprite *coverRight;
    Sprite *coverUp;
    MenuItemSprite *btnReady;
    MenuItemSprite *btnPlay;
    MenuItemSprite *btnPass;
    MenuItemSprite *btnTip;
    Label *labelClock;
    Poker *lastTouchPoker;
    
    //method
    void setGameType(int type);
    void setupPoker();
    void showBtn(int btnType);
    void callbackReady(Ref* pSender);
    void callbackPlay(Ref* pSender);
    void callbackPass(Ref* pSender);
    void callbackTip(Ref* pSender);
    void callbackBack(Ref* pSender);
    void updateClock(float delta);
    void initTable();
    void cleanTable();
    void checkTouchPoker(Touch* touch);
    void showTalk(Player *player, const char *talk);
    void removeTalk(int side);
    void removeAllTalk();
    void logic();
    void callbackLogic(Node* pSender);
    void initPlayer();
    void addPlayer(Player *player);
    Player* getPlayer(int side);
    int randomFirstHand();
    int nextState();
    void callbackRemove(Node* pSender);
    void showTip(const char *tip);
    void updatePlayList(Poker *poker, bool isAdd);
    void pokerUp(Poker *poker, bool isSetUp);
    void resetPlayList();
    void dealPoker();
    float moveSideAnimation(Sprite *sprite, float delay, int sideFlag);
    void callbackMoveRightSide(Node* pSender);
    void callbackMoveUpSide(Node* pSender);
    void callbackMoveLeftSide(Node* pSender);
    void updatePlayerPokerCount(Player *player, Label *label);
    float moveAnimation(Sprite *sprite, Point targetPos, float delay, float time);
    void callbackMove(Node* pSender);
    void callbackEnableTouch(float delta);
    Player* nextPlayer();
    void hidePlayedPoker(Vector<Poker *> playedList);
    Player* loadPlayerById(int pid, int side);
    void showCount(bool isShow);
    void updatePlayerCoin();
    float delayCallLogic(Sprite *sprite, float delay);
    Player* getNextPlayer(Player *player);
    void aiPlay(Player *player);
    float moveScaleAnimation(Sprite *sprite, Point targetPos, float scaleTo, float delay, float time);
    void sortPlaySprite(Vector<Poker *> list);
    float sortPokerSprite(Player *player, float delay, bool callLogic);
    bool checkGameOver();
    void showResult(float delta);
    void callbackResult(Ref* pSender);
    void restart();
    int getFirstHand();
    void saveGame();
    bool loadGame();
};

class ResultLayer : public Layer {
public:
    virtual bool init() override;
    CREATE_FUNC(ResultLayer);
    void setData(int coin, int winSide, Vector<Player *> playerList, const ccMenuCallback& callback);
    void setDataHundred(int side100, int endWinSide, int winSide, Vector<Player *> playerList, const ccMenuCallback& callback);
private:
    ccMenuCallback _callback;
    void callbackNext(cocos2d::Ref* pSender);
    ResultLayer();
    virtual ~ResultLayer();
    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch* touch, Event* event) override;
    void onTouchMoved(Touch* touch, Event* event) override;
    void onTouchEnded(Touch* touch, Event* event) override;
};

#endif /* defined(__BigTwo__SceneBigTwo__) */
