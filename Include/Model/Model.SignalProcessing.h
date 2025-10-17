#pragma once
#include "Include/Model/Common/Model.Object.h"
#include "Include/Model/Plots/Model.Plot.h"
#include "Include/Model/Runs/Model.RunSignals.h"

namespace ARP::Model
{
	using CheckListItem = std::pair<string, bool>;

	// ������ ���������� ��������
	class SignalProcessing : Object
	{
	public:
		SignalProcessing() : Object("Signal processing", "Signal processing") {};
		SignalProcessing(const rj::Value& doc) : Object("Signal processing", "Signal processing") { Load(doc); };
		~SignalProcessing() = default;

		// ��������� � json-��������
		virtual rj::Document Save(rj::MemoryPoolAllocator<>& allocator) override;
		// ��������� �� json-���������
		virtual void Load(const rj::Value& doc) override;

		bool OpenRunSignals(string iPath);

		// ���������� ��� ������������
		void SetInterpolType(InterpolTypeEnum iType) { interpolType = iType; };
		// ���������� ������ �������
		void SetPlot(PlotPtr iPlot) { plot = iPlot; };

		// �������� ��� ������������
		InterpolTypeEnum GetInterpolType() const { return interpolType; };

		PlotPtr GetPlot() const { return plot; };

		RunSignalsPtr GetRunSignals() const { return runSignals; };


		function<void(size_t)> SignalOnProgress = nullptr;		// ������ ��� ��������� ��������� ������ �����

	private:
		string path = "";
		vector<CheckListItem> channels;
		RunSignalsPtr runSignals = nullptr;
		PlotPtr plot = nullptr;
		InterpolTypeEnum interpolType = InterpolTypeEnum::Linear;			// ��� ������������

	};
	using SignalProcessingPtr = std::shared_ptr<SignalProcessing>;

}
