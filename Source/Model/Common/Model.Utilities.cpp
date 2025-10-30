#include "Include/Model/Common/Model.Utilities.h"

namespace Model
{
	vector<string> Tokenize(string iStr, string delims)
	{
		vector<string> tmp, out;
		if (delims.empty()) return out;

		// выделим 1й делитель
		string del = delims.substr(0, 1);
		delims = delims.substr(1, delims.size() - 1);
		size_t pos;

		// меняем все делители на 1й
		for (char ch : delims)
		{
			while ((pos = iStr.find(ch)) != string::npos)
				iStr.replace(pos, 1, del);
		}

		// разбиваем строку по 1му делителю
		std::stringstream ss(iStr);
		string s;
		while (std::getline(ss, s, del[0]))
			tmp.push_back(s);

		// пропустить пустые элементы
		for (auto& item : tmp)
			if (!item.empty())
				out.push_back(item);

		return out;
	}

}