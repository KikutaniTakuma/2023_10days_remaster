/// ================================
/// ==  AudioManagerクラスの宣言  ==
/// ================================


#pragma once
#include <memory>
#include <unordered_map>
#include "Audio/Audio.h"
#include "Utils/SafePtr.h"
#include "Engine/EngineUtils/LambPtr/LambPtr.h"

/// <summary>
/// Audioを管理
/// </summary>
class AudioManager {
	friend Audio;

private:
	AudioManager();
	AudioManager(const AudioManager&) = delete;
	AudioManager(AudioManager&&) noexcept = delete;
	AudioManager& operator=(const AudioManager&) = delete;
	AudioManager& operator=(AudioManager&&) noexcept = delete;
public:
	~AudioManager();

public:
	static void Inititalize();
	static void Finalize();
	static inline AudioManager* const GetInstance() {
		return instance_.get();
	}

private:
	static Lamb::SafePtr<AudioManager> instance_;

public:
	void Load(const std::string& fileName);
	Audio* const Get(const std::string& fileName);

	void Unload(const std::string& fileName);
	void Unload(Audio* audio);

	IXAudio2MasteringVoice* GetMasterVoice() {
		return masterVoice_.get();
	}

private:
	Lamb::LambPtr<IXAudio2> xAudio2_;
	Lamb::SafePtr<IXAudio2MasteringVoice> masterVoice_;

	std::unordered_map<std::string, std::unique_ptr<Audio>> audios_;
};