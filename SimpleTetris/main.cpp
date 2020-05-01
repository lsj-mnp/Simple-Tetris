#include "SimpleTetris.h"
//shift+f12 : 참조검색. 해당 함수 혹은 변수가 사용된 위치를 알려줌.

static constexpr fs::int32 g_kWidth{ 400 };
static constexpr fs::int32 g_kHeight{ 600 };
static fs::SimpleTetris g_simpleTetris{ g_kWidth, g_kHeight };

LRESULT WINAPI WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return g_simpleTetris.processWindowProc(hWnd, Msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	using namespace fs;

	g_simpleTetris.create(L"Simple Tetris", hInstance, WinProc);

	g_simpleTetris.addFont(L"Consolas", 20, false);
	g_simpleTetris.addFont(L"Jokerman", 46, false);

	const Position2 boardPosition{ 10, 60 };
	Color clearColor{ 240, 240, 255 };
	Color fpsColor{ 0, 0, 100 };

	while (g_simpleTetris.update() == true)
	{
		if (g_simpleTetris.tickInput() == true)
		{
			//0x8001: key가 처음 눌렸을 때를 의미함.
			if (GetAsyncKeyState(VK_LEFT) == SHORT(0x8001))
			{
				g_simpleTetris.move(EDirection::E);
			}
			if (GetAsyncKeyState(VK_RIGHT) == SHORT(0x8001))
			{
				g_simpleTetris.move(EDirection::W);
			}
			if (GetAsyncKeyState(VK_UP) == SHORT(0x8001))
			{
				g_simpleTetris.move(EDirection::N);
			}
			if (GetAsyncKeyState(VK_DOWN) == SHORT(0x8001))
			{
				g_simpleTetris.move(EDirection::S);
			}
			if (GetAsyncKeyState(VK_SPACE) == SHORT(0x8001))
			{
				g_simpleTetris.rotate();
			}
		}

		g_simpleTetris.beginRendering(clearColor);
		{
			g_simpleTetris.drawBoard(boardPosition, Color(0, 60, 100), Color(200, 200, 200));

			g_simpleTetris.useFont(1);
			g_simpleTetris.drawTextToScreen(Position2(0, 0), Size2(SimpleTetris::kBoardSizePixel.x + 20, boardPosition.y - 10), L"TETRIS", Color(200, 100, 100),
				EHorzAlign::Center, EVertAlign::Center);

			g_simpleTetris.useFont(0);
			g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 150, 25), L"POS: " +
				std::to_wstring(int32(g_simpleTetris.getCurrPosition().x)) + L", " +
				std::to_wstring(int32(g_simpleTetris.getCurrPosition().y)), Color(0, 0, 0));

			g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 100, 0), L"FPS: " + g_simpleTetris.getFpsWstring(), fpsColor);
		}
		g_simpleTetris.endRendering();
	}
	return 0;
}
