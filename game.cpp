#include "game.h"

//-----------------------------------------------------------------------------------------
AGame::AGame(CHAR_INFO* screen_buffer, unsigned short& screen_width, unsigned short& screen_height, bool& need_update, bool dedager)
	: Screen_Buffer(screen_buffer), Screen_Width(screen_width), Screen_Height(screen_height), Need_Update(need_update), Debager(dedager)
{
}
//-----------------------------------------------------------------------------------------
AGame::~AGame() {
	//Очистка колод карт
	delete[] Player_Card;
	delete[] Computer_Card;
	delete[] Player_Table;
	delete[] Computer_Table;
	//
	delete Deck;
	Deck = nullptr;
}
//-----------------------------------------------------------------------------------------
void AGame::Init() {
	//
	Player_Card = new SCard[36];
	Computer_Card = new SCard[36];
	Player_Table = new SCard[36];
	Computer_Table = new SCard[36];
	//Создаем екземпляр колоды и тусуем её.
	Deck = new ADeck();
	Deck->Init();
	Deck->Shuffle();
	// индекс верхней карты
	Сur_Сart = 36;
	//Наполняем руки игроков картами
	Give_Card(Player_Card, Player_Range);
	Give_Card(Computer_Card, Computer_Range);
	unsigned short pos_start_x = (Screen_Width / 2) - (Table_Width / 2);
	unsigned short pos_start_y = 5;
	Start_Array_Pos = pos_start_x + (Screen_Width * pos_start_y);
	if (!Debager) {
		Game_Menu[0] = "Information";
	}
}
//-----------------------------------------------------------------------------------------
bool AGame::Update() {
	// Draw FRAMES;
	//-----------------------------------------------------------
	for (unsigned short column = 0; column < Table_Height * 3; column++) {
		unsigned short cur_pos = Start_Array_Pos + (column * Screen_Width);
		Screen_Buffer[cur_pos].Char.UnicodeChar = L'┃';
		Screen_Buffer[cur_pos].Attributes = 0x1e;
		Screen_Buffer[cur_pos + Table_Width - 1].Char.UnicodeChar = L'┃';
		Screen_Buffer[cur_pos + Table_Width - 1].Attributes = 0x1e;
	}
	for (unsigned short row = 0; row < Table_Width; row++) {
		unsigned short cur_pos = Start_Array_Pos + row;
		Screen_Buffer[cur_pos].Char.UnicodeChar = L'━';
		Screen_Buffer[cur_pos].Attributes = 0x1e;
		Screen_Buffer[cur_pos + (Table_Height) * Screen_Width].Char.UnicodeChar = L'━';
		Screen_Buffer[cur_pos + (Table_Height) * Screen_Width].Attributes = 0x1e;
		Screen_Buffer[cur_pos + (Table_Height * 2) * Screen_Width].Char.UnicodeChar = L'━';
		Screen_Buffer[cur_pos + (Table_Height * 2) * Screen_Width].Attributes = 0x1e;
		Screen_Buffer[cur_pos + (Table_Height * 3) * Screen_Width].Char.UnicodeChar = L'━';
		Screen_Buffer[cur_pos + (Table_Height * 3 ) * Screen_Width].Attributes = 0x1e;
		if (row == 0) {
			Screen_Buffer[cur_pos].Char.UnicodeChar = L'┏';
			Screen_Buffer[cur_pos + (Table_Height) * Screen_Width].Char.UnicodeChar = L'┣';
			Screen_Buffer[cur_pos + (Table_Height * 2) * Screen_Width].Char.UnicodeChar = L'┣';
			Screen_Buffer[cur_pos + (Table_Height * 3) * Screen_Width].Char.UnicodeChar = L'┗';
		}
		if (row == Table_Width - 1) {
			Screen_Buffer[cur_pos].Char.UnicodeChar = L'┓';
			Screen_Buffer[cur_pos + (Table_Height)*Screen_Width].Char.UnicodeChar = L'┫';
			Screen_Buffer[cur_pos + (Table_Height * 2) * Screen_Width].Char.UnicodeChar = L'┫';
			Screen_Buffer[cur_pos + (Table_Height * 3) * Screen_Width].Char.UnicodeChar = L'┛';
		}
	}
	//Draw menus
	unsigned short menu_postion = Start_Array_Pos + 1 + Screen_Width;
	for (short row = 0; row < 3; row++) {
		int len2 = Game_Menu[row].length();
		for (short i = 0; i < len2; i++) {
			Screen_Buffer[menu_postion + i + row * (Screen_Width * Table_Height)].Char.UnicodeChar = Game_Menu[row][i];
			Screen_Buffer[menu_postion + i + row * (Screen_Width * Table_Height)].Attributes = 0x16;
		}
	}
	// Draw Trump & count of card
	unsigned short date_pos = Start_Array_Pos - Screen_Width + 1;
	for (short i = 0; i < Trump_str.length(); i++) {
		Screen_Buffer[date_pos].Char.UnicodeChar = Trump_str[i];
		Screen_Buffer[date_pos].Attributes = 0x1f;
		++date_pos;
	}
	++date_pos;
	Screen_Buffer[date_pos].Char.UnicodeChar = Deck->Suits[Deck->Trump];
	Screen_Buffer[date_pos].Attributes = 0x1f;
	++date_pos;
	++date_pos;
	for (short i = 0; i < Count_str.length(); i++) {
		Screen_Buffer[date_pos].Char.UnicodeChar = Count_str[i];
		Screen_Buffer[date_pos].Attributes = 0x1f;
		++date_pos;
	}
	++date_pos;
	std::string s = std::to_string(Сur_Сart);
	for (short i = 0; i < s.length(); ++i) {
		Screen_Buffer[date_pos].Char.UnicodeChar = s[i];
		Screen_Buffer[date_pos].Attributes = 0x1f;
		++date_pos;
	}
	//
	if (Player_Range == 0 && Сur_Сart == 0) {
		Game_Menu[0] = "Player Win!";
	}
	if (Computer_Range == 0 && Сur_Сart == 0) {
		Game_Menu[0] = "Computer Win!";
	}
	//Draw Cards
	Draw_Player_Card();
	Draw_Player_Table();
	Draw_Computer_Table();
	if (Debager) {
		Draw_Computer_Card();
	}

	return true;
}
//---------------------------------------------------------------------------------------
void AGame::Player_Pick_Card() {
	bool result = false;
	//-----------------------------------
	if (Player_Turn) {
		if (Player_Table_Range == 0 && Computer_Table_Range == 0 && Computer_Range > 0) {
			Subject_Choose_Card(Player_Card, Player_Range, Player_Table, Player_Table_Range, HighLight_Position);
			HighLight_Position = 0;
		}
		else if (Player_Table_Range > 0) {
			for (short i = 0; i < Player_Table_Range; ++i) {
				if (Player_Table[i].Rank == Player_Card[HighLight_Position].Rank
					&& Computer_Range > 0) {
					Subject_Choose_Card(Player_Card, Player_Range, Player_Table, Player_Table_Range, HighLight_Position);
					HighLight_Position = 0;
					result = true;
					break;
				}
			}
			if (!result) {
				for (short i = 0; i < Computer_Table_Range; ++i) {
					if (Computer_Table[i].Rank == Player_Card[HighLight_Position].Rank
						&& Computer_Range > 0) {
						Subject_Choose_Card(Player_Card, Player_Range, Player_Table, Player_Table_Range, HighLight_Position);
						HighLight_Position = 0;
						break;
					}
				}
			}

		}
		// Computer response
		Computer_Turn();
	}
	//-----------------------------------
	// Player response
	if (!Player_Turn) {
		if ((Player_Card[HighLight_Position].Rank > Computer_Table[Player_Table_Range].Rank
			&& Player_Card[HighLight_Position].Suit == Computer_Table[Player_Table_Range].Suit
			&& Computer_Table_Range > Player_Table_Range) || (Player_Card[HighLight_Position].Trump 
				&& !Computer_Table[Player_Table_Range].Trump)) {
			Subject_Choose_Card(Player_Card, Player_Range, Player_Table, Player_Table_Range, HighLight_Position);
			HighLight_Position = 0;
		}
		// Computer turned
		Computer_Turn();
	}
}
//---------------------------------------------------------------------------------------
void AGame::Give_Card(SCard* subject_card, short &subject_range) {
	// Если у игрока меньше 6 карт и в колоде еще есть карты даем верхнюю карту
	if (subject_range < 6 && Сur_Сart > 0) {
		for (short i = subject_range; i < 6; ++i) {
			subject_card[i].Rank = Deck->Deck[Сur_Сart - 1].Rank;
			subject_card[i].Suit = Deck->Deck[Сur_Сart - 1].Suit;
			subject_card[i].Trump = Deck->Deck[Сur_Сart - 1].Trump;
			++subject_range;
			--Сur_Сart;
		}
	}
}
//---------------------------------------------------------------------------------------
void AGame::Draw_Player_Card() {
	unsigned short pos = Start_Array_Pos + (Screen_Width * ((Table_Height * 2) + 2) + 1);
	// Draw Player_Card
	unsigned short start_pos = pos;
	unsigned short rows = Player_Range < 19 ? 1 : 2;
	unsigned short start = 0;
	unsigned short end = Player_Range < 19 ? Player_Range : 18;
	char atribute = 0x2f;
	for (short j = 0; j < rows; ++j) {
		for (short i = start; i < end; ++i) {
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Ranks_Additional[Player_Card[i].Rank];
			Screen_Buffer[pos].Attributes = atribute;
			++pos;
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Ranks[Player_Card[i].Rank];
			Screen_Buffer[pos].Attributes = atribute;
			++pos;
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Suits[Player_Card[i].Suit];
			Screen_Buffer[pos].Attributes = atribute;
			pos += 2;
		}
		pos = start_pos + (Screen_Width * 2);
		start = 18;
		end += Player_Range - 18;
	}
	// Draw HighLight
	HighLight_Position = HighLight_Position < 0 ? Player_Range - 1 : HighLight_Position > Player_Range - 1 ? 0 : HighLight_Position;
	unsigned short higligh_pos = HighLight_Position < 18 ? start_pos : start_pos + (Screen_Width * 2);
	unsigned short cur_h_pos = HighLight_Position < 18 ? HighLight_Position : HighLight_Position - 18;
	atribute = 0x6f;
	for (short i = 0; i < 3; ++i) {
		Screen_Buffer[higligh_pos + i + (cur_h_pos * 4)].Attributes = atribute;
	}
}
//---------------------------------------------------------------------------------------
void AGame::Draw_Computer_Card() {
	unsigned short pos = Start_Array_Pos + (Screen_Width * 2) + 1;
	unsigned short start_pos = pos;
	unsigned short rows = Computer_Range < 19 ? 1 : 2;
	unsigned short start = 0;
	unsigned short end = Computer_Range < 19 ? Computer_Range : 18;
	char atribute = 0x4f;
	for (short j = 0; j < rows; ++j) {
		for (short i = start; i < end; ++i) {
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Ranks_Additional[Computer_Card[i].Rank];
			Screen_Buffer[pos].Attributes = atribute;
			++pos;
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Ranks[Computer_Card[i].Rank];
			Screen_Buffer[pos].Attributes = atribute;
			++pos;
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Suits[Computer_Card[i].Suit];
			Screen_Buffer[pos].Attributes = atribute;
			pos += 2;
		}
		pos = start_pos + (Screen_Width * 2);
		start = 18;
		end += Computer_Range - 18;
	}
}
//---------------------------------------------------------------------------------------
void AGame::Draw_Player_Table() {
	// Draw Player Table
	unsigned short pos = Start_Array_Pos + (Screen_Width * ((Table_Height) + 3) + 1);
	unsigned short start_pos = pos;
	unsigned short rows = Player_Table_Range < 19 ? 1 : 2;
	unsigned short start = 0;
	unsigned short end = Player_Table_Range < 19 ? Player_Table_Range : 18;
	char atribute = 0x3f;
	for (short j = 0; j < rows; ++j) {
		for (short i = start; i < end; ++i) {
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Ranks_Additional[Player_Table[i].Rank];
			Screen_Buffer[pos].Attributes = atribute;
			++pos;
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Ranks[Player_Table[i].Rank];
			Screen_Buffer[pos].Attributes = atribute;
			++pos;
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Suits[Player_Table[i].Suit];
			Screen_Buffer[pos].Attributes = atribute;
			pos += 2;
		}
		pos = start_pos + (Screen_Width);
		start = 18;
		end += Player_Table_Range - 18;
	}
}
//---------------------------------------------------------------------------------------
void AGame::Draw_Computer_Table() {
	// Draw Computer Table
	unsigned short pos = Start_Array_Pos + (Screen_Width * ((Table_Height)+1) + 1);
	unsigned short start_pos = pos;
	unsigned short rows = Computer_Table_Range < 19 ? 1 : 2;
	unsigned short start = 0;
	unsigned short end = Computer_Table_Range < 19 ? Computer_Table_Range : 18;
	char atribute = 0x5f;
	for (short j = 0; j < rows; ++j) {
		for (short i = start; i < end; ++i) {
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Ranks_Additional[Computer_Table[i].Rank];
			Screen_Buffer[pos].Attributes = atribute;
			++pos;
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Ranks[Computer_Table[i].Rank];
			Screen_Buffer[pos].Attributes = atribute;
			++pos;
			Screen_Buffer[pos].Char.UnicodeChar = Deck->Suits[Computer_Table[i].Suit];
			Screen_Buffer[pos].Attributes = atribute;
			pos += 2;
		}
		pos = start_pos + (Screen_Width);
		start = 18;
		end += Computer_Table_Range - 18;
	}
}
//---------------------------------------------------------------------------------------
void AGame::Subject_Choose_Card(SCard* subject_card, short& subject_range,
	SCard* subject_table_card, short& subject_table_range, unsigned short card_index) {
	subject_table_card[subject_table_range].Rank = subject_card[card_index].Rank;
	subject_table_card[subject_table_range].Suit = subject_card[card_index].Suit;
	subject_table_card[subject_table_range].Trump = subject_card[card_index].Trump;
	++subject_table_range;
	Normalization_Array(subject_card, subject_range, card_index);
}
//---------------------------------------------------------------------------------------
void AGame::Normalization_Array(SCard* subject_card, short& subject_range, unsigned short card_index) {
	for (short i = card_index + 1; i < subject_range; ++i) {
		subject_card[i - 1].Rank = subject_card[i].Rank;
		subject_card[i - 1].Suit = subject_card[i].Suit;
		subject_card[i - 1].Trump = subject_card[i].Trump;
	}
	--subject_range;
}
//---------------------------------------------------------------------------------------
void AGame::Computer_Turn() {
	bool result = true;
	// computer response
	if (!Computer_Go) {
		for (short i = Computer_Table_Range; i < Player_Table_Range; ++i) {
			short cur_cart_index = Find_Smallest_Rank(Player_Table[i].Rank, Player_Table[i].Suit);
			if (Player_Range > 0) {
				if (cur_cart_index >= 0) {
					Subject_Choose_Card(Computer_Card, Computer_Range, Computer_Table, Computer_Table_Range, cur_cart_index);
					break;
				}
			}
			else {
				result = false;
			}
		}
		if (!result) {
			Give_Card(Player_Card, Player_Range);
			Take_All_Cart(Computer_Card, Computer_Range);
		}
	}
	// computer go
	if (Computer_Go) {
		if (Computer_Table_Range == 0) {
			short cur_cart_index = Find_Smallest_Cart();
			if (cur_cart_index >= 0) {
				if (Player_Range > 0) {
					Subject_Choose_Card(Computer_Card, Computer_Range, Computer_Table, Computer_Table_Range, cur_cart_index);
					result = true;
				}
			}
			else {
				result = false;
			}
		}
		else if (Computer_Table_Range > 0) {
			for (short i = 0; i < Computer_Table_Range; ++i) {
				short cur_cart_index = Computer_Added_The_Cart(Computer_Table[i].Rank);
				if (cur_cart_index >= 0) {
					if (Player_Range > 0) {
						Subject_Choose_Card(Computer_Card, Computer_Range, Computer_Table, Computer_Table_Range, cur_cart_index);
						result = true;
					}
				}
				else {
					result = false;
				}
			}
			for (short i = 0; i < Player_Table_Range; ++i) {
				short cur_cart_index = Computer_Added_The_Cart(Player_Table[i].Rank);
				if (cur_cart_index >= 0) {
					if (Player_Range > 0) {
						Subject_Choose_Card(Computer_Card, Computer_Range, Computer_Table, Computer_Table_Range, cur_cart_index);
						result = true;
					}
				}
				else {
					result = false;
				}
			}
			if (!result && Player_Table_Range == Computer_Table_Range) {
				Chage_Part();
			}
		}
	}
	Need_Update = true;
}
//---------------------------------------------------------------------------------------
void AGame::Chage_Part() {
	if (Player_Table_Range == Computer_Table_Range && Player_Turn) {
		Game_Menu[0] = "Computer turn";
		Player_Turn = false;
		Computer_Go = true;
		// Очищаем в отбой игровой стол
		Player_Table_Range = 0;
		Computer_Table_Range = 0;
		// Раздаем карты игрокам если есть в колоде
		Give_Card(Player_Card, Player_Range);
		Give_Card(Computer_Card, Computer_Range);
		Computer_Turn();

	}
	// player take all carts
	else if (!Player_Turn && Player_Table_Range != Computer_Table_Range) {
		// player takes all cart
		Take_All_Cart(Player_Card, Player_Range);
		// Очищаем в отбой игровой стол
		Player_Table_Range = 0;
		Computer_Table_Range = 0;
		// Раздаем карты если есть в колоде
		Give_Card(Computer_Card, Computer_Range);
		Player_Turn = false;
		Computer_Go = true;
		Computer_Turn();
	}
	//
	if (Player_Table_Range == Computer_Table_Range && !Player_Turn) {
		Game_Menu[0] = "Player Turn";
		Player_Turn = true;
		Computer_Go = false;
		// Очищаем в отбой игровой стол
		Player_Table_Range = 0;
		Computer_Table_Range = 0;
		// Раздаем карты игрокам если есть в колоде
		Give_Card(Player_Card, Player_Range);
		Give_Card(Computer_Card, Computer_Range);
	}
}
//---------------------------------------------------------------------------------------
int AGame::Find_Smallest_Rank(short rank, short suit) {
	short cur_runk = Computer_Card[0].Rank;
	short trump_index = -1;
	short cur_index = -1;
	for (short i = 0; i < Computer_Range; ++i) {
		// Ищу наименьший ранк карты той же масти из имеющихся у компьютера
		if (rank < Computer_Card[i].Rank && suit == Computer_Card[i].Suit) {
			if (cur_index >= 0) {
				if (Computer_Card[i].Rank < Computer_Card[cur_index].Rank) {
					cur_index = i;
				}
			}
			else {
				cur_index = i;
			}
		}
		// Ищн наименьший козырь
		if (Computer_Card[i].Trump && suit != Deck->Trump) {
			if (trump_index >= 0) {
				if (Computer_Card[i].Rank < Computer_Card[trump_index].Rank) {
					trump_index = i;
				}
			}
			else {
				trump_index = i;
			}
		}
	}
	if (cur_index >= 0) {
		return cur_index;
	}
	if (trump_index >= 0) {
		return trump_index;
	}
	return -2;
}
//---------------------------------------------------------------------------------------
void AGame::Take_All_Cart(SCard* subject_card, short& subject_range) {
	// Субьект не может отбится и забирает все карты со стола
	// Забираеt все карты со стола игрока
	short len = Player_Table_Range;
	for (short i = 0; i < len; ++i) {
		subject_card[subject_range].Rank = Player_Table[i].Rank;
		subject_card[subject_range].Suit = Player_Table[i].Suit;
		subject_card[subject_range].Trump = Player_Table[i].Trump;
		--Player_Table_Range;
		++subject_range;
	}
	// Забираеt все карты со стола компьютера
	len = Computer_Table_Range;
	for (short i = 0; i < len; ++i) {
		subject_card[subject_range].Rank = Computer_Table[i].Rank;
		subject_card[subject_range].Suit = Computer_Table[i].Suit;
		subject_card[subject_range].Trump = Computer_Table[i].Trump;
		--Computer_Table_Range;
		++subject_range;
	}
}
//---------------------------------------------------------------------------------------
int AGame::Find_Smallest_Cart() {
	short cur_runk = Computer_Card[0].Rank;
	short trump_index = -1;
	short cur_index = -1;
	for (short i = 0; i < Computer_Range; ++i) {
		// Ищу наименьший ранк карты имеющихся у компьютера !НЕ КОЗЫРЬ!
		if (!Computer_Card[i].Trump) {
			if (cur_index >= 0) {
				if (Computer_Card[i].Rank < Computer_Card[cur_index].Rank) {
					cur_index = i;
				}
			}
			else {
				cur_index = i;
			}
		}
		// Ищн наименьший козырь
		if (Computer_Card[i].Trump) {
			if (trump_index >= 0) {
				if (Computer_Card[i].Rank < Computer_Card[trump_index].Rank) {
					trump_index = i;
				}
			}
			else {
				trump_index = i;
			}
		}
	}
	if (cur_index >= 0) {
		return cur_index;
	}
	if (trump_index >= 0) {
		return trump_index;
	}
	return -2;
}
//---------------------------------------------------------------------------------------
int AGame::Computer_Added_The_Cart(short rank) {
	short trump_index = -1;
	short cur_index = -1;
	for (short i = 0; i < Computer_Range; ++i) {
		// Ищу ранк карты имеющихся у компьютера чтобы подкинуть
		if (rank == Computer_Card[i].Rank && !Computer_Card[i].Trump) {
			cur_index = i;
		}
		// Ищн козырь
		if (rank == Computer_Card[i].Rank && Computer_Card[i].Trump) {
			trump_index = i;
		}
	}
	if (cur_index >= 0) {
		return cur_index;
	}
	if (trump_index >= 0) {
		return trump_index;
	}
	return -2;
}
//---------------------------------------------------------------------------------------