#pragma once
#include "Include/Model/Common/Model.Object.h"
#include "Include/Model/Model.SignalProcessing.h"
#include "Include/Model/Model.ResultComparison.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace ARP::Model
{
	// Модель проекта
	class Document : public Object
	{
	public:
		Document() = default;
		Document(string iPath);
		~Document() = default;

		// Сохранить в файл по пути
		virtual void SaveToFile(string iPath);

		// Сохранить в json-документ
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override;
		// Загрузить из json-документа
		virtual void Load(const rj::Value& doc) override;

		string path;	// Путь к файлу

		// Добавить модель сравнений результатов
		void AddResultComparison(ResultComparisonPtr pResComp) { resComps.push_back(pResComp); };
		// Установить модель процессора сигналов
		void SetSignalProc(SignalProcessingPtr iSignalProc) { signalProc = iSignalProc; };

		// Получить модели сравнения результатов
		vector<ResultComparisonPtr> GetResultComparisons() const { return resComps; };
		// Получить модель процессора сигналов
		SignalProcessingPtr GetSignalProc() const { return signalProc; };

	protected:
		SignalProcessingPtr signalProc = nullptr;	// Модель процессора сигналов
		vector<ResultComparisonPtr> resComps;		// Модели сравнения результатов
	};
	using DocumentPtr = std::shared_ptr<Document>;
}