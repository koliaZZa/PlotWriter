#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <iomanip>

using std::vector, std::string, std::minmax, std::pair;

namespace Model
{
	// Разделить строку на токены по всем делителям в delims
	vector<string> Tokenize(string iStr, string delims = " \t\r");

	// Сравнить разбитую строку с эталоном
	inline bool CompareLines(vector<string> sVec, string standart)
	{
		if (!sVec.size()) return false;
		// Сборка разбитой строки с разделителем-пробелом
		string res = sVec[0];
		for (auto it = sVec.begin() + 1; it != sVec.end(); it++)
			res += " " + *it;

		return standart == res;
	}

	// Найти минимум и максимум в векторе
	template <typename T>
	pair<T, T> FindMinMaxInVector(const vector<T>& vec)
	{
		T min{}, max{};
		auto [it_min, it_max] = std::minmax_element(vec.begin(), vec.end());

		if (it_min != vec.end())
			min = *it_min;
		if (it_max != vec.end())
			max = *it_max;

		return { min, max };
	}

	template <typename T>
	inline T roundTo(T val, size_t precision)
	{
		val *= (int)pow(10, precision);
		val = round(val);
		val /= (int)pow(10, precision);
		return val;
	}

	template <typename T>
	inline T mean(vector<T>& vec, size_t precision)
	{
		return roundTo(std::accumulate(vec.begin(), vec.end(), (T)0)/vec.size(), precision);
	}

	template <typename T>
	inline string to_str(T val, size_t precision)
	{
		std::stringstream ss;
		ss << std::setprecision(precision) << val;
		return ss.str();
	}

}