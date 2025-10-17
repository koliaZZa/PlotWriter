#include "Include/Model/Common/Model.RJUtilities.h"

namespace rapidjson::utils
{
	void AddString(rj::Document& iDoc, rj::MemoryPoolAllocator<>& allocator, const string& iName, const string& iValue)
	{
		rj::Value json_name, json_val;
		json_name.SetString(iName.c_str(), allocator);
		json_val.SetString(iValue.c_str(), allocator);
		iDoc.AddMember(json_name, json_val, allocator);
	};

	void AddBool(rj::Document& iDoc, rj::MemoryPoolAllocator<>& allocator, const string& iName, const bool& iValue)
	{
		rj::Value json_name, json_val;
		json_name.SetString(iName.c_str(), allocator);
		json_val.SetBool(iValue);
		iDoc.AddMember(json_name, json_val, allocator);
	}
	void AddFloat(rj::Document& iDoc, rj::MemoryPoolAllocator<>& allocator, const string& iName, const float& iValue)
	{
		rj::Value json_name, json_val;
		json_name.SetString(iName.c_str(), allocator);
		json_val.SetFloat(iValue);
		iDoc.AddMember(json_name, json_val, allocator);
	}
	void AddDouble(rj::Document& iDoc, rj::MemoryPoolAllocator<>& allocator, const string& iName, const double& iValue)
	{
		rj::Value json_name, json_val;
		json_name.SetString(iName.c_str(), allocator);
		json_val.SetDouble(iValue);
		iDoc.AddMember(json_name, json_val, allocator);
	}
	void AddInt(rj::Document& iDoc, rj::MemoryPoolAllocator<>& allocator, const string& iName, const int& iValue)
	{
		rj::Value json_name, json_val;
		json_name.SetString(iName.c_str(), allocator);
		json_val.SetInt(iValue);
		iDoc.AddMember(json_name, json_val, allocator);
	}
	bool ReadString(const rj::Value& doc, const string tag, string& dest)
	{
		if (doc.HasMember(tag.c_str()))
		{
			dest = doc[tag.c_str()].GetString();
			return true;
		}
		return false;
	}
	bool ReadBool(const rj::Value& doc, const string tag, bool& dest)
	{
		if (doc.HasMember(tag.c_str()))
		{
			dest = doc[tag.c_str()].GetBool();
			return true;
		}
		return false;
	}
	bool ReadFloat(const rj::Value& doc, const string tag, float& dest)
	{
		if (doc.HasMember(tag.c_str()))
		{
			dest = doc[tag.c_str()].GetFloat();
			return true;
		}
		return false;
	}
	bool ReadDouble(const rj::Value& doc, const string tag, double& dest)
	{
		if (doc.HasMember(tag.c_str()))
		{
			dest = doc[tag.c_str()].GetDouble();
			return true;
		}
		return false;
	}
	bool ReadInt(const rj::Value& doc, const string tag, int& dest)
	{
		if (doc.HasMember(tag.c_str()))
		{
			dest = doc[tag.c_str()].GetInt();
			return true;
		}
		return false;
	}
	bool ReadInt(const rj::Value& doc, const string tag, size_t& dest)
	{
		if (doc.HasMember(tag.c_str()))
		{
			dest = doc[tag.c_str()].GetInt();
			return true;
		}
		return false;
	}
}