#pragma once
#include <FMOD/fmod.hpp>
#include <vector>
#include <unordered_map>
#include <string>

class Audio
{
	

public:
	Audio();
	~Audio();

private:
	void create();
	void release();

public:
	void soundCreate(const char* soundfilename, FMOD_MODE mode, const std::string& soundkey);
	void soundPlay(const std::string& soundkey);
	void soundStop(const std::string& soundkey);

private:
	FMOD::System*				_FmodSystem{};

private:
	std::vector<FMOD::Sound*>    _vSounds{};
	std::vector<FMOD::Channel*>  _vChannels{};

private:
	std::unordered_map<std::string, size_t> _soundMap{};
};