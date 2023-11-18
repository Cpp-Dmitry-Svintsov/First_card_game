#include "game.h"
//----------------------------------------------------------------------------------
class AMenu {
public:
	//AMenu(CHAR_INFO* screen_buffer, unsigned short &screen_width, unsigned short &screen_height);
	//CHAR_INFO* Screen_Buffer;
	//unsigned short Screen_Width;
	//unsigned short Screen_Height;
	unsigned short Frame_Width = 0;
	unsigned short Frame_Height = 0;
	short Highlight_Position = 0;
	unsigned short Highlight_Lenght = 0;
	std::string Menu_Header = "Menu:";
	std::string List_Menu[3] = {"debagging", "game", "Exit"};
	bool Update(CHAR_INFO* screen_buffer, unsigned short screen_width, unsigned short screen_height);
};