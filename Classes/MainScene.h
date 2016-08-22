//
//  MainScene.h
//  KillTime
//
//  Created by keltonxian on 7/11/14.
//
//

#ifndef __KillTime__MainScene__
#define __KillTime__MainScene__

#include "GameHeader.h"

class GameKind : public Ref {
public:
    static GameKind* create(int type, const std::string& title, const std::string& subTitle1, int subNum1);
    int getType();
    std::string getTitle();
    std::string getSubTitle1();
    int getSubNum1();
protected:
    int _type;
    std::string _title;
    std::string _subTitle1;
    int _subNum1;
    GameKind();
    virtual ~GameKind();
};

class MainScene : public Layer, public TableViewDataSource, public TableViewDelegate {
public:
    static Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(MainScene);
private:
    Vector<GameKind *> listGameKind;
    int point;
    int hundredWinCount;
    int cellWidth;
    int cellHeight;
    void initPlayerData();
    void initMyData();
    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch* touch, Event* event) override;
    void onTouchMoved(Touch* touch, Event* event) override;
    void onTouchEnded(Touch* touch, Event* event) override;
    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) override;
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
    
    void callbackRandomMatch(cocos2d::Ref* pSender);
    void callbackRank(cocos2d::Ref* pSender);
    void callbackRule(cocos2d::Ref* pSender);
    
    virtual void scrollViewDidScroll(ScrollView* view) override;
    virtual void scrollViewDidZoom(ScrollView* view) override;
    virtual void tableCellTouched(TableView* table, TableViewCell* cell) override;
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) override;
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(TableView *table) override;
};

#endif /* defined(__KillTime__MainScene__) */
