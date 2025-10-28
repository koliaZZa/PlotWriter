#pragma once
#include <string>
#include <vector>
#include <functional>

#include <TCanvas.h>
#include <TH1F.h>
#include <TApplication.h>
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TMath.h"
#include "TF1.h"
#include "TLine.h"
#include "TLatex.h"
#include "TArrow.h"
#include "TGraphPolar.h"

#include "Include/Model/Runs/Model.Quantity.h"

using std::vector, std::function;

namespace ARP::Model
{
	// Функция для настройки осей в центре
	void setupCentralAxes(TMultiGraph* mg, std::pair<double, double> yScale = {0.0, 0.0}, std::string xname = "", std::string yname = "");

	void setupGridAxes(TMultiGraph* mg, std::string xname, std::string yname);

	TCanvas* drawMultipleGraphs();

	enum class GraphType
	{
		Balances,
		CpPhi,
		CpX
	};

	class DrawGraphs;
	using DrawGraphsPtr = std::shared_ptr<DrawGraphs>;

	class DrawGraphs
	{
	public:
		DrawGraphs() { Init(); };
		DrawGraphs(std::string ititle, std::string ixtitle, std::string iytitle, std::string ixname, std::string iyname, GraphType iGraphType = GraphType::Balances);

		std::string title = "Nazvanie_plaisholder";
		std::string xtitle = "plaisholder_x";
		std::string ytitle = "plaisholder_y";
		std::string xname = "x";
		std::string yname = "y";

		TMultiGraph* multiGraph = nullptr;
		TCanvas* canvas = nullptr;

		static DrawGraphsPtr CreateDrawGraphs(std::string ititle, std::string ixtitle, std::string iytitle, std::string ixname, std::string iyname)
		{
			DrawGraphsPtr graph = std::make_shared<DrawGraphs>();
			graph->title = ititle;
			graph->xtitle = ixtitle;
			graph->ytitle = iytitle;
			graph->xname = ixname;
			graph->yname = iyname;
			graph->multiGraph->SetTitle((ititle + ";" + ixtitle + ";" + iytitle).c_str());

			return graph;
		}

		void AddLine(Model::Quantity x, Model::Quantity y, std::string grname, bool dotted = false);

		void AddLinePolar(Model::Quantity x, Model::Quantity y, std::string grname, bool first = false, bool dotted = false);

		void DrawAndPrint(string path, std::pair<double, double> yScale = {0.0, 0.0});

		void DrawAndPrintPolar(string path)

		int GetAutoColor(int index);

		~DrawGraphs()
		{
			if (multiGraph)
				delete multiGraph;
			if (canvas)
				delete canvas;
			count--;
		}

	protected:
		void Init();

		static inline size_t count{0};

		size_t countLines{ 0 };

		GraphType graphType{GraphType::Balances};
	};
}