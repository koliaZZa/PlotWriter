#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

#include "Include/Model/Common/Model.Utilities.h"
#include "Include/Model/Common/Model.Object.h"
#include "Include/Model/Runs/Model.Quantity.h"
#include "Include/Model/Runs/Model.RunResult.h"
#include "Include/Model/Plots/Model.Graph.h"

namespace fs = std::filesystem;

using Table = std::vector<std::vector<std::string>>;

class NewRun
{
public:
	NewRun() = default;
	NewRun(std::string iFilePath, std::string iFilePath_Cords);

	std::vector<double> GetAllPfi();

	std::vector<double> GetAllX();

	std::pair<Model::Quantity, Model::Quantity> GetGraphDataForPhiConst(double Pfi, size_t i);
	std::tuple<Model::Quantity, Model::Quantity, std::string> GetGraphDataForXConst(size_t x_point, size_t start, size_t end);
	std::tuple<Model::Quantity, Model::Quantity, std::string> GetGraphDataForXConstPolar(double x, size_t col);

	//std::pair<Model::QuantityOpt, Model::QuantityOpt> GetGraphDataForPhiConst(double Pfi, size_t i);

	std::vector<Model::QuantityOpt> quantities;
	std::vector<std::string> quantitiesNames;
	std::vector<std::string> colNames;
	std::vector<Model::Quantity> cord_table;

};