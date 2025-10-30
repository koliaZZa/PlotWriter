#include "Include/Model/Runs/Model.CFDPressureData.h"

namespace Model
{
	CFDPressureRun::CFDPressureRun(std::string iRunPath)
	{
		vector<string> vTmp = Tokenize(iRunPath, ", =");
		mach = std::stod(vTmp[4]);
		gamma = std::stoi(Tokenize(vTmp[6], ".")[0]);

		ifstream iFile(iRunPath);
		if (iFile)
		{
			Table table = ReadTable(iFile);
			if (table.size())
			{
				size_t nCol = table[0].size();
				for (size_t col = 0; col < nCol; col += 2)
				{
					vector<string> vTmp1 = Tokenize(table[0][col + 1], ",=");
					double alpha = std::stod(vTmp1[5]);
					double phi = std::stod(vTmp1[7]);
					Quantity x, y;
					for (size_t row = 1; row < table.size(); row++)
					{
						if (table[row][col] != "-")
						{
							x.data.push_back(std::stof(table[row][col]));
							y.data.push_back(std::stof(table[row][col + 1]));
						}
						else break;
					}
					vLines.emplace_back(x, y,alpha, phi);
				}
			}
		}
	}

	vector<QuantityLine> CFDPressureRun::GetLines(double iPhi)
	{
		vector<QuantityLine> lines;
		auto iter = vLines.begin();

		while (iter != vLines.end())
		{
			iter = std::find_if(iter, vLines.end(), [&iPhi](QuantityLine& line) {return line.param == iPhi; });
			if (iter != vLines.end())
				lines.push_back(*iter++);
			else break;
		}
		return lines;
	}

	CFDPressureData::CFDPressureData(std::string iDataPath)
	{
		const fs::path dir{ iDataPath };
		for (const auto& entry : fs::directory_iterator{ dir })
		{
			try
			{
				runs.push_back(std::make_shared<CFDPressureRun>(entry.path().string()));
			}
			catch (const std::exception& e)
			{
				ErrorReporter::PushMessage(ErrorType::FileReading, "CFDPressureData", "Error accessing " + entry.path().string() + ": " + e.what());
			}
		}
	}

	vector<QuantityLine> CFDPressureData::GetRunLines(double mach, int gamma, double phi)
	{
		vector<QuantityLine> lines;
		auto iter = std::find_if(runs.begin(), runs.end(), [&mach, &gamma, &phi](CFDPressureRunPtr run) {return (run->GetMach() == mach && run->GetGamma() == gamma); });
		if (iter != runs.end())
			return (*iter)->GetLines(phi);

		return vector<QuantityLine>();
	}



}