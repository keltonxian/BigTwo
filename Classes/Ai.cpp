//
//  Ai.cpp
//  BigTwo
//
//  Created by keltonxian on 3/3/15.
//
//

#include "Ai.h"


#pragma mark TmpFaceCard ---
TmpFaceCard* TmpFaceCard::create(Poker *poker)
{
    TmpFaceCard *tempFaceCard = new TmpFaceCard();
    if (tempFaceCard)
    {
        tempFaceCard->init(poker);
        tempFaceCard->autorelease();
        return tempFaceCard;
    }
    CC_SAFE_DELETE(tempFaceCard);
    return NULL;
}

void TmpFaceCard::init(Poker *poker)
{
    this->_face = poker->getFace();
    (*this->cardList).pushBack(poker);
}

int TmpFaceCard::getFace()
{
    return this->_face;
}

int TmpFaceCard::getPokerIndexByNum(int num)
{
    for (int i = 0; i < cardList->size(); i++) {
        Poker *poker = cardList->at(i);
        if (poker->getNum() == num) {
            return i;
        }
    }
    return -1;
}

int TmpFaceCard::getPokerIndexByFace(int face)
{
    for (int i = 0; i < cardList->size(); i++) {
        Poker *poker = cardList->at(i);
        if (poker->getFace() == face) {
            return i;
        }
    }
    return -1;
}

int TmpFaceCard::getPokerIndexByNumLargeThanFace(int num, int face)
{
    for (int i = 0; i < cardList->size(); i++) {
        Poker *poker = cardList->at(i);
//        CCLOG("poker num[%d] face[%d]", poker->getNum(), poker->getFace());
        if (poker->getNum() == num && poker->getFace() > face) {
            return i;
        }
    }
    return -1;
}

Vector<Poker *>* TmpFaceCard::getCardList()
{
    return this->cardList;
}

TmpFaceCard::TmpFaceCard():_face(0)
{
    this->cardList = new Vector<Poker *>();
}

TmpFaceCard::~TmpFaceCard()
{
    CC_SAFE_DELETE(this->cardList);
}
#pragma mark ---
#pragma mark CardList ---
CardList* CardList::create(Vector<Poker *> cards)
{
    CardList *cardList = new CardList();
    if (cardList)
    {
        cardList->init(cards);
        cardList->autorelease();
        return cardList;
    }
    CC_SAFE_DELETE(cardList);
    return NULL;
}

void CardList::init(Vector<Poker*> cards)
{
    size_t size = cards.size();
    this->listSpade = new ValueVector();
    this->listHeart = new ValueVector();
    this->listClub = new ValueVector();
    this->listDiamond = new ValueVector();
    ValueMapIntKey map;
//    CCLOG("CardList::init cardNum[%d] size[%zd]", cardNum, size);
    for (int i = 0; i < size; i++) {
        Poker *card = cards.at(i);
        int cFace = card->getFace();
        ValueVector *list;
        if (cFace == POKER_FACE::SPADE) {
            list = this->listSpade;
        } else if (cFace == POKER_FACE::HEART) {
            list = this->listHeart;
        } else if (cFace == POKER_FACE::CLUB) {
            list = this->listClub;
        } else if (cFace == POKER_FACE::DIAMOND) {
            list = this->listDiamond;
        } else {
            continue;
        }
        this->cardNum++;
        list->push_back(Value(card->getNum()));
    }
//    CCLOG("CardList::init size spade[%zd],heart[%zd],club[%zd],diamond[%zd]", listSpade->size(), listHeart->size(), listClub->size(), listDiamond->size());
    sort(this->listSpade);
    sort(this->listHeart);
    sort(this->listClub);
    sort(this->listDiamond);
}

int CardList::compare(Value v1, Value v2)
{
    return v1.asInt() < v2.asInt();
}

void CardList::sort(ValueVector *cards)
{
    
    std::sort( std::begin(*cards), std::end(*cards), CardList::compare );
}

CardList::CardList():listSpade(nullptr),listHeart(nullptr),listClub(nullptr),listDiamond(nullptr),cardNum(0)
{
    
}

CardList::~CardList()
{
    CC_SAFE_DELETE(this->listSpade);
    CC_SAFE_DELETE(this->listHeart);
    CC_SAFE_DELETE(this->listClub);
    CC_SAFE_DELETE(this->listDiamond);
}

int CardList::getCardNum()
{
    return this->cardNum;
}

ValueVector* CardList::getListSpade()
{
    return this->listSpade;
}

ValueVector* CardList::getListHeart()
{
    return this->listHeart;
}

ValueVector* CardList::getListClub()
{
    return this->listClub;
}

ValueVector* CardList::getListDiamond()
{
    return this->listDiamond;
}

