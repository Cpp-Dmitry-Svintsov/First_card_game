#include "common.h"

class AGame {
public:
	AGame(CHAR_INFO* screen_buffer, unsigned short &screen_width, unsigned short &screen_height,
		bool &need_update, bool dedager);
	~AGame();
	CHAR_INFO* Screen_Buffer;
	unsigned short Screen_Width;
	unsigned short Screen_Height;
	bool Need_Update;
	bool Debager;
	short HighLight_Position = 0;
	short Player_Range = 0;
	void Init();
	bool Update();
	void Player_Pick_Card();
	void Computer_Turn();
	void Chage_Part();
private:
	void Give_Card(SCard* subject_card, short &subject_range);
	void Draw_Player_Card();
	void Draw_Computer_Card();
	void Draw_Player_Table();
	void Draw_Computer_Table();
	void Subject_Choose_Card(SCard* subject_card, short& subject_range, SCard* subject_table_card,
		short& subject_table_range, unsigned short card_index);
	void Normalization_Array(SCard* subject_card, short& subject_range, unsigned short card_index);
	int Find_Smallest_Rank(short rank, short suit);
	void Take_All_Cart(SCard* subject_card, short& subject_range);
	int Computer_Added_The_Cart(short rank);
	int Find_Smallest_Cart();
	unsigned short Table_Width = 80;
	unsigned short Table_Height = 5;
	unsigned short Start_Array_Pos = 0;
	unsigned short Сur_Сart;
	std::string Game_Menu[3] = { "Computer_Table", "", "Player_Table" };
	std::string Trump_str = "Trmp -->";
	std::string Count_str = "Card in the deck -->";
	SCard* Player_Card = 0;
	SCard* Computer_Card = 0;
	short Computer_Range = 0;
	SCard* Player_Table = 0;
	short Player_Table_Range = 0;
	SCard* Computer_Table = 0;
	short Computer_Table_Range = 0;
	ADeck* Deck = nullptr;
	bool Player_Turn = true;
	bool Computer_Go = false;
};