#include "Include/Model/Runs/Model.RunResult.h"

namespace ARP::Model
{
	RunResult::RunResult(string iFilePath, ProtocolType iProtocolType) : protocolType{ iProtocolType }
	{
		ifstream iFile(iFilePath);
		ReadFile(iFile, iProtocolType);
		iFile.close();
	}
	RunResult::RunResult(ifstream& iFile, ProtocolType iProtocolType) : protocolType{ iProtocolType }
	{
		ReadFile(iFile, iProtocolType);
	}
	rj::Document RunResult::Save(rj::MemoryPoolAllocator<>& allocator)
	{
		rj::Document doc;
		doc.SetObject();

		// Сохранение общих данных
		WriteHeader(doc, allocator);
		rju::AddBool(doc, allocator, "Is active", isActive);
		rju::AddInt(doc, allocator, "Number of points", pointsNum);

		// Сохранение физ. величин
		rj::Value arrQuantities(rj::kArrayType);
		for (auto& q : quantities)
			arrQuantities.PushBack(q.Save(allocator), allocator);
		doc.AddMember("Quantities", arrQuantities, allocator);

		return doc;
	}
	void RunResult::Load(const rj::Value& doc)
	{
		// Считывание общих данных
		ReadHeader(doc);
		bool integrity = true;
		integrity = integrity && rju::ReadBool(doc, "Is active", isActive);
		integrity = integrity && rju::ReadInt(doc, "Number of points", pointsNum);

		// Считывание физ. величин
		for (auto& item : doc["Quantities"].GetArray())
		{
			quantities.emplace_back(item);
			quantitiesNames.push_back(quantities.rbegin()->name);
		}
		if (!integrity) ErrorReporter::PushMessage(ErrorType::FileReading, "Run " + name, "Missing additional data");
	}
	bool RunResult::WriteProtocol(string iFilePath)
	{
		fs::path filePath = fs::u8path(iFilePath);
		std::ofstream iFile(filePath, std::ios::binary);
		if (!iFile)
		{
			ErrorReporter::PushMessage(ErrorType::FileSaving, "Run " + name, "Protocol file " + iFilePath + " didn't open");
			return false;
		}
		if (quantities.empty() || !pointsNum)
		{
			ErrorReporter::PushMessage(ErrorType::FileSaving, "Run " + name, "No data in run");
			return false;
		}
		unsigned char bom[] = { 0xEF,0xBB,0xBF };
		iFile.write((char*)bom, sizeof(bom));
		iFile << "Run " + name + "; protocol type - " + ProtocolTypeEnum::str(protocolType) + "; run type - " + RunTypeEnum::str(runType) + "; M_nom = " << std::setprecision(2) << MachNom.value() << "; comment - " + comment << std::endl;

		for (auto& quantity : quantities)
			iFile << std::setw(12) << std::left << quantity.title;
		iFile << std::endl;

		for (size_t i = 0; i < pointsNum; i++)
		{
			for (auto& quantity : quantities)
				iFile << std::setw(12) << std::left << std::setprecision(5) << quantity.data[i];
			iFile << std::endl;
		}

		iFile.close();
		return true;
	}
	void RunResult::CalcNominalParams(size_t precision)
	{
		switch (runType)
		{
		case RunTypeEnum::AlphaVar:
		{
			Quantity Mach = GetQuantity("Mach");
			MachNom = mean(Mach.data, precision);

			Quantity alphaNom = GetQuantity("alpha");
			for (auto& val : alphaNom.data)
				val = round(val);
			alphaNom.SetNames(u8"alpha_nom", u8"alpha_nom", u8"α_nom");	//

			auto iter = std::find_if(quantities.begin(), quantities.end(), [](Quantity& q) {return q.name == "Mach"; });
			quantities.emplace(iter, alphaNom);
			break;
		}
		case RunTypeEnum::MachVar:
		{
			Quantity Alpha = GetQuantity("alpha");
			AlphaNom = mean(Alpha.data, 0);

			Quantity MachNom = GetQuantity("Mach");
			for (auto& val : MachNom.data)
				val = roundTo(val, precision);
			MachNom.SetNames("Mach_nom", "Mach_nom", "M_ном");

			auto iter = std::find_if(quantities.begin(), quantities.end(), [](Quantity& q) {return q.name == "Mach"; });
			quantities.emplace(iter, MachNom);

			break;
		}
		default:
			break;
		}
	}
	void RunResult::ApplyGamma()
	{
		if (gamma == 0) return;
		if (gamma == 90)
		{
			swap(GetQuantityPtr("alpha"), GetQuantityPtr("beta"));
			swap(GetQuantityPtr("Cy"), GetQuantityPtr("Cz"), false, true);
			swap(GetQuantityPtr("Cy_1"), GetQuantityPtr("Cz_1"), false, true);
			swap(GetQuantityPtr("my"), GetQuantityPtr("mz"), false, true);
			swap(GetQuantityPtr("my_1"), GetQuantityPtr("mz_1"), false, true);
		}
	}
	Quantity RunResult::GetQuantity(string iName) const
	{
		for (auto& item : quantities)
			if (item.name == iName)
				return item;
		return Quantity{};
	}

