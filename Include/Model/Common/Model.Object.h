#pragma once
#include <string>
#include <vector>

#include "Model.RJUtilities.h"
#include "Model.ErrorReporter.h"

namespace rj = rapidjson;
namespace rju = rapidjson::utils;
using std::vector, std::string;

namespace ARP::Model
{
	// Абстрактный класс модели объекта
	class Object
	{
	public:
		Object() = default;
		Object(string iTitle) { title = iTitle; };
		Object(string iTitle, string iName) { title = iTitle; name = iName; };
		virtual ~Object() = default;

		string title;	// Заголовок объекта
		string name;	// Внутреннее имя объекта
		string extName;	// Внешнее имя объекта
		string comment;	// Комментарий к объекту
		
		// Сохранить в json-документ
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) = 0;
		// Загрузить из json-документа
		virtual void Load(const rj::Value& doc) = 0;
		// Записать заголовок объекта
		void WriteHeader(rj::Document& doc, rj::MemoryPoolAllocator<>& allocator) const;
		// Считать заголовок объекта
		void ReadHeader(const rj::Value& doc);
	protected:

	};
}