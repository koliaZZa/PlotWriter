#pragma once
#include "Include/Model/Common/Model.Object.h"
#include "Include/Model/Common/Model.Functions.h"
#include "Include/Model/Runs/Model.RunResult.h"
#include "Include/Model/Plots/Model.Graph.h"

namespace ARP::Model
{

	// Модель коллекции графиков
	class PlotCollection : public Object
	{
	public:
		PlotCollection() : Object("Plot collection", "Plot collection") {};
		PlotCollection(int iLineWidth) : Object("Plot collection", "Plot collection") { lineWidth = iLineWidth; };
		PlotCollection(const rj::Value& doc) : Object("Plot collection", "Plot collection") { Load(doc); };
		~PlotCollection() = default;

		// Сохранить в json-документ
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override { return rj::Document(); };
		// Загрузить из json-документа
		virtual void Load(const rj::Value& doc) override {};

		// Открыть пуск из файла
		string OpenRun(string path, ProtocolType protocolType = ProtocolTypeEnum::T117);
		// Открыть пуски из одного файла
		void OpenRunsFromFile(string path, ProtocolType protocolType = ProtocolTypeEnum::T117);
		// Добавить существующий пуск в коллекцию
		void AddRun(RunResultPtr ipRun);
		// Прочитать расшифровку каналов из файла
		void ReadChannels(string path);
		// Прочитать список комментариев к пускам
		void ReadRunsList(string path);
		// Удалить пуск
		void DeleteRun(string iRunName);
		// Переместить график в списке выше
		void MovePlotUp(size_t index);
		// Переместить график в списке ниже
		void MovePlotDown(size_t index);

		// Обработать пуски
		void ProcessRuns();

		void WriteRuns();

		// Создать модель графика
		DrawGraphsPtr CreatePlot(string iTitle, string iyAxisName);
		// Установить тип интерполяции
		void SetInterpolType(InterpolTypeEnum iType) { interpolType = iType; };
		// Установить ось X
		void SetXAxis(string ixAxis);

		// Получить название оси X
		string GetXAxis() const { return xAxisName; };
		// Получить последний пуск в списке
		const RunResultPtr GetLastRun() const { return runs.empty() ? nullptr : runs.back(); }
		// Получить список пусков
		const vector<RunResultPtr>& GetRuns() const { return runs; };
		// Получить пуск по имени
		RunResultPtr GetRun(string iRunName) const;
		// Получить толщину линий
		int GetLineWidth() const { return lineWidth; };
		// Получить тип интерполяции
		InterpolTypeEnum GetInterpolType() const { return interpolType; };
		// Получить модели графиков
		const vector<DrawGraphsPtr>& GetPlots() const { return plots; };

	protected:
		// Добавить пуск на график
		void AddRunOnPlot(RunResultPtr iRun, DrawGraphsPtr iPlot);

		vector<RunResultPtr> runs;				// Список пусков
		vector<DrawGraphsPtr> plots;					// Список графиков
		vector<ChannelInfo> channels;			// Расшифровка каналов
		vector<RunInfo> runsComments;			// Комментарии к пускам

		string xAxisName;					// Название величины на оси X
		int lineWidth = 2;					// Ширина линий графиков
		InterpolTypeEnum interpolType = InterpolTypeEnum::Linear;			// Тип интерполяции

	};
	using PlotCollectionPtr = std::shared_ptr<PlotCollection>;
}