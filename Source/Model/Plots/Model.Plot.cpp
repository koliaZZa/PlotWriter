#include "Include/Model/Plots/Model.Plot.h"

namespace ARP::Model
{
	PlotLine::PlotLine(string iTitle, Quantity ix, Quantity iy, bool iVisibility, int iWidth, InterpolTypeEnum iType, bool isHeavy) : Object(iTitle, iTitle), width{ iWidth }, isVisible{ iVisibility }, heavy{isHeavy}
	{
		Init();
		SetData(ix, iy);
		SetInterpolType(iType);
	}

	rj::Document PlotLine::Save(rj::MemoryPoolAllocator<>& allocator)
	{
		rj::Document doc;
		doc.SetObject();

		rj::Value json_val;
		// Сохранение общих данных
		WriteHeader(doc, allocator);
		rju::AddBool(doc, allocator, "Is visible", isVisible);
		rju::AddBool(doc, allocator, "Is tracing", isTracing);
		rju::AddInt(doc, allocator, "Line width", width);

		// Сохранение типа интерполяции
		switch (interpolType)
		{
		case InterpolTypeEnum::Linear:
			json_val.SetString("Linear", allocator);
			break;
		case InterpolTypeEnum::Quadratic:
			json_val.SetString("Quadratic", allocator);
			break;
		case InterpolTypeEnum::Cubic:
			json_val.SetString("Cubic", allocator);
			break;
		default:
			break;
		}
		doc.AddMember("Interpolation", json_val, allocator);

		// Сохранение физ. величин
		doc.AddMember("X Quantity", xQuantity.Save(allocator), allocator);
		doc.AddMember("Y Quantity", yQuantity.Save(allocator), allocator);

		return doc;
	}

	void PlotLine::Load(const rj::Value& doc)
	{
		// Считывание общих данных
		ReadHeader(doc);
		bool integrity = true;
		integrity = integrity && rju::ReadBool(doc, "Is visible", isVisible);
		integrity = integrity && rju::ReadBool(doc, "Is tracing", isTracing);
		integrity = integrity && rju::ReadInt(doc, "Line width", width);

		// Считывание типа интерполяции
		string interpol;
		integrity = integrity && rju::ReadString(doc, "Interpolation", interpol);
		if		(interpol == "Linear")		SetInterpolType(InterpolTypeEnum::Linear);
		else if (interpol == "Quadratic")	SetInterpolType(InterpolTypeEnum::Quadratic);
		else if (interpol == "Cubic")		SetInterpolType(InterpolTypeEnum::Cubic);
		else								SetInterpolType(InterpolTypeEnum::Linear);

		if (!integrity) ErrorReporter::PushMessage(ErrorType::FileReading, "Plot line " + name, "Missing additional data");

		// Считывание физ. величин
		if (doc.HasMember("X Quantity"))
			xQuantity.Load(doc["X Quantity"]);
		else ErrorReporter::PushMessage(ErrorType::FileReading, "Plot line " + name, "Missing X quantity data");

		if (doc.HasMember("Y Quantity"))
			yQuantity.Load(doc["Y Quantity"]);
		else ErrorReporter::PushMessage(ErrorType::FileReading, "Plot line " + name, "Missing Y quantity data");

		UpdateData();
	}

	void PlotLine::SetData(Quantity ix, Quantity iy)
	{
		xQuantity = ix;
		yQuantity = iy;
		UpdateData();
	}

	void PlotLine::SetXAxis(Quantity ix)
	{
		SetData(ix, yQuantity);
	}

	void PlotLine::Init()
	{
		linInterpolFunc = std::make_shared<Model::LinInterpolation>();
		spline2InterpolFunc = std::make_shared<Model::Spline2Interpolation>();
		spline3InterpolFunc = std::make_shared<Model::Spline3Interpolation>();
	}

	void PlotLine::UpdateData()
	{
		linInterpolFunc->SetData(xQuantity.data, yQuantity.data);
		if (!heavy)
		{
			spline2InterpolFunc->SetData(xQuantity.data, yQuantity.data);
			spline3InterpolFunc->SetData(xQuantity.data, yQuantity.data);
		}
	}

