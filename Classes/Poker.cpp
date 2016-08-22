//
//  Poker.cpp
//  BigTwo
//
//  Created by keltonxian on 2/6/15.
//
//

#include "Poker.h"

void Poker::getPack(Vector<Poker *> *list)
{
    for (int i = POKER_TYPE::SPADE_3; i < POKER_TYPE::CARD_COUNT; i++) {
        (*list).pushBack(Poker::create(0, STATE_FRONT, i));
    }
    randomPack(list);
}

void Poker::randomPack(Vector<Poker *> *list)
{
    // need random seed
    srand((int)time(NULL));
    for (int i = 0; i < 100; i++) {
        ssize_t index1 = rand() % (*list).size();
        ssize_t index2 = rand() % (*list).size();
        (*list).swap(index1, index2);
    }
}

Poker* Poker::create(int style, int state, int ptype)
{
    Poker *poker = new Poker(ptype);
    poker->_style = style;
    poker->_state = state;
    char path[500];
    if (state == STATE_FRONT) {
        KUtil::getPath(path, 500, F_POKER, "card_1.png");
    } else {
        KUtil::getPath(path, 500, F_POKER, "cover_1.png");
    }
    if (poker && poker->initWithFile(path)) {
        poker->setSize();
        if (state == STATE_FRONT) {
            poker->setFront();
        }
        poker->autorelease();
        return poker;
    }
    CC_SAFE_DELETE(poker);
    return NULL;
}

void Poker::setSize()
{
    _width = this->getContentSize().width;
    _height = this->getContentSize().height;
}

Rect Poker::getRect()
{
    auto scale = this->getScale();
    return Rect(-_width*scale / 2, -_height*scale / 2, _width*scale, _height*scale);
}

Size Poker::getSizeScale()
{
    auto scale = this->getScale();
    return Size(_width*scale, _height*scale);
}

Poker::Poker(int ptype):_ptype(ptype),_isPress(false),_state(0),_style(0),_num(0),_face(0)
{
//    if (ptype == POKER_TYPE::JOKER_B) {
//        _num = POKER_NUM::J_B;
//        _face = POKER_FACE::JOKER_BLACK;
//    } else if (ptype == POKER_TYPE::JOKER_R) {
//        _num = POKER_NUM::J_R;
//        _face = POKER_FACE::JOKER_RED;
//    } else {
        _num = ptype % 13;
        _face = floor(ptype / 13);
//    }
}

Poker::~Poker()
{
    
}

void Poker::onEnter()
{
    Sprite::onEnter();
}

void Poker::onExit()
{
    Sprite::onExit();
}

bool Poker::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void Poker::onTouchMoved(Touch* touch, Event* event)
{
    
}

void Poker::onTouchEnded(Touch* touch, Event* event)
{
    
}

void Poker::setFront()
{
    this->setPattern();
    this->setNumIcon();
    this->setFace();
}

void Poker::setNumIcon()
{
//    if (_num == POKER_NUM::J_B || _num == POKER_NUM::J_R) {
//        return;
//    }
    char numStr[20] = {0};
    memset(numStr, 0, 20);
    if (_num == POKER_NUM::JACK) {
        sprintf(numStr, "%s", "j");
    } else if (_num == POKER_NUM::QUEEN) {
        sprintf(numStr, "%s", "q");
    } else if (_num == POKER_NUM::KING) {
        sprintf(numStr, "%s", "k");
    } else if (_num == POKER_NUM::ACE) {
        sprintf(numStr, "%s", "a");
    } else if (_num == POKER_NUM::TWO) {
        sprintf(numStr, "%s", "2");
    } else {
        sprintf(numStr, "%d", _num+3);
    }
    char fname1[20] = {0};
    memset(fname1, 0, 20);
    if (_face == POKER_FACE::SPADE || _face == POKER_FACE::CLUB/** || _face == POKER_FACE::JOKER_BLACK*/) {
        sprintf(fname1, "b_%s", numStr);
    } else {
        sprintf(fname1, "r_%s", numStr);
    }
    int x = 10;
    int y = _height - 15;
    char fname[20] = {0};
    memset(fname, 0, 20);
    sprintf(fname, "%s_1.png", fname1);
    KUtil::addSprite(this, KUtil::getPath(F_POKER, fname), Point(x, y), ANCHOR_LEFT_UP, 15);
}

