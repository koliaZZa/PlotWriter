#pragma once
#include <string>
#include <queue>

using std::string, std::queue;

namespace Model
{
	enum class ErrorType
	{
		FileReading,
		FileSaving,
		MathCalc,
		InternalIntegrity
	};

	class ErrorReporter
	{
	public:
		static void PushMessage(ErrorType errType, string object, string message);
		static string PopMessage();
		static bool IsEmpty() { return messages.empty(); };

	protected:
		static inline queue<string> messages;

	private:
		ErrorReporter() = default;
		~ErrorReporter() = default;

	};
}