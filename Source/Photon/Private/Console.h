#include "OutputListener.h"

class ConsoleImplementation;

class Console : public OutputListener
{
public:
	Console(void);
	~Console(void);
	void update(void);
	void write(const ExitGames::Common::JString& str);
	void writeLine(const ExitGames::Common::JString& str=ExitGames::Common::JString());
	static Console& get(void);

	// Util
	ExitGames::Common::JString ToJString(FString fstr);
	FString ToFString(ExitGames::Common::JString jstr);
	TArray<FString> ToTArray(const ExitGames::Common::JVector<ExitGames::Common::JString>& jVector);
	TArray<bool> ToTArray(const ExitGames::Common::JVector<bool>& jVector);
private:
	ConsoleImplementation* mpImp;
};