#include "menu.h";

//----------------------------------------------------------------------------------
//AMenu::AMenu(CHAR_INFO* screen_buffer, unsigned short screen_width, unsigned short screen_height)
//	: Screen_Buffer(screen_buffer), Screen_Width(screen_width), Screen_Height(screen_height)
//{
//}
//----------------------------------------------------------------------------------
bool AMenu::Update(CHAR_INFO* Screen_Buffer, unsigned short Screen_Width, unsigned short Screen_Height) {
	//Draw Frame
	//----------------------------------------------------------------------------------
	Frame_Width = Screen_Width / 4;
	Frame_Height = Screen_Height / 3;
	Highlight_Lenght = Frame_Width - 2;
	unsigned short pos_start_x = (Screen_Width / 2 - Frame_Width / 2) - 1;
	unsigned short pos_start_y = (Screen_Height / 2 - Frame_Height / 2) - 1;
	unsigned short start_array_pos = pos_start_x + (Screen_Width * pos_start_y);
	for (unsigned short column = 0; column < Frame_Height; column++) {
		unsigned short cur_pos = start_array_pos + (column * Screen_Width);
		Screen_Buffer[cur_pos].Char.UnicodeChar = L'║';
		Screen_Buffer[cur_pos].Attributes = 0x1B;
		Screen_Buffer[cur_pos + Frame_Width - 1].Char.UnicodeChar = L'║';
		Screen_Buffer[cur_pos + Frame_Width - 1].Attributes = 0x1B;
	}
	for (unsigned short row = 0; row < Frame_Width; row++) {
		unsigned short cur_pos = start_array_pos + row;
		Screen_Buffer[cur_pos].Char.UnicodeChar = L'═';
		Screen_Buffer[cur_pos].Attributes = 0x1B;
		Screen_Buffer[cur_pos + (Frame_Height - 1) * Screen_Width].Char.UnicodeChar = L'═';
		Screen_Buffer[cur_pos + (Frame_Height - 1) * Screen_Width].Attributes = 0x1B;
		if (row == 0) {
			Screen_Buffer[cur_pos].Char.UnicodeChar = L'╔';
			Screen_Buffer[cur_pos + (Frame_Height - 1) * Screen_Width].Char.UnicodeChar = L'╚';
		}
		if (row == Frame_Width - 1) {
			Screen_Buffer[cur_pos].Char.UnicodeChar = L'╗';
			Screen_Buffer[cur_pos + (Frame_Height - 1) * Screen_Width].Char.UnicodeChar = L'╝';
		}
	}
	// Highlight limiter

	Highlight_Position = (Highlight_Position < 0) ? 0 : Highlight_Position > 2 ? 2 : Highlight_Position;
	//Draw Menu Item
	//----------------------------------------------------------------------------------
	unsigned short header_position = start_array_pos + Screen_Width + (Frame_Width / 2) - 3;
	//Draw Header
	for (short i = 0; i < Menu_Header.length(); i++) {
		Screen_Buffer[header_position + i].Char.UnicodeChar = Menu_Header[i];
		Screen_Buffer[header_position + i].Attributes = 0x1f;
	}
	//Draw menus
	unsigned short menu_postion = start_array_pos + 1 + (3 * Screen_Width);
	for (short row = 0; row < 3; row++) {
		int len2 = List_Menu[row].length();
		for (short i = 0; i < len2; i++) {
			Screen_Buffer[menu_postion + i + row * Screen_Width + 2].Char.UnicodeChar = List_Menu[row][i];
			Screen_Buffer[menu_postion + i + row * Screen_Width + 2].Attributes = 0x10;
		}
	}
	//----------------------------------------------------------------------------------
		//Draw Highlight
	//----------------------------------------------------------------------------------
	unsigned short highlight_postion = menu_postion + (Highlight_Position * Screen_Width);
	for (unsigned short i = 0; i < Highlight_Lenght; i++) {
		Screen_Buffer[highlight_postion + i].Attributes = 0x3e;
	}
	//----------------------------------------------------------------------------------
	std::string str = "Hello";
	for (int i = 0; i < str.length(); i++) {
		Screen_Buffer[i].Char.UnicodeChar = str[i];
		Screen_Buffer[i].Attributes = 0x50;
	}
	return true;
}
//----------------------------------------------------------------------------------