	void PlotLine::SetInterpolType(InterpolTypeEnum iType)
	{
		interpolType = iType;
		switch (interpolType)
		{
		case InterpolTypeEnum::Linear:
		{
			selectedFunc = linInterpolFunc;
			break;
		}
		case InterpolTypeEnum::Quadratic:
		{
			selectedFunc = spline2InterpolFunc;
			break;
		}
		case InterpolTypeEnum::Cubic:
		{
			selectedFunc = spline3InterpolFunc;
			break;
		}
		default:
			break;
		}
	}

	Plot::Plot(string iTitle, string ixAxis, string iyAxis, int iLineWidth, InterpolTypeEnum iInterpolType) : Object(iTitle, iyAxis)
	{
		xAxis = ixAxis;
		yAxis = iyAxis;
		width = iLineWidth;
		interpolType = iInterpolType;
	}

	rj::Document Plot::Save(rj::MemoryPoolAllocator<>& allocator)
	{
		rj::Document doc;
		doc.SetObject();

		rj::Value json_val;

		// Сохранение общих данных
		WriteHeader(doc, allocator);
		rju::AddString(doc, allocator, "xAxisName", xAxis);
		rju::AddString(doc, allocator, "yAxisName", yAxis);
		rju::AddInt(doc, allocator, "Line width", width);

		// Сохранение типа интерполяции
		switch (interpolType)
		{
		case InterpolTypeEnum::Linear:
			json_val.SetString("Linear", allocator);
			break;
		case InterpolTypeEnum::Quadratic:
			json_val.SetString("Quadratic", allocator);
			break;
		case InterpolTypeEnum::Cubic:
			json_val.SetString("Cubic", allocator);
			break;
		default:
			break;
		}
		doc.AddMember("Interpolation", json_val, allocator);

		// Сохранение линий графика
		rj::Value array(rj::kArrayType);
		for (auto& line : lines)
			array.PushBack(line->Save(allocator), allocator);
		doc.AddMember("Lines", array, allocator);

		return doc;
	}

	void Plot::Load(const rj::Value& doc)
	{
		// Считывание общих данных
		ReadHeader(doc);
		bool integrity = true;
		integrity = integrity && rju::ReadString(doc, "xAxisName", xAxis);
		integrity = integrity && rju::ReadString(doc, "yAxisName", yAxis);
		integrity = integrity && rju::ReadInt(doc, "Line width", width);

		// Считывание типа интерполяции
		string interpol;
		integrity = integrity && rju::ReadString(doc, "Interpolation", interpol);
		if		(interpol == "Linear")		interpolType = InterpolTypeEnum::Linear;
		else if (interpol == "Quadratic")	interpolType = InterpolTypeEnum::Quadratic;
		else if (interpol == "Cubic")		interpolType = InterpolTypeEnum::Cubic;
		else								interpolType = InterpolTypeEnum::Linear;

		if (!integrity) ErrorReporter::PushMessage(ErrorType::FileReading, "Plot " + name, "Missing additional data");

		// Считывание линий
		for (auto& item : doc["Lines"].GetArray())
			lines.push_back(std::make_shared<PlotLine>(item));
	}

	void Plot::Normalize(bool normalize)
	{
		for (auto& line : lines)
			line->Normalize(normalize);
	}

	PlotLinePtr Plot::AddLine(string iTitle, Quantity ix, Quantity iy, bool isHeavy)
	{
		PlotLinePtr line = std::make_shared<PlotLine>(iTitle, ix, iy, true, width, interpolType, isHeavy);
		lines.push_back(line);
		return line;
	}

	void Plot::DeleteLine(string iLineName)
	{
		auto itLine = std::find_if(lines.begin(), lines.end(), [iLineName](PlotLinePtr line) { return line->name == iLineName; });
		if (itLine != lines.end())
			lines.erase(itLine);
	}

	void Plot::SetInterpolType(InterpolTypeEnum iType)
	{
		interpolType = iType;
	}
	PlotLinePtr Plot::GetLine(string iLineName)
	{
		auto itLine = std::find_if(lines.begin(), lines.end(), [iLineName](PlotLinePtr line) { return line->name == iLineName; });
		if (itLine != lines.end())
			return *itLine;
		return nullptr;
	}

	string Plot::GetXAxisTitle() const
	{
		if (!lines.size()) return string{};
		return (*lines.begin())->GetXAxisTitle();
	}
	string Plot::GetYAxisTitle() const
	{
		if (!lines.size()) return string{};
		return (*lines.begin())->GetYAxisTitle();
	}
}