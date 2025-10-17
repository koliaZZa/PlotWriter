#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <functional>
#include <filesystem>

#include "Include/Model/Common/Model.Utilities.h"
#include "Include/Model/Common/Model.Object.h"
#include "Include/Model/Runs/Model.Quantity.h"

using std::vector, std::string, std::optional, std::function;
using std::ifstream;
namespace fs = std::filesystem;

namespace ARP::Model
{
	class RunSignals : public Object
	{
	public:
		RunSignals() = default;
		RunSignals(string iFilePath, double iFreq, function<void(size_t)> iProgressFunc);
		RunSignals(const rj::Value& doc) : Object() { Load(doc); };

		// Сохранить в json-документ
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override;
		// Загрузить из json-документа
		virtual void Load(const rj::Value& doc) override;

		// Проверка на пустоту
		bool IsEmpty() const { return !signals.size(); };

		// Получить i-ый сигнал
		optional<Quantity> GetSignal(size_t i) const;
		optional<Quantity> GetSignal() const;
		Quantity GetTimeQuantity() const { return time; };

		function<void(size_t)> SignalOnProgress = nullptr;		// Сигнал при изменении прогресса чтения файла

	protected:
		void FormTimeQuantity();

		vector<Quantity> signals;
		Quantity time;
		mutable size_t currInd = 0;
		double frequency = 100;
	};
	using RunSignalsPtr = std::shared_ptr<RunSignals>;
}