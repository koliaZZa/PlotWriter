#include "Include/Model/Model.Document.h"

namespace ARP::Model
{
	Document::Document(string iPath)
	{
		if (!iPath.empty())
		{
			path = iPath;

			// Открытие json-документа
			fs::path filePath = fs::u8path(path);
			std::ifstream file(filePath, std::ios::binary);

			rj::IStreamWrapper bis(file);
			rj::EncodedInputStream<rj::UTF8<>, rj::IStreamWrapper> eis(bis);
			rj::Document doc;
			doc.ParseStream<0, rj::UTF8<>>(eis);
			if (!doc.HasParseError() && doc.IsObject())
			{
				// Считывание общих данных
				ReadHeader(doc);

				// Считывание основного тела
				if (doc.HasMember("Body"))
					Load(doc["Body"].GetObject());
				else ErrorReporter::PushMessage(ErrorType::FileReading, "Project" + name, "No body");
			}
			else ErrorReporter::PushMessage(ErrorType::FileReading, "Initializing project", "Parsing file " + iPath + " failed");

			file.close();
		}
	}
	void Document::SaveToFile(string iPath)
	{
		if (!iPath.empty())
			path = iPath;

		if (path.empty())
			path = name + ".json";

		// Открытие файла и создание json-документа
		std::ofstream file(fs::u8path(path), std::ios::binary);
		rj::Document doc;
		doc.SetObject();
		auto& allocator = doc.GetAllocator();

		// Добавление общих данных
		WriteHeader(doc, allocator);

		// Добавление основного тела
		rj::Document bodyDoc = Save(allocator);
		doc.AddMember("Body", bodyDoc, allocator);

		// Запись в файл
		rj::OStreamWrapper osw(file);
		typedef rj::EncodedOutputStream<rj::UTF8<>, rj::OStreamWrapper> OutputStream;
		OutputStream eos(osw, true);
		rj::PrettyWriter<OutputStream, rj::UTF8<>, rj::UTF8<>> writer(eos);
		writer.SetIndent('\t', 1);
		bool res = doc.Accept(writer);
		file.close();
		if (!res) ErrorReporter::PushMessage(ErrorType::FileSaving, "Project" + name, "Writing failed");
	}
	rj::Document Document::Save(rj::MemoryPoolAllocator<>& allocator)
	{
		rj::Document doc;
		doc.SetObject();

		// Сохраняем вектор результатов сравнений
		rj::Value array(rj::kArrayType);
		for (auto& resComp : resComps)
			array.PushBack(resComp->Save(allocator), allocator);
		doc.AddMember("ResultComparisons", array, allocator);

		return doc;
	}
	void Document::Load(const rj::Value& doc)
	{
		for (auto& item : doc["ResultComparisons"].GetArray())
			resComps.push_back(std::make_shared<ResultComparison>(item));
	}
}

