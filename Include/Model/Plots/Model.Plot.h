#pragma once
#include <string>
#include <vector>
#include <functional>

#include "Include/Model/Common/Model.Object.h"
#include "Include/Model/Runs/Model.RunResult.h"
#include "Include/Model/Common/Model.Functions.h"

using std::vector, std::string, std::function;

namespace ARP::Model
{
	// Модель линии графика
	class PlotLine : public Object
	{
	public:
		PlotLine() : Object() { Init(); };
		PlotLine(string iTitle, Quantity ix, Quantity iy, bool iVisibility = false, int iWidth = 2, InterpolTypeEnum iType = InterpolTypeEnum::Linear, bool isHeavy = false);
		PlotLine(const rj::Value& doc) : Object() { Init();  Load(doc); };
		~PlotLine() = default;

		// Сохранить в json-документ
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override;
		// Загрузить из json-документа
		virtual void Load(const rj::Value& doc) override;

		// Отнормировать данные
		void Normalize(bool normalize) { yQuantity.Normalize(normalize); UpdateData(); };

		// Установить тип интерполяции
		void SetInterpolType(InterpolTypeEnum iType);
		// Установить данные
		void SetData(Quantity ix, Quantity iy);
		// Установить ось X
		void SetXAxis(Quantity ix);

		// Получить тип интерполяции
		InterpolTypeEnum				GetInterpolType()		 const { return interpolType; };
		// Получить текущую интерполяционную функцию
		Model::InterpolationPtr			GetSelectedFunc()		 const { return selectedFunc; };
		// Получить линейную интерполяционную функцию
		Model::LinInterpolationPtr		GetLinInterpolFunc()	 const { return linInterpolFunc; };
		// Получить интерполяционную функцию сплайна 2го порядка
		Model::Spline2InterpolationPtr	GetSpline2InterpolFunc() const { return spline2InterpolFunc; };
		// Получить интерполяционную функцию сплайна 3го порядка
		Model::Spline3InterpolationPtr	GetSpline3InterpolFunc() const { return spline3InterpolFunc; };
		// Получить точки для отображения кривой
		virtual pair<vector<double>, vector<double>> GetDisplayPoints() const { return selectedFunc->GetDisplayPoints(); };
		// Получить точки
		virtual pair<vector<double>, vector<double>> GetPoints()		const { return { xQuantity.data, yQuantity.data }; };
		//virtual pair<vector<double>, vector<double>> GetRarePoints(size_t skip)	const;

		string GetXAxisTitle() const { return xQuantity.title; };
		string GetYAxisTitle() const { return yQuantity.title; };


		bool isTracing = false;				// Статус отслеживания
		bool isVisible = false;				// Видимость
		int width = 2;						// Толщина линий
		bool heavy = false;

	protected:
		// Инициализация
		void Init();
		// Обновить данные объектов интерполяций на основе значений в xQuantity и yQuantity
		void UpdateData();

		InterpolTypeEnum interpolType = InterpolTypeEnum::Linear;		// Тип интерполяции

		LinInterpolationPtr		linInterpolFunc;				// Линейная интерполяционная функция
		Spline2InterpolationPtr spline2InterpolFunc;			// Сплайновая 2го порядка интерполяционная функция
		Spline3InterpolationPtr spline3InterpolFunc;			// Сплайновая 3го порядка интерполяционная функция

		Quantity xQuantity;		// Значения по оси X
		Quantity yQuantity;		// Значения по оси Y

		InterpolationPtr selectedFunc = linInterpolFunc;		// Выбранная интерполяционная функция
	};
	using PlotLinePtr = std::shared_ptr<PlotLine>;

	// Модель графика
	class Plot : public Object
	{
	public:
		Plot() = default;
		Plot(string iTitle, string ixAxis, string iyAxis, int iLineWidth = 2, InterpolTypeEnum iInterpolType = InterpolTypeEnum::Linear);
		Plot(const rj::Value& doc) : Object() { Load(doc); };
		~Plot() = default;

		// Сохранить в json-документ
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override;
		// Загрузить из json-документа
		virtual void Load(const rj::Value& doc) override;

		// Отнормировать данные
		void Normalize(bool normalize);

		// Добавить линию
		PlotLinePtr AddLine(string iTitle, Quantity ix, Quantity iy, bool isHeavy = false);
		// Удалить линию
		void DeleteLine(string iLineName);
		// Установить название оси X
		void SetXAxis(string ixAxis) { xAxis = ixAxis; };
		// Установить тип интерполяции
		void SetInterpolType(InterpolTypeEnum iType);

		// Получить надпись оси X
		string GetXAxisTitle() const;
		// Получить надпись оси Y
		string GetYAxisTitle() const;
		// Получить название оси X
		string GetXAxis() const { return xAxis; };
		// Получить название оси Y
		string GetYAxis() const { return yAxis; };

		// Получить тип интеполяции
		InterpolTypeEnum GetInterpolType() const { return interpolType; };
		// Получить список линий
		const vector<PlotLinePtr> GetLines() const { return lines; };
		// Получить линию по имени
		PlotLinePtr GetLine(string iLineName);
		// Получить линию по индексу
		PlotLinePtr GetLine(size_t ind) { return lines[ind]; };

		int width = 2;						// Толщина линий

		function<bool(string, bool)>		SignalShowLine = nullptr;		// Сигнал при событии отображения линии
		function<void(Model::PlotLinePtr)>	SignalAddLine = nullptr;		// Сигнал при событии добавления линии

	protected:
		vector<PlotLinePtr> lines;											// Список линий
		InterpolTypeEnum interpolType = InterpolTypeEnum::Linear;			// Тип интерполяции

		string xAxis;		// Название оси X
		string yAxis;		// Название оси Y

	};
	using PlotPtr = std::shared_ptr<Plot>;
}