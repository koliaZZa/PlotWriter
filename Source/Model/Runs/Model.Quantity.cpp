#include "Include/Model/Runs/Model.Quantity.h"

namespace ARP::Model
{
	Quantity::Quantity(string iName, vector<double>&& iData) : Object(iName, iName), data{ iData }
	{
		size_t overLimit = 0, thresholdNum = 100;
		bool allZeros = true;
		double threshold = 1e7;

		double max = data[0], min = data[0];
		for (auto& num : data)
		{
			if (num > max) max = num;
			if (num < min) min = num;

			if (abs(num) > threshold)
			{
				if (overLimit > thresholdNum)
				{
					status = SignalStatus::Exceeded;
					break;
				}
				else overLimit++;
			}
			else if (num != 0.0)
				allZeros = false;
		}
		if (allZeros)
			status = SignalStatus::Zero;

		if ((max > 0) && (min > 0))
			range = max;
		else if ((max < 0) && (min < 0))
			range = -min;
		else range = max - min;
	}
	rj::Document Quantity::Save(rj::MemoryPoolAllocator<>& allocator)
	{
		rj::Document doc;
		doc.SetObject();

		// —охранение общих данных
		WriteHeader(doc, allocator);
		rju::AddDouble(doc, allocator, "Range", range);

		// —охранение числовых данных
		rj::Value array(rj::kArrayType);
		for (auto item : data)
			array.PushBack(item, allocator);
		doc.AddMember("Data", array, allocator);
		return doc;
	}
	void Quantity::Load(const rj::Value& doc)
	{
		// —читывание общих данных
		ReadHeader(doc);

		// —читывание данных
		for (auto& item : doc["Data"].GetArray())
			data.push_back(item.GetDouble());

		bool integrity = rju::ReadDouble(doc, "Range", range);
		if (!integrity) ErrorReporter::PushMessage(ErrorType::FileReading, "Quantity " + name, "Missing additional data");
	}
	void Quantity::Normalize(bool normalize)
	{
		if (!range)
		{
			ErrorReporter::PushMessage(ErrorType::MathCalc, "Quantity " + name, "Normalizing by zero");
			return;
		}
		double k = normalize ? 1 / range : range;
		for (auto& x : data)
			x = x * k;
	}
	void Quantity::SetNames(string iextName, string iName, string iTitle)
	{
		extName = iextName;
		name = iName;
		title = iTitle;
	}
}