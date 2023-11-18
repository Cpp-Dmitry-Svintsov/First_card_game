#include "common.h"

//----------------------------------------------------------------------------
ADeck::~ADeck() {
    //Очистка колоды карт

    delete[] Deck;
    Deck = nullptr;
    delete Shuffle_Сard;
    Shuffle_Сard = nullptr;
}
//----------------------------------------------------------------------------
void ADeck::Init() {
    Deck = new SCard[36];
    Shuffle_Сard = new SCard();
    //-----------------------------------
    bool cur_trump;
    Trump = 0 + rand() % 4;
    short index = 0;
    for (short suit = 0; suit < 4; suit++) {
        cur_trump = suit == Trump ? true : false;
        for (short rank = 0; rank < 9; rank++) {
            Deck[index].Rank = rank;
            Deck[index].Suit = suit;
            Deck[index].Trump = cur_trump;
            ++index;
        }
    }
}
//----------------------------------------------------------------------------
void ADeck::Shuffle() {
    for (short i = 0; i < 36; i++) {

        short random_i = 0 + rand() % 36;
        //----------------------------------
        Shuffle_Сard->Rank = Deck[i].Rank;
        Shuffle_Сard->Suit = Deck[i].Suit;
        Shuffle_Сard->Trump = Deck[i].Trump;
        //-------------------------------------
        Deck[i].Rank = Deck[random_i].Rank;
        Deck[i].Suit = Deck[random_i].Suit;
        Deck[i].Trump = Deck[random_i].Trump;
        //----------------------------------------
        Deck[random_i].Rank = Shuffle_Сard->Rank;
        Deck[random_i].Suit = Shuffle_Сard->Suit;
        Deck[random_i].Trump = Shuffle_Сard->Trump;
    }
}