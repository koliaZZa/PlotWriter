#include "Include/Model/Plots/Model.PlotCollection.h"

namespace ARP::Model
{
	rj::Document PlotCollection::Save(rj::MemoryPoolAllocator<>& allocator)
	{
		rj::Document doc;
		doc.SetObject();

		// Сохранение общих данных
		rj::Value json_val;
		rju::AddString(doc, allocator, "xAxisName", xAxisName);
		rju::AddInt(doc, allocator, "Line width", lineWidth);

		// Сохранение типа интерполяции
		switch (interpolType)
		{
		case InterpolTypeEnum::Linear:
			json_val.SetString("Linear", allocator);
			break;
		case InterpolTypeEnum::Quadratic:
			json_val.SetString("Quadratic", allocator);
			break;
		case InterpolTypeEnum::Cubic:
			json_val.SetString("Cubic", allocator);
			break;
		default:
			break;
		}
		doc.AddMember("Interpolation", json_val, allocator);

		// Сохранение результатов пусков
		rj::Value arrRuns(rj::kArrayType);
		for (auto& run : runs)
			arrRuns.PushBack(run->Save(allocator), allocator);
		doc.AddMember("RunResults", arrRuns, allocator);

		// Сохранение графиков
		rj::Value arrPlots(rj::kArrayType);
		for (auto& plot : plots)
			arrPlots.PushBack(plot->Save(allocator), allocator);
		doc.AddMember("Plots", arrPlots, allocator);

		return doc;
	}
	void PlotCollection::Load(const rj::Value& doc)
	{
		// Считывание общих данных
		bool integrity = true;
		integrity = integrity && rju::ReadString(doc, "xAxisName", xAxisName);
		integrity = integrity && rju::ReadInt(doc, "Line width", lineWidth);


		// Считывание типа интерполяции
		string interpol;
		integrity = integrity && rju::ReadString(doc, "Interpolation", interpol);
		if (interpol == "Linear")		interpolType = InterpolTypeEnum::Linear;
		else if (interpol == "Quadratic")	interpolType = InterpolTypeEnum::Quadratic;
		else if (interpol == "Cubic")		interpolType = InterpolTypeEnum::Cubic;
		else								interpolType = InterpolTypeEnum::Linear;

		if (!integrity) ErrorReporter::PushMessage(ErrorType::FileReading, "Plot collection " + name, "Missing additional data");

		// Считывание результатов пусков
		for (auto& item : doc["RunResults"].GetArray())
			runs.push_back(std::make_shared<RunResult>(item));

		// Считывание графиков
		for (auto& item : doc["Plots"].GetArray())
			plots.push_back(std::make_shared<Plot>(item));

	}
	string PlotCollection::OpenRun(string path, ProtocolType protocolType)
	{
		ARP::Model::RunResultPtr pRun = std::make_shared<ARP::Model::RunResult>(path, protocolType);
		if (pRun->IsEmpty())
			return "";
		runs.push_back(pRun);
		return pRun->GetName();
	}

	void PlotCollection::AddRun(RunResultPtr ipRun)
	{
		runs.push_back(ipRun);
	}

	void PlotCollection::ReadChannels(string path)
	{
		fs::path filePath = fs::u8path(path);
		ifstream iFile(filePath, std::ios::binary);
		if (iFile)
		{
			string line1, line2, line3;
			vector<string> vExtNames, vNames, vTitles;
			std::getline(iFile, line1);
			std::getline(iFile, line2);
			std::getline(iFile, line3);
			vExtNames = Tokenize(line1);
			vNames = Tokenize(line2);
			vTitles = Tokenize(line3);
			if ((vExtNames.size() == vNames.size()) && (vExtNames.size() == vTitles.size()))
			{
				for (size_t i = 0; i < vExtNames.size(); i++)
					channels.push_back({ vExtNames[i], vNames[i], vTitles[i] });
			}
			else ErrorReporter::PushMessage(ErrorType::FileReading, "Plot collection " + name, "Wrong number of channels in channel file " + path);
		}
		else ErrorReporter::PushMessage(ErrorType::FileReading, "Plot collection " + name, "Channel file " + path + " didn't open");
		iFile.close();

	}

	void PlotCollection::ReadRunsList(string path)
	{
		fs::path filePath = fs::u8path(path);
		ifstream iFile(filePath, std::ios::binary);
		if (iFile)
		{
			string line;
			vector<string> vTmp;
			std::getline(iFile, line);
			while (std::getline(iFile, line))
			{
				vTmp = Tokenize(line);
				if (vTmp.size() < 4) continue;

				if (vTmp.size() == 4)
					runsComments.push_back({ vTmp[0], RunTypeEnum::enumer(vTmp[1]), ReStatusEnum::enumer(vTmp[2]), std::stoi(vTmp[3]), ""});
				else runsComments.push_back({ vTmp[0], RunTypeEnum::enumer(vTmp[1]), ReStatusEnum::enumer(vTmp[2]), std::stoi(vTmp[3]), vTmp[4] });
			}
		}
		else ErrorReporter::PushMessage(ErrorType::FileReading, "Plot collection " + name, "RunsList file " + path + " didn't open");
		iFile.close();
	}

