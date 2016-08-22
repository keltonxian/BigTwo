//
//  Poker.h
//  BigTwo
//
//  Created by keltonxian on 2/6/15.
//
//

#ifndef __BigTwo__Poker__
#define __BigTwo__Poker__

#include "GameHeader.h"

#define STATE_COVER  0
#define STATE_FRONT  1

typedef enum {
    SPADE_3=0,SPADE_4,SPADE_5,SPADE_6,SPADE_7,SPADE_8,SPADE_9,SPADE_10,SPADE_J,SPADE_Q,SPADE_K,SPADE_A,SPADE_2,
    HEART_3,HEART_4,HEART_5,HEART_6,HEART_7,HEART_8,HEART_9,HEART_10,HEART_J,HEART_Q,HEART_K,HEART_A,HEART_2,
    CLUB_3,CLUB_4,CLUB_5,CLUB_6,CLUB_7,CLUB_8,CLUB_9,CLUB_10,CLUB_J,CLUB_Q,CLUB_K,CLUB_A,CLUB_2,
    DIAMOND_3,DIAMOND_4,DIAMOND_5,DIAMOND_6,DIAMOND_7,DIAMOND_8,DIAMOND_9,DIAMOND_10,DIAMOND_J,DIAMOND_Q,DIAMOND_K,DIAMOND_A,DIAMOND_2,
    /**JOKER_B,JOKER_R, */CARD_COUNT
} POKER_TYPE;

typedef enum {
    THREE = 0, FOUR, FIVE, SIX, SEVEN, EIGHT, NIGHT, TEN, JACK, QUEEN, KING, ACE, TWO/**, J_B, J_R*/
} POKER_NUM;

typedef enum {
    DIAMOND = 0, CLUB, HEART, SPADE/**, JOKER_RED, JOKER_BLACK,*/
} POKER_FACE;

class Poker : public Sprite {
public:
    static void getPack(Vector<Poker *> *list);
    static void randomPack(Vector<Poker *> *list);
    static Poker* create(int style, int state, int ptype);
    bool isInRect(Touch* touch);
    void setPress(bool isPressed);
    bool isPress();
    int getPtype();
    int getNum();
    int getFace();
    void flipFront(float delay, float time);
    void flipBack(float delay, float time);
    Size getSizeScale();
private:
    int _style;
    int _state;
    int _ptype;
    int _num;
    int _face;
    float _width;
    float _height;
    bool _isPress;
    void setSize();
    Rect getRect();
    Poker(int ptype);
    virtual ~Poker();
    
    void setFront();
    void setFace();
    void setNumIcon();
    void setPattern();
    
    void addActionFlipFront(Vector<FiniteTimeAction *> *actionList, float time);
    void cbTurnFront(Node *pSender);
    void turnFront();
    void addActionFlipBack(Vector<FiniteTimeAction *> *actionList, float time);
    void cbTurnBack(Node *pSender);
    void turnBack();
    
    virtual void onEnter() override;
    virtual void onExit() override;
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
};

#endif /* defined(__BigTwo__Poker__) */
