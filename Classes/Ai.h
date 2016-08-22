//
//  Ai.h
//  BigTwo
//
//  Created by keltonxian on 3/3/15.
//
//

#ifndef __BigTwo__Ai__
#define __BigTwo__Ai__

#include "GameHeader.h"
#include "Poker.h"

/**
 用于排序卡牌
 */
class TmpFaceCard : public Ref {
public:
    static TmpFaceCard* create(Poker *poker);
    int getFace();
    Vector<Poker *>* getCardList();
    int getPokerIndexByNum(int num);
    int getPokerIndexByFace(int face);
    int getPokerIndexByNumLargeThanFace(int num, int face);
protected:
    void init(Poker *poker);
    TmpFaceCard();
    virtual ~TmpFaceCard();
private:
    int _face;
    Vector<Poker *> *cardList;
};

/**
 用于组合牌，例如夫佬，那就第一个队列三张，第二个队列两张
 */
class CardList : public Ref {
public:
    static CardList* create(Vector<Poker *> cards);
    int getCardNum();
    ValueVector* getListSpade();
    ValueVector* getListHeart();
    ValueVector* getListClub();
    ValueVector* getListDiamond();
//    ValueVector* getOneList();
//    int getFirstList(int index);
//    int getSecondList(int index);
//    int getThirdList(int index);
//    int getFourthList(int index);
    bool hasNum(int num, ValueVector *list);
    int getCountByNum(int num);
    int getCountByFace(int face);
    int getFaceByNumCount(int num, int count);
    int getNumByFaceCount(int face, int count);
protected:
    CardList();
    virtual ~CardList();
private:
    int cardNum; // total poker in hand
    void init(Vector<Poker *> cards);
    ValueVector *listSpade;
    ValueVector *listHeart;
    ValueVector *listClub;
    ValueVector *listDiamond;
    static int compare(Value v1, Value v2);
    void sort(ValueVector *cards);
};

/**
 牌型：
 属性：组合，最大数字，花色
 同花顺(花) > 金刚(花) > 夫佬(数字) > 花(花) > 顺(数字)
 黑桃 > 红心 > 梅花 > 方块
 */
class CardType : public Ref {
public:
    static CardType* create(int cardType, int num, int face);
    static CardType* create(int cardType, CardList *cardList);
    bool isIllegeal();
    bool isNone();
    int getCardType();
    int getNum();
    int getFace();
protected:
    CardType(int cardType, int num, int face);
    virtual ~CardType();
private:
    int _cardType;
    int _num;
    int _face;
};

class Ai {
public:
    static const int CARD_TYPE_ILLEGAL = 0;
    static const int CARD_TYPE_NONE	= 1;
    static const int CARD_TYPE_SINGLE = 2;	             // 单张
    static const int CARD_TYPE_PAIR = 3;	             // 一对
    static const int CARD_TYPE_THREE = 4;	             // 三张
    static const int CARD_TYPE_STRAIGHT = 5;	         // 顺
    static const int CARD_TYPE_FLOWER = 6;	             // 花
    static const int CARD_TYPE_FULLHOUSE = 7;	         // 夫佬
    static const int CARD_TYPE_KINGKONG = 8;	         // 金刚
    static const int CARD_TYPE_STRAIGHTFLUSH = 9;	     // 同花顺
    
    static int choosePoker(Vector<Poker *> myPokerList, Vector<Poker *> pokerList, CardType *lastCardType, int lastPlayListSize, int aiLevel, Vector<Poker *> *tipPokerList, int nextPlayerHandCard);
    static void copy(TmpFaceCard *tfc, Vector<Poker *> *dst, int len);
    static void copy(TmpFaceCard *tfc, Vector<Poker *> *dst, int from, int len);
    static void copyByIndex(TmpFaceCard *tfc, Vector<Poker *> *dst, int index);
    static bool chooseSingle(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list, int nextPlayerHandCard);
    static bool choosePair(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list);
    static bool chooseThree(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list);
    static bool chooseStraight(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list);
    static bool chooseFlower(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list);
    static bool chooseFullhouse(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list);
    static bool chooseKingkong(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list);
    static bool chooseStraightflush(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list);
    
    static CardType* validCardsAndGetCardType(CardType *lastCardType, int lastPlayListSize, Vector<Poker *> pokerList);
    static CardType* cardTypeNull();
    static CardType* getNormalCardType(CardList *cards);
    static CardType* getCardType(Vector<Poker *> pokerList);
};

#endif /* defined(__BigTwo__Ai__) */