	void PlotCollection::ShowRun(string iRunName, bool show)
	{
		RunResultPtr pRun = GetRun(iRunName);
		if (!pRun) return;
		pRun->isActive = show;

		// Добавить пуск на каждый график
		for (auto& plot : plots)
		{
			// Попытка переключить видимость линии пуска
			if (!plot->SignalShowLine(iRunName, show))
			{
				// Если неудачно, значит пуска нет;
				// Если надо скрыть, то ничего не предпринимать
				if (!show) continue;
				// Иначе добавить пуск на график
				plot->SignalAddLine(AddRunOnPlot(pRun, plot));
			}
		}
	}

	void PlotCollection::ShowRuns(bool show)
	{
		for (auto& pRun : runs)
		{
			pRun->isActive = show;
			// Добавить пуск на каждый график
			for (auto& plot : plots)
			{
				// Попытка переключить видимость линии пуска
				if (!plot->SignalShowLine(pRun->GetName(), show))
				{
					// Если неудачно, значит пуска нет;
					// Если надо скрыть, то ничего не предпринимать
					if (!show) continue;
					// Иначе добавить пуск на график
					plot->SignalAddLine(AddRunOnPlot(pRun, plot));
				}
			}
		}
	}


	void PlotCollection::DeleteRun(string iRunName)
	{
		if (runs.empty()) return;
		auto it = std::find_if(runs.begin(), runs.end(), [iRunName](Model::RunResultPtr pRun) { return pRun->GetName() == iRunName; });
		if (it != runs.end())
			runs.erase(it);
	}

	PlotPtr PlotCollection::CreatePlot(string iTitle, string iyAxisName)
	{
		PlotPtr plot = std::make_shared<Plot>(iTitle, xAxisName, iyAxisName, lineWidth, interpolType);
		for (auto& run : runs)
			AddRunOnPlot(run, plot);
		plots.push_back(plot);
		return plot;
	}

	void PlotCollection::DeletePlot(string iplotName)
	{
		auto plotIter = std::find_if(plots.begin(), plots.end(), [iplotName](PlotPtr pPlot) { return pPlot->name == iplotName; });
		if (plotIter != plots.end())
			plots.erase(plotIter);
	}

	void PlotCollection::SetXAxis(string ixAxis)
	{
		xAxisName = ixAxis;
		for (auto& plot : plots)
		{
			plot->SetXAxis(ixAxis);
			for (auto& run : runs)
			{
				if (!run->isActive) continue;
				auto xQuantity = run->GetQuantity(ixAxis);
				if (xQuantity.IsEmpty()) continue;
				PlotLinePtr line = plot->GetLine(run->GetName());
				if (!line) continue;
				line->SetXAxis(xQuantity);
			}
		}
	}

	RunResultPtr PlotCollection::GetRun(string iRunName) const
	{
		if (runs.empty()) return nullptr;
		auto it = std::find_if(runs.begin(), runs.end(), [iRunName](RunResultPtr pRun) { return pRun->GetName() == iRunName; });
		if (it != runs.end())
			return *it;
		else return nullptr;
	}

	PlotLinePtr PlotCollection::AddRunOnPlot(RunResultPtr iRun, PlotPtr iPlot)
	{
		auto xQuantity = iRun->GetQuantity(xAxisName);				// Величина из пуска, соответствующая оси X графика
		auto yQuantity = iRun->GetQuantity(iPlot->GetYAxis());		// Величина из пуска, соответствующая оси Y графика
		if (!xQuantity.IsEmpty() && !yQuantity.IsEmpty())
			return iPlot->AddLine(iRun->title, xQuantity, yQuantity);
		return nullptr;
	}

	void PlotCollection::MovePlotUp(size_t index)
	{
		if (plots.size() <= index || index == 0) return;
		// Переставляем местами текущий и предыдущий графики
		std::swap(plots[index], plots[index - 1]);
	}
	void PlotCollection::MovePlotDown(size_t index)
	{
		if (index + 1 <= plots.size()) return;
		// Переставляем местами текущий и следующий графики
		std::swap(plots[index], plots[index + 1]);
	}

	void PlotCollection::ProcessRuns()
	{
		if (channels.size())
		{
			for (auto& run : runs)
				run->SetQuantitiesNames(channels);
		}

		if (runsComments.size())
		{
			for (auto& runComm : runsComments)
			{
				auto it = std::find_if(runs.begin(), runs.end(), [&runComm](RunResultPtr run) {return run->GetName() == runComm.name; });
				if (it == runs.end()) continue;

				(*it)->runType = runComm.runType;
				(*it)->reStatus = runComm.reStatus;
				(*it)->gamma = runComm.gamma;
				(*it)->comment = runComm.comment;
			}
		}

		for (auto& run : runs)
		{
			run->CalcNominalParams();
			run->ApplyGamma();
			run->FormTitle();
		}
	}

	void PlotCollection::WriteRuns()
	{
		for (auto& run : runs)
			run->WriteProtocol("Protocol " + run->name + ".txt");
	}

}