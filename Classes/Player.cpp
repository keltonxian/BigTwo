//
//  Player.cpp
//  BigTwo
//
//  Created by keltonxian on 2/6/15.
//
//

#include "Player.h"

Player* Player::createPlayer(int pid, int side, std::string name, int icon, bool isAi, int point)
{
    Player *player = new Player();
    player->autorelease();
    player->_pid = pid;
    player->_side = side;
    player->_name = name;
    player->_icon = icon;
    player->_isAi = isAi;
    player->_point = point;
    return player;
}

int Player::getPid()
{
    return _pid;
}

int Player::getSide()
{
    return _side;
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

Player::Player():_pokerList(NULL),_side(0),_icon(0),_isAi(0),_pid(0),_point(0)
{
    _pokerList = new Vector<Poker *>();
    point1 = 0;
    point2 = 0;
    point3 = 0;
    point4 = 0;
    point5 = 0;
    point6 = 0;
    point7 = 0;
    point8 = 0;
    _pointChanged = 0;
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

void Player::setPoint(int point)
{
    _point = point;
}

void Player::addPoint(int point)
{
    _point += point;
    _pointChanged = point;
}

int Player::getPoint()
{
    return _point;
}

int Player::getPointChanged()
{
    return _pointChanged;
}

void Player::setPointVar(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8)
{
    point1 = p1;
    point2 = p2;
    point3 = p3;
    point4 = p4;
    point5 = p5;
    point6 = p6;
    point7 = p7;
    point8 = p8;
}

int Player::getHundredWinCount()
{
    return point3;
}






