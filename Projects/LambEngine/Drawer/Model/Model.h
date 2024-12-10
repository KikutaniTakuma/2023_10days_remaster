/// =========================
/// ==  Modelクラスの宣言  ==
/// =========================

#pragma once
#include "../BaseDrawer.h"

/// <summary>
/// 3Dモデルの描画
/// </summary>
class Model : public BaseDrawer {
public:
	static constexpr uint32_t kMaxDrawCount = 512;

public:
	struct Data {
		Mat4x4 worldMatrix = Mat4x4::kIdentity;
		Mat4x4 camera = Mat4x4::kIdentity;
		uint32_t color = 0xffffffff;
		BlendType blend = BlendType::kNone;
		bool isLighting = true;
	};
	struct Instance {
		QuaternionTransform transform;
		uint32_t color = 0xffffffff;
		bool isLighting = true;
	};

public:
	Model() = default;
	Model(const std::string& fileName);
	Model(const Model&) = default;
	Model(Model&& right) noexcept = default;
	virtual ~Model() = default;

	Model& operator=(const Model& right) = default;
	Model& operator=(Model&& right) noexcept = default;

public:
	virtual void Load(const std::string& fileName);

	virtual void Draw(
		const Mat4x4& worldMatrix,
		const Mat4x4& camera,
		uint32_t color,
		BlendType blend,
		bool isLighting = true
	);

	void Draw(const Data& data);

public:
	const Node& GetNode() const;
	const ModelData& GetModelData() const;
};

class ModelInstance {
public:
	ModelInstance() = default;
	~ModelInstance() = default;

public:
	void Load(const std::string& fileName);
	
	void Update();

	void Draw(const Mat4x4& cameraMat);

	void SetParent(ModelInstance* parent);

	const Mat4x4& GetWorldMatrix() const;

public:
	Vector3 scale = Vector3::kIdentity;
	Vector3 rotate;
	Vector3 pos;

	uint32_t color = 0xffffffff;
	BlendType blend = BlendType::kNone;
	bool isLighting = true;

private:
	Lamb::SafePtr<Model> model_;
	Lamb::SafePtr<ModelInstance> parent_;
	Mat4x4 worldMat_ = Mat4x4::kIdentity;
};