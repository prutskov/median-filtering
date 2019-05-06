﻿
// MedianFilteringDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "MedianFiltering.h"
#include "MedianFilteringDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CMedianFilteringDlg



CMedianFilteringDlg::CMedianFilteringDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MEDIANFILTERING_DIALOG, pParent)
	, _isAddNoise(TRUE)
	, _percentNoise(30)
	, _maskType(0)
	, _acceleratorType(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMedianFilteringDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK1, _isAddNoise);
	DDX_Text(pDX, IDC_EDIT1, _percentNoise);
	DDX_Radio(pDX, IDC_RADIO_MASK3, _maskType);
	DDX_Control(pDX, IDC_LOG, _logElement);
	DDX_Radio(pDX, IDC_ACC_HOST, _acceleratorType);
	DDX_Control(pDX, IDC_COMBO1, _devicesNames);
	DDX_Control(pDX, IDC_PIC3X3, chart3x3);
}

BEGIN_MESSAGE_MAP(CMedianFilteringDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_IMAGE, &CMedianFilteringDlg::OnBnClickedOpenImage)
	ON_BN_CLICKED(IDC_FILTER, &CMedianFilteringDlg::OnBnClickedFilter)
	ON_BN_CLICKED(IDC_BENCHMARK, &CMedianFilteringDlg::OnBnClickedBenchmark)
END_MESSAGE_MAP()


// Обработчики сообщений CMedianFilteringDlg

BOOL CMedianFilteringDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	srand(static_cast<uint>(time(NULL)));
	_log = new Log(&_logElement);
	cvHelper = new CVHelper(_log);

	std::shared_ptr<FilterDevice> filterDevice = std::shared_ptr<FilterDevice>(new FilterDevice(_log));

	auto devices = filterDevice->getDevices();
	for (int i = 0; i < devices.size(); i++)
	{
		CString str(devices[i].c_str());
		_devicesNames.AddString(str);
	}

	_devicesNames.SetCurSel(0);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMedianFilteringDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMedianFilteringDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMedianFilteringDlg::loadImage()
{
	CFileDialog fd(true, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY |
		OFN_LONGNAMES | OFN_PATHMUSTEXIST, _T("All Files (*.*)|*.*| Bitmap files (*.bmp)|*.bmp| JPEG files (*.jpg)|*.jpg| |"), NULL, 0, TRUE);

	if (fd.DoModal() != IDOK)
	{
		MessageBox(L"File is not open!", L"Warning", MB_ICONWARNING);
	}
	else
	{
		CString pathBMP = fd.GetPathName();
		CT2CA pathBuf(pathBMP);
		std::string str(pathBuf);
		cvHelper->loadImage(str, IMREAD_COLOR);
		cvHelper->imageShow(WINDOW_NORMAL);
	}
}

void CMedianFilteringDlg::runBenchmark(size_t beginSize, size_t endSize, size_t step)
{
	std::shared_ptr<Filter> filterHost = std::shared_ptr<Filter>(new FilterHost(_log));
	std::shared_ptr<Filter> filterDevice = std::shared_ptr<Filter>(new FilterDevice(_log));
	size_t nPoints = (endSize - beginSize) / step;
	Parameter param = { Mask::MASK3X3 };
	const size_t count = 5;

	std::vector<PointF> hostResult;
	std::vector<PointF> device0Result;
	std::vector<PointF> device1Result;
	std::vector<PointF> device2Result;

	for (size_t imageSize = beginSize; imageSize <= endSize; imageSize += step)
	{
		/*Generate frame with current size*/
		Frame frame = generateFrame(imageSize, imageSize);


		/*Benchmark host*/
		filterHost->setParameter(param);
		filterHost->setFrame(frame);

		PointF pointHost(imageSize, 0);
		/*Compute median count times*/
		for(size_t i = 0; i < count; i++)
		{
			pointHost.Y += filterHost->compute();
		}
		pointHost.Y = static_cast<Gdiplus::REAL>(pointHost.Y / count);
		hostResult.push_back(pointHost);

		//////////////////////////////////////////////////////////////////
		/*Benchmark device0*/
		param.activeDevice = 0;
		filterDevice->setParameter(param);
		filterDevice->setFrame(frame);

		PointF pointDevice0(imageSize, 0);
		/*Compute median count times*/
		for (size_t i = 0; i < count; i++)
		{
			pointDevice0.Y += filterDevice->compute();
		}
		pointDevice0.Y = static_cast<Gdiplus::REAL>(pointDevice0.Y / count);
		device0Result.push_back(pointDevice0);

		//////////////////////////////////////////////////////////////////
		/*Benchmark device1*/
		param.activeDevice = 1;
		filterDevice->setParameter(param);
		filterDevice->setFrame(frame);

		PointF pointDevice1(imageSize, 0);
		/*Compute median count times*/
		for (size_t i = 0; i < count; i++)
		{
			pointDevice1.Y += filterDevice->compute();
		}
		pointDevice1.Y = static_cast<Gdiplus::REAL>(pointDevice1.Y / count);
		device1Result.push_back(pointDevice1);

		//////////////////////////////////////////////////////////////////
		/*Benchmark device2*/
		param.activeDevice = 2;
		filterDevice->setParameter(param);
		filterDevice->setFrame(frame);

		PointF pointDevice2(imageSize, 0);
		/*Compute median count times*/
		for (size_t i = 0; i < count; i++)
		{
			pointDevice2.Y += filterDevice->compute();
		}
		pointDevice2.Y = static_cast<Gdiplus::REAL>(pointDevice2.Y / count);
		device2Result.push_back(pointDevice2);

	}
	chart3x3._pointsHost = hostResult;
	chart3x3._pointsHost = device0Result;
	chart3x3._pointsHost = device1Result;
	chart3x3._pointsHost = device2Result;
}

