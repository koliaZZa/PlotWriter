#pragma once
#include "Include/Model/Common/Model.Object.h"
#include "Include/Model/Runs/Model.RunResult.h"
#include "Include/Model/Plots/Model.PlotCollection.h"

namespace ARP::Model
{
	using CheckListItem = std::pair<string, bool>;

	// Модель сравнения результатов
	class ResultComparison : public Object
	{
	public:
		ResultComparison() = default;
		ResultComparison(const rj::Value& doc) : Object() { Load(doc); };
		~ResultComparison() = default;

		// Сохранить в json-документ
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override;
		// Загрузить из json-документа
		virtual void Load(const rj::Value& doc) override;

		// Установить модель коллекции графиков
		void SetPlotCollection(PlotCollectionPtr iPlotCollection) { plotCollection = iPlotCollection; };
		// Получить коллекцию графиков
		PlotCollectionPtr GetPlotCollection() { return plotCollection; };

		// Получить список новых величин из пуска
		vector<string> UpdateQuantitiesByLast();

		// Открыть пуск из файла
		string OpenRun(string path);

		vector<CheckListItem> quantities;   // Список величин и их статусов
		vector<CheckListItem> runNames;     // Список пусков и их статусов

	protected:
		PlotCollectionPtr plotCollection = nullptr;	// Модель коллекции графиков
	};
	using ResultComparisonPtr = std::shared_ptr<ResultComparison>;
}