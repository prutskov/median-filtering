﻿
// MedianFiltering.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMedianFilteringApp:
// Сведения о реализации этого класса: MedianFiltering.cpp
//

class CMedianFilteringApp : public CWinApp
{
public:
	CMedianFilteringApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMedianFilteringApp theApp;