Frame CMedianFilteringDlg::generateFrame(size_t nRows, size_t nCols)
{
	auto redDataPtr = std::shared_ptr<uchar[]>(new uchar[nRows*nCols], std::default_delete<uchar[]>());
	auto greenDataPtr = std::shared_ptr<uchar[]>(new uchar[nRows*nCols], std::default_delete<uchar[]>());
	auto blueDataPtr = std::shared_ptr<uchar[]>(new uchar[nRows*nCols], std::default_delete<uchar[]>());

	for (size_t i = 0; i < nRows*nCols; i++)
	{
		redDataPtr[i]	= rand() % 256;
		greenDataPtr[i] = rand() % 256;
		blueDataPtr[i]	= rand() % 256;
	}

	Frame frame(nRows, nCols, redDataPtr, greenDataPtr, blueDataPtr);

	return frame;
}



void CMedianFilteringDlg::OnBnClickedOpenImage()
{
	loadImage();
	Frame image = cvHelper->getImage();
}


void CMedianFilteringDlg::OnBnClickedFilter()
{
	UpdateData(TRUE);
	
	if (cvHelper->isNullImage())
	{
		_log->add("Image is null. Load image for continue...");
		MessageBox(L"Please, load image.", L"Warning", MB_ICONINFORMATION);
		return;
	}

	std::shared_ptr<Filter> filter;
	if (_acceleratorType == 0)
	{
		filter = std::shared_ptr<Filter>(new FilterHost(_log));
		Parameter parameter = { _maskType == 0 ? Mask::MASK3X3 : Mask::MASK5X5 };
		filter->setParameter(parameter);
		filter->setFrame(cvHelper->getImage());

		_log->add(L"Selected host.");

		parameter.mask == Mask::MASK3X3 ? _log->add("Mask: 3x3") : _log->add("Mask: 5x5");

		if (_isAddNoise)
		{
			filter->generateNoise(_percentNoise / 100.0F);
			cvHelper->imageShow("Noised image", filter->getFrame(), WINDOW_NORMAL);
		}
		float duration = filter->compute();
		_log->add(L"Filter: Host algorithm. Timing: " + std::to_wstring(duration) + L" ms");
		cvHelper->imageShow("Host algorithm.", filter->getFrame(), WINDOW_NORMAL);
	}
	else if (_acceleratorType == 1)
	{
		filter = std::shared_ptr<Filter>(new FilterDevice(_log));
		Parameter parameter;
		parameter.mask = _maskType == 0 ? Mask::MASK3X3 : Mask::MASK5X5;
		parameter.activeDevice = _devicesNames.GetCurSel();

		filter->setParameter(parameter);
		filter->setFrame(cvHelper->getImage());
		
		CString str;
		_devicesNames.GetLBText(parameter.activeDevice, str);

		_log->add(L"Selected device: " + std::wstring(str));
		parameter.mask == Mask::MASK3X3 ? _log->add("Mask: 3x3") : _log->add("Mask: 5x5");
		if (_isAddNoise)
		{
			filter->generateNoise(_percentNoise / 100.0F);
			cvHelper->imageShow("Noised image", filter->getFrame(), WINDOW_NORMAL);
		}
		float duration = filter->compute();
		_log->add(L"Filter: Device algorithm. Timing: " + std::to_wstring(duration) + L" ms");
		cvHelper->imageShow("Device algorithm. ", filter->getFrame(), WINDOW_NORMAL);
	}
}


void CMedianFilteringDlg::OnBnClickedBenchmark()
{
	runBenchmark(100, 2000, 100);
	chart3x3.RedrawWindow();
}
