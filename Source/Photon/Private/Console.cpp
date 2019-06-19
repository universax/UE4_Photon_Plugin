#include "Console.h"
#include "GameFramework/Actor.h"

Console::Console(void)
	: mpImp(NULL)
{
}

Console::~Console(void)
{
}

void Console::update(void)
{
}

void Console::write(const ExitGames::Common::JString& str)
{
	log(str, FColor::Blue);
	std::wcout << str;
}

void Console::writeLine(const ExitGames::Common::JString& str)
{
	write(str);
	std::wcout << std::endl;
}

Console& Console::get(void)
{
	static Console console;
	return console;
}

// ------------------------------------------------------------
ExitGames::Common::JString Console::ToJString(FString fstr) {
	return ExitGames::Common::JString(TCHAR_TO_UTF8(*fstr));
}

FString Console::ToFString(ExitGames::Common::JString jstr) {
	std::string cstr = jstr.UTF8Representation().cstr();
	return cstr.c_str();
}

TArray<FString> Console::ToTArray(const ExitGames::Common::JVector<ExitGames::Common::JString>& jVector)
{
	TArray<FString> tArray;
	for (unsigned int i = 0; i < jVector.getSize(); i++)
	{
		FString str = ToFString(jVector[i]);
		tArray.Add(str);
	}
	return tArray;
}

TArray<bool> Console::ToTArray(const ExitGames::Common::JVector<bool>& jVector)
{
	TArray<bool> tArray;
	for (unsigned int i = 0; i < jVector.getSize(); i++)
	{
		bool b = jVector[i];
		tArray.Add(b);
	}
	return tArray;
}