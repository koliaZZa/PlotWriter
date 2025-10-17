#pragma once
#include <vector>
#include <array>
#include <optional>
#include <variant>

#include "Include/Model/Common/Model.Utilities.h"

using std::vector, std::pair, std::optional, std::array;

namespace ARP::Model
{
	// Матрица 3x3
	using matrix3 = array<array<double, 3>, 3>;
	// Матрица 4x4
	using matrix4 = array<array<double, 4>, 4>;
	// Вектор размером 3
	using vec3 = array<double, 3>;
	// Вектор размером 4
	using vec4 = array<double, 4>;
	// Детерминант матрицы 3x3
	double matrix_determ(const matrix3& m);
	// Детерминант матрицы 4x4
	double matrix_determ(const matrix4& m);
	// Решение СЛАУ методом Крамера
	optional<vec3> cramer(const matrix3& a, const vec3& b);
	// Решение СЛАУ методом Крамера
	optional<vec4> cramer(const matrix4& a, const vec4& b);
	// Квадрат числа
	inline double sqr(const double& x) { return x * x; };
	// Куб числа
	inline double cube(const double& x) { return x * x * x; };
	// Четное ли число
	inline bool is_even(const int& x) { return x % 2 == 0; };

	// Тип интерполяции
	enum class InterpolTypeEnum
	{
		// Линейная
		Linear,
		// Квадратичный сплайн
		Quadratic,
		// Кубический сплайн
		Cubic
	};

	// Абстрактный класс интерполяционной функции
	class Interpolation
	{
	public:
		Interpolation() = default;
		Interpolation(vector<double> iVecX, vector<double> iVecY);
		~Interpolation() = default;

		// Установить данные
		virtual void SetData(vector<double> iVecX, vector<double> iVecY, size_t displayDiscret = 100);
		// Вычисление значения интерполированной функции
		virtual optional<double> operator()(double x) const = 0;
		// Получить точки для отображения кривой
		virtual pair<vector<double>, vector<double>> GetDisplayPoints() const { return { display_x, display_y }; };

	protected:
		// Проверка, находится ли данный x в диапазоне интерполяции
		bool InRange(double x) const;
		// Найти интервал, в котором находится аргумент
		optional<size_t> FindSegment(double x) const;

		// Границы
		double xmin = 0.0;
		double xmax = 0.0;
		double ymin = 0.0;
		double ymax = 0.0;

		// Набор данных
		vector<double> vec_x;
		vector<double> vec_y;

		// Набор точек для отображения кривой
		vector<double> display_x;
		vector<double> display_y;
	};

	// Класс линейной интерполяционной функции
	class LinInterpolation : public Interpolation
	{
	public:
		LinInterpolation() : Interpolation() {};
		LinInterpolation(vector<double> iVecX, vector<double> iVecY) : Interpolation(iVecX, iVecY) {};
		~LinInterpolation() = default;

		// Вычисление значения интерполированной функции
		virtual optional<double> operator()(double x) const override;

	protected:

	};

	// Класс интерполяционной функции со сплайном 2го порядка
	class Spline2Interpolation : public Interpolation
	{
	public:
		Spline2Interpolation() : Interpolation() {};
		Spline2Interpolation(vector<double> iVecX, vector<double> iVecY);
		~Spline2Interpolation() = default;

		// Установить данные
		virtual void SetData(vector<double> iVecX, vector<double> iVecY, size_t displayDiscret = 100) override;
		// Вычисление значения интерполированной функции
		virtual optional<double> operator()(double x) const override;

	protected:
		// Коэффициенты парабол на интервалах
		vector<vec3> coeffs;
	};

	// Класс интерполяционной функции со сплайном 3го порядка
	class Spline3Interpolation : public Interpolation
	{
	public:
		Spline3Interpolation() : Interpolation() {};
		Spline3Interpolation(vector<double> iVecX, vector<double> iVecY);
		~Spline3Interpolation() = default;

		// Установить данные
		virtual void SetData(vector<double> iVecX, vector<double> iVecY, size_t displayDiscret = 100) override;
		// Вычисление значения интерполированной функции
		virtual optional<double> operator()(double x) const override;

	protected:
		// Коэффициенты парабол на интервалах
		vector<vec4> coeffs;
	};

	using InterpolationPtr = std::shared_ptr<Interpolation>;
	using LinInterpolationPtr = std::shared_ptr<LinInterpolation>;
	using Spline2InterpolationPtr = std::shared_ptr<Spline2Interpolation>;
	using Spline3InterpolationPtr = std::shared_ptr<Spline3Interpolation>;
}