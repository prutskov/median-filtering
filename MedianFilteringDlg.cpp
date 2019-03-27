
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
}

BEGIN_MESSAGE_MAP(CMedianFilteringDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_IMAGE, &CMedianFilteringDlg::OnBnClickedOpenImage)
	ON_BN_CLICKED(IDC_FILTER, &CMedianFilteringDlg::OnBnClickedFilter)
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
		cvHelper->loadImage(str, IMREAD_GRAYSCALE);
		cvHelper->imageShow(WINDOW_NORMAL);
	}
}



void CMedianFilteringDlg::OnBnClickedOpenImage()
{
	loadImage();
}


void CMedianFilteringDlg::OnBnClickedFilter()
{
	UpdateData(TRUE);
	Parameter parameter = { _maskType == 0 ? Mask::MASK3X3 : Mask::MASK5X5 };

	if (cvHelper->isNullImage())
	{
		_log->add("Image is null. Load image for continue...");
		MessageBox(L"Please, load image.", L"Warning", MB_ICONINFORMATION);
		return;
	}
	FilterHost filter(parameter, cvHelper->getImage());
	if (_isAddNoise)
	{
		filter.generateNoise(_percentNoise / 100.0F);
		cvHelper->imageShow("Noised image", filter.getFrame(), WINDOW_NORMAL);
	}
	auto start = std::chrono::high_resolution_clock::now();
	filter.compute();
	auto end = std::chrono::high_resolution_clock::now();
	float duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()/1000.0F;
	_log->add("Filter: HostAlg. Elapsed time: " + std::to_string(duration) + "ms");
	cvHelper->imageShow("Filtered image", filter.getFrame(), WINDOW_NORMAL);
}
