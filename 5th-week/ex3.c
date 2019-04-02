#include <windows.h>
#include <stdio.h>

int main(void){
    HANDLE hIn, h0ut;
    INPUT_RECORD rec;
    DWORD dw;
    hIn = GetStdHandle(STD_INPUT_HANDLE);
    h0ut = GetStdHandle(STD_OUTPUT_HANDLE);
    while(1){
        ReadConsoleInput(hIn, &rec, 1, &dw);

        if (rec.EventType == MOUSE_EVENT){
            if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED){
                SetConsoleCursorPosition(h0ut,rec.Event.MouseEvent.dwMousePosition);
                fputc('*',stdout);
                if (rec.Event.MouseEvent.dwEventFlags & DOUBLE_CLICK){
                    break;
                }
            }
        }
    }
}