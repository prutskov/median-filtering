
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
}

BEGIN_MESSAGE_MAP(CMedianFilteringDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_IMAGE, &CMedianFilteringDlg::OnBnClickedOpenImage)
	ON_BN_CLICKED(IDC_FILTER, &CMedianFilteringDlg::OnBnClickedFilter)
	ON_BN_CLICKED(IDC_OPEN_VIDEO, &CMedianFilteringDlg::OnBnClickedOpenVideo)
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

	filterHost = std::shared_ptr<FilterHost>(new FilterHost(_log));
	filterDevice = std::shared_ptr<FilterDevice>(new FilterDevice(_log));

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

	if (_acceleratorType == 0)
	{
		Parameter parameter = { _maskType == 0 ? Mask::MASK3X3 : Mask::MASK5X5 };
		filterHost->setParameter(parameter);
		filterHost->setFrame(cvHelper->getImage());

		_log->add(L"Selected host.");
		if (parameter.mask == Mask::MASK3X3)
		{
			_log->add("Mask: 3x3");
		}
		else
		{
			_log->add("Mask: 5x5");
		}


		if (_isAddNoise)
		{
			filterHost->generateNoise(_percentNoise / 100.0F);
			cvHelper->imageShow("Noised image", filterHost->getFrame(), WINDOW_NORMAL);
		}
		filterHost->compute(true);
		cvHelper->imageShow("Host algorithm.", filterHost->getFrame(), WINDOW_NORMAL);
	}
	else if (_acceleratorType == 1)
	{
		ParameterDevice parameterDev;
		parameterDev.mask = _maskType == 0 ? Mask::MASK3X3 : Mask::MASK5X5;
		parameterDev.activeDevice = _devicesNames.GetCurSel();

		filterDevice->setParameter(parameterDev);
		filterDevice->setFrame(cvHelper->getImage());
		
		CString str;
		_devicesNames.GetLBText(parameterDev.activeDevice, str);

		_log->add(L"Selected device: " + std::wstring(str));
		if (parameterDev.mask == Mask::MASK3X3)
		{
			_log->add("Mask: 3x3");
		}
		else
		{
			_log->add("Mask: 5x5");
		}

		if (_isAddNoise)
		{
			filterDevice->generateNoise(_percentNoise / 100.0F);
			cvHelper->imageShow("Noised image", filterDevice->getFrame(), WINDOW_NORMAL);
		}
		filterDevice->compute(true);
		cvHelper->imageShow("Device algorithm. ", filterDevice->getFrame(), WINDOW_NORMAL);
	}
}


void CMedianFilteringDlg::OnBnClickedOpenVideo()
{
	UpdateData(TRUE);
	VideoCapture video(0);
	if(!video.isOpened())
	{
		_log->add("Camera don't opened.");
		return;
	}

	namedWindow("Filtered video", WINDOW_AUTOSIZE);
	if (_acceleratorType == 0)
	{
		Parameter parameter = { _maskType == 0 ? Mask::MASK3X3 : Mask::MASK5X5 };
		filterHost->setParameter(parameter);

		_log->add(L"Selected host.");
		if (parameter.mask == Mask::MASK3X3)
		{
			_log->add("Mask: 3x3");
		}
		else
		{
			_log->add("Mask: 5x5");
		}

		for (;;)
		{
			Mat frame;
			video >> frame; // get a new frame from camera
			cvHelper->imageShow("Camera:", frame, WINDOW_AUTOSIZE);
			filterHost->setFrame(cvHelper->convertToPtr(frame.clone()));
			filterHost->compute(false);
			Frame frameFiltered = filterHost->getFrame();
			imshow("Filtered video", cvHelper->convertToMat(frameFiltered));
			if (waitKey(30) >= 0) break;
		}
	}
	else if (_acceleratorType == 1)
	{
		ParameterDevice parameterDev;
		parameterDev.mask = _maskType == 0 ? Mask::MASK3X3 : Mask::MASK5X5;
		parameterDev.activeDevice = _devicesNames.GetCurSel();

		filterDevice->setParameter(parameterDev);

		CString str;
		_devicesNames.GetLBText(parameterDev.activeDevice, str);

		_log->add(L"Selected device: " + std::wstring(str));
		if (parameterDev.mask == Mask::MASK3X3)
		{
			_log->add("Mask: 3x3");
		}
		else
		{
			_log->add("Mask: 5x5");
		}

		for (;;)
		{
			Mat frame;
			video >> frame; // get a new frame from camera
			filterDevice->setFrame(cvHelper->convertToPtr(frame.clone()));
			filterDevice->generateNoise(_percentNoise / 100.0F);
			cvHelper->imageShow("Camera:", filterDevice->getFrame(), WINDOW_NORMAL);
			filterDevice->compute(false);
			Frame frameFiltered = filterDevice->getFrame();
			imshow("Filtered video", cvHelper->convertToMat(frameFiltered));
			if (waitKey(30) >= 0) break;
		}
	}
	
}
