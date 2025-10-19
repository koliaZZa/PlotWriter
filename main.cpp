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

void Run()
{
	std::ofstream log("log.txt");
	//std::vector<double> machs{ 0.4, 0.6, 0.8, 0.85, 0.9, 0.95, 1.0, 1.05, 1.1, 1.2, 1.4 };
	//std::vector<std::string> quantities{ "Cxf1", "Cy1", "Cz1", "mx1", "my1", "mz1", "Xcp1", "Cxf2", "Cy2", "Cz2", "mx2", "my2", "mz2", "Xcp2" };

	std::vector<double> machs{ 0.4, 0.6 };
	std::vector<std::string> quantities{ "Cxf1", "Cxf2" };


	Model::PlotCollection allRuns;
	allRuns.OpenRunsFromFile("./data/balances/S5 T-128 Report Runs.txt", Model::ProtocolTypeEnum::T128);
	allRuns.ReadChannels("./data/balances/channels.txt");
	allRuns.ReadRunsList("./data/balances/runs list.txt");
	allRuns.ProcessRuns();

	std::vector<Model::PlotCollection> plotColls;

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
			plotColls.back().CreatePlot(quantity, "./graphs/balances/M=" + Model::to_str(mach, 3));
		}
	}

	FlushErrorMessages(log);
	log.close();
}

int main() {
	TApplication app("app", nullptr, nullptr);

	// Для проверки
	//Model::drawMultipleGraphs();
	Run();

	app.Run();

	return 0;
}