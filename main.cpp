#include <iostream>
#include <Windows.h>

using namespace std;

const int nScreenWidth = 120;
const int nScreenHeight = 40;

int main()
{
    // Create screen buffer
    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    screen[nScreenWidth * nScreenHeight - 1] = '\0';
    WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
    return 0;
}