#pragma once
#include "afxwin.h"
#include <gdiplus.h>
#include <vector>
#include <iostream>

using namespace Gdiplus;
using namespace std;

class DrawGraph : public CStatic
{

public:
	double xmin, xmax, ymin, ymax, step_x, step_y;
	vector<PointF> _pointsHost;
	vector<PointF> _pointsDevice0;
	vector<PointF> _pointsDevice1;
	vector<PointF> _pointsDevice2;
	DrawGraph();
	~DrawGraph();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	REAL X(LPDRAWITEMSTRUCT RECT, float x);
	REAL Y(LPDRAWITEMSTRUCT RECT, float y);
	REAL Width(LPDRAWITEMSTRUCT RECT, float width);
	REAL Height(LPDRAWITEMSTRUCT RECT, float height);

};