//ValueVector* CardList::getOneList()
//{
//    if (0 < this->listDiamond->size()) {
//        return this->listDiamond;
//    }
//    if (0 < this->listClub->size()) {
//        return this->listClub;
//    }
//    if (0 < this->listHeart->size()) {
//        return this->listHeart;
//    }
//    return this->listSpade;
//}

//int CardList::getFirstList(int index)
//{
//    if (firstList == nullptr || 0 == (*firstList).size() || index < 0 || index >= (*firstList).size()) {
//        return -1;
//    }
//    return (*firstList).at(index).asInt();
//}
//
//int CardList::getSecondList(int index)
//{
//    if (secondList == nullptr || 0 == (*secondList).size() || index < 0 || index >= (*secondList).size()) {
//        return -1;
//    }
//    return (*secondList).at(index).asInt();
//}
//
//int CardList::getThirdList(int index)
//{
//    if (thirdList == nullptr || 0 == (*thirdList).size() || index < 0 || index >= (*thirdList).size()) {
//        return -1;
//    }
//    int num = thirdList->at(index).asInt();
//    //    CCLOG("thirdList index[%d] is [%d]", index, num);
//    return num;
//}
//
//int CardList::getFourthList(int index)
//{
//    if (fourthList == nullptr || 0 == (*fourthList).size() || index < 0 || index >= (*fourthList).size()) {
//        return -1;
//    }
//    return (*fourthList).at(index).asInt();
//}

bool CardList::hasNum(int num, ValueVector *list)
{
    for (int i = 0; i < (*list).size(); i++) {
        int n = (*list).at(i).asInt();
        if (num == n) {
            return true;
        }
    }
    return false;
}

int CardList::getCountByNum(int num)
{
    int count = 0;
    std::vector<ValueVector *> list;
    list.push_back(listDiamond);
    list.push_back(listClub);
    list.push_back(listHeart);
    list.push_back(listSpade);
    for (int i = 0; i < list.size(); i++) {
        ValueVector *l = list.at(i);
        for (int j = 0; j < l->size(); j++) {
            int n = l->at(j).asInt();
            if (n == num) {
                count++;
                break;
            }
        }
    }
    return count;
}

int CardList::getCountByFace(int face)
{
    int count = 0;
    std::vector<ValueVector *> list;
    list.push_back(listDiamond);
    list.push_back(listClub);
    list.push_back(listHeart);
    list.push_back(listSpade);
    for (int i = 0; i < list.size(); i++) {
        if (i != face) {
            continue;
        }
        return (int)list.at(i)->size();
    }
    return count;
}

int CardList::getFaceByNumCount(int num, int count)
{
    int face = -1;
    int cc = 0;
    std::vector<ValueVector *> list;
    list.push_back(listDiamond);
    list.push_back(listClub);
    list.push_back(listHeart);
    list.push_back(listSpade);
    for (int i = 0; i < list.size(); i++) {
        ValueVector *l = list.at(i);
        for (int j = 0; j < l->size(); j++) {
            int n = l->at(j).asInt();
            if (n == num) {
                cc++;
                if (i > face) {
                    face = i;
                }
                break;
            }
        }
    }
    if (cc != count) {
        face = -1;
    }
    return face;
}

int CardList::getNumByFaceCount(int face, int count)
{
    int num = -1;
    std::vector<ValueVector *> list;
    list.push_back(listDiamond);
    list.push_back(listClub);
    list.push_back(listHeart);
    list.push_back(listSpade);
    for (int i = 0; i < list.size(); i++) {
        if (i != face) {
            continue;
        }
        auto l = list.at(i);
        if (l->size() != count) {
            continue;
        }
        return l->at(l->size()-1).asInt();
    }
    return num;
}
#pragma mark ---
#pragma mark CardType ---
CardType* CardType::create(int cardType, int num, int face)
{
    CardType *ct = new CardType(cardType, num, face);
    if (ct)
    {
        ct->autorelease();
        return ct;
    }
    CC_SAFE_DELETE(ct);
    return NULL;
}

CardType* CardType::create(int cardType, CardList *cardList)
{
    int num = -1;
    int face = -1;
    if (0 < cardList->getListDiamond()->size()) {
        int index = (int)cardList->getListDiamond()->size() - 1;
        face = POKER_FACE::DIAMOND;
        num = cardList->getListDiamond()->at(index).asInt();
    } else if (0 < cardList->getListClub()->size()) {
        int index = (int)cardList->getListClub()->size() - 1;
        face = POKER_FACE::CLUB;
        num = cardList->getListClub()->at(index).asInt();
    } else if (0 < cardList->getListHeart()->size()) {
        int index = (int)cardList->getListHeart()->size() - 1;
        face = POKER_FACE::HEART;
        num = cardList->getListHeart()->at(index).asInt();
    } else if (0 < cardList->getListSpade()->size()) {
        int index = (int)cardList->getListSpade()->size() - 1;
        face = POKER_FACE::SPADE;
        num = cardList->getListSpade()->at(index).asInt();
    }
    return CardType::create(cardType, num, face);
}

