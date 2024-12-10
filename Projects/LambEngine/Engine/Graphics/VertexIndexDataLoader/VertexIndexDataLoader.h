#pragma once
#include "../GraphicsStructs.h"
#include <chrono>
namespace Assimp {
	class Importer;
};

class VertexIndexDataLoader final {
public:
	struct IndexData {
		uint32_t vertNum;
		uint32_t uvNum;
		uint32_t normalNum;
		uint32_t textureHandle;

		inline bool operator==(const IndexData& right) const {
			return vertNum == right.vertNum
				&& uvNum == right.uvNum
				&& normalNum == right.normalNum
				&& textureHandle == right.textureHandle;
		}
		inline bool operator!=(const IndexData& right) const {
			return !(*this == right);
		}
	};

public:
	/// <summary>
	/// obj, gltfファイルのロード
	/// </summary>
	/// <param name="fileName">obj, gltfファイルパス</param>
	/// <returns>頂点とインデックス情報</returns>
	static [[nodiscard]] ModelData LoadModel(const std::string& fileName);

	/// <summary>
	/// gltfアニメーションロード
	/// </summary>
	/// <param name="fileName">gltfファイルのロード</param>
	/// <returns></returns>
	static [[nodiscard]] Animations* LoadAnimation(const std::string& fileName);

private:
	static const struct aiScene* ReadFile(Assimp::Importer& importer, const std::string& fileName);

	/// <summary>
	/// Node読み込み
	/// </summary>
	/// <param name="node"></param>
	/// <returns></returns>
	static Node ReadNode(struct aiNode* node);


	static void LoadMtl(const struct aiScene* scene, const std::string& directorypath, std::vector<uint32_t>& result);

	static void StartLoadTimeCount();

	static void EndLoadTimeCountAndAddLog(const std::string& fileName);

private:
	static std::chrono::steady_clock::time_point loadStartTime_;
};