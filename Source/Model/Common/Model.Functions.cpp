#include "Include/Model/Common/Model.Functions.h"

namespace ARP::Model
{
	Interpolation::Interpolation(vector<double> iVecX, vector<double> iVecY)
	{
		SetData(iVecX, iVecY);
	}
	bool Interpolation::InRange(double x) const
	{
		return (x >= xmin) && (x <= xmax);
	}
	optional<size_t> Interpolation::FindSegment(double x) const
	{
		for (size_t i = 0; i < vec_x.size() - 1; i++)
		{
			if ((vec_x[i] <= x) && (vec_x[i + 1] >= x))
				return i;
		}
		return std::nullopt;
	}
	void Interpolation::SetData(vector<double> iVecX, vector<double> iVecY, size_t displayDiscret)
	{
		vec_x = iVecX;
		vec_y = iVecY;

		display_x = iVecX;
		display_y = iVecY;

		auto xMinMax = FindMinMaxInVector(vec_x);
		auto yMinMax = FindMinMaxInVector(vec_y);

		xmin = xMinMax.first;
		xmax = xMinMax.second;
		ymin = yMinMax.first;
		ymax = yMinMax.second;
	}
	optional<double> LinInterpolation::operator()(double x) const
	{
		if (!InRange(x))
			return std::nullopt;
		// Ищем интервал, в котором находится аргумент
		optional<size_t> ind = FindSegment(x);
		if (ind)
		{
			size_t i = ind.value();
			return vec_y[i] + (vec_y[i + 1] - vec_y[i]) / (vec_x[i + 1] - vec_x[i]) * (x - vec_x[i]);
		}
		else return std::nullopt;
	}
	Spline2Interpolation::Spline2Interpolation(vector<double> iVecX, vector<double> iVecY) : Interpolation(iVecX, iVecY)
	{
		SetData(iVecX, iVecY);
	}
	void Spline2Interpolation::SetData(vector<double> iVecX, vector<double> iVecY, size_t displayDiscret)
	{
		coeffs.clear();
		Interpolation::SetData(iVecX, iVecY);

		coeffs.resize(vec_x.size() - 1);

		matrix3 a{};
		vec3 b{};
		optional<vec3> res;
		double k = 0;

		// Определяем коэффициенты для каждого интервала
		for (size_t i = 0; i < vec_x.size() - 1; i++)
		{
			a = matrix3{
				vec3{2 * vec_x[i], 1, 0},
				vec3{sqr(vec_x[i]), vec_x[i], 1},
				vec3{sqr(vec_x[i + 1]), vec_x[i + 1], 1} };
			if (i == 0)
				b = vec3{ (vec_y[i + 1] - vec_y[i]) / (vec_x[i + 1] - vec_x[i]), vec_y[i], vec_y[i + 1] };
			else
				b = vec3{ 2 * vec_x[i] * coeffs[i - 1][0] + coeffs[i - 1][1], vec_y[i], vec_y[i + 1] };

			res = cramer(a, b);
			if (res)
				coeffs[i] = res.value();
			else
			{
				// Если решения СЛАУ нет, интерполируем отрезком
				k = (vec_y[i + 1] - vec_y[i]) / (vec_x[i + 1] - vec_x[i]);
				coeffs[i] = vec3{ 0, k, vec_y[i] - k * vec_x[i] };
			}
		}
		// Определяем точки для отображения кривой
		double x = 0.0, dx = 0.0;
		display_x.clear();
		display_y.clear();
		for (size_t i = 0; i < vec_x.size() - 1; i++)
		{
			x = vec_x[i];
			dx = (vec_x[i + 1] - vec_x[i]) / displayDiscret;
			while (x < vec_x[i + 1])
			{
				display_x.push_back(x);
				optional<double> y = operator()(x);
				if (y) display_y.push_back(y.value());
				else
					break;
				x += dx;
			}
		}
	}
	optional<double> Spline2Interpolation::operator()(double x) const
	{
		if (!InRange(x))
			return std::nullopt;
		// Ищем интервал, в котором находится аргумент
		optional<size_t> ind = FindSegment(x);
		if (ind)
		{
			size_t i = ind.value();
			return coeffs[i][0] * sqr(x) + coeffs[i][1] * x + coeffs[i][2];
		}
		else return std::nullopt;
	}
	double matrix_determ(const matrix3& m)
	{
		return m[0][0] * m[1][1] * m[2][2] + m[1][0] * m[2][1] * m[0][2] + m[0][1] * m[1][2] * m[2][0] - m[0][2] * m[1][1] * m[2][0] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2];
	}
	double matrix_determ(const matrix4& m)
	{
		double res = 0;
		auto create_matrix = [](const matrix4& m, const size_t& ind)
		{
			matrix3 res;
			size_t j = 0;
			for (size_t i = 0; i < 4; i++)
			{
				if (i == ind) continue;
				res[0][j] = m[1][i];
				res[1][j] = m[2][i];
				res[2][j] = m[3][i];
				j++;
			}
			return res;
		};

		for (size_t i = 0; i < 4; i++)
		{
			if (m[0][i] != 0)
			{
				double add = m[0][i] * matrix_determ(create_matrix(m, i));
				res = is_even(i) ? res + add : res - add;
			}
		}

		return res;
	}
	optional<vec3> cramer(const matrix3& a, const vec3& b)
	{
		const double det = matrix_determ(a);
		if (det == 0.0) return std::nullopt;

		// Лямбда, создающая матрицы для частных детерминантов
		auto create_matrix = [](const matrix3& m, const vec3& b, const size_t& ind)
		{
			matrix3 res = m;
			res[0][ind] = b[0];
			res[1][ind] = b[1];
			res[2][ind] = b[2];
			return res;
		};

		// Вычисляем частные детерминанты
		const double det0 = matrix_determ(create_matrix(a, b, 0));
		const double det1 = matrix_determ(create_matrix(a, b, 1));
		const double det2 = matrix_determ(create_matrix(a, b, 2));

		return vec3{ det0 / det, det1 / det, det2 / det };
	}
	optional<vec4> cramer(const matrix4& a, const vec4& b)
	{
		const double det = matrix_determ(a);
		if (det == 0.0) return std::nullopt;

		// Лямбда, создающая матрицы для частных детерминантов
		auto create_matrix = [](const matrix4& m, const vec4& b, const size_t& ind)
		{
			matrix4 res = m;
			res[0][ind] = b[0];
			res[1][ind] = b[1];
			res[2][ind] = b[2];
			res[3][ind] = b[3];
			return res;
		};

		// Вычисляем частные детерминанты
		const double det0 = matrix_determ(create_matrix(a, b, 0));
		const double det1 = matrix_determ(create_matrix(a, b, 1));
		const double det2 = matrix_determ(create_matrix(a, b, 2));
		const double det3 = matrix_determ(create_matrix(a, b, 3));

		return vec4{ det0 / det, det1 / det, det2 / det, det3 / det };
	}
	Spline3Interpolation::Spline3Interpolation(vector<double> iVecX, vector<double> iVecY) : Interpolation(iVecX, iVecY)
	{
		SetData(iVecX, iVecY);
	}
	void Spline3Interpolation::SetData(vector<double> iVecX, vector<double> iVecY, size_t displayDiscret)
	{
		coeffs.clear();
		Interpolation::SetData(iVecX, iVecY);

		coeffs.resize(vec_x.size() - 1);

		matrix4 a{};
		vec4 b{};
		optional<vec4> res;
		double k = 0;

		// Определяем коэффициенты для каждого интервала
		for (size_t i = 0; i < vec_x.size() - 1; i++)
		{
			a = matrix4{
				vec4{6 * vec_x[i], 2, 0, 0},
				vec4{3 * sqr(vec_x[i]), 2 * vec_x[i], 1, 0},
				vec4{cube(vec_x[i]), sqr(vec_x[i]), vec_x[i], 1},
				vec4{cube(vec_x[i + 1]), sqr(vec_x[i + 1]), vec_x[i + 1], 1} };
			if (i == 0)
				b = vec4{ 0, (vec_y[i + 1] - vec_y[i]) / (vec_x[i + 1] - vec_x[i]), vec_y[i], vec_y[i + 1] };
			else
				b = vec4{
				6 * vec_x[i] * coeffs[i - 1][0] + 2 * coeffs[i - 1][1],
				3 * sqr(vec_x[i]) * coeffs[i - 1][0] + 2 * vec_x[i] * coeffs[i - 1][1] + coeffs[i - 1][2],
				vec_y[i], vec_y[i + 1] };

			res = cramer(a, b);
			if (res)
				coeffs[i] = res.value();
			else
			{
				// Если решения СЛАУ нет, интерполируем отрезком
				k = (vec_y[i + 1] - vec_y[i]) / (vec_x[i + 1] - vec_x[i]);
				coeffs[i] = vec4{ 0, 0, k, vec_y[i] - k * vec_x[i] };
			}
		}

		// Определяем точки для отображения кривой
		double x = 0.0, dx = 0.0;
		display_x.clear();
		display_y.clear();
		for (size_t i = 0; i < vec_x.size() - 1; i++)
		{
			x = vec_x[i];
			dx = (vec_x[i + 1] - vec_x[i]) / displayDiscret;
			while (x < vec_x[i + 1])
			{
				display_x.push_back(x);
				optional<double> y = operator()(x);
				if (y) display_y.push_back(y.value());
				else
					break;
				x += dx;
			}
		}
	}
	optional<double> Spline3Interpolation::operator()(double x) const
	{
		if (!InRange(x))
			return std::nullopt;
		// Ищем интервал, в котором находится аргумент
		optional<size_t> ind = FindSegment(x);
		if (ind)
		{
			size_t i = ind.value();
			return coeffs[i][0] * cube(x) + coeffs[i][1] * sqr(x) + coeffs[i][2] * x + coeffs[i][3];
		}
		else return std::nullopt;
	}
}