CardType::CardType(int cardType, int num, int face):_cardType(cardType), _num(num), _face(face)
{
    
}

CardType::~CardType()
{
    
}

bool CardType::isIllegeal()
{
    return _cardType == Ai::CARD_TYPE_ILLEGAL;
}

bool CardType::isNone()
{
    return _cardType == Ai::CARD_TYPE_NONE;
}

int CardType::getCardType()
{
    return _cardType;
}

int CardType::getNum()
{
    return _num;
}

int CardType::getFace()
{
    return _face;
}
#pragma mark ---
#pragma mark Ai ---
int Ai::choosePoker(Vector<Poker *> myPokerList, Vector<Poker *> pokerList, CardType *lastCardType, int lastPlayListSize, int aiLevel, Vector<Poker *> *tipPokerList, int nextPlayerHandCard)
{
    if (myPokerList.size() == 0) {
        return 0;
    }
    bool hasDiamondThree = false;
    // 按花分成四个TmpFaceCard，每个TmpFaceCard里的队列按数由小到大排列
    Vector<TmpFaceCard *> list;
    for (int i = 0; i < myPokerList.size(); i++) {
        Poker *poker = myPokerList.at(i);
        if (false == hasDiamondThree && poker->getNum() == POKER_NUM::THREE && poker->getFace() == POKER_FACE::DIAMOND) {
            hasDiamondThree = true;
        }
        int idx = 0;
        bool exist = false;
        for (int j = 0; j < list.size(); j++) {
            TmpFaceCard *tmp = list.at(j);
            if (tmp->getFace() == poker->getFace()) {
                exist = true;
                Vector<Poker *> *tl = tmp->getCardList();
                bool isInserted = false;
                for (int k = 0; k < tl->size(); k++) {
                    auto tp = tl->at(k);
                    if (poker->getNum() < tp->getNum()) {
                        isInserted = true;
                        tl->insert(k, poker);
                        break;
                    } else {
                        continue;
                    }
                }
                if (false == isInserted) {
                    (*tmp->getCardList()).pushBack(poker);
                }
                break;
            } else if (tmp->getFace() > poker->getFace()) {
                break;
            } else {
                idx++;
            }
        }
        if (false == exist) {
            list.insert(idx, TmpFaceCard::create(poker));
        }
    }
    // 自己出
    if (lastCardType->isIllegeal() || lastCardType->isNone()) {
        Vector<Poker *> cs = (*(list.at(0))->getCardList());
        if (true == hasDiamondThree) {
            if (true == chooseSingle(tipPokerList, nullptr, &list, nextPlayerHandCard)) {
                return 0;
            }
        }
        if (true == chooseStraightflush(tipPokerList, nullptr, &list)) {
            return 0;
        }
        if (true == chooseKingkong(tipPokerList, nullptr, &list)) {
            return 0;
        }
        if (true == chooseFullhouse(tipPokerList, nullptr, &list)) {
            return 0;
        }
        if (true == chooseFlower(tipPokerList, nullptr, &list)) {
            return 0;
        }
        if (true == chooseStraight(tipPokerList, nullptr, &list)) {
            return 0;
        }
        if (true == chooseThree(tipPokerList, nullptr, &list)) {
            return 0;
        }
        if (true == choosePair(tipPokerList, nullptr, &list)) {
            return 0;
        }
        if (true == chooseSingle(tipPokerList, nullptr, &list, nextPlayerHandCard)) {
            return 0;
        }
        return 0;
    }
    tipPokerList->clear();
    switch (lastCardType->getCardType()) {
        case CARD_TYPE_SINGLE:
            chooseSingle(tipPokerList, lastCardType, &list, nextPlayerHandCard);
            break;
        case CARD_TYPE_PAIR:
            choosePair(tipPokerList, lastCardType, &list);
            break;
        case CARD_TYPE_THREE:
            chooseThree(tipPokerList, lastCardType, &list);
            break;
        case CARD_TYPE_STRAIGHT:
            if (false == chooseStraight(tipPokerList, lastCardType, &list)) {
                if (true == chooseFlower(tipPokerList, nullptr, &list)) {
                    break;
                }
                if (true == chooseFullhouse(tipPokerList, nullptr, &list)) {
                    break;
                }
                if (true == chooseKingkong(tipPokerList, nullptr, &list)) {
                    break;
                }
                if (true == chooseStraightflush(tipPokerList, nullptr, &list)) {
                    break;
                }
            }
            break;
        case CARD_TYPE_FLOWER:
            if (false == chooseFlower(tipPokerList, lastCardType, &list)) {
                if (true == chooseFullhouse(tipPokerList, nullptr, &list)) {
                    break;
                }
                if (true == chooseKingkong(tipPokerList, nullptr, &list)) {
                    break;
                }
                if (true == chooseStraightflush(tipPokerList, nullptr, &list)) {
                    break;
                }
            }
            break;
        case CARD_TYPE_FULLHOUSE:
            if (false == chooseFullhouse(tipPokerList, lastCardType, &list)) {
                if (true == chooseKingkong(tipPokerList, nullptr, &list)) {
                    break;
                }
                if (true == chooseStraightflush(tipPokerList, nullptr, &list)) {
                    break;
                }
            }
            break;
        case CARD_TYPE_KINGKONG:
            if (false == chooseKingkong(tipPokerList, lastCardType, &list)) {
                if (true == chooseStraightflush(tipPokerList, nullptr, &list)) {
                    break;
                }
            }
            break;
        case CARD_TYPE_STRAIGHTFLUSH:
            chooseStraightflush(tipPokerList, lastCardType, &list);
            break;
        default:
            break;
    }
    // clean TmpFaceCard
    return 0;
}

