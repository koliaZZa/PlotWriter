#include "Include/Model/Common/Model.Object.h"

namespace Model
{
	void Object::WriteHeader(rj::Document& doc, rj::MemoryPoolAllocator<>& allocator) const
	{
		rju::AddString(doc, allocator, "Name", name);
		rju::AddString(doc, allocator, "Title", title);
	}
	void Object::ReadHeader(const rj::Value& doc)
	{
		bool hasName = rju::ReadString(doc, "Name", name);
		bool hasTitle = rju::ReadString(doc, "Title", title);

		if (!hasName && !hasTitle) ErrorReporter::PushMessage(ErrorType::FileReading, "Object", "Fully unnamed object");
		if (!hasName) ErrorReporter::PushMessage(ErrorType::FileReading, "Object titled" + title, "Object hasn't internal name");
		if (!hasTitle) ErrorReporter::PushMessage(ErrorType::FileReading, "Object " + name, "Object hasn't title");
	}
}