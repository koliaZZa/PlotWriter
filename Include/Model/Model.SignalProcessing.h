#pragma once
#include "Include/Model/Common/Model.Object.h"
#include "Include/Model/Plots/Model.Plot.h"
#include "Include/Model/Runs/Model.RunSignals.h"

namespace ARP::Model
{
	using CheckListItem = std::pair<string, bool>;

	// Модель процессора сигналов
	class SignalProcessing : Object
	{
	public:
		SignalProcessing() : Object("Signal processing", "Signal processing") {};
		SignalProcessing(const rj::Value& doc) : Object("Signal processing", "Signal processing") { Load(doc); };
		~SignalProcessing() = default;

		// Сохранить в json-документ
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override;
		// Загрузить из json-документа
		virtual void Load(const rj::Value& doc) override;

		bool OpenRunSignals(string iPath);

		// Установить тип интерполяции
		void SetInterpolType(InterpolTypeEnum iType) { interpolType = iType; };
		// Установить модель графика
		void SetPlot(PlotPtr iPlot) { plot = iPlot; };

		// Получить тип интерполяции
		InterpolTypeEnum GetInterpolType() const { return interpolType; };

		PlotPtr GetPlot() const { return plot; };

		RunSignalsPtr GetRunSignals() const { return runSignals; };


		function<void(size_t)> SignalOnProgress = nullptr;		// Сигнал при изменении прогресса чтения файла

	private:
		string path = "";
		vector<CheckListItem> channels;
		RunSignalsPtr runSignals = nullptr;
		PlotPtr plot = nullptr;
		InterpolTypeEnum interpolType = InterpolTypeEnum::Linear;			// Тип интерполяции

	};
	using SignalProcessingPtr = std::shared_ptr<SignalProcessing>;

}
