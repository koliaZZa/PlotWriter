#include "Include/Model/Model.SignalProcessing.h"

namespace ARP::Model
{
	rj::Document SignalProcessing::Save(rj::MemoryPoolAllocator<>& allocator)
	{
		return rj::Document();
	}
	void SignalProcessing::Load(const rj::Value& doc)
	{
	}
	bool SignalProcessing::OpenRunSignals(string iPath)
	{
		if (iPath.empty()) return false;
		runSignals = std::make_shared<RunSignals>(iPath, 100, SignalOnProgress);
		if (runSignals->IsEmpty()) return false;
		return true;
	}
}