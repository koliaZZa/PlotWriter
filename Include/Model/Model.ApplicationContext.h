#pragma once
#include <vector>
#include <string>
#include <fstream>

#include "Include/Model/Common/Model.Object.h"
#include "Include/Model/Model.Document.h"

using std::vector, std::string;

namespace ARP::Model
{
	// Класс контекста приложения
	class ApplicationContext : public Object
	{
	public:
		ApplicationContext(string iPath = "ARP.Settings.json");
		~ApplicationContext() = default;

		// Сохранить настройки в файл
		bool Save(string iPath = "ARP.Settings.json");

		// Сохранить в json-документ
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override;
		// Загрузить из json-документа
		virtual void Load(const rj::Value& doc) override;

		// Добавить файл в список последних
		bool AddRecentFile(string iPath);
		// Получить список последних файлов
		vector<string> GetRecentFiles() const { return recentProjects; };
		// Получить файл из списка последних по индексу
		string GetRecentFile(int ind) const { return recentProjects[ind]; };

		// Добавить проект с список открытых документов
		void AddDoc(DocumentPtr iDoc) { openDocs.push_back(iDoc); };
		// Удалить проект из списка открытых
		void DelDoc(DocumentPtr iDoc);

	protected:
		vector<string> recentProjects;		// Список последних файлов
		vector<DocumentPtr> openDocs;		// Список открытых документов
	};
	using ApplicationContextPtr = std::shared_ptr<ApplicationContext>;
}