	Quantity* RunResult::GetQuantityPtr(string iName)
	{
		for (auto& item : quantities)
			if (item.name == iName)
				return &item;
		return nullptr;
	}

	void RunResult::SetQuantitiesNames(vector<ChannelInfo>& ivNames)
	{
		string qName;
		for (auto& quantity : quantities)
		{
			qName = quantity.name;
			auto it = std::find_if(ivNames.begin(), ivNames.end(), [&qName](ChannelInfo& chInfo) {return qName == chInfo.extName; });
			if (it == ivNames.end()) continue;

			quantity.SetNames(qName, it->name, it->title);
		}
	}

	void RunResult::ParseTable(ifstream& iFstream)
	{
		Table table = ReadTable(iFstream);
		if (!table.size()) return;

		size_t nCol = table[0].size();
		for (size_t col = 0; col < nCol; col++)
		{
			string name = table[0][col];
			if (name == "N") continue;		// Пропускаем столбец с номером отсчета
			vector<double> data;
			quantitiesNames.push_back(name);
			for (size_t row = 1; row < table.size(); row++)
			{
				if (table[row][col] != "-")
					data.push_back(std::stof(table[row][col]));
				else data.push_back(0.0);
			}
			quantities.emplace_back(name, std::move(data));
		}
	}

	void RunResult::ReadT117Protocol(ifstream& iFile)
	{
		string line;
		vector<string> vTmp;
		// Построчное чтение файла
		while (std::getline(iFile, line))
		{
			vTmp = Tokenize(line);
			if (name.empty() && (vTmp.size() >= 2))
			{
				// Сохраняем номер протокола как название пуска
				if (vTmp[0] == "Протокол:")
					name = vTmp[1];
			}
			if (CompareLines(vTmp, "ПАРАМЕТРЫ ПОТОКА"))					// Считывание таблицы с параметрами потока
				ParseTable(iFile);
			if (CompareLines(vTmp, "АЭРОДИНАМИЧЕСКИЕ КОЭФФИЦИЕНТЫ"))	// Считывание таблицы с аэродинамическими коэффициентами
				ParseTable(iFile);
		}
	}

	void RunResult::ReadUnifiedProtocol(ifstream& iFile)
	{
		string line;
		std::getline(iFile, line);
		vector<string> vTmp = Tokenize(line);
		size_t i = 0;
		while ((vTmp[i] != u8"Пуск") && (vTmp[i] != u8"Расчёт"))
		{
			i++;
			if (i >= vTmp.size())
				break;
		}
		// Сохраняем номер протокола как название пуска
		name = vTmp[i + 1].substr(0, vTmp[i + 1].size() - 1);

		// Выставляем тип источника данных
		if (vTmp[0] == u8"Пуск") expType = ExperimentType::WindTunnelTest;
		else if (vTmp[0] == u8"Расчёт") expType = ExperimentType::CFD;

		ParseTable(iFile);		// Считывание таблицы с данными
	}

