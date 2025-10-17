#include "Include/Model/Model.ResultComparison.h"

namespace ARP::Model
{
	string ResultComparison::OpenRun(string path)
	{
		string runName = plotCollection->OpenRun(path);
		if (!runName.empty())
			runNames.emplace_back(runName, false);
		return runName;
	};
	rj::Document ResultComparison::Save(rj::MemoryPoolAllocator<>& allocator)
	{
		rj::Document doc;
		doc.SetObject();

		// Сохранение общих данных
		WriteHeader(doc, allocator);

		// Сохранение статуса выбора физ. величин
		rj::Value json_val;
		rj::Value arrQuantities(rj::kArrayType);
		for (auto& q : quantities)
		{
			rj::Document dArray;
			dArray.SetArray();
			json_val.SetString(q.first.c_str(), allocator);
			dArray.PushBack(json_val, allocator);
			json_val.SetBool(q.second);
			dArray.PushBack(json_val, allocator);
			arrQuantities.PushBack(dArray, allocator);
		}
		doc.AddMember("Quantities status", arrQuantities, allocator);

		// Сохранение статуса выбора пусков
		rj::Value arrRuns(rj::kArrayType);
		for (auto& r : runNames)
		{
			rj::Document dArray;
			dArray.SetArray();
			json_val.SetString(r.first.c_str(), allocator);
			dArray.PushBack(json_val, allocator);
			json_val.SetBool(r.second);
			dArray.PushBack(json_val, allocator);
			arrRuns.PushBack(dArray, allocator);
		}
		doc.AddMember("Runs status", arrRuns, allocator);

		// Сохранение коллекции графиков
		doc.AddMember("Plot collection", plotCollection->Save(allocator), allocator);

		return doc;
	}
	void ResultComparison::Load(const rj::Value& doc)
	{
		ReadHeader(doc);

		for (auto& item : doc["Quantities status"].GetArray())
		{
			auto pair = item.GetArray();
			quantities.push_back({ pair[0].GetString(), pair[1].GetBool() });
		}
		if (quantities.empty()) ErrorReporter::PushMessage(ErrorType::FileReading, "Result comparison " + name, "Missing quantities status");

		for (auto& item : doc["Runs status"].GetArray())
		{
			auto pair = item.GetArray();
			runNames.push_back({ pair[0].GetString(), pair[1].GetBool() });
		}
		if (quantities.empty()) ErrorReporter::PushMessage(ErrorType::FileReading, "Result comparison " + name, "Missing runs status");

		if (doc.HasMember("Plot collection"))
			plotCollection = std::make_shared<PlotCollection>(doc["Plot collection"]);
	}
	vector<string> ResultComparison::UpdateQuantitiesByLast()
	{
		vector<string> newOptions;
		RunResultPtr pRun = plotCollection->GetLastRun();
		if (!pRun) return newOptions;

		auto vNames = pRun->GetQuantitiesNames();
		for (auto& name : vNames)
		{
			// Добавить уникальные величины к результату
			if (std::find_if(quantities.begin(), quantities.end(), [name](CheckListItem item) {return name == item.first; }) == quantities.end())
			{
				quantities.emplace_back(name, false);
				newOptions.push_back(name);
			}
		}
		return newOptions;
	}


}