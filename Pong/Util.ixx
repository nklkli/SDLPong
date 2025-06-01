module;
#include <windows.h>
export module Util;
import std;
using namespace std;

export
namespace Util {

	string get_filename_without_extension(const string& fname_with_extension)
	{
		auto dot_pos = fname_with_extension.find_last_of(".");
		return fname_with_extension.substr(0, dot_pos);
	}


	// Convert a UTF-16 (wchar_t) string to UTF-8
	std::string utf16ToUtf8(const std::wstring& utf16Str) {
		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, utf16Str.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string utf8Str(sizeNeeded, 0);
		WideCharToMultiByte(CP_UTF8, 0, utf16Str.c_str(), -1, &utf8Str[0], sizeNeeded, nullptr, nullptr);
		return utf8Str;
	}

}
