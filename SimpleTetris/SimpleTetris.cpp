#include "SimpleTetris.h"


fs::SimpleTetris::SimpleTetris(int32 width, int32 height) : IGraphicalWindow(width, height)
{
	__noop;
}

fs::SimpleTetris::~SimpleTetris()
{
	__noop;
}

void fs::SimpleTetris::create(const std::wstring& title, HINSTANCE hInstance, WNDPROC windowProc)
{
	createInternal(title, hInstance, windowProc);

	createBlock(EBlockType::Used, Color(100, 100, 100));
	createBlock(EBlockType::I, Color(0, 0, 255));
	createBlock(EBlockType::T, Color(255, 0, 255));

	// I형 블록
	{
		_blocks[(uint32)EBlockType::I][(uint32)EDirection::N].set(
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0
		);

		_blocks[(uint32)EBlockType::I][(uint32)EDirection::W].set(
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0
		);

		_blocks[(uint32)EBlockType::I][(uint32)EDirection::S].set(
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0
		);

		_blocks[(uint32)EBlockType::I][(uint32)EDirection::E].set(
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0,
			0, 0, 0, 0
		);
	}
	
	// T형 블록
	{
		_blocks[(uint32)EBlockType::T][(uint32)EDirection::N].set(
			0, 1, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(uint32)EBlockType::T][(uint32)EDirection::W].set(
			0, 1, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(uint32)EBlockType::T][(uint32)EDirection::S].set(
			0, 0, 0, 0,
			1, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);

		_blocks[(uint32)EBlockType::T][(uint32)EDirection::E].set(
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 1, 0, 0,
			0, 0, 0, 0
		);
	}

	_board[10][4] = 1;
	_board[6][0] = 1;
}

void fs::SimpleTetris::drawBoard(const Position2& position, const Color& borderColor, const Color& boardColor)
{
	// 테두리
	drawRectangleToScreen(position - Position2(10, 10), kBoardSizePixel + Size2(20, 20), borderColor);

	// 판
	drawRectangleToScreen(position, kBoardSizePixel, boardColor);

	// 현재 블록
	drawBlockToBoard(_currBlockType, _currPosition, _currDirection);

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
}

void fs::SimpleTetris::move(EDirection eDirection)
{
	drawBlockToBoard(_currBlockType, _currPosition, _currDirection, true);

	switch (eDirection)
	{
	case fs::EDirection::N:
		if (canDrawBlock(_currBlockType, _currPosition - Position2(0, 1), _currDirection) == true)
		{
			_currPosition.y -= 1;
		}
		break;
	case fs::EDirection::W:
		if (canDrawBlock(_currBlockType, _currPosition + Position2(1, 0), _currDirection) == true)
		{
			_currPosition.x += 1;
		}
		break;
	case fs::EDirection::S:
		if (canDrawBlock(_currBlockType, _currPosition + Position2(0, 1), _currDirection) == true)
		{
			_currPosition.y += 1;
		}
		break;
	case fs::EDirection::E:
		if (canDrawBlock(_currBlockType, _currPosition - Position2(1, 0), _currDirection) == true)
		{
			_currPosition.x -= 1;
		}
		break;
	default:
		break;
	}
}

void fs::SimpleTetris::rotate()
{
	drawBlockToBoard(_currBlockType, _currPosition, _currDirection, true);

	int32 currDirection{ int32(_currDirection) };

	++currDirection;

	if (currDirection == 4)
	{
		currDirection = 0;
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

const bool fs::SimpleTetris::getRotatablePosition(EDirection eNextDirection, fs::Position2& outPosition) const
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

const fs::Position2& fs::SimpleTetris::getCurrPosition() const
{
	return _currPosition;
}

void fs::SimpleTetris::createBlock(EBlockType eBlockType, const Color& color, uint8 alpha)
{
	_iiBlocks[(uint32)eBlockType] = createBlankImage(kBlockSize);
	drawBlockUnitToImage(eBlockType, Position2(0, 0), color, alpha);
}

void fs::SimpleTetris::drawBlockUnitToImage(EBlockType eBlockType, const Position2& position, const Color& color, uint8 alpha)
{
	const auto imageIndex{ _iiBlocks[(uint32)eBlockType] };
	drawRectangleToImage(imageIndex, position, kBlockSize, color, alpha);

	//border: 테두리
	drawRectangleToImage(imageIndex, position + Position2(kBlockSize.x, 0) - Position2(kBlockBorder, 0),
		Size2(kBlockBorder, kBlockSize.y), Color::sub(color, Color(60, 60, 60)), alpha);
	drawRectangleToImage(imageIndex, position + Position2(0, kBlockSize.y) - Position2(0, kBlockBorder),
		Size2(kBlockSize.x, kBlockBorder), Color::sub(color, Color(60, 60, 60)), alpha);

	drawRectangleToImage(imageIndex, position,
		Size2(kBlockSize.x - 1, kBlockBorder), Color::add(color, Color(60, 60, 60)), alpha);
	drawRectangleToImage(imageIndex, position,
		Size2(kBlockBorder, kBlockSize.y - 1), Color::add(color, Color(60, 60, 60)), alpha);
}

void fs::SimpleTetris::drawBlockToBoard(EBlockType eBlockType, const Position2& position, EDirection eDirection, bool bErase)
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
			if (finalY < 0 || finalY >= (uint32)kBoardSize.y) continue;
			if (finalX < 0 || finalX >= (uint32)kBoardSize.x) continue;
			_board[finalY][finalX] = blockType;
		}
	}
}

bool fs::SimpleTetris::canDrawBlock(EBlockType eBlockType, const Position2& position, EDirection eDirection) const
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
			if (finalY >= (uint32)kBoardSize.y) return false;
			if (finalX < 0 || finalX >= (uint32)kBoardSize.x) return false;
			
			if (_board[finalY][finalX] != 0)
			{
				return false;
			}
		}
	}
	return true;
}
