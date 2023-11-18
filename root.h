#include "menu.h"

//----------------------------------------------------------------------------------
class ARoot {
public:
	~ARoot();
	bool Init();
	bool Get_Screen_Buffer();
	int Run();
	bool Update();
	bool Draw();
private:
	HANDLE Std_Input_Hendle = 0;
	HANDLE Std_Output_Hendle = 0;
	HANDLE Screen_Buffer_Hendle = 0;
	COORD Screen_Buffer_Size{};
	COORD Screen_Buffer_Pos{};
	CHAR_INFO* Screen_Buffer = 0;
	CONSOLE_SCREEN_BUFFER_INFO Screen_Buffer_Info{};
	AMenu* Menu = 0;
	AGame* Game = 0;
	unsigned short Screen_Width = 0;
	unsigned short Screen_Height = 0;
	unsigned short Screen_Buffer_Lenght = 0;
	bool Is_Program = false;
	bool Can_Run = true;
	bool Need_Redraw = true;
};
//----------------------------------------------------------------------------------
