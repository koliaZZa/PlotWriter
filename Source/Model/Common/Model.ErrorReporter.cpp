#include "Include/Model/Common/Model.ErrorReporter.h"

namespace ARP::Model
{
	void ErrorReporter::PushMessage(ErrorType errType, string object, string message)
	{
		string type;
		switch (errType)
		{
		case ErrorType::FileReading:		type = "[FILE READING] ";		break;
		case ErrorType::FileSaving:			type = "[FILE SAVING] ";		break;
		case ErrorType::MathCalc:			type = "[MATH CALC] ";			break;
		case ErrorType::InternalIntegrity:	type = "[INTERNAL INTEGRITY] "; break;
		default:break;
		}
		messages.push("ERROR " + type + object + ": " + message);
	}
	string ErrorReporter::PopMessage()
	{
		string message = messages.front();
		messages.pop();
		return message;
	}
}