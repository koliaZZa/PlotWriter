#pragma once
#include <string>
#include <vector>
#include <memory>

#include "Include/Model/Common/Model.Object.h"

using std::vector, std::string;


enum class SignalStatus
{
	// Нормальный
	Normal,
	// Нулевой уровень
	Zero,
	// Зашкал
	Exceeded
};

namespace ARP::Model
{
	// Класс физической величины
	class Quantity : public Object
	{
	public:
		Quantity() = default;
		Quantity(string iName, vector<double>&& iData);
		Quantity(const rj::Value& doc) : Object() { Load(doc); };
		~Quantity() = default;

		// Сохранить в json-документ
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override;
		// Загрузить из json-документа
		virtual void Load(const rj::Value& doc) override;

		// Отнормировать данные
		void Normalize(bool normalize);
		// Установить имена и заголовок
		void SetNames(string iextName, string iName, string iTitle);

		vector<double> data;							// Список значений
		double range = 0;								// Диапазон данных
		SignalStatus status = SignalStatus::Normal;		// Статус сигнала

		// Получить кол-во значений
		size_t GetSize() const { return data.size(); };
		// Проверка на пустоту
		bool IsEmpty() const { return data.empty(); };

	protected:

	};
	using QuantityPtr = std::shared_ptr<Quantity>;

	inline bool swap(Quantity* q1, Quantity* q2, bool inverseFirst = false, bool inverseSecond = false)
	{
		if (q1->GetSize() != q2->GetSize())
			return false;

		Quantity tmp{ *q2 };

		q2->comment = q1->comment;
		q2->extName = q1->extName;
		q2->name = q1->name;
		q2->title = q1->title;

		q1->comment = tmp.comment;
		q1->extName = tmp.extName;
		q1->name = tmp.name;
		q1->title = tmp.title;

		auto applyInverse = [](Quantity* q)
			{
				for (auto& value : q->data)
					value = -value;
			};

		if (inverseFirst)
			applyInverse(q1);
		if (inverseSecond)
			applyInverse(q2);

		return true;
	}
}