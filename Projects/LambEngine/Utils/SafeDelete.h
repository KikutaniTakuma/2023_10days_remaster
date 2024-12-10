#pragma once
#include "Utils/Concepts.h"

namespace Lamb {
	template<IsPtr T>
	void SafeDelete(T& ptr) {
		delete ptr;
		ptr = nullptr;
	}
}