// FastFPS.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <Windows.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <utility>
#include <algorithm>



int SceenWidth = 120;
int SceenHeight = 40;


float fPlayerX = 8.f;
float fPlayerY = 8.f;
float fPlayerA = 0.f;


int MapWidth = 16;
int MapHeight = 16;

float fFOV = 3.14159265358979323846f / 4.0f;

float Depth = 16.0f;


int main()
{
	
	wchar_t* screen = new wchar_t[SceenWidth * SceenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);


	std::wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#########......#";
	map += L"#..............#";
	map += L"#...############";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#....###########";
	map += L"#..............#";
	map += L"#..............#";
	map += L"########.......#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";
	DWORD dwBytesWritten = 0;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();
	while (1) {

		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float felapsedTime = elapsedTime.count();
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
			fPlayerA -= (1.8f * felapsedTime);
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
			fPlayerA += (1.8f * felapsedTime);
		}

		if (GetAsyncKeyState((unsigned short)'W') & 0x8000) {
			fPlayerX += sinf(fPlayerA) * 5.f * felapsedTime;
			fPlayerY += cosf(fPlayerA) * 5.f * felapsedTime;

			if (map[(int)fPlayerY * MapWidth + (int)fPlayerX] == '#') {
				fPlayerX -= sinf(fPlayerA) * 5.f * felapsedTime;
				fPlayerY -= cosf(fPlayerA) * 5.f * felapsedTime;
			}
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000) {
			fPlayerX -= sinf(fPlayerA) * 5.f * felapsedTime;
			fPlayerY -= cosf(fPlayerA) * 5.f * felapsedTime;

			if (map[(int)fPlayerY * MapWidth + (int)fPlayerX] == '#') {
				fPlayerX += sinf(fPlayerA) * 5.f * felapsedTime;
				fPlayerY += cosf(fPlayerA) * 5.f * felapsedTime;
			}
		}

		for (int x = 0;x < SceenWidth; ++x) {
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)SceenWidth) * fFOV;

			float fDistanceToWall = 0.f;

			float EyeX = sinf(fRayAngle);
			float EyeY = cosf(fRayAngle);

			bool HitWall = false;
			bool Boundary = false;
			while (!HitWall && fDistanceToWall < Depth) {
				fDistanceToWall += 0.1f;
				int TestX = (int)(fPlayerX + EyeX * fDistanceToWall);
				int TestY = (int)(fPlayerY + EyeY * fDistanceToWall);
			
				if (TestX < 0 || TestX >=MapWidth || TestY < 0 || TestY >= MapHeight) {
					HitWall = true;
					fDistanceToWall = Depth;
				}
				else {
					if (map[TestY * MapWidth + TestX] == '#') {
						HitWall = true;

						std::vector<std::pair<float, float>> p;
						for (int tx = 0;tx < 2; ++tx) {
							for (int ty = 0;ty < 2; ++ty) {
								float vy = (float)TestY + ty - fPlayerY;
								float vx = (float)TestX + tx - fPlayerX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (EyeX * vx / d) + (EyeY * vy / d);
								p.push_back(std::make_pair(d, dot));
							}

							std::sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });
						
							float fBound = 0.01;

							if (acos(p.at(0).second) < fBound) Boundary = true;
							if (acos(p.at(1).second) < fBound) Boundary = true;
							
						}
					}

				}
			}

			int Ceiling = (float)(SceenHeight / 2.0) - SceenHeight / fDistanceToWall;
			int Floor = SceenHeight - Ceiling;

			short Shade = ' ';
			short Shade2 = ' ';
			if (fDistanceToWall <= Depth / 4.0f) {
				Shade = 0x2588;
			}
			else if (fDistanceToWall < Depth / 3.0f) {
				Shade = 0x2593;
			}
			else if (fDistanceToWall < Depth / 2.0f) {
				Shade = 0x2592;
			}
			else if (fDistanceToWall < Depth) {
				Shade = 0x2591;
			}
			else
				Shade = ' ';

			if (Boundary)Shade = ' ';

			for (int y = 0;y < SceenHeight; ++y) {

				if (y <Ceiling) 
					screen[y * SceenWidth + x] = ' ';
				else if(y > Ceiling && y <= Floor) {
					screen[y * SceenWidth + x] = Shade;
				}
				else
				{
					float b = 1.0f - (((float)y - SceenHeight / 2.0f) / ((float)SceenHeight / 2.0f));
					if (b < 0.25) {
						Shade2 = '#';
					}
					else if (b < 0.5) {
						Shade2 = 'x';
					}
					else if (b < 0.75) {
						Shade2 = '.';
					}
					else if (b < 0.9) {
						Shade2 = '_';
					}else Shade2 = ' ';
					screen[y * SceenWidth + x] = Shade2;
					//screen[y * SceenWidth + x] = ' ';
				}
			}
		}

		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", fPlayerX, fPlayerY, fPlayerA, 1 / felapsedTime);
		for (int nx = 0; nx < MapWidth;++nx) {
			for (int ny = 0;ny < MapWidth; ++ny) {
				screen[(ny + 1) * SceenWidth + nx] = map[ny * MapWidth + nx];
			}
		}

		screen[((int)fPlayerY + 1) * SceenWidth + (int)fPlayerX] = 'M';
		screen[SceenWidth * SceenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, SceenWidth * SceenHeight, { 0,0 }, &dwBytesWritten);

	}
	
	
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
