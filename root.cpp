#include "root.h"

//----------------------------------------------------------------------------------
ARoot::~ARoot() {
    // Restore the original active screen buffer.
    if (!SetConsoleActiveScreenBuffer(Std_Output_Hendle))
    {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
    }
    delete Screen_Buffer;
    delete Menu;
    delete Game;
}
//----------------------------------------------------------------------------------
bool ARoot::Init() {
	Std_Input_Hendle = GetStdHandle(STD_INPUT_HANDLE);
	Std_Output_Hendle = GetStdHandle(STD_OUTPUT_HANDLE);
    Screen_Buffer_Hendle = CreateConsoleScreenBuffer(
        GENERIC_READ |           // read/write access
        GENERIC_WRITE,
        FILE_SHARE_READ |
        FILE_SHARE_WRITE,        // shared
        NULL,                    // default security attributes
        CONSOLE_TEXTMODE_BUFFER, // must be TEXTMODE
        NULL);

    if (!SetConsoleActiveScreenBuffer(Screen_Buffer_Hendle))
    {
        printf("SetConsoleActiveScreenBuffer failed - (%d)\n", GetLastError());
        return false;
    }

    if (!Get_Screen_Buffer()) {
        printf("Get_Screen_Buffer failed - (%d)\n", GetLastError());
        return false;
    }
    Menu = new AMenu();
    return true;
}
//----------------------------------------------------------------------------------
bool ARoot::Get_Screen_Buffer() {
    if (!GetConsoleScreenBufferInfo(Screen_Buffer_Hendle, &Screen_Buffer_Info))
    {
        printf("GetConsoleScreenBufferInfo failed - (%d)\n", GetLastError());
        return false;
    }

    Screen_Buffer_Size = Screen_Buffer_Info.dwSize;
    Screen_Width = Screen_Buffer_Size.X;
    Screen_Height = Screen_Buffer_Size.Y;
    Screen_Buffer_Lenght = Screen_Width * Screen_Height;
    Screen_Buffer = new CHAR_INFO[Screen_Buffer_Lenght];
    return true;
}
//----------------------------------------------------------------------------------
int ARoot::Run() {
    unsigned long records_count;
    INPUT_RECORD input_record[128];
    while (Can_Run) {
        if (PeekConsoleInput(Std_Input_Hendle, input_record, 128, &records_count)) {
            if (ReadConsoleInput(Std_Input_Hendle, input_record, 1, &records_count)) {
                if (records_count != 0) {
                    if (input_record[0].EventType == KEY_EVENT && input_record[0].Event.KeyEvent.bKeyDown) {
                        //Обработчик нажатия клавиш меню
                        if (!Is_Program) {
                            switch (input_record[0].Event.KeyEvent.wVirtualKeyCode) {
                            case VK_F10:
                                Can_Run = false;
                                break;
                            case VK_UP:
                                --Menu->Highlight_Position;
                                Need_Redraw = true;
                                break;
                            case VK_DOWN:
                                ++Menu->Highlight_Position;
                                Need_Redraw = true;
                                break;
                            case VK_RETURN:
                                if (Menu->Highlight_Position == 0) {
                                    Is_Program = true;
                                    Game = new AGame(Screen_Buffer, Screen_Width, Screen_Height, Need_Redraw, true);
                                    Game->Init();
                                }
                                if (Menu->Highlight_Position == 1) {
                                    Is_Program = true;
                                    Game = new AGame(Screen_Buffer, Screen_Width, Screen_Height, Need_Redraw, false);
                                    Game->Init();
                                }
                                if (Menu->Highlight_Position == 2) {
                                    Can_Run = false;
                                }
                                Need_Redraw = true;
                                break;
                            }
                        }
                        //Обрабочик нажатия клавиш игры
                        else {
                            switch (input_record[0].Event.KeyEvent.wVirtualKeyCode) {
                            case VK_F10:
                                Is_Program = false;
                                delete Game;
                                Game = nullptr;
                                Need_Redraw = true;
                                //Переводим ползунок меню в менулевое положение
                                Menu->Highlight_Position = 0;
                                break;
                            case VK_RIGHT:
                                if (Game->Player_Range > 0) {
                                    ++Game->HighLight_Position;
                                }
                                Need_Redraw = true;
                                break;
                            case VK_LEFT:
                                if (Game->Player_Range > 0) {
                                    --Game->HighLight_Position;
                                }
                                Need_Redraw = true;
                                break;
                            case VK_SPACE:
                                if (Game->Player_Range > 0) {
                                    Game->Player_Pick_Card();
                                }
                                Need_Redraw = true;
                                break;
                            case VK_RETURN:
                                Game->Chage_Part();
                                Need_Redraw = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
        //----------------------------------------------------------------------------------
        if (Need_Redraw)
        {
            //memset(Screen_Buffer, 0x1b, (Screen_Buffer_Info.dwSize.X * Screen_Buffer_Info.dwSize.Y) * sizeof(CHAR_INFO));
            //------------------------------------------------------------------------------
            if (!Update())
                return -1;
            if (!Draw())
                return -1;
            Need_Redraw = false;
        }
        // Позволяет потоку нашей программы уснуть на ~50 мс
        Sleep(50);
    }
    return 1;
}

//----------------------------------------------------------------------------------
bool ARoot::Update() {
    // Clear Screen
    for (short i = 0; i < (Screen_Buffer_Lenght); i++) {
        Screen_Buffer[i].Char.UnicodeChar = L' ';
        Screen_Buffer[i].Attributes = 0x1b;
    }
    //-------------------------------------------------------
    if (!Is_Program) {
        if (!Menu->Update(Screen_Buffer, Screen_Width, Screen_Height)) {
            printf("Menu failed - (%d)\n", GetLastError());
            return false;
        }
    }
    else {
        //-------------------------------------------------------------------------------
        if (!Game->Update()) {
            printf("Game failed - (%d)\n", GetLastError());
            return false;
        }
    }
    return true;
}
//----------------------------------------------------------------------------------
bool ARoot::Draw() {
    if (!WriteConsoleOutput(
        Screen_Buffer_Hendle, // screen buffer to write to
        Screen_Buffer,        // buffer to copy from
        Screen_Buffer_Size,     // col-row size of chiBuffer
        Screen_Buffer_Pos,    // top left src cell in chiBuffer
        &Screen_Buffer_Info.srWindow))
    {
        printf("WriteConsoleOutput failed - (%d)\n", GetLastError());
        return 1;
    }
    return true;
}

