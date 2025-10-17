#include "Include/Model/Model.ApplicationContext.h"

namespace ARP::Model
{
	ApplicationContext::ApplicationContext(string iPath)
	{
		std::ifstream file(iPath, std::ios::binary);
		if (file)
		{
			rj::IStreamWrapper bis(file);
			rj::EncodedInputStream<rj::UTF8<>, rj::IStreamWrapper> eis(bis);
			rj::Document doc;
			doc.ParseStream<0, rj::UTF8<>>(eis);
			if (!doc.HasParseError() && doc.IsObject())
				Load(doc);
			else ErrorReporter::PushMessage(ErrorType::FileReading, "AppContext", "Parsing file " + iPath + " failed");
		}
		else ErrorReporter::PushMessage(ErrorType::FileReading, "AppContext", "File " + iPath + " didn't open");
		file.close();
	}
	bool ApplicationContext::Save(string iPath)
	{
		rj::Document doc;
		doc = Save(doc.GetAllocator());

		std::ofstream file(iPath);
		rj::OStreamWrapper osw(file);

		typedef rj::EncodedOutputStream<rj::UTF8<>, rj::OStreamWrapper> OutputStream;
		OutputStream eos(osw, true);
		rj::PrettyWriter<OutputStream, rj::UTF8<>, rj::UTF8<>> writer(eos);
		writer.SetIndent('\t', 1);
		bool res = doc.Accept(writer);
		file.close();

		return res;
	}
	rj::Document ApplicationContext::Save(rj::MemoryPoolAllocator<>& allocator)
	{
		rj::Document doc;
		auto& alloc = doc.GetAllocator();
		doc.SetObject();

		rj::Value json_val, str_val;

		// Сохраняем список последних файлов
		json_val.SetArray();
		for (string& item : recentProjects)
		{
			str_val.SetString(item.c_str(), alloc);
			json_val.PushBack(str_val, alloc);
		}
		doc.AddMember("Recent projects", json_val, alloc);

		return doc;
	}
	void ApplicationContext::Load(const rj::Value& doc)
	{
		// Загружаем спсиок последних файлов
		if (doc.HasMember("Recent projects"))
			for (auto& val : doc["Recent projects"].GetArray())
				recentProjects.emplace_back(val.GetString());
	}
	bool ApplicationContext::AddRecentFile(string iPath)
	{
		bool needMenuRemake = false;
		auto foundedPos = std::find(recentProjects.begin(), recentProjects.end(), iPath);
		if (foundedPos == recentProjects.end())
		{
			if (recentProjects.size() >= 5)
			{
				recentProjects.erase(recentProjects.begin());
				needMenuRemake = true;
			}
		}
		else
		{
			recentProjects.erase(foundedPos);
			needMenuRemake = true;
		}
		recentProjects.push_back(iPath);
		return needMenuRemake;
	}
	void ApplicationContext::DelDoc(DocumentPtr iDoc)
	{
		auto itDoc = std::find(openDocs.begin(), openDocs.end(), iDoc);
		if (itDoc != openDocs.end())
			openDocs.erase(itDoc);
	}
}