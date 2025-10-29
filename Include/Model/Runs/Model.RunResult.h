#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <memory>
#include <optional>
#include <numeric>
#include <iomanip>
#include <filesystem>

#include "Include/Model/Common/Model.Utilities.h"
#include "Include/Model/Common/Model.Object.h"
#include "Include/Model/Runs/Model.Quantity.h"

namespace fs = std::filesystem;

using std::vector, std::string, std::pair;
using std::ifstream, std::ofstream;
using std::shared_ptr;

namespace ARP::Model
{
	typedef int ProtocolType;
	typedef int RunType;
	typedef int ReStatus;

	enum class ExperimentType
	{
		WindTunnelTest,
		CFD
	};

	struct ProtocolTypeEnum
	{
		static const ProtocolType Unified = 0;
		static const ProtocolType T109 = 1;
		static const ProtocolType T117 = 2;
		static const ProtocolType T128 = 3;

		static ProtocolType enumer(string s)
		{
			if (s == "Unified") return Unified;
			else if (s == "T109") return T109;
			else if (s == "T117") return T117;
			else if (s == "T128") return T128;
			else return T117;
		}

		static string str(ProtocolType enumer)
		{
			switch (enumer)
			{
			case Unified:
				return "Unified";
				break;
			case T109:
				return "T109";
				break;
			case T117:
				return "T117";
				break;
			case T128:
				return "T128";
				break;
			default:
				return "T117";
				break;
			}
		}
	};

	struct RunTypeEnum
	{
		static const RunType AlphaVar = 1;
		static const RunType MachVar = 2;

		static RunType enumer(string s)
		{
			if (s == "alpha") return AlphaVar;
			else if (s == "Mach") return MachVar;
			else return AlphaVar;
		}

		static string str(RunType enumer)
		{
			switch (enumer)
			{
			case AlphaVar:
				return "alpha";
				break;
			case MachVar:
				return "Mach";
				break;
			default:
				return "alpha";
				break;
			}
		}
	};

	struct ReStatusEnum
	{
		static const ReStatus Nom = 1;
		static const ReStatus Max = 2;

		static ReStatus enumer(string s)
		{
			if (s == "nom") return Nom;
			else if (s == "max") return Max;
			else return Nom;
		}

		static string str(ReStatus enumer)
		{
			switch (enumer)
			{
			case Nom:
				return "nom";
				break;
			case Max:
				return "max";
				break;
			default:
				return "nom";
				break;
			}
		}
	};

	struct RunInfo
	{
		string name{};
		string wtunnel{};
		size_t number{};
		RunType runType = RunTypeEnum::AlphaVar;
		ReStatus reStatus = ReStatusEnum::Nom;
		double machNom = 0.0;
		int gamma = 0;
		string comment{};
	};

	struct ChannelInfo
	{
		string extName{};
		string name{};
		string title{};
	};

	// Набор координат XY
	using XYarray = pair<vector<double>, vector<double>>;
	// Двумерный массив
	using Table = vector<vector<string>>;

	// Считать двумерный массив из файлового потока
	Table ReadTable(ifstream& iFstream);


	// Класс результатов пуска
	class RunResult : public Object
	{
	public:
		RunResult() = default;
		RunResult(string iFilePath, ProtocolType iProtocolType = ProtocolTypeEnum::Unified);
		RunResult(ifstream& iFile, ProtocolType iProtocolType = ProtocolTypeEnum::Unified);
		RunResult(const rj::Value& doc) : Object() { Load(doc); };

		// Сохранить в json-документ
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override;
		// Загрузить из json-документа
		virtual void Load(const rj::Value& doc) override;
		// Записать протокол в файл
		bool WriteProtocol(string iFilePath);

		// Посчитать номинальные параметры
		void CalcNominalParams(size_t precision);

		void ApplyGamma();

		void FormTitle(string locale = "rus");
		void FormLatexTitle(bool useRe = false, string locale = "rus");

		// Получить величину по названию
		Quantity GetQuantity(string iName) const;
		// Получить ссылку на величину по названию
		Quantity* GetQuantityPtr(string iName);
		// Получить названия величин
		vector<string> GetQuantitiesNames() const { return quantitiesNames; };
		// Получить название пуска
		string GetName() const { return name; };

		ExperimentType GetExperimentType() const { return expType; };

		std::optional<double> GetMachNom() const { return MachNom; };
		std::optional<double> GetAlphaNom() const { return AlphaNom; };

		// Установить названия физ. величин
		void SetQuantitiesNames(vector<ChannelInfo>& ivNames);
		// Есть ли данные
		bool IsEmpty() const { return !pointsNum; };
		// Статус отображения
		bool isActive = false;

		ProtocolType protocolType = ProtocolTypeEnum::T117;
		RunType runType = RunTypeEnum::AlphaVar;
		ReStatus reStatus = ReStatusEnum::Nom;
		int gamma = 0;

	protected:
		size_t pointsNum = 0;				// Кол-во отсчетов
		vector<Quantity> quantities;		// Список величин
		vector<string> quantitiesNames;		// Список названий величин

		std::optional<double> MachNom = std::nullopt;
		std::optional<double> AlphaNom = std::nullopt;

		ExperimentType expType = ExperimentType::WindTunnelTest;

		void ParseTable(ifstream& iFstream);	// Прочитать таблицу величин из файлового потока

		void ReadT117Protocol(ifstream& iFile);
		void ReadUnifiedProtocol(ifstream& iFile);

		void ReadFile(ifstream& iFile, ProtocolType iProtocolType = ProtocolTypeEnum::T128);
	};
	using RunResultPtr = std::shared_ptr<RunResult>;
}
