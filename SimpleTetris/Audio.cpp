#include "Audio.h"
#include <Windows.h>

Audio::Audio()
{
	create();
}

Audio::~Audio()
{
	release();
}

void Audio::create()
{
	if (FMOD::System_Create(&_FmodSystem) != FMOD_OK)
	{
		MessageBox(nullptr, L"ERROR: System Create에 실패했습니다.", L"ERROR MESSAGE", MB_ICONERROR);
	}

	if (_FmodSystem->init(32, FMOD_INIT_NORMAL, nullptr) != FMOD_OK)
	{
		MessageBox(nullptr, L"ERROR: System Initialize에 실패했습니다.", L"ERROR MESSAGE", MB_ICONERROR);
	}
}

void Audio::release()
{
	for (auto& sound : _vSounds)
	{
		sound->release();
		sound = nullptr;
	}

	_FmodSystem->release();
	_FmodSystem = nullptr;
}

void Audio::soundCreate(const char* soundfilename, FMOD_MODE mode, const std::string& soundkey)
{
	_vSounds.emplace_back();

	_vChannels.emplace_back();


	if (_FmodSystem->createSound(soundfilename, mode, nullptr, &_vSounds.back()) != FMOD_OK)
	{
		MessageBox(nullptr, L"ERROR: Sound Create에 실패했습니다.", L"ERROR MESSAGE", MB_ICONERROR);

		_vSounds.pop_back();

		_vChannels.pop_back();

		return;
	}
	
	//key와 값을 1:1로 연결해줌.
	_soundMap.insert(std::make_pair(soundkey, _vSounds.size() - 1));
}

void Audio::soundPlay(const std::string& soundkey)
{
	//iterator를 리턴함.
	auto findkey{ _soundMap.find(soundkey) };

	//begin은 첫 항목을 가리키는 iterator지만 end는 마지막 값이 아닌 마지막 다음 없는 값을 가리킴.
	if (findkey != _soundMap.end())
	{
		if (_FmodSystem->playSound(_vSounds[_soundMap.at(soundkey)], nullptr
			, false, &_vChannels[_soundMap.at(soundkey)]) != FMOD_OK)
		{
			MessageBox(nullptr, L"ERROR: Sound Play에 실패했습니다.", L"ERROR MESSAGE", MB_ICONERROR);
		}
	}
}

void Audio::soundStop(const std::string& soundkey)
{
	auto findkey{ _soundMap.find(soundkey) };

	if (findkey != _soundMap.end())
	{
		if (_vChannels[_soundMap.at(soundkey)]->stop() != FMOD_OK)
		{
			MessageBox(nullptr, L"ERROR: Sound Stop에 실패했습니다.", L"ERROR MESSAGE", MB_ICONERROR);
		}
	}
}