void Ai::copy(TmpFaceCard *tfc, Vector<Poker *> *dst, int len)
{
    Ai::copy(tfc, dst, 0, len);
}

void Ai::copy(TmpFaceCard *tfc, Vector<Poker *> *dst, int from, int len)
{
    for (int i = 0; i < len; i++) {
        auto src = tfc->getCardList();
        if (from == src->size()) {
            return;
        }
        auto poker = src->at(from+i);
        dst->pushBack(poker);
//        src->erase(from);
    }
}

void Ai::copyByIndex(TmpFaceCard *tfc, Vector<Poker *> *dst, int index)
{
    auto src = tfc->getCardList();
    if (index >= src->size()) {
        return;
    }
    auto poker = src->at(index);
    dst->pushBack(poker);
//    src->erase(index);
}

bool Ai::chooseSingle(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list, int nextPlayerHandCard)
{
    int lastNum = 0;
    int lastFace = -1;
    if (nullptr != lastCardType) {
        lastNum = lastCardType->getNum();
        lastFace = lastCardType->getFace();
    }
    int maxNumListIndex = -1;
    int maxNumIndex = -1;
    for (int i = lastNum; i <= POKER_NUM::TWO; i++) {
        for (int j = 0; j < list->size(); j++) {
            TmpFaceCard *card = list->at(j);
            int index = -1;
            if (i > lastNum) {
                index = card->getPokerIndexByNum(i);
            } else {
                index = card->getPokerIndexByNumLargeThanFace(i, lastFace);
            }
            if (1 == nextPlayerHandCard) { // 顶大
                if (-1 != index) {
                    maxNumListIndex = j;
                    maxNumIndex = index;
                }
                continue;
            }
            if (-1 != index) {
                copyByIndex(card, tipPokerList, index);
                return true;
            }
        }
    }
    if (1 == nextPlayerHandCard && -1 != maxNumIndex) {
        copyByIndex(list->at(maxNumListIndex), tipPokerList, maxNumIndex);
        return true;
    }
    return false;
}

