#include "stdafx.h"
#include "Draw2D.h"
#include <math.h>


using namespace Gdiplus;
ULONG_PTR token2;
DrawGraph::DrawGraph()
{
	Status s;
	GdiplusStartupInput input;
	s = GdiplusStartup(&token2, &input, NULL);
	if (s != Ok)
	{
		MessageBox(L"ERROR!!!", L"Ошибка", MB_ICONERROR);
	}
}


DrawGraph::~DrawGraph()
{
	GdiplusShutdown(token2);
}


void DrawGraph::DrawItem(LPDRAWITEMSTRUCT RECT)
{
	double maxY = 1;
	double maxX = 1;
	if (_pointsHost.size() > 0)
	{
		maxY = 0;
		int size = _pointsHost.size();
		for (int i = 0; i < size; ++i)
		{
			if (fabs(_pointsHost[i].Y) > maxY) maxY = fabs(_pointsHost[i].Y);
			if (_pointsHost[i].X > maxX) maxX = _pointsHost[i].X;
		}
		if (maxY == 0 || maxX == 0)
		{
			maxY = 1;
			maxX = 1;
		}
	}
	ymax = maxY + maxY / 3.f;
	ymin = -ymax / 14.f;
	step_y = ymax / 4.f;

	xmax = maxX;
	xmin = -xmax / 100.f;
	step_x = xmax / 8.f;
	
	Graphics gr(RECT->hDC);
	Bitmap bmp(RECT->rcItem.right, RECT->rcItem.bottom, &gr);
	Graphics grBmp(&bmp);

	FontFamily podpis_osi(L"Arial");
	Gdiplus::Font podpis(&podpis_osi, 12, FontStyleRegular, UnitPixel);

	SolidBrush brush(Color::White);
	Pen graph_pen(Color::Yellow, 4);
	Pen device0_pen(Color::Red, 4);
	Pen device1_pen(Color::Blue, 4);
	Pen device2_pen(Color::Green, 4);

	Pen bottom_pen(Color::Aqua, 4);
	Pen osi_pen(Color::White, 3);
	Pen setka_pen(Color::LightGray, 1);
	setka_pen.SetDashStyle(DashStyle::DashStyleDash);

	grBmp.SetSmoothingMode(SmoothingModeAntiAlias);
	grBmp.Clear(Color::Black);

	//главные оси
	grBmp.DrawLine(&osi_pen, X(RECT, xmin), Y(RECT, 0), X(RECT, xmax), Y(RECT, 0));
	grBmp.DrawLine(&osi_pen, X(RECT, 0), Y(RECT, ymin), X(RECT, 0), Y(RECT, ymax));

	//сетка
	//вертикальные линии
	for (double x = step_x; x <= xmax; x += step_x)
	{
		grBmp.DrawLine(&setka_pen, X(RECT, x), Y(RECT, ymin), X(RECT, x), Y(RECT, ymax));
	}
	for (double x = -step_x; x >= xmin; x -= step_x)
	{
		grBmp.DrawLine(&setka_pen, X(RECT, x), Y(RECT, ymin), X(RECT, x), Y(RECT, ymax));
	}

	//горизонтальные линии
	for (double y = step_y; y <= ymax; y += step_y)
	{
		grBmp.DrawLine(&setka_pen, X(RECT, xmin), Y(RECT, y), X(RECT, xmax), Y(RECT, y));
	}
	for (double y = -step_y; y >= ymin; y -= step_y)
	{
		grBmp.DrawLine(&setka_pen, X(RECT, xmin), Y(RECT, y), X(RECT, xmax), Y(RECT, y));
	}

	//подпись точек на горизонтали
	for (double x = step_x; x <= xmax; x += step_x)
	{
		CString str;
		str.Format(_T("%.1f"), x);
		grBmp.DrawString(str, -1, &podpis, PointF(X(RECT, x), Y(RECT, 0) + 2.f), NULL, &brush);
	}
	for (double x = -step_x; x >= xmin; x -= step_x)
	{
		CString str;
		str.Format(_T("%.1f"), x);
		grBmp.DrawString(str, -1, &podpis, PointF(X(RECT, x), Y(RECT, 0) + 2.f), NULL, &brush);
	}

	//подпись точек на вертикали
	for (double y = step_y; y <= ymax; y += step_y)
	{
		CString str;
		str.Format(_T("%.5f"), y);
		grBmp.DrawString(str, -1, &podpis, PointF(X(RECT, 0), Y(RECT, y) + 2.f), NULL, &brush);
	}
	for (double y = 0; y >= ymin; y -= step_y)
	{
		CString str;
		str.Format(_T("%.5f"), y);
		grBmp.DrawString(str, -1, &podpis, PointF(X(RECT, 0), Y(RECT, y) + 2.f), NULL, &brush);
	}
	if (!_pointsHost.empty())
	{
		int size = _pointsHost.size();
		if (size > 1)
			for (int i = 0; i < size - 1; ++i)
			{
				grBmp.DrawLine(&graph_pen, X(RECT, _pointsHost[i].X), Y(RECT, _pointsHost[i].Y),
					X(RECT, _pointsHost[i + 1].X), Y(RECT, _pointsHost[i + 1].Y));
				grBmp.DrawLine(&device0_pen, X(RECT, _pointsDevice0[i].X), Y(RECT, _pointsDevice0[i].Y),
					X(RECT, _pointsDevice0[i + 1].X), Y(RECT, _pointsDevice0[i + 1].Y));
				grBmp.DrawLine(&device1_pen, X(RECT, _pointsDevice1[i].X), Y(RECT, _pointsDevice1[i].Y),
					X(RECT, _pointsDevice1[i + 1].X), Y(RECT, _pointsDevice1[i + 1].Y));
				grBmp.DrawLine(&device2_pen, X(RECT, _pointsDevice2[i].X), Y(RECT, _pointsDevice2[i].Y),
					X(RECT, _pointsDevice2[i + 1].X), Y(RECT, _pointsDevice2[i + 1].Y));
			}
	}
	gr.DrawImage(&bmp, 0, 0);
}


REAL DrawGraph::X(LPDRAWITEMSTRUCT RECT, float x)
{
	return (REAL)(RECT->rcItem.right) / (xmax - xmin)*((x)-xmin);
}

REAL DrawGraph::Y(LPDRAWITEMSTRUCT RECT, float y)
{
	return -(REAL)(RECT->rcItem.bottom) / (ymax - ymin)*((y)-ymax);
}

REAL DrawGraph::Width(LPDRAWITEMSTRUCT RECT, float width)
{
	return (REAL)(RECT->rcItem.right) / (xmax - xmin)*width;
}

REAL DrawGraph::Height(LPDRAWITEMSTRUCT RECT, float height)
{
	return (REAL)(RECT->rcItem.bottom) / (ymax - ymin)*height;
}
