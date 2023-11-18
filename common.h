#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <string>

//-------------------------------------------------------------------------------------------------------
// This is player_card
struct SCard {
	//SCard(short rank, short suit, bool trump)
	//	:Rank(rank), Suit(suit), Trump(trump)
	//{
	//}
	short Rank;
	short Suit;
	bool Trump;
};
//-------------------------------------------------------------------------------------------------------
class ADeck {
public:
	~ADeck();
	void Init();
	void Shuffle();
	//CHAR_INFO* Ranks_Alternate;
	wchar_t Ranks_Additional[9] = { L' ', L' ', L' ', L' ', L'1', L' ', L' ', L' ', L' ' };
	wchar_t Ranks[9] = { L'6', L'7', L'8', L'9', L'0', L'J', L'Q', L'K', L'A'};
	wchar_t Suits[4] = { L'♦', L'♥', L'♠', L'♣' };
	SCard* Deck = 0;
	SCard* Shuffle_Сard = 0;
	unsigned short Trump = 0;
};