#pragma once
#include <string>
#include <codecvt>

class Log
{
public:
	void add(std::wstring text);
	void add(std::string text);
	void clear();
	std::wstring getText();
	Log();
	Log(CEdit *logElement);
	std::wstring stringToWstring(const std::string& t_str);

private:
	virtual ~Log();


private:
	CEdit *_logElement;
	std::wstring _text;
	size_t numRow = 0;
};

