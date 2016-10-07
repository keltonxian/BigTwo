//
//  Player.cpp
//  BigTwo
//
//  Created by keltonxian on 2/6/15.
//
//

#include "Player.h"

Player* Player::createPlayer(int pid, std::string name, int icon, bool isAi, int score, int matchWinCount, int matchCount, int side)
{
    Player *player = new Player();
    player->autorelease();
    player->_pid = pid;
    player->_side = side;
    player->_name = name;
    player->_icon = icon;
    player->_isAi = isAi;
    player->_score = score;
    player->_matchWinCount = matchWinCount;
    player->_matchCount = matchCount;
    return player;
}

int Player::getPid()
{
    return _pid;
}

std::string Player::getName()
{
    return _name;
}

int Player::getIcon()
{
    return _icon;
}

bool Player::isAi()
{
    return _isAi;
}

void Player::setSprite(Sprite *sprite)
{
    _sprite = sprite;
}

Sprite* Player::getSprite()
{
    return _sprite;
}

Player::Player():_pokerList(NULL),_side(0),_icon(0),_isAi(0),_pid(0),_score(0),_scoreChanged(0),_matchWinCount(0),_matchCount(0)
{
    _pokerList = new Vector<Poker *>();
//    point1 = 0;
//    point2 = 0;
//    point3 = 0;
//    point4 = 0;
//    point5 = 0;
//    point6 = 0;
//    point7 = 0;
//    point8 = 0;
}

Player::~Player()
{
    CC_SAFE_DELETE(_pokerList);
}

Vector<Poker *>* Player::getPokerList()
{
    return _pokerList;
}

void Player::insertPoker(Poker* poker)
{
    (*_pokerList).pushBack(poker);
}

void Player::insertPoker(Poker* poker, int pos)
{
    if (pos >= (*_pokerList).size()) {
        insertPoker(poker);
        return;
    }
    (*_pokerList).insert(pos, poker);
}

void Player::removePoker(Poker* poker)
{
    (*_pokerList).eraseObject(poker);
}

void Player::cleanPokerList()
{
    (*_pokerList).clear();
}

int Player::getPokerCount()
{
    return (int)(*_pokerList).size();
}

bool Player::PokerListComparisonLess(Poker* p1, Poker* p2)
{
    int p1Num = p1->getNum();
    int p1Face = p1->getFace();
    int p2Num = p2->getNum();
    int p2Face = p2->getFace();
    if (p1Num > p2Num) {
        return true;
    }
    if (p1Num == p2Num) {
        if (p1Face > p2Face) {
            return true;
        }
    }
    return false;
}

void Player::sortPokerList()
{
    std::sort( std::begin(*_pokerList), std::end(*_pokerList), Player::PokerListComparisonLess );
}

void Player::printPoker()
{
    for (int i = 0; i < (*_pokerList).size(); i++) {
        auto p = (*_pokerList).at(i);
        CCLOG("player[%s] p[%d,%d]", _name.c_str(), p->getFace(), p->getNum());
    }
}

void Player::setScore(int score)
{
    _score = score;
}

void Player::addScore(int point)
{
    _score += point;
    _scoreChanged = point;
}

int Player::getScore()
{
    return _score;
}

int Player::getScoreChanged()
{
    return _scoreChanged;
}

//void Player::setPointVar(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8)
//{
//    point1 = p1;
//    point2 = p2;
//    point3 = p3;
//    point4 = p4;
//    point5 = p5;
//    point6 = p6;
//    point7 = p7;
//    point8 = p8;
//}

void Player::setSide(int side)
{
    _side = side;
}

int Player::getSide()
{
    return _side;
}

void Player::setMatchWinCount(int matchWinCount)
{
    _matchWinCount = matchWinCount;
}

int Player::getMatchWinCount()
{
    return _matchWinCount;
}

void Player::setMatchCount(int matchCount)
{
    _matchCount = matchCount;
}

int Player::getMatchCount()
{
    return _matchCount;
}






