#pragma once
#include <string>
#include <codecvt>

class Log
{
public:
	void add(std::string text);
	void clear();
	std::string getText();
	Log();
	Log(CEdit *logElement);

private:
	virtual ~Log();
	std::wstring stringToWstring(const std::string& t_str);


private:
	CEdit *_logElement;
	std::string _text;
	size_t numRow = 0;
};