void Poker::setFace()
{
    char filename[20] = {0};
    memset(filename, 0, 20);
    switch (_face) {
        case POKER_FACE::SPADE:
            sprintf(filename, "%s", "spade");
            break;
        case POKER_FACE::HEART:
            sprintf(filename, "%s", "heart");
            break;
        case POKER_FACE::DIAMOND:
            sprintf(filename, "%s", "diamond");
            break;
        case POKER_FACE::CLUB:
            sprintf(filename, "%s", "club");
            break;
//        case POKER_FACE::JOKER_RED:
//            sprintf(filename, "%s", "t_joker_r");
//            break;
//        case POKER_FACE::JOKER_BLACK:
//            sprintf(filename, "%s", "t_joker_b");
//            break;
            
        default:
            return;
    }
    char fname[20] = {0};
    memset(fname, 0, 20);
    sprintf(fname, "%s_1.png", filename);
    int x = 0;
    int y = 0;
//    if (_face == POKER_FACE::JOKER_BLACK || _face == POKER_FACE::JOKER_RED) {
//        x = 10;
//        y = _height - 15;
//    } else {
        x = 10;
        y = _height - 80;
//    }
    KUtil::addSprite(this, KUtil::getPath(F_POKER, fname), Point(x, y), ANCHOR_LEFT_UP, 15);
}

void Poker::setPattern()
{
    char numStr[20] = {0};
    memset(numStr, 0, 20);
    if (_num == POKER_NUM::JACK) {
        sprintf(numStr, "%s", "jack");
    } else if (_num == POKER_NUM::QUEEN) {
        sprintf(numStr, "%s", "queen");
    } else if (_num == POKER_NUM::KING) {
        sprintf(numStr, "%s", "king");
    }
//    else if (_num == POKER_NUM::J_B) {
//        sprintf(numStr, "%s", "joker");
//    } else if (_num == POKER_NUM::J_R) {
//        sprintf(numStr, "%s", "joker");
//    }
    else {
        return;
    }
    char fname1[20] = {0};
    memset(fname1, 0, 20);
    if (_face == POKER_FACE::SPADE || _face == POKER_FACE::CLUB/** || _face == POKER_FACE::JOKER_BLACK*/) {
        sprintf(fname1, "%s_b", numStr);
    } else {
        sprintf(fname1, "%s_r", numStr);
    }
    char fname[20] = {0};
    memset(fname, 0, 20);
    sprintf(fname, "icon_%s_1.png", fname1);
    KUtil::addSprite(this, KUtil::getPath(F_POKER, fname), Point(_width-5, 5), ANCHOR_RIGHT_DOWN, 10);
}

bool Poker::isInRect(Touch* touch)
{
    if (getRect().containsPoint(convertTouchToNodeSpaceAR(touch))) {
        return true;
    }
    return false;
}

void Poker::setPress(bool isPressed)
{
    _isPress = isPressed;
}

bool Poker::isPress()
{
    return _isPress;
}

int Poker::getNum()
{
    return _num;
}

int Poker::getFace()
{
    return _face;
}

int Poker::getPtype()
{
    return _ptype;
}

void Poker::flipFront(float delay, float time)
{
    if (_state == STATE_FRONT) {
        return;
    }
    
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    addActionFlipFront(&list, time);
    
    action = Sequence::create(list);
    this->runAction(action);
}

void Poker::addActionFlipFront(Vector<FiniteTimeAction *> *actionList, float time)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = OrbitCamera::create(time/2, 1, 0, 0, -90, 0, 0);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(Poker::cbTurnFront, this));
    list.pushBack(action);
    action = OrbitCamera::create(time/2, 1, 0, 90, -90, 0, 0);
    list.pushBack(action);
    action = Sequence::create(list);
    (*actionList).pushBack(action);
}

void Poker::cbTurnFront(Node *pSender)
{
    this->turnFront();
}

void Poker::turnFront()
{
    if (_state == STATE_FRONT) {
        return;
    }
    this->removeAllChildren();
    char path[500];
    KUtil::getPath(path, 500, F_POKER, "card_1.png");
    this->setTexture(path);
    this->setFront();
    _state = STATE_FRONT;
}

void Poker::flipBack(float delay, float time)
{
    if (_state == STATE_COVER) {
        return;
    }
    
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = DelayTime::create(delay);
    list.pushBack(action);
    addActionFlipBack(&list, time);
    action = Sequence::create(list);
    this->runAction(action);
}

void Poker::addActionFlipBack(Vector<FiniteTimeAction *> *actionList, float time)
{
    Vector<FiniteTimeAction *> list;
    FiniteTimeAction *action;
    action = OrbitCamera::create(time/2, 1, 0, 0, -90, 0, 0);
    list.pushBack(action);
    action = CallFuncN::create(CC_CALLBACK_1(Poker::cbTurnBack, this));
    list.pushBack(action);
    action = OrbitCamera::create(time/2, 1, 0, 90, -90, 0, 0);
    list.pushBack(action);
    action = Sequence::create(list);
    (*actionList).pushBack(action);
}

void Poker::cbTurnBack(Node *pSender)
{
    this->turnBack();
}

void Poker::turnBack()
{
    if (_state == STATE_COVER) {
        return;
    }
    this->removeAllChildren();
    char path[500];
    KUtil::getPath(path, 500, F_POKER, "cover_1.png");
    this->setTexture(path);
    _state = STATE_COVER;
}




