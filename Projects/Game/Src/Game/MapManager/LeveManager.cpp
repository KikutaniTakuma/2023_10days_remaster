#include "LevelManager.h"

#include <fstream>
#include <list>

LevelInfo::LevelInfo(const std::string &fileName) {
	fileName_ = fileName;
}

void LevelManager::AddLevel(const std::string &fileName) {
	levelList_.push_back(std::make_unique<LevelInfo>(fileName));
}

void LevelManager::LoadAllLevel(const std::string &fileName) {
	std::ifstream file(kLevelDirectory_ + fileName);	//	読み込む管理ファイル
	std::list<std::string> lines;			// 格納するコンテナ
	std::string lineBuff;						// 一時保存
	while (std::getline(file, lineBuff)) {
		lines.push_back(lineBuff);
	}
	file.close();

	// 読み込んだ文字列を適用する
	for (const auto &line : lines) {
		AddLevel(line);
	}

	// 読み込みが完了したことを通知
	isLoadComplete_ = true;
}

std::string LevelManager::GetLevelName(uint32_t index) {
	return kLevelDirectory_ + levelList_[index]->GetFileName();
}
