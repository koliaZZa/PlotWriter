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
	allRuns.ProcessRuns(true, 2);

	std::vector<Model::PlotCollection> plotColls;
	std::pair<double, double> yScale{ 0.0, 0.0 };
	
	// Alpha var
	if (bool alphaVar = false)
	{
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
				else if (quantity == "Xcp2") yScale = { 0.0, 0.15 };
				else yScale = { 0.0, 0.0 };
				plotColls.back().CreatePlot(quantity, "./graphs/balances/M=" + Model::to_str(mach, 3), yScale);
			}
		}
	}

	//Mach var
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



void RunTwo()
{
	NewRun run = NewRun("Cp.txt");
	std::vector< std::shared_ptr<ARP::Model::DrawGraphs>> graphcollections;

	std::string RunName = "";

	auto pfi_vec = run.GetAllPfi();
	for (auto pfi : pfi_vec)
	{
		for (size_t i = 0; i < run.colNames.size(); i++)
		{
			if (RunName == run.colNames[i])
			{
				auto data = run.GetGraphDataForPhiConst(pfi, i);
				graphcollections.back().get()->AddLine(data.first, data.second, std::to_string(run.quantities[7].data[i]));
			}
			else
			{
				if (!graphcollections.empty())
					graphcollections.back().get()->DrawAndPrint("", { 0.0, 0.0 });

				RunName = run.colNames[i];

				graphcollections.push_back(std::make_shared<ARP::Model::DrawGraphs>(RunName, "x", "Cp", "", ""));
				auto data = run.GetGraphDataForPhiConst(pfi, i);
				graphcollections.back().get()->AddLine(data.first, data.second, std::to_string(run.quantities[7].data[i]));

			}
			graphcollections.back().get()->DrawAndPrint("", { 0.0, 0.0 });
		}
	}
	graphcollections.clear();

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
					graphcollections.back().get()->AddLine(std::get<0>(data), std::get<1>(data), std::get<2>(data));
				}

			}
			graphcollections.back().get()->DrawAndPrint("", { 0.0, 0.0 });
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

int main() {
	TApplication app("app", nullptr, nullptr);

	//// Для проверки
	////Model::drawMultipleGraphs();
	//Run();
	RunTwo();

	//ProcessBalancesT128(log);
	/*ProcessBalancesT109(log);
	log.close();*/
	app.Run();

	return 0;
}