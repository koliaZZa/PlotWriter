#include "Include/Model/Runs/Model.RunSignals.h"

namespace ARP::Model
{
	RunSignals::RunSignals(string iFilePath, double iFreq, function<void(size_t)> iProgressFunc) : Object(), frequency{ iFreq }, SignalOnProgress{ iProgressFunc }
	{
		ifstream iFile(iFilePath);
		if (iFile)
		{
			fs::path path{ iFilePath };

			size_t fileSize = fs::file_size(path);
			size_t currPr = 0, progress = 0;
			size_t bytes = 0;

			string line;
			vector<string> vTmp;
			vector<vector<double>> data;

			// Построчное чтение файла
			while (std::getline(iFile, line))
			{
				vTmp = Tokenize(line);
				
				bytes += line.size();
				currPr = round(float(bytes) / float(fileSize) * 100);
				if (currPr != progress)
				{
					progress = currPr;
					SignalOnProgress(progress);
				}

				auto vtmpIt = vTmp.begin();
				if (!data.size())
					data.resize(vTmp.size());
				for (auto& signal : data)
					signal.push_back(std::stod(*vtmpIt++));
			}

			for (size_t i{}; i < data.size(); i++)
				signals.emplace_back(string("Ch") + std::to_string(i), std::move(data[i]));
		}
		else ErrorReporter::PushMessage(ErrorType::FileReading, "RunSignals " + name, "File " + iFilePath + " didn't open");
		iFile.close();

		if (!signals.size())
			ErrorReporter::PushMessage(ErrorType::FileReading, "RunResult " + name, "No data read");
		FormTimeQuantity();
	}
	rj::Document RunSignals::Save(rj::MemoryPoolAllocator<>& allocator)
	{
		rj::Document doc;
		doc.SetObject();

		// Сохранение общих данных
		WriteHeader(doc, allocator);
		rju::AddDouble(doc, allocator, "Frequency", frequency);

		// Сохранение физ. величин
		rj::Value arrQuantities(rj::kArrayType);
		for (auto& q : signals)
			arrQuantities.PushBack(q.Save(allocator), allocator);
		doc.AddMember("Signals", arrQuantities, allocator);

		return doc;
	}
	void RunSignals::Load(const rj::Value& doc)
	{
		// Считывание общих данных
		ReadHeader(doc);
		bool integrity = rju::ReadDouble(doc, "Frequency", frequency);

		// Считывание физ. величин
		for (auto& item : doc["Signals"].GetArray())
			signals.emplace_back(item);
	}
	optional<Quantity> RunSignals::GetSignal(size_t i) const
	{
		if (i < signals.size())
			return signals[i];
		else return std::nullopt;
	}
	optional<Quantity> RunSignals::GetSignal() const
	{
		if (currInd >= signals.size())
		{
			currInd = 0;
			return std::nullopt;
		}
		return signals[currInd++];
	}
	void RunSignals::FormTimeQuantity()
	{
		if (signals.empty()) return;
		assert(frequency != 0);

		size_t n = signals[0].GetSize();
		vector<double> times;
		for (size_t i = 0; i < n; i++)
			times.push_back(i / frequency);
		time = Quantity("t", std::move(times));
	}
}