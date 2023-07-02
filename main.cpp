#include <iostream>
#include <math.h>
#include <Windows.h>

using namespace std;

const int nScreenWidth = 120;
const int nScreenHeight = 40;

const float fPlayerX = 8.0f;
const float fPlayerY = 8.0f;
const float fPlayerA = 0.0f;

const int nMapHeight = 16;
const int nMapWidth = 16;

const float fFOV = 3.14159f / 4.0f;
const float fDepth = 16.0f;

int main()
{
    // Create screen buffer
    wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    wstring map;

    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    // Game loop
    while (true) {
        for (int x = 0; x < nScreenWidth; x++) {
            // For each column, calculate the projected ray angle into world space
            const float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

            float fDistanceToWall = 0.0f;
            bool bHitWall = false;

            const float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
            const float fEyeY = cosf(fRayAngle);

            while (!bHitWall && fDistanceToWall < fDepth) {
                fDistanceToWall += 0.1f;

                const int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                const int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

                // Test if ray is out of bounds
                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
                    bHitWall = true; // Just set distance to maximum depth
                    fDistanceToWall = fDepth;
                } else {
                    // Ray is inbounds so test to see if the ray cell is a wall block
                    if (map[nTestY * nMapWidth + nTestX] == '#') {
                        bHitWall = true;
                    }
                }
            }

            // Calculate distance to ceiling and floor
            const int nCeiling  = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
            const int nFloor = nScreenHeight - nCeiling;

            for (int y = 0; y < nScreenHeight; y++) {
                if (y < nCeiling) screen[y*nScreenWidth + x] = ' ';
                else if (y > nCeiling && y <= nFloor) screen[y*nScreenWidth + x] = '#';
                else screen[y*nScreenWidth + x] = ' ';
            }
        }

        screen[nScreenWidth * nScreenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, {0, 0}, &dwBytesWritten);
    }

    return 0;
}
