#pragma once
#include <string>
#include <vector>
#include <fstream>

#include "Include/Model/Plots/Model.PlotCollection.h"
#include "Include/Model/NewRun.h"

using namespace ARP;

inline void FlushErrorMessages(std::ofstream& iFile)
{
	while (!Model::ErrorReporter::IsEmpty())
		iFile << Model::ErrorReporter::PopMessage() << std::endl;
	iFile << std::endl;
}

void ProcessBalancesT128(std::ofstream& iLog, bool alphaVar = true, bool machVar = false)
{
	std::vector<double> machs{ 0.4, 0.6, 0.8, 0.85, 0.9, 0.95, 1.0, 1.05, 1.1, 1.2, 1.4 };
	std::vector<std::string> quantities{ "Cxf1", "Cy1", "Cz1", "mx1", "my1", "mz1", "Xcp1", "Cxf2", "Cy2", "Cz2", "mx2", "my2", "mz2", "Xcp2" };

	//std::vector<double> machs{ 0.4, 0.85 };
	//std::vector<std::string> quantities{ "mx1", "Xcp1" };

	Model::PlotCollection allRuns;
	allRuns.OpenRunsFromFile("./data/balances/S5 T-128 Report Runs.txt", Model::ProtocolTypeEnum::Unified);
	allRuns.ReadChannels("./data/balances/channels T128.txt");
	allRuns.ReadRunsList("./data/balances/runs list T128.txt");
	allRuns.ProcessRuns(true, 2);

	Model::PlotCollection CFDRuns;
	CFDRuns.OpenRunsFromFile("./data/balances/S5 CFD T128 Report Data.txt", Model::ProtocolTypeEnum::Unified);
	CFDRuns.ReadChannels("./data/balances/channels CFD.txt");
	CFDRuns.ReadRunsList("./data/balances/runs list CFD T128.txt");
	CFDRuns.ProcessRuns(false, 2);

	std::pair<double, double> yScale{ 0.0, 0.0 };
	
	// Alpha var
	if (alphaVar)
	{
		for (auto mach : machs)
		{
			Model::PlotCollection plotColl("alpha");
			// Experiment
			for (auto& run : allRuns.GetRuns())
			{
				if ((run->runType == Model::RunTypeEnum::AlphaVar) && (run->GetMachNom() == mach))
					plotColl.AddRun(run);
			}
			for (auto& quantity : quantities)
			{
				if (quantity == "Xcp1") yScale = { 0.0, 0.5 };
				else if (quantity == "Xcp2") yScale = { 0.0, 0.15 };
				else yScale = { 0.0, 0.0 };
				plotColl.CreatePlot(quantity, "./graphs/balances/M=" + Model::to_str(mach, 3), yScale);
			}

			// Add CFD
			size_t nCFDRuns{0};
			for (auto& run : CFDRuns.GetRuns())
			{
				if ((run->runType == Model::RunTypeEnum::AlphaVar) && (run->GetMachNom() == mach))
				{
					plotColl.AddRun(run);
					nCFDRuns++;
				}
			}
			if (!nCFDRuns) continue;
			for (auto& quantity : quantities)
			{
				if (quantity == "Xcp1") yScale = { 0.0, 0.5 };
				else if (quantity == "Xcp2") yScale = { 0.0, 0.15 };
				else yScale = { 0.0, 0.0 };
				plotColl.CreatePlot(quantity, "./graphs/balances/M=" + Model::to_str(mach, 3) + "+CFD", yScale);
			}
		}
	}

	//Mach var
	if (machVar)
	{
		Model::PlotCollection machVarPlots("Mach");
		for (auto& run : allRuns.GetRuns())
		{
			if (run->runType == Model::RunTypeEnum::MachVar)
				machVarPlots.AddRun(run);
		}
		for (auto& quantity : quantities)
		{
			if (quantity == "Xcp1") yScale = { 0.0, 0.5 };
			else if (quantity == "Xcp2") yScale = { 0.0, 0.1 };
			else yScale = { 0.0, 0.0 };
			machVarPlots.CreatePlot(quantity, "./graphs/balances/Mach var", yScale);
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
	allRuns.ProcessRuns(false, 1);

	Model::PlotCollection CFDRuns;
	CFDRuns.OpenRunsFromFile("./data/balances/S5 CFD T109 Report Data.txt", Model::ProtocolTypeEnum::Unified);
	CFDRuns.ReadChannels("./data/balances/channels CFD.txt");
	CFDRuns.ReadRunsList("./data/balances/runs list CFD T109.txt");
	CFDRuns.ProcessRuns(false, 1);

	std::pair<double, double> yScale{ 0.0, 0.0 };

	for (auto mach : machs)
	{
		Model::PlotCollection plotColl("alpha");
		// Experiment
		for (auto& run : allRuns.GetRuns())
		{
			if ((run->runType == Model::RunTypeEnum::AlphaVar) && (run->GetMachNom() == mach))
				plotColl.AddRun(run);
		}
		for (auto& quantity : quantities)
		{
			if (quantity == "Xcp1") yScale = { 0.0, 0.5 };
			else if (quantity == "Xcp2") yScale = { -0.45, 0.45 };
			else yScale = { 0.0, 0.0 };
			plotColl.CreatePlot(quantity, "./graphs/balances/M=" + Model::to_str(mach, 2), yScale);
		}

		// Add CFD
		size_t nCFDRuns{ 0 };
		for (auto& run : CFDRuns.GetRuns())
		{
			if ((run->runType == Model::RunTypeEnum::AlphaVar) && (run->GetMachNom() == mach))
			{
				plotColl.AddRun(run);
				nCFDRuns++;
			}
		}
		if (!nCFDRuns) continue;
		for (auto& quantity : quantities)
		{
			if (quantity == "Xcp1") yScale = { 0.0, 0.5 };
			else if (quantity == "Xcp2") yScale = { -0.45, 0.45 };
			else yScale = { 0.0, 0.0 };
			plotColl.CreatePlot(quantity, "./graphs/balances/M=" + Model::to_str(mach, 2) + "+CFD", yScale);
		}
	}

	FlushErrorMessages(iLog);
}

void ProcessPressure(string iDataPath, string iPointsDataPath, string iGraphsPath)
{
	NewRun run = NewRun(iDataPath, iPointsDataPath);
	std::vector< std::shared_ptr<ARP::Model::DrawGraphs>> graphcollections;

	std::string RunName = "";

	//auto phi_vec = run.GetAllPfi();
	vector<double> phi_vec{ 0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300, 330 };
	for (auto phi : phi_vec)
	{
		for (size_t i = 0; i < run.colNames.size(); i++)
		{
			if (RunName == run.colNames[i])
			{
				auto data = run.GetGraphDataForPhiConst(phi, i);
				graphcollections.back()->AddLine(data.first, data.second, "#alpha=" + Model::to_str(run.quantities[7].data[i], 2) + "#circ");
			}
			else
			{
				if (!graphcollections.empty())
					graphcollections.back()->DrawAndPrint(iGraphsPath+RunName+"/phi const/", {0.0, 0.0});

				RunName = run.colNames[i];

				vector<string> runStr = Model::Tokenize(RunName, "_r");
				string graphTitle = runStr[0] + u8", \\hbox{пуск} " + runStr[1] + ", \\phi=" + std::to_string(int(phi)) + "^{\\circ}";
				graphcollections.push_back(std::make_shared<ARP::Model::DrawGraphs>(graphTitle, "x", "C_{p}", "x", "phi=" + std::to_string(int(phi)), Model::GraphType::CpPhi));
				auto data = run.GetGraphDataForPhiConst(phi, i);
				graphcollections.back()->AddLine(data.first, data.second, "#alpha=" + Model::to_str(run.quantities[7].data[i], 2) + "#circ");

			}
			graphcollections.back()->DrawAndPrint(iGraphsPath + RunName + "/phi const/", { 0.0, 0.0 });
		}
	}
	graphcollections.clear();

	if (false)
	{
		//
		RunName = run.colNames[0];

		vector<size_t> start_points{ 0 };
		for (int i = 1; i < run.colNames.size(); i++)
			if (RunName != run.colNames[i])
			{
				start_points.push_back(i);
				RunName = run.colNames[i];
			}
		start_points.push_back(run.colNames.size() - 1);


		auto x_vec = run.GetAllX();
		for (size_t p = 0; p < start_points.size() - 1; p++)
		{
			size_t start = start_points[p];
			size_t end = start_points[p + 1];

			for (auto x : x_vec)
			{
				graphcollections.push_back(std::make_shared<ARP::Model::DrawGraphs>(RunName, "#alpha", "Cp", "", ""));
				for (size_t row = 0; row < run.cord_table.size(); row++)
				{
					size_t calbr_row = row + 8;

					if (x == ARP::Model::roundTo(run.cord_table[row].data[0], 5))
					{
						auto data = run.GetGraphDataForXConst(calbr_row, start, end);
						graphcollections.back()->AddLine(std::get<0>(data), std::get<1>(data), std::get<2>(data));
					}
				}
				graphcollections.back()->DrawAndPrint("", { 0.0, 0.0 });
			}
		}

		graphcollections.clear();

		//

		for (auto x : x_vec)
		{
			for (size_t i = 0; i < run.colNames.size(); i++)
			{
				if (RunName == run.colNames[i])
				{
					auto data = run.GetGraphDataForXConstPolar(x, i);
					graphcollections.back().get()->AddLine(std::get<0>(data), std::get<1>(data), std::get<2>(data));

				}
				else
				{
					if (!graphcollections.empty())
						graphcollections.back().get()->DrawAndPrint("", { 0.0, 0.0 });

					RunName = run.colNames[i];

					graphcollections.push_back(std::make_shared<ARP::Model::DrawGraphs>(RunName, "fi", "Cp", "", ""));
					auto data = run.GetGraphDataForXConstPolar(x, i);
					graphcollections.back().get()->AddLine(std::get<0>(data), std::get<1>(data), std::get<2>(data));
				}
			}

			graphcollections.back().get()->DrawAndPrint("", { 0.0, 0.0 });
		}
	}


}

void InitPressureFolders(string iRootDir, string iRunsList)
{
	ifstream runsList(iRunsList);
	if (!runsList) return;

	Model::Table table = Model::ReadTable(runsList);
	vector<string> runNames;
	if (table.empty()) return;
	if (table.begin()->empty()) return;

	size_t ind = 0;
	while (table[0][ind] != "Run_name") ind++;
	if (ind >= table.begin()->size()) return;

	for (size_t i = 1; i < table.size(); i++)
		runNames.push_back(table[i][ind]);

	for (string runName : runNames)
	{
		fs::create_directories({ iRootDir + "/" + runName + "/phi const/" });
		fs::create_directories({ iRootDir + "/" + runName + "/x const/" });
		fs::create_directories({ iRootDir + "/" + runName + "/x const, polar/" });
	}
}

int main() {
	TApplication app("app", nullptr, nullptr);

	//// Для проверки
	////Model::drawMultipleGraphs();
	
	std::ofstream log("./log.txt");
	//InitPressureFolders("./graphs/pressure/D4.11", "./data/pressure/Cp runs list D4.11.txt");
	//InitPressureFolders("./graphs/pressure/D5.2", "./data/pressure/Cp runs list D5.2.txt");
	ProcessPressure("./data/pressure/Cp_d4.11.txt", "./data/pressure/Points_coords_D4.11.txt", "./graphs/pressure/D4.11/");

	//ProcessBalancesT128(log, true, true);
	//ProcessBalancesT109(log);
	log.close();
	app.Run();

	return 0;
}