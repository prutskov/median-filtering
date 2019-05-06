﻿
// MedianFilteringDlg.h: файл заголовка
//

#pragma once
#include <string>
#include "CVHelper.h"
#include "FilterHost.h"
#include "FilterDevice.h"
#include "Log.h"
#include "Draw2D.h"

// Диалоговое окно CMedianFilteringDlg
class CMedianFilteringDlg : public CDialogEx
{
// Создание
public:
	CMedianFilteringDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEDIANFILTERING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	void loadImage();
	void runBenchmark(size_t beginSize, size_t endSize, size_t step);
	Frame generateFrame(size_t nRows, size_t nCols);
	afx_msg void OnBnClickedOpenImage();
	afx_msg void OnBnClickedFilter();
	BOOL _isAddNoise;
	float _percentNoise;
	int _maskType;
	CEdit _logElement;

	CVHelper* cvHelper;
	Log *_log;
	
	int _acceleratorType;
	CComboBox _devicesNames;
	afx_msg void OnBnClickedBenchmark();
	DrawGraph chart3x3;

	CProgressCtrl _benchmarkProgress;
	CStatic _percentProgress;
};
