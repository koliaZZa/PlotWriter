#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <filesystem>

#include "Include/Model/Common/Model.Utilities.h"
#include "Include/Model/Runs/Model.Quantity.h"
#include "Include/Model/Runs/Model.RunResult.h"
#include "Include/Model/Common/Model.ErrorReporter.h"

namespace fs = std::filesystem;
using std::string, std::vector, std::ifstream;

namespace Model
{
	class QuantityLine
	{
	public:
		QuantityLine() = default;
		QuantityLine(Quantity ix, Quantity iy, double iAlpha, double iParam) : x{ ix }, y{ iy }, alpha{ iAlpha }, param { iParam } {};
		~QuantityLine() = default;

		Quantity x;
		Quantity y;
		double alpha;
		double param;
	};

	class CFDPressureRun
	{
	public:
		CFDPressureRun() = default;
		CFDPressureRun(string iRunPath);
		~CFDPressureRun() = default;

		string GetName() const { return name; };
		double GetMach() const { return mach; };
		int GetGamma() const { return gamma; };

		vector<QuantityLine> GetLines(double iPhi);

	protected:
		string name = "";
		double mach = 0.0;
		int gamma = 0.0;

		vector<QuantityLine> vLines;
	};
	using CFDPressureRunPtr = std::shared_ptr<CFDPressureRun>;

	class CFDPressureData
	{
	public:
		CFDPressureData() = default;
		CFDPressureData(string iDataPath);
		~CFDPressureData() = default;

		vector<QuantityLine> GetRunLines(double mach, int gamma, double phi);

	protected:
		vector<CFDPressureRunPtr> runs;
	};
}