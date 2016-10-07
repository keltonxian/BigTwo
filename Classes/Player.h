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
    static Player* createPlayer(int pid, std::string name, int icon, bool isAi, int score, int matchWinCount, int matchCount, int side = 0);
    static bool PokerListComparisonLess(Poker* p1, Poker* p2);
    int getPid();
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
    void setScore(int score);
    void addScore(int point);
    int getScore();
    int getScoreChanged();
//    void setPointVar(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8);
//    int point1;
//    int point2;
//    int point3;
//    int point4;
//    int point5;
//    int point6;
//    int point7;
//    int point8;
    void setSide(int side);
    int getSide();
    void setMatchWinCount(int matchWinCount);
    int getMatchWinCount();
    void setMatchCount(int matchCount);
    int getMatchCount();
private:
    int _side;
    Sprite *_sprite;
    bool _isAi;
    std::string _name;
    int _pid;
    int _icon;
    int _score;
    int _scoreChanged;
    int _matchWinCount;
    int _matchCount;
    Vector<Poker *> *_pokerList;
};

#endif /* defined(__BigTwo__Player__) */
