#pragma once
#include <unordered_map>
#include <variant>
#include <string>
#include <cassert>
#include "Math/Vector2.h"
#include "Math/Vector3.h"

#include "VariantItem.h"
#include "Utils/Math/ValueRange.h"

class GlobalVariables {
public:
	using Item = std::variant<int32_t, float, Vector2, Vector3, std::string>;
	using Group = std::unordered_map<std::string, Item>;

public:
	GlobalVariables() = default;
	GlobalVariables(const std::string &fileNmae);
	GlobalVariables(const GlobalVariables &) = delete;
	GlobalVariables(GlobalVariables &&) noexcept = delete;
	~GlobalVariables() = default;

	GlobalVariables &operator=(const GlobalVariables &) = delete;
	GlobalVariables &operator=(GlobalVariables &&) noexcept = delete;

public:
	void CreateGroup(const std::string &groupName);

	const Group &GetGroup(const std::string &groupName) const;

	virtual void Update();

	void SaveFile(const std::string &groupName);
	virtual void LoadFile();
	virtual void LoadFile(const std::string &groupName);

	template<typename T>
	void AddItem(const std::string &groupName, const std::string &key, const T &value);
	template<typename T>
	void SetValue(const std::string &groupName, const std::string &key, const T &value);

	template<typename T>
	void AddItem(const std::string &groupName, const VariantItem<T> &item);
	template<typename T>
	void SetValue(const std::string &groupName, const VariantItem<T> &item);

#pragma region ValueRange用の読み書き

	template<typename T>
	void AddItem(const std::string &groupName, const VariantItem<ValueRange<T>> &item);
	template<typename T>
	void SetValue(const std::string &groupName, const VariantItem<ValueRange<T>> &item);

#pragma endregion

	const Item &Get(const std::string &groupName, const std::string &key) const;

	template<typename T>
	T Get(const std::string &groupName, const std::string &key) const;

	int32_t GetIntValue(const std::string &groupName, const std::string &key) const;
	float GetFloatValue(const std::string &groupName, const std::string &key)const;
	Vector3 GetVector3Value(const std::string &groupName, const std::string &key) const;
	std::string GetStringValue(const std::string &groupName, const std::string &key) const;

protected:
	std::unordered_map<std::string, Group> datas;

	bool isCreate;
	std::string groupNameAdd;

	std::string dataFileName;
};

template <typename T>
inline T GlobalVariables::Get(const std::string &groupName, const std::string &key) const {

	// アイテムの検索
	const Item &item = Get(groupName, key);
	// 型が正しいか
	if (std::holds_alternative<T>(item)) {
		return std::get<T>(item);
	}
	assert(!"正しくない型を取得しようとしました。");
	return T{};
}


template<typename T>
void GlobalVariables::AddItem(
	const std::string &groupName, const std::string &key, const T &value) {
	auto &gruop = datas[groupName];
	auto itItem = gruop.find(key); // キーがあるか
	if (itItem == gruop.end()) {
		SetValue(groupName, key, value);
		return;
	}
	return;
}

template<typename T>
void GlobalVariables::SetValue(
	const std::string &groupName, const std::string &key, const T &value) {
	Group &group = datas[groupName];
	group[key] = value;
}

template<typename T>
inline void GlobalVariables::AddItem(const std::string &groupName, const VariantItem<T> &item) {
	AddItem(groupName, item.GetKey(), item.GetItem());
}

template<typename T>
inline void GlobalVariables::SetValue(const std::string &groupName, const VariantItem<T> &item) {
	SetValue(groupName, item.GetKey(), item.GetItem());
}

template<typename T>
inline void GlobalVariables::AddItem(const std::string &groupName, const VariantItem<ValueRange<T>> &item) {
	AddItem(groupName, item.GetKey() + "_min", item.GetItem().min_);
	AddItem(groupName, item.GetKey() + "_max", item.GetItem().max_);
}

template<typename T>
inline void GlobalVariables::SetValue(const std::string &groupName, const VariantItem<ValueRange<T>> &item) {
	SetValue(groupName, item.GetKey() + "_min", item.GetItem().min_);
	SetValue(groupName, item.GetKey() + "_max", item.GetItem().max_);
}


template<typename T>
void operator>> (const GlobalVariables::Group &group, VariantItem<T> &value) {

	// キーがあるか
	const auto &itItem = group.find(value.GetKey());
	if (itItem == group.end()) { return; }

	// アイテムの参照
	const auto &item = itItem->second;

	// 型が正しいか
	if (std::holds_alternative<T>(item)) {
		value = std::get<T>(item);
		return;
	}
	assert(0 && "間違った型を取得しようとしました。");
	return;
}


template<typename T>
inline void operator>> (const GlobalVariables::Group &group, VariantItem<ValueRange<T>> &value) {

	const auto &itMin = group.find(value.GetKey() + "_min");
	if (itMin != group.end()) {
		const auto &minValue = itMin->second;
		// 型が正しいか
		if (std::holds_alternative<T>(minValue)) {
			value->min_ = std::get<T>(minValue);
		}
	}

	const auto &itMax = group.find(value.GetKey() + "_max");
	if (itMax != group.end()) {
		const auto &maxValue = itMax->second;
		// 型が正しいか
		if (std::holds_alternative<T>(maxValue)) {
			value->max_ = std::get<T>(maxValue);
		}
	}
}

