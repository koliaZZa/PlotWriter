#include "Include/Model/NewRun.h"

NewRun::NewRun(std::string iFilePath, std::string iFilePath_Cords)
{
	std::ifstream iFile(iFilePath);
	Table table = ARP::Model::ReadTable(iFile);
	iFile.close();

	std::ifstream iFile_Cords(iFilePath_Cords);
	std::getline(iFile_Cords, std::string());
	Table cords_table = ARP::Model::ReadTable(iFile_Cords);
	iFile_Cords.close();


	for (size_t col = 1; col < table[0].size(); col++)
		colNames.push_back(table[0][col]);

	for (size_t row = 1; row < table.size(); row++)
	{
		string name = table[row][0];
		if (name == "N") continue;		// Пропускаем столбец с номером отсчета
		vector<double> data;
		quantitiesNames.push_back(name);
		for (size_t col = 1; col < table[1].size(); col++)
		{
			if (table[row][col] != "-")
				data.push_back(std::stof(table[row][col]));
			else data.push_back(0.0);
		}
		quantities.emplace_back(name, std::move(data));
	}

	for (size_t row = 0; row < cords_table.size(); row++)
	{
		string name = cords_table[row][0];
		if (name == "N") continue;		// Пропускаем столбец с номером отсчета
		vector<double> data;
		for (size_t col = 1; col < cords_table[0].size(); col++)
		{
			if (cords_table[row][col] != "-")
				data.push_back(std::stof(cords_table[row][col]));
			else data.push_back(0.0);
		}
		cord_table.emplace_back(name, std::move(data));
	}

}

std::vector<double> NewRun::GetAllPfi()
{
	vector<double> data;
	for (auto quantity : cord_table)
	{
		bool flag = true;
		double phi = ARP::Model::roundTo(quantity.data[1], 2);
		for (auto item : data)
			if (phi == ARP::Model::roundTo(item, 2))
			{
				flag = false;
				break;
			}

		if (flag)
			data.push_back(phi);

	}
	return data;
}

std::vector<double> NewRun::GetAllX()
{
	vector<double> data;
	for (auto quantity : cord_table)
	{
		bool flag = true;
		double x = ARP::Model::roundTo(quantity.data[0], 5);
		for (auto item : data)
			if (x == ARP::Model::roundTo(item, 5))
			{
				flag = false;
				break;
			}

		if (flag)
			data.push_back(x);

	}
	return data;
}

std::pair<ARP::Model::Quantity, ARP::Model::Quantity> NewRun::GetGraphDataForPhiConst(double Pfi, size_t i)
{
	vector<double> x_data;
	vector<double> y_data;
	
	for (size_t col = 0; col < cord_table.size(); col++)
	{
		size_t j = col + 8;
		auto Pfi_cheak = ARP::Model::roundTo(cord_table[col].data[1], 2);
		if (Pfi == ARP::Model::roundTo(cord_table[col].data[1], 2))
		{
			x_data.push_back(cord_table[col].data[0]);
			y_data.push_back(quantities[j].data[i]);
		}
	}

	ARP::Model::Quantity x { "", std::move(x_data)};
	ARP::Model::Quantity y { "", std::move(y_data)};
	
	return std::pair<ARP::Model::Quantity, ARP::Model::Quantity> {x, y};
}

std::tuple<ARP::Model::Quantity, ARP::Model::Quantity, std::string> NewRun::GetGraphDataForXConst(size_t x_point, size_t start, size_t end)
{
	vector<double> x_data;
	vector<double> y_data;
	string name = quantities[x_point].name;

	for (size_t i = start; i < end; i++)
	{
		x_data.push_back(quantities[7].data[i]);
		y_data.push_back(quantities[x_point].data[i]);
	}

	ARP::Model::Quantity qx{ "", std::move(x_data) };
	ARP::Model::Quantity qy{ "", std::move(y_data) };

	return  std::tuple<ARP::Model::Quantity, ARP::Model::Quantity, std::string> {qx, qy, name};
}

std::tuple<ARP::Model::Quantity, ARP::Model::Quantity, std::string> NewRun::GetGraphDataForXConstPolar(double x, size_t col)
{
	vector<double> x_data;
	vector<double> y_data;
	string name ;

	for (size_t i = 0; i < cord_table.size(); i++)
	{
		size_t j = i + 8;
		if (x == ARP::Model::roundTo(cord_table[i].data[0], 5))
		{
			x_data.push_back(cord_table[i].data[1] * TMath::Pi() / 180.0);
			y_data.push_back(quantities[j].data[col]);
		}
	}

	ARP::Model::Quantity qx{ "", std::move(x_data) };
	ARP::Model::Quantity qy{ "", std::move(y_data) };

	return  std::tuple<ARP::Model::Quantity, ARP::Model::Quantity, std::string> {qx, qy, name};
}