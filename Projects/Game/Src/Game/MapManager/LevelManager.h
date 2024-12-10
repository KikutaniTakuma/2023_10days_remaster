#pragma once
#include <string>
#include <array>
#include <stdint.h>
#include <vector>
#include <memory>

class LevelInfo final {
public:
	LevelInfo(const std::string& fileName); // 本来はここでCSVから読みこんで処理を行う。
	~LevelInfo() = default;

	const std::string& GetFileName()const { return fileName_; }

	bool isClear_ = false;

private:
	static constexpr uint32_t kLevelHight_ = 10u;	// ステージの縦方向のタイル数
	static constexpr uint32_t kLevelWidth_ = 10u;	// ステージの横方向のタイル数

	// ファイル名
	std::string fileName_;

	// マップチップの保存先。現状未使用。
	std::array<std::array<uint32_t, kLevelWidth_>, kLevelHight_> mapData_;

};

class LevelManager final {

	LevelManager() = default;
	LevelManager(const LevelManager&) = delete;
	LevelManager& operator=(const LevelManager&) = delete;
	~LevelManager() = default;

public:

	static LevelManager* const GetInstance() {
		static LevelManager instance{};
		return &instance;
	}
	/// @brief ステージを1ステージごとに読み込む
	/// @param fileName Directory下のファイル名を入れる。
	void AddLevel(const std::string& fileName);

	/// @brief 特定のファイルを読み込んで、全レベルを保存する
	void LoadAllLevel(const std::string& fileName = "LevelList.txt");

	const char* const kLevelDirectory_ = "./Resources/StageData/";

	/// @brief 添え字からフルパスを取得する
	/// @param index ステージの添え字
	/// @return 各Levelのファイル名
	std::string GetLevelName(uint32_t index);

	/// @brief レベルのリストの取得
	/// @return const参照のLevelInfo配列
	const auto& GetLevelList() const { return levelList_; }
	auto& GetLevelList() { return levelList_; }

	/// @brief ロード完了確認
	/// @return ロード完了なら true
	bool IsLoadComplete()const { return isLoadComplete_; }

private:

	bool isLoadComplete_ = false;

	// 選択中のレベル
	uint32_t selectLevel_ = 0u;

	// ステージのリスト
	std::vector<std::unique_ptr<LevelInfo>> levelList_;
};