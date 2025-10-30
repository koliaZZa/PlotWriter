#pragma once
#include "Include/Model/Runs/Model.RunResult.h"

namespace Model
{
	class RunsInfo
	{
	public:
		RunsInfo() = default;
		RunsInfo(string iRunsListPath);
		~RunsInfo() = default;

		double GetMach(string iRunName);
		int GetGamma(string iRunName);
		string GetReStatus(string iRunName);
		RunType GetRunType(string iRunName);

	protected:
		vector<RunInfo> data;
		RunInfo& GetRun(string iRunName);
	};
}
