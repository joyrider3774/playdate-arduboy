#pragma once

constexpr const char * FlashString(const char * string) {

	return reinterpret_cast<const char *>(string);

}

template<typename T> T absT(const T & v) {

	return (v < 0) ? -v : v;

}
