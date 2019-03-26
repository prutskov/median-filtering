
// MedianFilteringDlg.h: файл заголовка
//

#pragma once
#include <string>
#include "CVHelper.h"
#include "FilterHost.h"

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
	CVHelper cvHelper;

	void loadImage();
	afx_msg void OnBnClickedOpenImage();
	afx_msg void OnBnClickedFilter();
	BOOL _isAddNoise;
	float _percentNoise;
};