	void RunResult::ReadFile(ifstream& iFile, ProtocolType iProtocolType)
	{
		protocolType = iProtocolType;
		if (iFile)
		{
			switch (iProtocolType)
			{
			case ProtocolTypeEnum::T117:
			{
				ReadT117Protocol(iFile);
				break;
			}
			case ProtocolTypeEnum::Unified:
			{
				ReadUnifiedProtocol(iFile);
				break;
			}
			default:
				break;
			}
		}
		else ErrorReporter::PushMessage(ErrorType::FileReading, "RunResult " + name, "File didn't open");
		if (quantities.size())
			pointsNum = quantities.begin()->GetSize();
		else ErrorReporter::PushMessage(ErrorType::FileReading, "RunResult " + name, "No data read");
	}

	void RunResult::FormTitle(string locale)
	{
		if (locale == "rus")
		{
			switch (expType)
			{
			case ARP::Model::ExperimentType::WindTunnelTest:
			{
				title = u8"Пуск " + name + ", Re - " + ReStatusEnum::str(reStatus) + u8", φ=" + std::to_string(gamma) + u8"°";
				break;
			}
			case ARP::Model::ExperimentType::CFD:
			{
				title = "CFD, " + string(u8", φ=") + std::to_string(gamma) + u8"°";
				break;
			}
			default:
				break;
			}
		}
		else if (locale == "eng")
		{
			switch (expType)
			{
			case ARP::Model::ExperimentType::WindTunnelTest:
			{
				title = "Run " + name + ", Re - " + ReStatusEnum::str(reStatus) + u8", φ=" + std::to_string(gamma) + u8"°";
				break;
			}
			case ARP::Model::ExperimentType::CFD:
			{
				title = "CFD, " + string(u8", φ=") + std::to_string(gamma) + u8"°";
				break;
			}
			default:
				break;
			}
		}
	}

	void RunResult::FormLatexTitle(bool useRe, string locale)
	{
		if (locale == "rus")
		{
			switch (expType)
			{
			case ARP::Model::ExperimentType::WindTunnelTest:
			{
				switch (runType)
				{
				case RunTypeEnum::AlphaVar:
				{
					title = u8"\\hbox{Пуск }" + name + (useRe ? ", Re - " + ReStatusEnum::str(reStatus) : "") + ", \\phi=" + std::to_string(gamma) + "^{\\circ}";
					break;
				}
				case RunTypeEnum::MachVar:
				{
					title = u8"\\hbox{Пуск }" + name + (useRe ? ", Re - " + ReStatusEnum::str(reStatus) : "") + ", \\alpha=" + to_str(AlphaNom.value(), 0) + "^{\\circ}" + ", \\phi=" + std::to_string(gamma) + "^{\\circ}";
					break;
				}
				default: break;
				}
				break;
			}
			case ARP::Model::ExperimentType::CFD:
			{
				title = "CFD, " + string("\\phi=") + std::to_string(gamma) + "^{\\circ}";
				break;
			}
			default:
				break;
			}
		}
		else if (locale == "eng")
		{
			switch (expType)
			{
			case ARP::Model::ExperimentType::WindTunnelTest:
			{
				title = "Run " + name + (useRe ? ", Re - " + ReStatusEnum::str(reStatus) : "") + ", #phi=" + std::to_string(gamma) + "#circ";
				break;
			}
			case ARP::Model::ExperimentType::CFD:
			{
				title = "CFD, " + string(", #phi=") + std::to_string(gamma) + "#circ";
				break;
			}
			default:
				break;
			}
		}

	}

	Table ReadTable(ifstream& iFstream)
	{
		Table res;
		size_t nCols = 0;		// Кол-во эл-тов в предыдущей строке

		string line;
		vector<string> vTmp;
		while (std::getline(iFstream, line))
		{
			vTmp = Tokenize(line);
			if (vTmp.empty()) break;

			if (nCols && (vTmp.size() != nCols)) break;		// Если кол-во эл-тов в строке не совпадает с предыдущей строкой, выход
			else nCols = vTmp.size();

			res.emplace_back(vTmp.begin(), vTmp.end());
		};

		return res;
	}

}