#include "Utils/ExecutionLog.h"
#include "Engine//EngineUtils/ErrorCheck/ErrorCheck.h"
#include <fstream>
#include <filesystem>
#include "Error/Error.h"

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"

namespace Lamb {
	std::string TimeToString(std::chrono::milliseconds ms) {
		// 1秒、1分、1時間のミリ秒
		auto h = std::chrono::duration_cast<std::chrono::hours>(ms);
		ms -= h;
		auto m = std::chrono::duration_cast<std::chrono::minutes>(ms);
		ms -= m;
		auto s = std::chrono::duration_cast<std::chrono::seconds>(ms);
		ms -= s;

		// 文字列フォーマット
		return std::to_string(h.count()) + "h " +
			std::to_string(m.count()) + "m " +
			std::to_string(s.count()) + "s " +
			std::to_string(ms.count()) + "ms";
	}

	bool AddLog(const std::string& text) {
		const std::filesystem::path fileName = "./Log/Execution.log";
		static bool isOpned = false;

#ifdef USE_DEBUG_CODE
		DebugLog(text);
#endif // USE_DEBUG_CODE


		if (!std::filesystem::exists(fileName.parent_path())) {
			try {
				std::filesystem::create_directories(fileName.parent_path());
			}
			catch (const std::exception& err) {
				MessageBoxA(
					NULL,
					err.what(), ("Error : " + std::string{ typeid(ErrorCheck).name() }).c_str(),
					MB_OK | MB_SYSTEMMODAL | MB_ICONERROR
				);

				return false;
			}
		}

		std::ofstream file;
		if (isOpned) {
			file.open(fileName, std::ios::app);
		}
		else {
			file.open(fileName);
		}
		if (file.fail()) {
			return false;
		}

		file << NowTime() << " : " << text << std::endl;

		isOpned = true;

		return true;
	}

	void DebugLog(const std::string& text) {
		OutputDebugStringA((text + "\n").c_str());
	}

	void DebugLog(const std::string& text, const Vector2& vec) {
		OutputDebugStringA((text + std::string{ " : " } + std::format("{}, {}", vec.x, vec.y) + "\n").c_str());
	}

	void DebugLog(const std::string& text, const Vector3& vec) {
		OutputDebugStringA((text + std::string{ " : " } + std::format("{}, {}, {}", vec.x, vec.y, vec.z) + "\n").c_str());
	}

	void DebugLog(const std::string& text, const Vector4& vec) {
		OutputDebugStringA((text + std::string{ " : " } + std::format("{}, {}, {}, {}", vec.vec.x, vec.vec.y, vec.vec.z, vec.vec.w) + "\n").c_str());
	}

	void DebugLog(const std::string& text, const Quaternion& quaternion) {
		DebugLog(text, quaternion.vector4);
	}

	void ErrorLog(
		const std::string& erroerMassage,
		const std::string& functionName,
		const std::string& className
	) {
		static ErrorCheck* const errorCheck = ErrorCheck::GetInstance();
		if (functionName.empty()) {
			errorCheck->ErrorTextBox(
				"Failed : " + erroerMassage,
				className
			);
		}
		else {
			errorCheck->ErrorTextBox(
				functionName + " failed : " + erroerMassage,
				className
			);
		}
	}

	void ErrorLog(const Error& err) {
		static ErrorCheck* const errorCheck = ErrorCheck::GetInstance();
		errorCheck->ErrorTextBox(
			err.SourceFileName() + " " + err.CodeLineNumber() + "\n" + err.FunctionName() + " failed : " + err.What() + "\n",
			err.ClassName()
		);
	}

	std::string NowTime() {
		auto now = std::chrono::system_clock::now();
		auto nowSec = std::chrono::floor<std::chrono::seconds>(now);
		std::chrono::zoned_time zt{ "Asia/Tokyo", nowSec };

		return std::format("{:%Y/%m/%d %H:%M:%S}", zt);
	}
}