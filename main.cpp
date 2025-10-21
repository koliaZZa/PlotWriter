#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "Include/Model/Plots/Model.PlotCollection.h"

using namespace ARP;

inline void FlushErrorMessages(std::ofstream& iFile)
{
	while (!Model::ErrorReporter::IsEmpty())
		iFile << Model::ErrorReporter::PopMessage() << std::endl;
	iFile << std::endl;
}

void ProcessBalancesT128(std::ofstream& iLog)
{
	std::vector<double> machs{ 0.4, 0.6, 0.8, 0.85, 0.9, 0.95, 1.0, 1.05, 1.1, 1.2, 1.4 };
	std::vector<std::string> quantities{ "Cxf1", "Cy1", "Cz1", "mx1", "my1", "mz1", "Xcp1", "Cxf2", "Cy2", "Cz2", "mx2", "my2", "mz2", "Xcp2" };

	//std::vector<double> machs{ 0.4, 0.85 };
	//std::vector<std::string> quantities{ "mx1", "Xcp1" };

	Model::PlotCollection allRuns;
	allRuns.OpenRunsFromFile("./data/balances/S5 T-128 Report Runs.txt", Model::ProtocolTypeEnum::Unified);
	allRuns.ReadChannels("./data/balances/channels T128.txt");
	allRuns.ReadRunsList("./data/balances/runs list T128.txt");
	allRuns.ProcessRuns(2);

	std::vector<Model::PlotCollection> plotColls;
	std::pair<double, double> yScale{ 0.0, 0.0 };

	for (auto mach : machs)
	{
		plotColls.emplace_back("alpha");
		for (auto& run : allRuns.GetRuns())
		{
			if ((run->runType == Model::RunTypeEnum::AlphaVar) && (run->GetMachNom() == mach))
				plotColls.back().AddRun(run);
		}
		for (auto& quantity : quantities)
		{
			if (quantity == "Xcp1") yScale = { 0.0, 0.5 };
			else if (quantity == "Xcp2") yScale = { 0.0, 0.1 };
			else yScale = { 0.0, 0.0 };
			plotColls.back().CreatePlot(quantity, "./graphs/balances/M=" + Model::to_str(mach, 3), yScale);
		}
	}

	FlushErrorMessages(iLog);
}

void ProcessBalancesT109(std::ofstream& iLog)
{
	std::vector<double> machs{ 1.7, 2.3, 3.0, 3.6 };
	std::vector<std::string> quantities{ "Cxf1", "Cy1", "Cz1", "mx1", "my1", "mz1", "Xcp1", "Cxf2", "Cy2", "Cz2", "mx2", "my2", "mz2", "Xcp2" };

	//std::vector<double> machs{ 0.4, 0.85 };
	//std::vector<std::string> quantities{ "mx1", "Xcp1" };

	Model::PlotCollection allRuns;
	allRuns.OpenRunsFromFile("./data/balances/S5 T-109 Report Runs.txt", Model::ProtocolTypeEnum::Unified);
	allRuns.ReadChannels("./data/balances/channels T109.txt");
	allRuns.ReadRunsList("./data/balances/runs list T109.txt");
	allRuns.ProcessRuns(1);

	std::vector<Model::PlotCollection> plotColls;
	std::pair<double, double> yScale{ 0.0, 0.0 };

	for (auto mach : machs)
	{
		plotColls.emplace_back("alpha");
		for (auto& run : allRuns.GetRuns())
		{
			if ((run->runType == Model::RunTypeEnum::AlphaVar) && (run->GetMachNom() == mach))
				plotColls.back().AddRun(run);
		}
		for (auto& quantity : quantities)
		{
			if (quantity == "Xcp1") yScale = { 0.0, 0.5 };
			else if (quantity == "Xcp2") yScale = { -0.35, 0.35 };
			else yScale = { 0.0, 0.0 };
			plotColls.back().CreatePlot(quantity, "./graphs/balances/M=" + Model::to_str(mach, 2), yScale);
		}
	}

	FlushErrorMessages(iLog);
}

int main() {
	TApplication app("app", nullptr, nullptr);

	// Для проверки
	//Model::drawMultipleGraphs();
	std::ofstream log("log.txt");

	//ProcessBalancesT128(log);
	ProcessBalancesT109(log);
	log.close();
	app.Run();

	return 0;
}