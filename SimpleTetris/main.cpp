#include "SimpleTetris.h"
#include "Timer.h"
//shift+f12 : 참조검색. 해당 함수 혹은 변수가 사용된 위치를 알려줌.
//ctrl + shift +t, alt + (달러표시) 해당 위치로 이동.
/*
todo
블록 콤보 이펙트 추가
*/

static constexpr mnp::int32 g_kWidth{ 600 };
static constexpr mnp::int32 g_kHeight{ 1000 };
static mnp::SimpleTetris g_simpleTetris{ g_kWidth, g_kHeight };

LRESULT WINAPI WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return g_simpleTetris.processWindowProc(hWnd, Msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	using namespace mnp;

	g_simpleTetris.set(L"Simple Tetris", hInstance, WinProc);

	Timer checkLevelTimer{};
	checkLevelTimer.set(2, Timer::EUnit::Sec);
	checkLevelTimer.start();

	g_simpleTetris.addFont(L"Consolas", 20, false);
	g_simpleTetris.addFont(L"Jokerman", 46, false);
	g_simpleTetris.addFont(L"휴먼매직체", 20, true);
	g_simpleTetris.addFont(L"휴먼매직체", 80, true);

	static constexpr Color kDefaultColor{ 255, 255, 255 };
	static constexpr Position2 boardPosition{ 10, 80 };
	Color clearColor{ 240, 240, 255 };
	Color normalFontColor{ kDefaultColor };
	Color pauseColor{ 0, 0, 0 };
	Color levelColor{ kDefaultColor };
	
	g_simpleTetris.restartGame();

	while (g_simpleTetris.update() == true)
	{
		g_simpleTetris.updateNextblockQueue();

		g_simpleTetris.updateGameLevel();

		//gameover가 아닌 상황.
		if (g_simpleTetris.isGameOver() == false)
		{
			if (g_simpleTetris.tickInput() == true)
			{
				if (g_simpleTetris.getIsPaused() == true)
				{
					__noop;
				}
				else
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
						g_simpleTetris.addCurrScore();
						g_simpleTetris.addCurrLevelScore();

						while (g_simpleTetris.move(EDirection::S) == true)
						{

						}
					}
					if (GetAsyncKeyState(VK_DOWN) == SHORT(0x8001))
					{
						g_simpleTetris.move(EDirection::S);
					}
				/*	if (GetAsyncKeyState(VK_SPACE) == SHORT(0x8001))
					{
						
					}*/
					if (GetAsyncKeyState('Z') == SHORT(0x8001))
					{
						g_simpleTetris.rotate(false);
					}
					if (GetAsyncKeyState('X') == SHORT(0x8001))
					{
						g_simpleTetris.rotate(true);
					}

#if defined DEBUG || _DEBUG
					if (GetAsyncKeyState('W') == SHORT(0x8001))
					{
						auto timerInterval{ g_simpleTetris.getTimerInterval() };

						g_simpleTetris.setTimerInterval(timerInterval - 50);
					}

					if (GetAsyncKeyState('Q') == SHORT(0x8001))
					{
						auto currBlockType = g_simpleTetris.getCurrBlockType();
						uint32 iNextBlockType{ (uint32)currBlockType + 1 };
						if (iNextBlockType >= (uint32)EBlockType::MAX)
						{
							iNextBlockType = 2;
						}
						g_simpleTetris.setCurrBlockType((EBlockType)iNextBlockType);
					}
					if (GetAsyncKeyState('L') == SHORT(0x8001))
					{
						g_simpleTetris.increasingGameLevel();
					}
#endif 
				}

				if (GetAsyncKeyState('P') == SHORT(0x8001))
				{
					g_simpleTetris.togglePause();
				}

				if (g_simpleTetris.getIsPaused() == true)
				{
					__noop;
				}
				else
				{
					if (g_simpleTetris.tickGameSpeedTimer() == true)
					{
						g_simpleTetris.move(EDirection::S);
					}
				}
			}
		}
		

		g_simpleTetris.beginRendering(clearColor);
		{
			g_simpleTetris.drawBoard(boardPosition, Color(0, 60, 100), Color(255, 255, 255));

			g_simpleTetris.useFont(1);
			g_simpleTetris.drawTextToScreen(Position2(0, 0), Size2(SimpleTetris::kBoardSizePixel.x + 20, boardPosition.y - 10), L"TETRIS", Color(200, 100, 100),
				EHorzAlign::Center, EVertAlign::Center);

			//g_simpleTetris.useFont(0);
			//g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 110, 25), L"POS: " +
			//	std::to_wstring(int32(g_simpleTetris.getCurrPosition().x)) + L", " +
			//	std::to_wstring(int32(g_simpleTetris.getCurrPosition().y)), Color(0, 0, 0));
#if defined DEBUG || _DEBUG
			g_simpleTetris.useFont(2);

			g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 250, 0), L"FPS: " + g_simpleTetris.getFpsWstring(), normalFontColor);
#endif 

			if (g_simpleTetris.getIsLeveluped() == true)
			{
				if (checkLevelTimer.tick() == false)
				{
					//addLevelColor += Color(5, 5, 5);

					levelColor += Color(0.5f, 1.0f, 1.5f);
				}
				else
				{
					checkLevelTimer.reset();

					levelColor = kDefaultColor;

					g_simpleTetris.endLevelUped();
				}
			}
			else
			{
				checkLevelTimer.reset();
			}

			g_simpleTetris.useFont(2);
			g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 250, 25), L"SCORE: " + std::to_wstring(g_simpleTetris.getCurrScore()), normalFontColor);
			g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 250, 65), L"EXP: " + std::to_wstring(g_simpleTetris.getCurrLevelScore()), normalFontColor);
			g_simpleTetris.drawTextToScreen(Position2(g_kWidth - 250, 45), L"LEVEL: " + std::to_wstring(g_simpleTetris.getCurrLevel()), levelColor);

			if (g_simpleTetris.isGameOver() == true)
			{
				g_simpleTetris.useFont(3);
				g_simpleTetris.drawTextToScreen(Position2(0, 0), Size2(g_kWidth, g_kHeight), L"GAME OVER"
					, normalFontColor, EHorzAlign::Center, EVertAlign::Center);
				if (GetAsyncKeyState('R') == SHORT(0x8001))
				{
					g_simpleTetris.restartGame();
				}
			}

			if (g_simpleTetris.getIsPaused() == true)
			{
				g_simpleTetris.useFont(3);
				g_simpleTetris.drawTextToScreen(Position2(0, 0), Size2(g_kWidth, g_kHeight), L"PAUSE"
					, pauseColor, EHorzAlign::Center, EVertAlign::Center);
			}
		}
		g_simpleTetris.endRendering();
	}
	return 0;
}
