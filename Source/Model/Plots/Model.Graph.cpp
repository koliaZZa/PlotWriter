#include "Include/Model/Plots/Model.Graph.h"

namespace ARP::Model
{
	int DrawGraphs::GetAutoColor(int index) {
		// Используем красивую палитру ROOT
		int palette[] = { kBlack, kRed, kBlue, kGreen, kOrange, kGray, kViolet, kCyan, 8, 9, 30, 42, 46, 38, 29 };
		return palette[index];
	}

	void ARP::Model::setupCentralAxes(TMultiGraph* mg, std::pair<double, double> yScale, std::string xname, std::string yname)
	{
		// Отключаем стандартные оси
		/*mg->GetXaxis()->SetAxisColor(0);  // Невидимые стандартные оси
		mg->GetYaxis()->SetAxisColor(0);
		mg->GetXaxis()->SetLabelColor(0);
		mg->GetYaxis()->SetLabelColor(0);
		mg->GetXaxis()->SetTickLength(0);
		mg->GetYaxis()->SetTickLength(0);*/

		// Создаем новые оси в центре
		Double_t xmin = mg->GetXaxis()->GetXmin();
		Double_t xmax = mg->GetXaxis()->GetXmax();

		Double_t ymin = yScale.first == 0.0 ? mg->GetYaxis()->GetXmin() : yScale.first;
		Double_t ymax = yScale.second == 0.0 ? mg->GetYaxis()->GetXmax() : yScale.second;
		ymin = ymin > 0.0 ? 0.0 : ymin;

		// Линия оси X (горизонтальная линия при y=0)
		TLine* xAxis = new TLine(xmin, 0, xmax, 0);
		xAxis->SetLineColor(kBlack);
		xAxis->SetLineWidth(2);
		xAxis->Draw();

		// Линия оси Y (вертикальная линия при x=0)
		TLine* yAxis = new TLine(0, ymin, 0, ymax);
		yAxis->SetLineColor(kBlack);
		yAxis->SetLineWidth(2);
		yAxis->Draw();

		// Стрелки на осях
		TArrow* xArrow = new TArrow(xmax, 0, xmax + (xmax - xmin) * 0.02, 0, 0.02, "|>");
		xArrow->SetLineColor(kBlack);
		xArrow->SetFillColor(kBlack);
		xArrow->Draw();

		TArrow* yArrow = new TArrow(0, ymax, 0, ymax + (ymax - ymin) * 0.02, 0.02, "|>");
		yArrow->SetLineColor(kBlack);
		yArrow->SetFillColor(kBlack);
		yArrow->Draw();

		// Подписи осей
		TLatex* xLabel = new TLatex(xmax + (xmax - xmin) * 0.03, -(ymax - ymin) * 0.03, xname.c_str());
		xLabel->SetTextSize(0.04);
		xLabel->SetTextAlign(22);
		xLabel->Draw();

		TLatex* yLabel = new TLatex(-(xmax - xmin) * 0.03, ymax + (ymax - ymin) * 0.03, yname.c_str());
		yLabel->SetTextSize(0.04);
		yLabel->SetTextAlign(22);
		yLabel->Draw();
	}
	void setupGridAxes(TMultiGraph* mg, std::string xname, std::string yname)
	{
		double xmin = mg->GetXaxis()->GetXmin();
		double xmax = mg->GetXaxis()->GetXmax();
		double ymin = mg->GetYaxis()->GetXmin();
		double ymax = mg->GetYaxis()->GetXmax();

		// Отключаем стандартные оси
		mg->GetXaxis()->SetAxisColor(0);  // Невидимые стандартные оси
		mg->GetYaxis()->SetAxisColor(0);
		mg->GetXaxis()->SetLabelColor(0);
		mg->GetYaxis()->SetLabelColor(0);
		mg->GetXaxis()->SetTickLength(0);
		mg->GetYaxis()->SetTickLength(0);

		// Создаем сетку

		// Вертикальные линии сетки
		{
			for (double x = 0; x <= xmax; x += 1.0) {
				TLine* vLine = new TLine(x, ymin, x, ymax);
				vLine->SetLineColor(kGray);
				vLine->SetLineStyle(3);
				vLine->Draw();

				// Подписи на оси X
				if (x != 0)
				{
					TLatex* label = new TLatex(x, 0 - (ymax - ymin) * 0.03, Form("%.0f", x));
					label->SetTextSize(0.03);
					label->SetTextAlign(21);
					label->Draw();
				}

			}
			for (double x = -1; x >= xmin; x -= 1.0) {
				TLine* vLine = new TLine(x, ymin, x, ymax);
				vLine->SetLineColor(kGray);
				vLine->SetLineStyle(3);
				vLine->Draw();

				// Подписи на оси X
				TLatex* label = new TLatex(x, 0 - (ymax - ymin) * 0.03, Form("%.0f", x));
				label->SetTextSize(0.03);
				label->SetTextAlign(21);
				label->Draw();
			}
		}


		// Горизонтальные линии сетки
		{
			for (double y = 0; y <= ymax; y += 1.0) {
				TLine* hLine = new TLine(xmin, y, xmax, y);
				hLine->SetLineColor(kGray);
				hLine->SetLineStyle(3);
				hLine->Draw();

				// Подписи на оси Y
				if (y != 0)
				{
					TLatex* label = new TLatex(0 - (xmax - xmin) * 0.03, y, Form("%.0f", y));
					label->SetTextSize(0.03);
					label->SetTextAlign(32);
					label->Draw();
				}
			}
			for (double y = -1; y >= ymin; y -= 1.0) {
				TLine* hLine = new TLine(xmin, y, xmax, y);
				hLine->SetLineColor(kGray);
				hLine->SetLineStyle(3);
				hLine->Draw();

				// Подписи на оси Y
				TLatex* label = new TLatex(0 - (xmax - xmin) * 0.03, y, Form("%.0f", y));
				label->SetTextSize(0.03);
				label->SetTextAlign(32);
				label->Draw();
			}
		}

		// Основные оси в центре
		TLine* xAxis = new TLine(xmin, 0, xmax, 0);
		xAxis->SetLineColor(kBlack);
		xAxis->SetLineWidth(2);
		xAxis->Draw();

		TLine* yAxis = new TLine(0, ymin, 0, ymax);
		yAxis->SetLineColor(kBlack);
		yAxis->SetLineWidth(2);
		yAxis->Draw();

		// Стрелки на осях
		TArrow* xArrow = new TArrow(xmax, 0, xmax + (xmax - xmin) * 0.02, 0, 0.02, "|>");
		xArrow->SetLineColor(kBlack);
		xArrow->SetFillColor(kBlack);
		xArrow->Draw();

		TArrow* yArrow = new TArrow(0, ymax, 0, ymax + (ymax - ymin) * 0.02, 0.02, "|>");
		yArrow->SetLineColor(kBlack);
		yArrow->SetFillColor(kBlack);
		yArrow->Draw();

		// Подписи осей
		TLatex* xLabel = new TLatex(xmax + (xmax - xmin) * 0.03, -(ymax - ymin) * 0.03, xname.c_str());
		xLabel->SetTextSize(0.04);
		xLabel->SetTextAlign(22);
		xLabel->Draw();

		TLatex* yLabel = new TLatex(-(xmax - xmin) * 0.03, ymax + (ymax - ymin) * 0.03, yname.c_str());
		yLabel->SetTextSize(0.04);
		yLabel->SetTextAlign(22);
		yLabel->Draw();
	}
	TCanvas* drawMultipleGraphs()
	{
		// Создаем канву для рисования
		TCanvas* canvas = new TCanvas("canvas", "", 1200, 800);

		/*canvas->SetFrameLineColor(0);
		canvas->SetBorderMode(0);
		canvas->SetFillColor(0);
		canvas->SetFrameLineColor(0);

		canvas->SetFrameBorderMode(0);
		canvas->SetTopMargin(0);
		canvas->SetBottomMargin(0);
		canvas->SetLeftMargin(0);
		canvas->SetRightMargin(0);*/

		// Создаем мультиграф для объединения нескольких графиков
		TMultiGraph* multiGraph = new TMultiGraph();

		std::string titlename = "Nazvanie_plaisholder";
		std::string xname = "plaisholder_x";
		std::string yname = "#alpha";

		// Установка названия графика
		multiGraph->SetTitle((titlename + ";;").c_str());

		//for
		while (false)
		{
			// количество точек
			const int nPoints = 1;
			// установка
			double x1[nPoints];
			double y1[nPoints];

			std::string grname = "Nazvanie_plaisholder";

			TGraph* graph = new TGraph(nPoints, x1, y1);
			// настройка толщины линии
			graph->SetLineWidth(2);
			// настройка типа маркера
			graph->SetMarkerStyle(20);
			// настройка размера маркера
			graph->SetMarkerSize(0.5);
			// установка названия графика
			graph->SetTitle(grname.c_str());

			multiGraph->Add(graph);
		}

		{
			// Данные для первого графика (синус)
			const int nPoints = 10;
			double x1[nPoints], y1[nPoints];
			for (int i = 0; i < nPoints; i++) {
				x1[i] = -i;
				y1[i] = TMath::Sin(x1[i]);
			}

			// Данные для второго графика (косинус)
			double x2[nPoints], y2[nPoints];
			for (int i = 0; i < nPoints; i++) {
				x2[i] = i;
				y2[i] = TMath::Cos(x2[i]);
			}

			// Данные для третьего графика (парабола)
			double x3[nPoints], y3[nPoints];
			for (int i = 0; i < nPoints; i++) {
				x3[i] = i;
				y3[i] = 0.1 * x3[i] * x3[i];
			}

			// Создаем графики
			TGraph* graph1 = new TGraph(nPoints, x1, y1);
			TGraph* graph2 = new TGraph(nPoints, x2, y2);
			TGraph* graph3 = new TGraph(nPoints, x3, y3);

			// Настраиваем внешний вид графиков
			//graph1->SetLineColor(kRed);
			graph1->SetLineWidth(2);
			//graph1->SetMarkerColor(kRed);
			graph1->SetMarkerStyle(20);
			graph1->SetMarkerSize(0.5);
			graph1->SetTitle("Sin(x)");

			//graph2->SetLineColor(kBlue);
			graph2->SetLineWidth(2);
			//graph2->SetMarkerColor(kBlue);
			graph2->SetMarkerStyle(21);
			graph2->SetMarkerSize(0.5);
			graph2->SetTitle("Cos(x)");

			//graph3->SetLineColor(kGreen);
			graph3->SetLineWidth(2);
			//graph3->SetMarkerColor(kGreen);
			graph3->SetMarkerStyle(22);
			graph3->SetMarkerSize(0.5);
			graph3->SetTitle("#alpha");

			// Добавляем графики в мультиграф
			multiGraph->Add(graph1);
			multiGraph->Add(graph2);
			multiGraph->Add(graph3);
		}

		// Рисуем мультиграф
		multiGraph->Draw("ACP pmc plc");  // A - оси, L - линии, C - курвы, P - точки, pmc - автоцвет точек, plc - автоцвет линий

		canvas->BuildLegend();

		// Настраиваем оси
		/*multiGraph->GetXaxis()->SetTitle("X values");
		multiGraph->GetYaxis()->SetTitle("Y values");
		multiGraph->GetXaxis()->CenterTitle(true);
		multiGraph->GetYaxis()->CenterTitle(true);*/

		setupCentralAxes(multiGraph);
		canvas->SetGrid();

		// Отрисовываем красивые декартовы координаты
		//setupGridAxes(multiGraph, xname, yname);

		// Обновляем канву
		canvas->Update();

		// Сохраняем в файл
		canvas->SaveAs("./multiple_graphs.png");
		//canvas->SaveAs("multiple_graphs.pdf");
		//canvas->SaveAs("multiple_graphs.root");

		//canvas->Close();

		return canvas;
	}
	DrawGraphs::DrawGraphs(std::string ititle, std::string ixtitle, std::string iytitle, std::string ixname, std::string iyname, GraphType iGraphType) :
		title{ ititle }, xtitle{ ixtitle }, ytitle{ iytitle }, xname{ ixname }, yname{ iyname }, graphType{iGraphType}
	{
		Init();
		multiGraph->SetTitle((ititle + ";" + ixtitle + ";" + iytitle).c_str());
	}
	void DrawGraphs::AddLine(Model::Quantity x, Model::Quantity y, std::string grname, bool dotted)
	{
		const size_t nPoints = x.data.size();
		// установка
		std::vector<double> x1(x.data.begin(), x.data.end());
		std::vector<double> y1(y.data.begin(), y.data.end());

		TGraph* graph = new TGraph(nPoints, x1.data(), y1.data());
		// настройка толщины линии
		graph->SetLineWidth(2);
		if (dotted)
			graph->SetLineStyle(9);
		// настройка типа маркера
		graph->SetMarkerStyle(20);
		// настройка размера маркера
		graph->SetMarkerSize(1);
		// настройка цвета
		graph->SetMarkerColor(GetAutoColor(countLines));
		graph->SetLineColor(GetAutoColor(countLines++));
		// установка названия графика
		graph->SetTitle(grname.c_str());

		multiGraph->Add(graph);
	}
	void DrawGraphs::AddLinePolar(Model::Quantity x, Model::Quantity y, std::string grname, bool first , bool dotted)
	{
		if (multiGraph)
			delete multiGraph;
		const size_t nPoints = x.data.size();
		// установка
		std::vector<double> x1(x.data.begin(), x.data.end());
		std::vector<double> y1(y.data.begin(), y.data.end());

		std::vector<double> x1(x.data.begin(), x.data.end());
		std::vector<double> y1(y.data.begin(), y.data.end());

		TGraphPolar* graph = new TGraphPolar(nPoints, x1.data(), y1.data());
		// настройка толщины линии
		graph->SetLineWidth(2);
		if (dotted)
			graph->SetLineStyle(9);
		// настройка типа маркера
		graph->SetMarkerStyle(20);
		// настройка размера маркера
		graph->SetMarkerSize(1);
		// настройка цвета
		graph->SetMarkerColor(GetAutoColor(countLines));
		graph->SetLineColor(GetAutoColor(countLines++));
		// установка названия графика
		graph->SetTitle(grname.c_str());

		if (first)
			graph->Draw("LP");
		else 
			graph->Draw("P SAME");
	}
	void DrawGraphs::DrawAndPrint(string path, std::pair<double, double> yScale)
	{
		// Рисуем мультиграф
		double min = yScale.first == 0.0 ? multiGraph->GetYaxis()->GetXmin() : yScale.first;
		double max = yScale.second == 0.0 ? multiGraph->GetYaxis()->GetXmax() : yScale.second;
		min = min > 0.0 ? 0.0 : min;
		max = max < 0.0 ? 0.0 : max;
		if (abs(max - min) < 0.05)
		{
			double k = 0.05 / abs(max - min);
			min *= k;
			max *= k;
		}
		multiGraph->GetYaxis()->SetRangeUser(min, max);
		multiGraph->GetXaxis()->SetNdivisions(520);

		switch (graphType)
		{
		case ARP::Model::GraphType::Balances:
			multiGraph->Draw("ACP");  // A - оси, L - линии, C - курвы, P - точки, pmc - автоцвет точек, plc - автоцвет линий
			break;
		case ARP::Model::GraphType::CpPhi:
			multiGraph->Draw("AP");
			break;
		case ARP::Model::GraphType::CpX:
			multiGraph->Draw("ACP");
			break;
		default:
			break;
		}

		if (countLines > 6)
			canvas->BuildLegend(0.6, 0.2, 0.6, 0.2)->SetNColumns(2);
		else canvas->BuildLegend();
		// Отрисовываем красивые декартовы координаты
		setupCentralAxes(multiGraph, {min, max});
		canvas->SetGrid();

		// Обновляем канву
		canvas->Update();

		// Сохраняем в файл
		path += "/" + yname + ".png";
		canvas->SaveAs(path.c_str());

		//canvas->Close();
	}
	void DrawGraphs::DrawAndPrintPolar(string path)
	{
		// Обновляем канву
		canvas->Update();

		// Сохраняем в файл
		path += "/" + yname + ".png";
		canvas->SaveAs(path.c_str());
	}
	void DrawGraphs::Init()
	{
		count++;
		multiGraph = new TMultiGraph();
		canvas = new TCanvas(("canvas" + std::to_string(count)).c_str(), "", 1200, 800);
	}
}