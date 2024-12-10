#pragma once

// 1メートル基準

static constexpr float mm  = 0.001f;
static constexpr float cm  = 0.01f;
static constexpr float dm  = 0.1f;
static constexpr float m   = 1.0f;
static constexpr float dam = 10.0f;
static constexpr float hm  = 100.0f;
static constexpr float km  = 1000.0f;

inline float operator""_mm(long double i) {
	return static_cast<float>(i) * mm;
}

inline float operator""_cm(long double i) {
	return static_cast<float>(i) * cm;
}
inline float operator""_dm(long double i) {
	return static_cast<float>(i) * dm;
}

inline float operator""_m(long double i) {
	return static_cast<float>(i) * m;
}

inline float operator""_dam(long double i) {
	return static_cast<float>(i) * dam;
}
inline float operator""_hm(long double i) {
	return static_cast<float>(i) * hm;
}
inline float operator""_km(long double i) {
	return static_cast<float>(i) * km;
}

