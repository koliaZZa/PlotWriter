#include "Include/Model/Runs/Model.RunsInfo.h"

namespace Model
{
	RunsInfo::RunsInfo(string iRunsListPath)
	{
		ifstream file(iRunsListPath);
		if (file)
		{
			string line;
			vector<string> vTmp;
			std::getline(file, line);
			vTmp = Tokenize(line);
			size_t nameInd{}, gammaInd{}, machInd{}, ReInd{}, WTInd{}, numInd{}, runTypeInd{};

			for (size_t i = 0; i < vTmp.size(); i++)
			{
				if (vTmp[i] == "Run_name") nameInd = i;
				else if (vTmp[i] == u8"АДТ") WTInd = i;
				else if (vTmp[i] == u8"Пуск") numInd = i;
				else if (vTmp[i] == u8"gamma") gammaInd = i;
				else if (vTmp[i] == u8"M") machInd = i;
				else if (vTmp[i] == u8"Re") ReInd = i;
				else if (vTmp[i] == u8"Type") runTypeInd = i;
			}

			while (std::getline(file, line))
			{
				vTmp = Tokenize(line);
				RunType rType = RunTypeEnum::enumer(vTmp[runTypeInd]);
				double mach = rType == RunTypeEnum::AlphaVar ? std::stod(vTmp[machInd]) : 0.0;
				RunInfo r{ vTmp[nameInd], vTmp[WTInd], std::stoi(vTmp[numInd]), rType, ReStatusEnum::enumer(vTmp[ReInd]), mach, std::stoi(vTmp[gammaInd]), "" };
				data.push_back(r);
			}
		}
	}
	double RunsInfo::GetMach(string iRunName)
	{
		return GetRun(iRunName).machNom;
	}
	int RunsInfo::GetGamma(string iRunName)
	{
		return GetRun(iRunName).gamma;
	}
	string RunsInfo::GetReStatus(string iRunName)
	{
		return ReStatusEnum::str(GetRun(iRunName).reStatus);
	}
	RunType RunsInfo::GetRunType(string iRunName)
	{
		return GetRun(iRunName).runType;
	}
	RunInfo& RunsInfo::GetRun(string iRunName)
	{
		auto& iter = std::find_if(data.begin(), data.end(), [&iRunName](RunInfo ri) {return ri.name == iRunName; });
		if (iter == data.end()) return RunInfo();
		return *iter;
	}
}