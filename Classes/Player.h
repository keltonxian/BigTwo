//
//  Player.h
//  BigTwo
//
//  Created by keltonxian on 2/6/15.
//
//

#ifndef __BigTwo__Player__
#define __BigTwo__Player__

#include "GameHeader.h"

#include "Poker.h"

class Player : public Ref {
public:
    static Player* createPlayer(int pid, int side, std::string name, int icon, bool isAi, int point);
    static bool PokerListComparisonLess(Poker* p1, Poker* p2);
    int getPid();
    int getSide();
    std::string getName();
    int getIcon();
    bool isAi();
    void setSprite(Sprite *sprite);
    Sprite* getSprite();
    Vector<Poker *>* getPokerList();
    void insertPoker(Poker* poker);
    void insertPoker(Poker* poker, int pos);
    void removePoker(Poker* poker);
    void cleanPokerList();
    int getPokerCount();
    void sortPokerList();
    Player();
    virtual ~Player();
    void printPoker();
    void setPoint(int point);
    void addPoint(int point);
    int getPoint();
    int getPointChanged();
    void setPointVar(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8);
    int point1;
    int point2;
    int point3;
    int point4;
    int point5;
    int point6;
    int point7;
    int point8;
    int getHundredWinCount();
private:
    int _side;
    Sprite *_sprite;
    bool _isAi;
    std::string _name;
    int _pid;
    int _icon;
    int _point;
    int _pointChanged;
    Vector<Poker *> *_pokerList;
};

#endif /* defined(__BigTwo__Player__) */
