module;
#include <windows.h>
export module Util;
import std;
using namespace std;

export
namespace Util {




	std::string UnicodeToUTF8(unsigned int codepoint)
	{
		std::string out;

		if (codepoint <= 0x7f)
			out.append(1, static_cast<char>(codepoint));
		else if (codepoint <= 0x7ff)
		{
			out.append(1, static_cast<char>(0xc0 | ((codepoint >> 6) & 0x1f)));
			out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
		}
		else if (codepoint <= 0xffff)
		{
			out.append(1, static_cast<char>(0xe0 | ((codepoint >> 12) & 0x0f)));
			out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
			out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
		}
		else
		{
			out.append(1, static_cast<char>(0xf0 | ((codepoint >> 18) & 0x07)));
			out.append(1, static_cast<char>(0x80 | ((codepoint >> 12) & 0x3f)));
			out.append(1, static_cast<char>(0x80 | ((codepoint >> 6) & 0x3f)));
			out.append(1, static_cast<char>(0x80 | (codepoint & 0x3f)));
		}
		return out;
	}


	string get_filename_without_extension(const string& fname_with_extension)
	{
		auto dot_pos = fname_with_extension.find_last_of(".");
		return fname_with_extension.substr(0, dot_pos);
	}


	// Convert a UTF-16 (wchar_t) string to UTF-8
	std::string utf16ToUtf8(const std::wstring& utf16Str) {

		int sizeNeeded = WideCharToMultiByte(
			CP_UTF8,
			0,
			utf16Str.c_str(), 
			-1, 
			nullptr, 
			0, 
			nullptr,
			nullptr);

		std::string utf8Str(sizeNeeded, 0);

		WideCharToMultiByte(CP_UTF8, 
			0, 
			utf16Str.c_str(), 
			-1,
			&utf8Str[0], 
			sizeNeeded, 
			nullptr, 
			nullptr);

		return utf8Str;
	}

}
