#include "stdafx.h"
#include "Log.h"


Log::Log() : _text(L""), _logElement(nullptr)
{
}

Log::Log(CEdit * logElement)
{
	_logElement = logElement;
}


Log::~Log()
{

}

void Log::add(std::wstring text)
{
	_text += L"["+std::to_wstring(numRow)+L"]>>" + text+ L"\r\n";
	//std::wstring textW = stringToWstring(_text);
	_logElement->SetWindowTextW((LPCTSTR)_text.c_str());
	numRow++;
}

void Log::add(std::string text)
{
	std::wstring textW = stringToWstring(text);
	_text += L"[" + std::to_wstring(numRow) + L"]>>" + textW + L"\r\n";	
	_logElement->SetWindowTextW((LPCTSTR)_text.c_str());
	numRow++;
}

void Log::clear()
{
	_text.clear();
	_text = L"";
}

std::wstring Log::getText()
{
	return _text;
}

std::wstring Log::stringToWstring(const std::string& t_str)
{
	//setup converter
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;

	//use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
	return converter.from_bytes(t_str);
}