bool Ai::choosePair(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list)
{
    int lastNum = 0;
    int lastFace = -1;
    if (nullptr != lastCardType) {
        lastNum = lastCardType->getNum();
        lastFace = lastCardType->getFace();
    }
    int outLen = 2;
    int listIndex[outLen];
    int pokerIndex[outLen];
    int count = 0;
    for (int i = lastNum; i <= POKER_NUM::TWO; i++) {
        count = 0;
        for (int j = 0; j < list->size(); j++) {
            TmpFaceCard *card = list->at(j);
            int index = -1;
            if (i > lastNum) {
                index = card->getPokerIndexByNum(i);
            } else {
                index = card->getPokerIndexByNumLargeThanFace(i, lastFace);
            }
//            CCLOG("i[%d] j[%d] lastFace[%d] index[%d]", i, j, lastFace, index);
            if (-1 != index) {
                listIndex[count] = j;
                pokerIndex[count] = index;
                count++;
                if (2 == count) {
                    for (int k = 0; k < outLen; k++) {
                        Ai::copyByIndex(list->at(listIndex[k]), tipPokerList, pokerIndex[k]);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

bool Ai::chooseThree(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list)
{
    int lastNum = 0;
    int lastFace = -1;
    if (nullptr != lastCardType) {
        lastNum = lastCardType->getNum();
        lastFace = lastCardType->getFace();
    }
    int outLen = 3;
    int listIndex[outLen];
    int pokerIndex[outLen];
    int count = 0;
    for (int i = lastNum; i <= POKER_NUM::TWO; i++) {
        count = 0;
        for (int j = 0; j < list->size(); j++) {
            TmpFaceCard *card = list->at(j);
            int index = -1;
            if (i > lastNum) {
                index = card->getPokerIndexByNum(i);
            } else {
                index = card->getPokerIndexByNumLargeThanFace(i, lastFace);
            }
            //            CCLOG("i[%d] j[%d] lastFace[%d] index[%d]", i, j, lastFace, index);
            if (-1 != index) {
                listIndex[count] = j;
                pokerIndex[count] = index;
                count++;
                if (3 == count) {
                    for (int k = 0; k < outLen; k++) {
                        Ai::copyByIndex(list->at(listIndex[k]), tipPokerList, pokerIndex[k]);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

bool Ai::chooseStraight(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list)
{
    // 还得考虑数字一样，但最后一张花色不同的情况
    int lastNum = -1;
    int lastFace = -1;
    if (nullptr != lastCardType) {
        lastNum = lastCardType->getNum();
        lastFace = lastCardType->getFace();
    }
    int outLen = 5;
    int listIndex[outLen];
    int pokerIndex[outLen];
    int count = 0;
    
    // 数比上一手大， 例如 56789 > 45678
    for (int i = lastNum+1; i <= POKER_NUM::TWO; i++) {
        for (int j = 0; j < list->size(); j++) {
            TmpFaceCard *tmp = list->at(j);
            int index = tmp->getPokerIndexByNum(i);
//            CCLOG("i[%d] j[%d] lastFace[%d] index[%d]", i, j, lastFace, index);
            if (-1 != index) {
                listIndex[count] = j;
                pokerIndex[count] = index;
                count++;
                if (5 == count) {
                    for (int k = 0; k < outLen; k++) {
                        Ai::copyByIndex(list->at(listIndex[k]), tipPokerList, pokerIndex[k]);
                    }
                    return true;
                }
                break;
            }
            if (j == list->size()-1) {
                count = 0;
            }
        }
    }
    
    count = 0;
    // 数跟上一手一样，最后的花大
    for (int i = lastNum; i <= lastNum+4; i++) {
        for (int j = 0; j < list->size(); j++) {
            TmpFaceCard *tmp = list->at(j);
            int index = tmp->getPokerIndexByNum(i);
            if (i == lastNum+4) {
                index = tmp->getPokerIndexByNumLargeThanFace(i, lastFace);
            } else {
                index = tmp->getPokerIndexByNum(i);
            }
            if (-1 != index) {
                listIndex[count] = j;
                pokerIndex[count] = index;
                count++;
                if (5 == count) {
                    for (int k = 0; k < outLen; k++) {
                        Ai::copyByIndex(list->at(listIndex[k]), tipPokerList, pokerIndex[k]);
                    }
                    return true;
                }
                break;
            }
            if (j == list->size()-1) {
                count = 0;
            }
        }
    }
    return false;
}

bool Ai::chooseFlower(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list)
{
    int lastNum = 0;
    int lastFace = -1;
    if (nullptr != lastCardType) {
        lastNum = lastCardType->getNum();
        lastFace = lastCardType->getFace();
    }
    for (int i = 0; i < list->size(); i++) {
        TmpFaceCard *card = list->at(i);
        Vector<Poker *> *l = card->getCardList();
        if (5 > l->size()) {
            continue;
        }
        auto pp = l->at(0);
        if (pp->getFace() < lastFace) {
            continue;
        }
        int markIndex = -1;
        for (int j = 0; j < l->size(); j++) {
            auto p = l->at(j);
            if (p->getNum() > lastNum) {
                markIndex = j;
                break;
            }
        }
        if (-1 == markIndex) {
            continue;
        }
        
        if (markIndex > 4) {
            Ai::copy(card, tipPokerList, 4);
            Ai::copyByIndex(card, tipPokerList, markIndex);
        } else {
            Ai::copy(card, tipPokerList, 5);
        }
        return true;
    }
    return false;
}

bool Ai::chooseFullhouse(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list)
{
    int lastNum = -1;
    int lastFace = -1;
    if (nullptr != lastCardType) {
        lastNum = lastCardType->getNum();
        lastFace = lastCardType->getFace();
    }
    int m3 = -1;
    for (int i = lastNum+1; i <= POKER_NUM::TWO; i++) {
        int count = 0;
        for (int j = 0; j < list->size(); j++) {
            TmpFaceCard *card = list->at(j);
            int index = card->getPokerIndexByNum(i);
            if (-1 != index) {
                count++;
            }
        }
        if (count == 3) {
            m3 = i;
            break;
        }
    }
    if (-1 == m3) {
        return false;
    }
    int m2 = -1;
    for (int i = lastNum+1; i <= POKER_NUM::TWO; i++) {
        if (m3 == i) {
            continue;
        }
        int count = 0;
        for (int j = 0; j < list->size(); j++) {
            TmpFaceCard *card = list->at(j);
            int index = card->getPokerIndexByNum(i);
            if (-1 != index) {
                count++;
            }
        }
        if (count == 2) {
            m2 = i;
            break;
        }
    }
    if (-1 == m2) {
        return false;
    }
    int count = 0;
    for (int j = 0; j < list->size(); j++) {
        TmpFaceCard *card = list->at(j);
        int index = card->getPokerIndexByNum(m3);
        if (-1 != index) {
            Ai::copyByIndex(card, tipPokerList, index);
            count++;
            if (3 == count) {
                break;
            }
        }
    }
    
    count = 2;
    for (int j = 0; j < list->size(); j++) {
        TmpFaceCard *card = list->at(j);
        int index = card->getPokerIndexByNum(m2);
        if (-1 != index) {
            Ai::copyByIndex(card, tipPokerList, index);
            count++;
            if (2 == count) {
                break;
            }
        }
    }
    
    return true;
}

bool Ai::chooseKingkong(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list)
{
    int lastNum = -1;
    int lastFace = -1;
    if (nullptr != lastCardType) {
        lastNum = lastCardType->getNum();
        lastFace = lastCardType->getFace();
    }
    int m4 = -1;
    for (int i = lastNum+1; i <= POKER_NUM::TWO; i++) {
        int count = 0;
        for (int j = 0; j < list->size(); j++) {
            TmpFaceCard *card = list->at(j);
            int index = card->getPokerIndexByNum(i);
            if (-1 != index) {
                count++;
            }
        }
        if (count == 4) {
            m4 = i;
            break;
        }
    }
    if (-1 == m4) {
        return false;
    }
    int indexList1 = -1;
    int index1 = -1;
    int m1 = -1;
    for (int i = lastNum+1; i <= POKER_NUM::TWO; i++) {
        if (m4 == i) {
            continue;
        }
        for (int j = 0; j < list->size(); j++) {
            TmpFaceCard *card = list->at(j);
            int index = card->getPokerIndexByNum(i);
            if (-1 != index) {
                m1 = i;
                index1 = index;
                indexList1 = j;
                break;
            }
        }
        if (-1 != m1) {
            break;
        }
    }
    if (-1 == m1) {
        return false;
    }
    int count = 0;
    for (int j = 0; j < list->size(); j++) {
        TmpFaceCard *card = list->at(j);
        int index = card->getPokerIndexByNum(m4);
        if (-1 != index) {
            Ai::copyByIndex(card, tipPokerList, index);
            count++;
            if (3 == count) {
                break;
            }
        }
    }
    
    Ai::copyByIndex(list->at(indexList1), tipPokerList, index1);
    
    return true;
}

bool Ai::chooseStraightflush(Vector<Poker *> *tipPokerList, CardType *lastCardType, Vector<TmpFaceCard *> *list)
{
//    CCLOG("Ai::chooseStraightflush");
    int lastNum = -1;
    int lastFace = -1;
    if (nullptr != lastCardType) {
        lastNum = lastCardType->getNum();
        lastFace = lastCardType->getFace();
    }
    int outLen = 5;
    int listIndex[outLen];
    int pokerIndex[outLen];
//    int count = 0;
    
    for (int i = 0; i < list->size(); i++) {
        TmpFaceCard *tmp = list->at(i);
        int count = 0;
        for (int j = lastNum+1; j <= POKER_NUM::TWO; j++) {
            int index = tmp->getPokerIndexByNum(j);
            if (-1 == index) {
                if (count>0) {
                    count = 0;
                }
                continue;
            }
            listIndex[count] = i;
            pokerIndex[count] = index;
            count++;
            if (5 == count) {
                for (int k = 0; k < outLen; k++) {
                    Ai::copyByIndex(list->at(listIndex[k]), tipPokerList, pokerIndex[k]);
                }
                return true;
            }
        }
    }
    return false;
    
//    for (int i = lastNum+1; i <= POKER_NUM::TWO; i++) {
//        for (int j = 0; j < list->size(); j++) {
//            TmpFaceCard *tmp = list->at(j);
//            int index = tmp->getPokerIndexByNum(i);
//            //            CCLOG("i[%d] j[%d] lastFace[%d] index[%d]", i, j, lastFace, index);
//            if (-1 != index) {
//                listIndex[count] = j;
//                pokerIndex[count] = index;
//                count++;
//                if (5 == count) {
//                    for (int k = 0; k < outLen; k++) {
//                        Ai::copyByIndex(list->at(listIndex[k]), tipPokerList, pokerIndex[k]);
//                    }
//                    return true;
//                }
//                break;
//            }
//            if (j == list->size()-1) {
//                count = 0;
//            }
//        }
//    }
//    
//    
//    
//    
//    int lastNum = lastCardType->getNum();
//    int lastFace = lastCardType->getFace();
//    int count = 0;
//    int markNum = -1;
//    for (int i = lastFace+1; i < list->size(); i++) {
//        TmpFaceCard *tmp = list->at(i);
//        auto l = tmp->getCardList();
//        count = 0;
//        int from = 0;
//        for (int j = 0; j < l->size(); j++) {
//            auto p = l->at(j);
//            int n = p->getNum();
//            if (n < lastNum) {
//                continue;
//            }
//            if (0 == count || markNum + count != n) {
//                from = j;
//                markNum = n;
//                count = 1;
//                continue;
//            }
//            if (markNum + count == n) {
//                count++;
//            }
//            if (5 == count) {
//                Ai::copy(tmp, tipPokerList, j, 5);
//                break;
//            }
//        }
//    }
}

CardType* Ai::validCardsAndGetCardType(CardType *lastCardType, int lastPlayListSize, Vector<Poker *> pokerList)
{
    if (0 == pokerList.size()) {
        CCLOG("valid 0 == pokerList.size()");
        return Ai::cardTypeNull();
    }
    CardList *cardList = CardList::create(pokerList);
    if (lastCardType->getCardType() <= CARD_TYPE_NONE || lastPlayListSize <= 0) {
        return Ai::getNormalCardType(cardList);
    }
    CardType *ct = Ai::getNormalCardType(cardList);
    if (ct->getCardType() == lastCardType->getCardType()) {
        if((ct->getNum() > lastCardType->getNum() || (ct->getNum() == lastCardType->getNum() && ct->getFace() > lastCardType->getFace())) && cardList->getCardNum() == lastPlayListSize){
            return ct;
        }
        return Ai::cardTypeNull();
    }
    if (lastCardType->getCardType() >= Ai::CARD_TYPE_STRAIGHT && ct->getCardType() > lastCardType->getCardType()) {
        return ct;
    }
    return Ai::cardTypeNull();
}

CardType* Ai::cardTypeNull()
{
    CardType *ct = CardType::create(Ai::CARD_TYPE_NONE, -1, -1);
    return ct;
}

CardType* Ai::getNormalCardType(CardList *cards)
{
    if (1 == cards->getCardNum()) {
        return CardType::create(CARD_TYPE_SINGLE, cards);
    }
    ValueVector *listSpade = cards->getListSpade();
    ValueVector *listHeart = cards->getListHeart();
    ValueVector *listClub = cards->getListClub();
    ValueVector *listDiamond = cards->getListDiamond();
//    CCLOG("Ai::getNormalCardType size spade[%zd],heart[%zd],club[%zd],diamond[%zd]", listSpade->size(), listHeart->size(), listClub->size(), listDiamond->size());
    std::vector<ValueVector *> list;
    list.push_back(listDiamond);
    list.push_back(listClub);
    list.push_back(listHeart);
    list.push_back(listSpade);
    // list size should be 4
    
    int face = -1;
    int m1 = -1;
    int m2 = -1;
    int m2count = 0;
    int m3 = -1;
    int m4 = -1;
    int m4count = 0;
    
    if (3 == cards->getCardNum()) {
        // 三张
        //        CCLOG("Ai::getNormalCardType 对");
        face = -1;
        m2 = -1;
        for (int i = POKER_NUM::THREE; i <= POKER_NUM::TWO; i++) {
            int retFace = cards->getFaceByNumCount(i, 3);
            if (-1 != retFace) {
                return CardType::create(CARD_TYPE_PAIR, i, retFace);
            }
        }
        return Ai::cardTypeNull();
    }
    if (2 == cards->getCardNum()) {
        // 对
//        CCLOG("Ai::getNormalCardType 对");
        face = -1;
        m2 = -1;
        for (int i = POKER_NUM::THREE; i <= POKER_NUM::TWO; i++) {
            int retFace = cards->getFaceByNumCount(i, 2);
            if (-1 != retFace) {
                return CardType::create(CARD_TYPE_PAIR, i, retFace);
            }
        }
        return Ai::cardTypeNull();
    }
    if (1 == cards->getCardNum()) {
        // 单张
//        CCLOG("Ai::getNormalCardType 单张");
        face = -1;
        if (cards->getCardNum() == 1) {
            for (int i = 0; i < list.size(); i++) {
                auto l = list.at(i);
                if (l->size() > 0) {
                    face = i;
                    auto num = l->at(0).asInt();
                    return CardType::create(CARD_TYPE_SINGLE, num, face);
                }
            }
        }
        return Ai::cardTypeNull();
    }
    
    if (cards->getCardNum() != 5) {
        return Ai::cardTypeNull();
    }
    
//    CCLOG("Ai::getNormalCardType 同花顺");
    // 同花顺
    face = -1;
    m2 = -1;
    m2count = 0;
    for (int i = POKER_FACE::DIAMOND; i <= POKER_FACE::SPADE; i++) {
        for (int j = POKER_NUM::THREE; j <= POKER_NUM::TWO; j++) {
            int count = cards->getCountByNum(j);
            int count2 = cards->getCountByFace(i);
            if (0 == count || 0 == count2) {
                m2count = 0;
                continue;
            }
            if (0 == m2count) {
                m2 = i;
                m2count = 1;
                continue;
            }
            m2count++;
            m2 = i;
            if (5 == m2count) {
                return CardType::create(CARD_TYPE_STRAIGHT, m2+4, face);
            }
        }
    }
//    
//    m4 = -1;
//    for (int i = 0; i < list.size(); i++) {
//        auto l = list.at(i);
//        if (l->size() != 5) {
//            continue;
//        }
//        m4 = l->at(0).asInt();;
//        m4count = 0;
//        for (int j = 0; j < l->size()-1; j++) {
//            int num = l->at(j).asInt();
//            int num2 = l->at(j+1).asInt();
//            if (num2 - num != 1) {
//                m4 = -1;
//                break;
//            }
//        }
//        if (-1 != m4) {
//            break;
//        }
//    }
//    if (m4 >= 0) {
//        return CardType::create(CARD_TYPE_STRAIGHTFLUSH, m4+4, face);
//    }
    
    // 金刚
//    CCLOG("Ai::getNormalCardType 金刚");
    face = -1;
    m4 = -1;
    m4count = 0;
    m1 = -1;
    for (int i = POKER_NUM::THREE; i <= POKER_NUM::TWO; i++) {
        int count = cards->getCountByNum(i);
        if (count == 4) {
            m4 = i;
        }
    }
    for (int i = POKER_NUM::THREE; i <= POKER_NUM::TWO; i++) {
        if (i == m4) {
            continue;
        }
        int count = cards->getCountByNum(i);
        if (count == 1) {
            m1 = i;
        }
    }
    if (m1 >= 0 && m4 >= 0) {
        return CardType::create(CARD_TYPE_KINGKONG, m4, face);
    }
    
    // 夫佬
//    CCLOG("Ai::getNormalCardType 夫佬");
    face = -1;
    m3 = -1;
    m2 = -1;
    for (int i = POKER_NUM::THREE; i <= POKER_NUM::TWO; i++) {
        int count = cards->getCountByNum(i);
        if (count == 3) {
            m3 = i;
            break;
        }
    }
    for (int i = POKER_NUM::THREE; i <= POKER_NUM::TWO; i++) {
        if (i == m3) {
            continue;
        }
        int count = cards->getCountByNum(i);
        if (count == 2) {
            m2 = i;
            break;
        }
    }
    if (m3 >= 0 && m2 >= 0) {
        return CardType::create(CARD_TYPE_FULLHOUSE, m3, face);
    }
    
    // 花
//    CCLOG("Ai::getNormalCardType 花");
    face = -1;
    for (int i = POKER_FACE::DIAMOND; i <= POKER_FACE::SPADE; i++) {
        int num = cards->getNumByFaceCount(i, 5);
        if (-1 != num) {
            return CardType::create(CARD_TYPE_FLOWER, num, i);
        }
    }
    
    // 顺
//    CCLOG("Ai::getNormalCardType 顺");
//    for (int i = 0; i < list.size(); i++) {
//        auto l = list.at(i);
//        for (int j = 0; j < l->size(); j++) {
//            int num = l->at(j).asInt();
//            CCLOG("num:[%d]", num);
//        }
//    }
    face = -1;
    m2 = -1;
    m2count = 0;
    for (int i = POKER_NUM::THREE; i <= POKER_NUM::TWO; i++) {
        int count = cards->getCountByNum(i);
        if (0 == count) {
            m2count = 0;
            continue;
        }
        if (0 == m2count) {
            m2 = i;
            m2count = 1;
            continue;
        }
        m2count++;
        m2 = i;
        if (5 == m2count) {
            face = cards->getFaceByNumCount(i, 1);
            return CardType::create(CARD_TYPE_STRAIGHT, m2+4, face);
        }
    }
    
    return Ai::cardTypeNull();
}

CardType* Ai::getCardType(Vector<Poker *> pokerList)
{
    if (0 == pokerList.size()) {
        return Ai::cardTypeNull();
    }
    CardList *list = CardList::create(pokerList);
    CardType *ct = getNormalCardType(list);
    return ct;
}
#pragma mark ---
