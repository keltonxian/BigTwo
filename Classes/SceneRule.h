//
//  SceneRule.h
//  BigTwo
//
//  Created by keltonxian on 3/23/15.
//
//

#ifndef __BigTwo__SceneRule__
#define __BigTwo__SceneRule__

#include "GameHeader.h"

class QAKind : public Ref {
public:
    static QAKind* create(int type, const std::string& title);
    int getType();
    std::string getTitle();
protected:
    int _type;
    std::string _title;
    QAKind();
    virtual ~QAKind();
};

class SceneRule : public Layer, public TableViewDataSource, public TableViewDelegate {
public:
    static Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(SceneRule);
private:
    Vector<QAKind *> listQAKind;
    int cellWidth;
    int cellHeight;
    Label *detailLabel;
    
    void callbackBtn(cocos2d::Ref* pSender);
    void callbackBack(cocos2d::Ref* pSender);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch* touch, Event* event) override;
    void onTouchMoved(Touch* touch, Event* event) override;
    void onTouchEnded(Touch* touch, Event* event) override;
    
    virtual void scrollViewDidScroll(ScrollView* view) override;
    virtual void scrollViewDidZoom(ScrollView* view) override;
    virtual void tableCellTouched(TableView* table, TableViewCell* cell) override;
    virtual Size tableCellSizeForIndex(TableView *table, ssize_t idx) override;
    virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) override;
    virtual ssize_t numberOfCellsInTableView(TableView *table) override;
};

#endif /* defined(__BigTwo__SceneRule__) */
