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

#include "Include/Model/Runs/Model.Quantity.h"

using std::vector, std::function;

namespace ARP::Model
{
	// Функция для настройки осей в центре
	void setupCentralAxes(TMultiGraph* mg, std::pair<double, double> yScale = {0.0, 0.0}, std::string xname = "", std::string yname = "");

	void setupGridAxes(TMultiGraph* mg, std::string xname, std::string yname);

	TCanvas* drawMultipleGraphs();

	class DrawGraphs;
	using DrawGraphsPtr = std::shared_ptr<DrawGraphs>;

	class DrawGraphs
	{
	public:
		DrawGraphs() { Init(); };
		DrawGraphs(std::string ititle, std::string ixtitle, std::string iytitle, std::string ixname, std::string iyname);

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

		void AddLine(Model::Quantity x, Model::Quantity y, std::string grname);

		void DrawAndPrint(string path, std::pair<double, double> yScale = {0.0, 0.0});

		int GetAutoColor(int index);

		~DrawGraphs()
		{
			delete multiGraph;
			delete canvas;
			count--;
		}

	protected:
		void Init();

		static inline size_t count{0};

		size_t countLines{ 0 };
	};
}