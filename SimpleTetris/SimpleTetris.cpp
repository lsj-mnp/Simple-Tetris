﻿#include "SimpleTetris.h"
#include <thread>
//assert는 release 빌드에서는 assert를 사용한 줄을 아예 실행이 되지 않음.
//따라서 release 빌드에서 쓰려면 다른 논리를 사용해야 함.
#include <cassert>

//#pragma comment(lib, "Lib/fmodL_vc.lib")


mnp::SimpleTetris::SimpleTetris(int32 width, int32 height) : IGraphicalWindow(width, height)
{
	_bingoTimer.set(100, Timer::EUnit::Milli);
}

mnp::SimpleTetris::~SimpleTetris()
{
}

void mnp::SimpleTetris::set(const std::wstring& title, HINSTANCE hInstance, WNDPROC windowProc)
{
	createInternal(title, hInstance, windowProc);

	srand(std::chrono::steady_clock::now().time_since_epoch().count());

	/*createBlock(EBlockType::Used,	Color(100, 100, 100));
	createBlock(EBlockType::I,		Color(255,  60,	 60));
	createBlock(EBlockType::T,		Color(160,  80, 200));
	createBlock(EBlockType::O,		Color(255, 255,   0));
	createBlock(EBlockType::L,		Color(255, 200,   0));
	createBlock(EBlockType::InvL,	Color(  0, 180, 255));
	createBlock(EBlockType::Z,		Color( 40, 255,	 40));
	createBlock(EBlockType::S,		Color(255, 127,	255));*/

	// [작업 디렉터리 설정]
	// '프로젝트 속성' - '디버깅' - '작업 디렉터리'
	// 작업 디렉터리는 상대 경로의 기준이 됨.
	// 예를 들어
	// $(ProjectDir)인 경우, 프로젝트의 디렉터리가 상대 경로의 기준이 되고
	// $(SolutionDir)인 경우, 솔루션의 디렉터리가 상대 경로의 기준이 된다.

	createBlock(EBlockType::Used, Color(100, 100, 100));
	createBlockFromImage(EBlockType::I,	    L"Asset/Blocks/skyblue.png");
	createBlockFromImage(EBlockType::T,	    L"Asset/Blocks/purple.png");
	createBlockFromImage(EBlockType::O,	    L"Asset/Blocks/yellow.png");
	createBlockFromImage(EBlockType::L,		L"Asset/Blocks/orange.png");
	createBlockFromImage(EBlockType::InvL,  L"Asset/Blocks/blue.png");
	createBlockFromImage(EBlockType::Z,     L"Asset/Blocks/red.png");
	createBlockFromImage(EBlockType::S,     L"Asset/Blocks/green.png");
	createBlockFromImage(EBlockType::Bingo, L"Asset/Blocks/line.png");

	createBackgroundFromImage(EBackground::Main,      L"Asset/main.png");
	createBackgroundFromImage(EBackground::Sea,       L"Asset/sea.png");
	createBackgroundFromImage(EBackground::Sunset,    L"Asset/sunset.png");
	createBackgroundFromImage(EBackground::Space,     L"Asset/space.png");
	createBackgroundFromImage(EBackground::Sun,       L"Asset/sun.png");
	createBackgroundFromImage(EBackground::BlackHole, L"Asset/blackhole.png");

	// I형 블록
	{
		_blocks[(int)EBlockType::I][(int)EDirection::N].set(
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0
		);

		_blocks[(int)EBlockType::I][(int)EDirection::W].set(
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::I][(int)EDirection::S].set(
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0
		);

		_blocks[(int)EBlockType::I][(int)EDirection::E].set(
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
	}

	// T자형 블록
	{
		_blocks[(int)EBlockType::T][(int)EDirection::N].set(
			0, 1, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::T][(int)EDirection::W].set(
			0, 1, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::T][(int)EDirection::S].set(
			0, 0, 0, 0,
			1, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::T][(int)EDirection::E].set(
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);
	}

	// O자형 블록
	{
		_blocks[(int)EBlockType::O][(int)EDirection::N].set(
			1, 1, 0, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::O][(int)EDirection::W].set(
			1, 1, 0, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::O][(int)EDirection::S].set(
			1, 1, 0, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::O][(int)EDirection::E].set(
			1, 1, 0, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
	}

	// L자형 블록
	{
		_blocks[(int)EBlockType::L][(int)EDirection::N].set(
			1, 0, 0, 0,
			1, 0, 0, 0,
			1, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::L][(int)EDirection::W].set(
			0, 0, 0, 0,
			0, 0, 1, 0,
			1, 1, 1, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::L][(int)EDirection::S].set(
			0, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::L][(int)EDirection::E].set(
			1, 1, 1, 0,
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
	}

	// InvL자형 블록
	{
		_blocks[(int)EBlockType::InvL][(int)EDirection::N].set(
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 1, 1, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::InvL][(int)EDirection::W].set(
			1, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::InvL][(int)EDirection::S].set(
			1, 1, 0, 0,
			1, 0, 0, 0,
			1, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::InvL][(int)EDirection::E].set(
			0, 0, 0, 0,
			1, 0, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0
		);
	}

	// Z자형 블록
	{
		_blocks[(int)EBlockType::Z][(int)EDirection::N].set(
			0, 1, 0, 0,
			1, 1, 0, 0,
			1, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::Z][(int)EDirection::W].set(
			0, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::Z][(int)EDirection::S].set(
			0, 0, 1, 0,
			0, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::Z][(int)EDirection::E].set(
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
	}

	// S자형 블록
	{
		_blocks[(int)EBlockType::S][(int)EDirection::N].set(
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 1, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::S][(int)EDirection::W].set(
			0, 1, 1, 0,
			1, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::S][(int)EDirection::S].set(
			1, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(int)EBlockType::S][(int)EDirection::E].set(
			0, 0, 0, 0,
			0, 1, 1, 0,
			1, 1, 0, 0,
			0, 0, 0, 0
		);
	}

	updateNextblockQueue();

	/*_board[10][4] = 1;
	_board[6][0] = 1;*/
}

bool mnp::SimpleTetris::update()
{
	if (_bingoTimer.tick() == true)
	{
		while (_bingoLineIndices.size() > 0)
		{
			clearBingoLine(_bingoLineIndices.back());

			_bingoLineIndices.pop_back();
		}

		_bingoTimer.stop();
	}

	return __super::update();
}

void mnp::SimpleTetris::drawBoard(const Position2& position, const Color& borderColor, const Color& boardColor)
{
	//createImageFromFile()로 이미지 파일을 받아와서 보드나 판 이미지를 그릴 수 있다.
	//받아온 이미지를 저장할 변수는 멤버변수로 따로 만들어줘야함.
	
	//drawImageToScreen(_iiBackground[(uint32)EBackground::Space], Position2(0,0));
	//createImageFromFile(L"C:/Users/munop/OneDrive/문서/GitHub/Simple-Tetris/Simple-Tetris/Asset/testbackground.png");
	
	if (_isGameStart == false)
	{
		drawImageToScreen(_iiBackground[(uint32)EBackground::Main], Position2(0, 0));
	}
	else
	{
		if (_currLevel <= 20)
		{
			drawImageToScreen(_iiBackground[(uint32)EBackground::Sea], Position2(0, 0));
		}
		else if (_currLevel >= 21 && _currLevel <= 40)
		{
			drawImageToScreen(_iiBackground[(uint32)EBackground::Sunset], Position2(0, 0));
		}
		else if (_currLevel >= 41 && _currLevel <= 60)
		{
			drawImageToScreen(_iiBackground[(uint32)EBackground::Space], Position2(0, 0));
		}
		else if (_currLevel >= 61 && _currLevel <= 80)
		{
			drawImageToScreen(_iiBackground[(uint32)EBackground::Sun], Position2(0, 0));
		}
		else
		{
			drawImageToScreen(_iiBackground[(uint32)EBackground::BlackHole], Position2(0, 0));
		}

		// 판 테두리
		drawRectangleToScreen(position - Position2(10, 10), kBoardSizePixel + Size2(20, 20), borderColor);

		// 판
		drawRectangleToScreen(position, kBoardSizePixel, boardColor);

		// 현재 블록
		setBlockToBoard(_currBlockType, _currPosition, _currDirection);

		drawGrid(position);

		Position2 nextBLockPosition{ position + Position2(kBoardSizePixel.x + 30, 10) };

		//다음 블록 테두리
		drawRectangleToScreen(nextBLockPosition, kBlockSize * Size2(5, 11), borderColor);

		//다음 블록들
		drawBlockToScreen(_nextBlockQueue[0], nextBLockPosition + kBlockSize, EDirection::N);
		drawBlockToScreen(_nextBlockQueue[1], nextBLockPosition + Size2(kBlockSize.x, kBlockSize.y * 6), EDirection::N);

		//보드에 블록을 그리는 for문
		for (float y = 0; y < kBoardSize.y; y += 1)
		{
			for (float x = 0; x < kBoardSize.x; x += 1)
			{
				uint8 block{ _board[uint32(y)][uint32(x)] };

				if (block != 0)
				{
					drawImageToScreen(_iiBlocks[block], position + kBlockSize * Position2(x, y));
				}
			}
		}

		setBlockToBoard(_currBlockType, _currPosition, _currDirection, true);
		
		if (_currLevel <= 30)
		{
			//보드에 반투명 블록을 그리는 for문
			bool shouldDraw{ false };
			//max = 둘 중 큰 숫자를 가져오는 매크로임. 따라서 최소값을 지정하려면 max를 사용해야 함.(더 작은 숫자를 무시함.)
			for (int i = max(_currPosition.y, 0); i < kBoardSize.y; ++i)
			{
				if (canDrawBlock(_currBlockType, Position2(_currPosition.x, i), _currDirection) == true)
				{
					shouldDraw = true;
				}
				else
				{
					if (shouldDraw == true)
					{
						uint32 iCurrBlockType{ (uint32)_currBlockType };
						uint32 iCurrDirection{ (uint32)_currDirection };
						for (int32 y = 0; y < kBlockContainerSize; ++y)
						{
							for (int32 x = 0; x < kBlockContainerSize; ++x)
							{
								if (_blocks[iCurrBlockType][iCurrDirection].data[y][x] == 0) continue;

								drawImageAlphaToScreen(_iiBlocks[iCurrBlockType],
									position + kBlockSize * Position2(_currPosition.x + x, i - 1 + y), (uint8)63);
							}
						}
					}
					break;
				}
			}
		}
		else
		{
			__noop;
		}
	}
}

void mnp::SimpleTetris::drawGrid(const mnp::Position2& startPosition)
{
	for (int y = 0; y < (int)kBoardSize.y; ++y)
	{
		drawLineToScreen(Position2(startPosition.x, startPosition.y + (y * kBlockSize.y))
			, Position2((startPosition.x + kBoardSizePixel.x - 1), startPosition.y + (y * kBlockSize.y)), Color(50, 50, 50));
	}

	for (int x = 0; x < (int)kBoardSize.x; ++x)
	{
		drawLineToScreen(Position2(startPosition.x + (x * kBlockSize.x), startPosition.y)
			, Position2(startPosition.x + (x * kBlockSize.x), startPosition.y + (kBoardSizePixel.y - 1)), Color(50, 50, 50));
	}
}

bool mnp::SimpleTetris::move(EDirection eDirection)
{
	//현재 블록을 지우기 위해 호출함.
	setBlockToBoard(_currBlockType, _currPosition, _currDirection, true);

	switch (eDirection)
	{
	case mnp::EDirection::N:
		if (canDrawBlock(_currBlockType, _currPosition - Position2(0, 1), _currDirection) == true)
		{
			_currPosition.y -= 1;

			return true;
		}
		break;
	case mnp::EDirection::W:
		if (canDrawBlock(_currBlockType, _currPosition + Position2(1, 0), _currDirection) == true)
		{
			_currPosition.x += 1;

			return true;
		}
		break;
	case mnp::EDirection::S:
		if (canDrawBlock(_currBlockType, _currPosition + Position2(0, 1), _currDirection) == true)
		{
			_currPosition.y += 1;

			return true;
		}
		else
		{
			//지워진 현재 블록을 다시 그림.
			setBlockToBoard(_currBlockType, _currPosition, _currDirection);

			if (_currPosition == getInitialBlockPosition())
			{
				_isGameOver = true;
			}

			// 새 블록 스폰
			spawnNewBlock();

			if (canDrawBlock(_currBlockType, _currPosition, _currDirection) == false)
			{
				_isGameOver = true;
			}

			_nextBlockQueue.pop_front();

			checkBingo();
		}
		break;
	case mnp::EDirection::E:
		if (canDrawBlock(_currBlockType, _currPosition - Position2(1, 0), _currDirection) == true)
		{
			_currPosition.x -= 1;

			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

void mnp::SimpleTetris::rotate(bool clockWise)
{
	setBlockToBoard(_currBlockType, _currPosition, _currDirection, true);

	_tetrisSound.soundPlay("뿅");

	int32 currDirection{ int32(_currDirection) };

	if (clockWise == true)
	{
		++currDirection;

		if (currDirection > 3)
		{
			currDirection = 0;
		}
	}
	else
	{
		--currDirection;

		if (currDirection < 0)
		{
			currDirection = 3;
		}
	}


	EDirection nextDirection = EDirection(currDirection);
	if (canDrawBlock(_currBlockType, _currPosition, nextDirection) == true)
	{
		_currDirection = nextDirection;
	}
	else
	{
		Position2 resultPosition{};
		if (getRotatablePosition(nextDirection, resultPosition) == true)
		{
			if (canDrawBlock(_currBlockType, resultPosition, nextDirection) == true)
			{
				_currPosition = resultPosition;
				_currDirection = nextDirection;
			}
		}
	}
}

const bool mnp::SimpleTetris::getRotatablePosition(EDirection eNextDirection, mnp::Position2& outPosition) const
{
	outPosition = _currPosition;
	bool shouldPush{ false };
	const auto& nextBlock{ _blocks[(uint32)_currBlockType][(uint32)eNextDirection] };
	int32 minX{};
	int32 maxX{};
	for (int32 y = 0; y < 4; ++y)
	{
		for (int32 x = 0; x < 4; ++x)
		{
			const uint8 blockValue{ nextBlock.data[y][x] };
			if (blockValue == 0) continue;

			const int32 nextX{ (int32)_currPosition.x + x };
			if (nextX < 0)
			{
				minX = min(minX, nextX);
			}
			else if (nextX >= (int32)kBoardSize.x)
			{
				maxX = max(maxX, nextX);
			}
		}
	}

	if (minX != 0)
	{
		//minX가 음수이고, outPosition이 음수인 상황이므로 -를 해줘야 함.
		//outPosition.x -= minX;
		outPosition.x = 0;
		shouldPush = true;
	}
	else if (maxX != 0)
	{
		outPosition.x -= (maxX - (int32)kBoardSize.x + 1);
		shouldPush = true;
	}
	return shouldPush;
}

const mnp::Position2& mnp::SimpleTetris::getCurrPosition() const
{
	return _currPosition;
}

void mnp::SimpleTetris::setCurrBlockType(EBlockType eBlockType)
{
	setBlockToBoard(_currBlockType, _currPosition, _currDirection, true);

	if (canDrawBlock(eBlockType, _currPosition, _currDirection) == true)
	{
		_currBlockType = eBlockType;
	}
	else
	{
		setBlockToBoard(_currBlockType, _currPosition, _currDirection);
	}
}

mnp::EBlockType mnp::SimpleTetris::getCurrBlockType() const
{
	return _currBlockType;
}

mnp::EBlockType mnp::SimpleTetris::getRandomBlockType() const
{
	int32 iBlockType{ getRandomBlockTypeInternal() };
	
	while (_prevRandomBlockType == iBlockType)
	{
		iBlockType = getRandomBlockTypeInternal();
	}

	_prevRandomBlockType = iBlockType;

	return (EBlockType)iBlockType;
}

mnp::int32 mnp::SimpleTetris::getRandomBlockTypeInternal() const
{
	return (int32)(((double)rand() / (double)(RAND_MAX + 1)) * 7.0) + 2;
}

void mnp::SimpleTetris::updateNextblockQueue()
{
	while (_nextBlockQueue.size() < kNextBlockQueueMinSize)
	{
		_nextBlockQueue.push_back(getRandomBlockType());

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	if (_nextBlockQueue.size() < kNextBlockQueueMaxSize && tickSecond() == true)
	{
		_nextBlockQueue.push_back(getRandomBlockType());
	}
}

void mnp::SimpleTetris::setTimerInterval(int32 interval)
{
	if (interval <= kTimerIntervalMin)
	{
		interval = kTimerIntervalMin;
	}

	_gameSpeed = interval;
}

mnp::int32 mnp::SimpleTetris::getTimerInterval() const
{
	return _gameSpeed;
}

bool mnp::SimpleTetris::tickGameSpeedTimer() const
{
	using namespace std::chrono;

	auto elapsed{ duration_cast<milliseconds>(steady_clock::now() - _prevTime) };

	if (elapsed.count() >= _gameSpeed)
	{
		_prevTime = steady_clock::now();

		return true;
	}

	return false;
}

void mnp::SimpleTetris::updateGameLevel()
{
	if (_currLevelScore >= _scoreForNextLevel && _currLevel < 100)
	{
		++_currLevel;
		_currLevelScore = 0;
		_scoreForNextLevel += 250;

		_isLevelUped = true;

		if (_gameSpeed > 50)
		{
			_gameSpeed -= 40;
		}
	}

	if (_currLevel >= 30)
	{
		if (_isBgmChange == true)
		{
			_tetrisSound.soundStop("배경");

			_tetrisSound.soundPlay("배경2");

			_isBgmChange = false;
		}
	}
}

void mnp::SimpleTetris::increasingGameLevel()
{
	if (_currLevel < 100)
	{
		_currLevel += 10;

		if (_currLevel >= 100)
		{
			_currLevel = 100;
		}
	}
}

mnp::uint32 mnp::SimpleTetris::getCurrScore() const
{
	return _currScore;
}

void mnp::SimpleTetris::addCurrScore()
{
	_currScore += 100;
}

void mnp::SimpleTetris::addCurrLevelScore()
{
	_currLevelScore += 100;
}

void mnp::SimpleTetris::togglePause() const
{
	//bool 변수를 이용한 토글 기능. ***
	_isPaused = !_isPaused;
}

void mnp::SimpleTetris::endLevelUped() const
{
	_isLevelUped = false;
}

mnp::uint32 mnp::SimpleTetris::getCurrLevel() const
{
	return _currLevel;
}

mnp::uint32 mnp::SimpleTetris::getCurrLevelScore() const
{
	return _currLevelScore;
}

mnp::uint32 mnp::SimpleTetris::getScoreForNextLevel() const
{
	return _scoreForNextLevel;
}

bool mnp::SimpleTetris::getIsPaused() const
{
	return _isPaused;
}

bool mnp::SimpleTetris::getIsLeveluped() const
{
	return _isLevelUped;
}

mnp::uint32 mnp::SimpleTetris::getComboCount() const
{
	return _comboCount;
}

bool mnp::SimpleTetris::isGameOver() const
{
	return _isGameOver;
}

bool mnp::SimpleTetris::isGameStart() const
{
	return _isGameStart;
}

void mnp::SimpleTetris::gameStart() 
{
	_isGameStart = true;
}

void mnp::SimpleTetris::restartGame()
{
	_isGameOver = false;

	spawnNewBlock();
	_currLevel = 1;
	_currScore = 0;
	_currLevelScore = 0;
	_scoreForNextLevel = 500;
	_gameSpeed = 1010;
	_comboCount = 0;

	//memset으로 하면 됨.
	/*for (int32 y = 0; y < (int32)kBoardSize.y; ++y)
	{
		for (int32 x = 0; x < (int32)kBoardSize.x; ++x)
		{
			_board[y][x] = 0;
		}
	}*/

	//float * float은 연산 오버플로우가 발생하므로 앞의 하나를 double로 캐스팅 한다.
	memset(_board, 0, (size_t)((double)kBoardSize.x * kBoardSize.y));

#if defined DEBUG || _DEBUG
	for (int y = 0; y < 2; ++y)
	{
		for (int x = 0; x < kBoardSize.x; ++x)
		{
			if (x == 3) continue;
			_board[(int32)kBoardSize.y - 1 - y][x] = (uint32)EBlockType::Used;
		}
	}
#endif 

	_nextBlockQueue.pop_front();
	_nextBlockQueue.pop_front();
	_nextBlockQueue.pop_front();

	_currScore = 0;
}

void mnp::SimpleTetris::spawnNewBlock()
{
	_currBlockType = _nextBlockQueue.front();
	switch (_currBlockType)
	{
	case mnp::EBlockType::I:
		_currDirection = EDirection::E;
		break;
	case mnp::EBlockType::T:
		_currDirection = EDirection::N;
		break;
	case mnp::EBlockType::O:
		_currDirection = EDirection::N;
		break;
	case mnp::EBlockType::L:
		_currDirection = EDirection::E;
		break;
	case mnp::EBlockType::InvL:
		_currDirection = EDirection::W;
		break;
	case mnp::EBlockType::Z:
		_currDirection = EDirection::E;
		break;
	case mnp::EBlockType::S:
		_currDirection = EDirection::W;
		break;
	case mnp::EBlockType::None:
	case mnp::EBlockType::Used:
	case mnp::EBlockType::Bingo:
	case mnp::EBlockType::MAX:
	default:
		break;
	}
	_currPosition = getInitialBlockPosition();
}

mnp::Position2 mnp::SimpleTetris::getInitialBlockPosition() const
{
	Position2 result{ (kBoardSize.x * 0.5) - (kBlockContainerSize * 0.5), 0};

	return result;
}

void mnp::SimpleTetris::checkBingo()
{
	int32 currY{ (int32)kBoardSize.y - 1 };
	uint32 bingoCount{};

	while (currY >= 0)
	{
		bool isBingo{ true };

		for (uint32 x = 0; x < (uint32)kBoardSize.x; ++x)
		{
			if (_board[currY][x] == 0)
			{
				isBingo = false;

				break;
			}
		}

		if (isBingo == true)
		{
			//memset(_board[currY], 0, (size_t)kBoardSize.x);
			changeBingoLineColor(currY);

			_bingoTimer.start();

			_bingoLineIndices.emplace_back(currY);

			_tetrisSound.soundPlay("빙고");
		}
		
		--currY;
	}

	uint32 deltaScore{ ((uint32)_bingoLineIndices.size() * (uint32)_bingoLineIndices.size()) * 100 };

	_currScore += deltaScore;
	_currLevelScore += deltaScore;
}

void mnp::SimpleTetris::changeBingoLineColor(int32 currY)
{
	for (int32 x = 0; x < kBoardSize.x; ++x)
	{
		_board[currY][x] = (uint32)EBlockType::Bingo;
	}
}

void mnp::SimpleTetris::clearBingoLine(int32 bingoY)
{
	for (int32 y = bingoY - 1; y > -3; --y)
	{
		memcpy(_board[y + 1], _board[y], (size_t)kBoardSize.x);
	}
}

void mnp::SimpleTetris::createBlock(EBlockType eBlockType, const Color& color, uint8 alpha)
{
	// === 버전 1
	_iiBlocks[(uint32)eBlockType] = createBlankImage(kBlockSize);
	drawBlockUnitToImage(eBlockType, Position2(0, 0), color, alpha);
	// ===
}

void mnp::SimpleTetris::createBlockFromImage(EBlockType eBlockType, const std::wstring &filename)
{
	// === 버전 2
	_iiBlocks[(uint32)eBlockType] = createImageFromFile(filename);

	// ===
}

void mnp::SimpleTetris::createBackgroundFromImage(EBackground eBackground, const std::wstring& filename)
{
	_iiBackground[(uint32)eBackground] = createImageFromFile(filename);
}

void mnp::SimpleTetris::createSound()
{
	//정적할당.
	// 얼마만큼의 메모리 공간을 사용할지 미리 알고있을 경우 처음부터 어느정도의 크기의 메모리공간을 사용할지 정해줄 수 있음.
	// 이 경우 어느 정도의 크기의 메모리 공간을 사용할지에 대한 정보는 exe 파일에 저장되고 실제 메모리 할당은 runtime에 함.

	//동적할당 = new.
	// 연산자 new는 내부적으로 사용할 '메모리 공간'의 '주소'를 리턴하는 함수를 호출한다.
	// 따라서 동적할당을 하기 위해 필요한 자료형은 포인터여야 한다.
	// 예) int* a = new int;

	_tetrisSound.soundCreate("Asset/Sound/pegasus.mp3", FMOD_LOOP_NORMAL, "배경");
	_tetrisSound.soundCreate("Asset/Sound/great.mp3", FMOD_LOOP_NORMAL, "배경2");
	_tetrisSound.soundCreate("Asset/Sound/bingo.mp3", FMOD_DEFAULT | FMOD_NONBLOCKING, "빙고");
	_tetrisSound.soundCreate("Asset/Sound/pyong.mp3", FMOD_DEFAULT | FMOD_NONBLOCKING, "뿅");
	_tetrisSound.soundCreate("Asset/Sound/whip.mp3", FMOD_DEFAULT | FMOD_NONBLOCKING, "휩");

	_tetrisSound.soundPlay("배경");
}

void mnp::SimpleTetris::playFastDownSound()
{
	_tetrisSound.soundPlay("휩");
}

//블록 하나를 이미지에 그리는 함수.
void mnp::SimpleTetris::drawBlockUnitToImage(EBlockType eBlockType, const Position2& position, const Color& color, uint8 alpha)
{
	const auto imageIndex{ _iiBlocks[(uint32)eBlockType] };

	// 중앙
	drawRectangleToImage(imageIndex, position, kBlockSize, color, alpha);

	//border: 테두리

	// 어두운 줄 (세로)
	drawRectangleToImage(imageIndex, position + Position2(kBlockSize.x, 0) - Position2(kBlockBorder, 0),
		Size2(kBlockBorder, kBlockSize.y), Color::sub(color, Color(60, 60, 60)), alpha);

	// 어두운 줄 (가로)
	drawRectangleToImage(imageIndex, position + Position2(0, kBlockSize.y) - Position2(0, kBlockBorder),
		Size2(kBlockSize.x, kBlockBorder), Color::sub(color, Color(60, 60, 60)), alpha);

	// 밝은 줄 (세로)
	drawRectangleToImage(imageIndex, position,
		Size2(kBlockSize.x - 1, kBlockBorder), Color::add(color, Color(60, 60, 60)), alpha);

	// 밝은 줄 (가로)
	drawRectangleToImage(imageIndex, position,
		Size2(kBlockBorder, kBlockSize.y - 1), Color::add(color, Color(60, 60, 60)), alpha);
}

void mnp::SimpleTetris::setBlockToBoard(EBlockType eBlockType, const Position2& position, EDirection eDirection, bool bErase)
{
	const int32 x{ int32(position.x) };
	const int32 y{ int32(position.y) };
	const uint8 blockType{ (bErase == true) ? uint8(0) : uint8(eBlockType) };
	const auto& block = _blocks[(uint32)eBlockType][(uint32)eDirection];
	for (int32 y_ = 0; y_ < 4; ++y_)
	{
		for (int32 x_ = 0; x_ < 4; ++x_)
		{
			const int32 finalX{ x + x_ };
			const int32 finalY{ y + y_ };
			const uint8 blockValue{ block.data[y_][x_] };
			if (blockValue == 0) continue;
			if (finalY < -kBlockContainerSize || finalY >= (int32)kBoardSize.y) continue;
			if (finalX < 0 || finalX >= (int32)kBoardSize.x) continue;
			_board[finalY][finalX] = blockType;
		}
	}
}

void mnp::SimpleTetris::drawBlockToScreen(EBlockType eBlockType, const Position2& position, EDirection eDirection)
{
	int32 blockType{ (int32)eBlockType };
	const auto& block{ _blocks[blockType][(uint32)eDirection] };

	for (float y = 0; y < kBlockContainerSize; y += 1)
	{
		for (float x = 0; x < kBlockContainerSize; x += 1)
		{
			if (block.data[uint32(y)][uint32(x)] != 0)
			{
				drawImageToScreen(_iiBlocks[blockType], position + kBlockSize * Position2(x, y));
			}
		}
	}
}

bool mnp::SimpleTetris::canDrawBlock(EBlockType eBlockType, const Position2& position, EDirection eDirection) const
{
	const int32 x{ int32(position.x) };
	const int32 y{ int32(position.y) };
	const auto& block = _blocks[(uint32)eBlockType][(uint32)eDirection];
	for (int32 y_ = 0; y_ < 4; ++y_)
	{
		for (int32 x_ = 0; x_ < 4; ++x_)
		{
			const int32 finalX{ x + x_ };
			const int32 finalY{ y + y_ };
			const uint8 blockValue{ block.data[y_][x_] };
			if (blockValue == 0) continue;
			if (finalX < 0 || finalX >= (int32)kBoardSize.x)
			{
				return false;
			}
			if (finalY < 0) continue;
			if (finalY >= (int32)kBoardSize.y)
			{
				return false;
			}

			if (_board[finalY][finalX] != 0)
			{
				return false;
			}
		}
	}
	return true;
}
