#pragma once
#include <string>
#include <vector>

#include "Libs/rapidjson/document.h"     
#include "Libs/rapidjson/prettywriter.h"
#include "Libs/rapidjson/istreamwrapper.h"
#include "Libs/rapidjson/ostreamwrapper.h"
#include "Libs/rapidjson/encodedstream.h"

namespace rj = rapidjson;
using std::string, std::vector;

namespace rapidjson::utils
{
	// Обернуть стандартную строку в обертку для rapidjson
	inline rj::GenericStringRef<char> WrapStr(const string& iStr)
	{
		return rj::GenericStringRef<char>(iStr.c_str());
	};

	// Добавить строковое значение в документ
	void AddString(rj::Document& iDoc, rj::MemoryPoolAllocator<>& allocator, const string& iName, const string& iValue);
	// Добавить булево значение в документ
	void AddBool(rj::Document& iDoc, rj::MemoryPoolAllocator<>& allocator, const string& iName, const bool& iValue);
	// Добавить число типа float в документ
	void AddFloat(rj::Document& iDoc, rj::MemoryPoolAllocator<>& allocator, const string& iName, const float& iValue);
	// Добавить число типа double в документ
	void AddDouble(rj::Document& iDoc, rj::MemoryPoolAllocator<>& allocator, const string& iName, const double& iValue);
	// Добавить число типа int в документ
	void AddInt(rj::Document& iDoc, rj::MemoryPoolAllocator<>& allocator, const string& iName, const int& iValue);

	bool ReadString(const rj::Value& doc, const string tag, string& dest);
	bool ReadBool(const rj::Value& doc, const string tag, bool& dest);
	bool ReadFloat(const rj::Value& doc, const string tag, float& dest);
	bool ReadDouble(const rj::Value& doc, const string tag, double& dest);
	bool ReadInt(const rj::Value& doc, const string tag, int& dest);
	bool ReadInt(const rj::Value& doc, const string tag, size_t& dest);

}