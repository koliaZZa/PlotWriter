#include "Include/Model/Plots/Model.PlotCollection.h"

namespace ARP::Model
{
	string PlotCollection::OpenRun(string path, ProtocolType protocolType)
	{
		ARP::Model::RunResultPtr pRun = std::make_shared<ARP::Model::RunResult>(path, protocolType);
		if (pRun->IsEmpty())
			return "";
		runs.push_back(pRun);
		return pRun->GetName();
	}

	void PlotCollection::OpenRunsFromFile(string path, ProtocolType protocolType)
	{
		ifstream iFile(path);
		if (!iFile) return;
		while(!iFile.eof())
			runs.push_back(std::make_shared<ARP::Model::RunResult>(iFile, protocolType));
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

	void PlotCollection::DeleteRun(string iRunName)
	{
		if (runs.empty()) return;
		auto it = std::find_if(runs.begin(), runs.end(), [iRunName](Model::RunResultPtr pRun) { return pRun->GetName() == iRunName; });
		if (it != runs.end())
			runs.erase(it);
	}

	void PlotCollection::CreatePlot(std::string iyName, string path, pair<double, double> yScale, std::string iTitle)
	{
		if (runs.empty()) return;
		Quantity x{ (*runs.begin())->GetQuantity(xAxisName) };
		Quantity y{ (*runs.begin())->GetQuantity(iyName) };

		if (iTitle.empty()) iTitle = y.title;

		DrawGraphsPtr plot = std::make_shared<DrawGraphs>(iTitle, x.title, y.title, xAxisName, iyName);
		string yname = plot->yname;
		for (auto& run : runs)
		{
			Quantity x{ run->GetQuantity(xAxisName) };
			plot->AddLine(x, run->GetQuantity(yname), run->title, run->GetExperimentType() == ExperimentType::CFD);
		}
		plot->DrawAndPrint(path, yScale);
	}

	void PlotCollection::SetXAxis(string ixAxis)
	{
		xAxisName = ixAxis;
	}

	RunResultPtr PlotCollection::GetRun(string iRunName) const
	{
		if (runs.empty()) return nullptr;
		auto it = std::find_if(runs.begin(), runs.end(), [iRunName](RunResultPtr pRun) { return pRun->GetName() == iRunName; });
		if (it != runs.end())
			return *it;
		else return nullptr;
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

	void PlotCollection::ProcessRuns(bool useReInfo, size_t precision)
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
			run->CalcNominalParams(precision);
			//run->ApplyGamma();
			run->FormLatexTitle(useReInfo);
		}
	}

	void PlotCollection::WriteRuns()
	{
		for (auto& run : runs)
			run->WriteProtocol("Protocol " + run->name + ".txt");
	}
}