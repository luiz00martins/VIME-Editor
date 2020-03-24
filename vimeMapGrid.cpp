#include "wx/wx.h"
#include "mainWindow.h"
#include "vimeMapGrid.h"

#define DOT_RADIUS 6

vimeMapGrid::vimeMapGrid(wxWindow* parent, wxWindowID winid) : wxPanel(parent, winid){
	mapZoom = 100;
	desloc.x = 0;
	desloc.y = 0;
	map = new vimeMap();

	isWallSelected0 = false;
	isWallSelected1 = false;
	isPointSelected_ = false;

	Bind(wxEVT_LEFT_DOWN, &vimeMapGrid::onDown, this);
	Bind(wxEVT_MOTION, &vimeMapGrid::onMotion, this);
	Bind(wxEVT_LEFT_DCLICK, &vimeMapGrid::onDoubleClick, this);
	Bind(wxEVT_RIGHT_DOWN, &vimeMapGrid::onRightClick, this);
	Bind(wxEVT_PAINT, &vimeMapGrid::onPaint, this);
	Bind(wxEVT_RIGHT_DCLICK, &vimeMapGrid::onRightDouble, this);
}
vimeMapGrid::~vimeMapGrid() {
	delete map;
}

wxPoint vimeMapGrid::toWxPoint(const vimePoint& p) {
	return wxPoint((p.x * mapZoom) - desloc.x, desloc.y - (p.y * mapZoom));
}
vimePoint vimeMapGrid::toVimePoint(const wxPoint& p) {
	return vimePoint((desloc.x + p.x)/ (double)mapZoom, (desloc.y - p.y)/ (double)mapZoom);
}
wxPoint vimeMapGrid::firstGridPoint() {
	// The calculations are different for positives and negatives
	int x = desloc.x > 0 ? (mapZoom - (int)desloc.x % mapZoom) : (-(int)desloc.x % mapZoom);
	int y = desloc.y < 0 ? (mapZoom + (int)desloc.y % mapZoom) : ((int)desloc.y % mapZoom);
	return wxPoint(x, y);
}

void vimeMapGrid::updateGrid(wxBitmap& bitmap) {
	memDC.SelectObject(bitmap);
	wxPoint point(0, 0);

	// Setting up and clearing screen
	memDC.SetBackground(wxBrush(wxColour((unsigned long)0xdddddd)));
	memDC.Clear();
	memDC.SetPen(wxPen(wxColour((unsigned long)0xaaaaaa), 1));

	// Drawing horizontal lines
	// Finding the first horizontal line
	point = firstGridPoint();
	// Drawing horizontal lines
	while (point.y <= this->GetSize().y) {
		memDC.DrawLine(0, point.y, this->GetSize().x, point.y);
		point.y += mapZoom;
	}

	// Drawing dots
	memDC.SetBrush(wxBrush(wxColour((unsigned long)0x888888)));
	// Finding the first point
	point = firstGridPoint();
	// Drawing points
	while (point.x <= this->GetSize().x) {
		memDC.DrawLine(point.x, 0, point.x, this->GetSize().y);
		while (point.y <= this->GetSize().y) {
			memDC.DrawCircle(point, DOT_RADIUS);
			point.y += mapZoom;
		}
		point.y = (int)desloc.y % mapZoom;
		point.x += mapZoom;
	}

	memDC.SelectObject(wxNullBitmap);
}
void vimeMapGrid::updateWalls(wxBitmap& bitmap) {
	memDC.SelectObject(bitmap);

	memDC.SetPen(wxPen(wxColour((unsigned long)0x111111), 4));

	for (int i = 0; i < map->getLen(); i++) {
		memDC.DrawLine(toWxPoint(map->getWall(i).p0), toWxPoint(map->getWall(i).p1));
	}

	memDC.SelectObject(wxNullBitmap);
}

void vimeMapGrid::redraw() {
	wxClientDC dc(this);

	// Using bitmaps to stop flicker
	wxBitmap bitmap(this->GetSize());

	// Setting up and clearing screen

	updateGrid(bitmap);
	updateWalls(bitmap);

	memDC.SelectObject(bitmap);

	// Drawing wall selection
	if (isWallSelected0) {
		memDC.SetPen(wxPen(wxColour((unsigned long)0x64dd00), 1));
		memDC.SetBrush(wxBrush(wxColour((unsigned long)0x64dd00)));

		memDC.DrawCircle(toWxPoint(selectedWall.p0), DOT_RADIUS);

		if (isWallSelected1) {
			memDC.SetPen(wxPen(wxColour((unsigned long)0x64dd00), DOT_RADIUS*2));
			memDC.DrawLine(toWxPoint(selectedWall.p0), toWxPoint(selectedWall.p1));
		}
	}

	// Drawing point selection
	if (isPointSelected_) {
		memDC.SetPen(wxPen(wxColour((unsigned long)0xbbbb32), 1));
		memDC.SetBrush(wxBrush(wxColour((unsigned long)0xbbbb32)));
		memDC.DrawCircle(toWxPoint(selectedPoint), DOT_RADIUS);
	}

	memDC.SelectObject(wxNullBitmap);

	dc.DrawBitmap(bitmap, 0, 0);
}

void vimeMapGrid::selectPoint(wxPoint selection) {
	wxPoint firstPoint = firstGridPoint();
	wxPoint relativeClick(selection.x % mapZoom, selection.y % mapZoom);

	// This bit of code corrects the following bug in which
	// if the grid is (almost) in line with the window in any axis, the click on the grid might not be detected
	relativeClick.x += abs(firstPoint.x - relativeClick.x) < mapZoom / 2 ? 0 : (((firstPoint.x - relativeClick.x) > 0 ? 1 : -1)* mapZoom);
	relativeClick.y += abs(firstPoint.y - relativeClick.y) < mapZoom / 2 ? 0 : (((firstPoint.y - relativeClick.y) > 0 ? 1 : -1)* mapZoom);

	// Calculatin the distance from the click to the nearest point
	double dist = sqrt(pow(relativeClick.x - firstPoint.x, 2) + pow(relativeClick.y - firstPoint.y, 2));

	// If the click the range of a grid point
	if (dist < DOT_RADIUS){
		// Centering the click on the actual grid
		wxPoint point(selection.x - (relativeClick.x - firstPoint.x), selection.y - (relativeClick.y - firstPoint.y));

		if (!isWallSelected0) {
			selectedWall.p0 = toVimePoint(point);
			isWallSelected0 = true;
		}
		else if (selectedWall.p0 == toVimePoint(point)) {
			// If wall two points were selected, and it deselected 0, swap 1 for 0 and deselect 1
			if (isWallSelected1) {
				selectedWall.p0 = selectedWall.p1;
				isWallSelected1 = false;
			}
			else {
				isWallSelected0 = false;
			}
		}
		else if (!isWallSelected1) {
			selectedWall.p1 = toVimePoint(point);
			isWallSelected1 = true;
		}
		else if (selectedWall.p1 == toVimePoint(point)) {
			isWallSelected1 = false;
		}
		else {
			isWallSelected1 = false;
			selectedWall.p0 = toVimePoint(point);
		}
	}
	// If it's not on the grid, select a random place
	else {
		if (!isPointSelected_) {
			isPointSelected_ = true;
			selectedPoint = toVimePoint(selection);
		}
		else {
			wxPoint prevSel(toWxPoint(selectedPoint));

			// If the click the range the previous selection
			if ((selection.y <= prevSel.y + DOT_RADIUS && selection.y >= prevSel.y - DOT_RADIUS)
				&& (selection.x <= prevSel.x + DOT_RADIUS && selection.x >= prevSel.x - DOT_RADIUS)) {
				isPointSelected_ = false;
			}
			else {
				selectedPoint = toVimePoint(selection);
			}
		}
	}
}
void vimeMapGrid::deselectAll() {
	isWallSelected0 = false;
	isWallSelected1 = false;
	isPointSelected_ = false;
}
void vimeMapGrid::save(std::string filePath) {
	map->save(filePath);
}

void vimeMapGrid::createSelectedWall() {
	if (isWallSelected()) {
		map->addWall(selectedWall);
	}
	else {
		throw new nothing_selected();
	}
}
void vimeMapGrid::destroySelectedWall() {
	if (isWallSelected()) {
		map->removeWall(selectedWall);
	}
	else {
		throw new nothing_selected();
	}
}
void vimeMapGrid::setTexturePathSelectedWall(std::string path) {
	if (isWallSelected()) {
		map->removeWall(selectedWall);
		selectedWall.setTexturePath(path);
		map->addWall(selectedWall);
		selectedWall.setTexturePath("");
	}
	else {
		throw new nothing_selected();
	}
}

bool vimeMapGrid::isWallSelected() {
	return (isWallSelected0 && isWallSelected1);
}
bool vimeMapGrid::isPointSelected() {
	return isPointSelected_;
}

void vimeMapGrid::onPaint(wxPaintEvent& evt) {
	redraw();
}
void vimeMapGrid::onDoubleClick(wxMouseEvent& evt) {
	selectPoint(evt.GetPosition());
	redraw();
}
void vimeMapGrid::onRightClick(wxMouseEvent& evt){
	deselectAll();
	redraw();
}
void vimeMapGrid::onDown(wxMouseEvent& evt) {
	clicked = wxGetMousePosition();
}
void vimeMapGrid::onMotion(wxMouseEvent& evt) {

	if (evt.Dragging()) {
		wxPoint otherPt = wxGetMousePosition();

		if (clicked.x != otherPt.x || clicked.y != otherPt.y) {
			desloc.x += clicked.x - otherPt.x;
			desloc.y -= clicked.y - otherPt.y;
			redraw();
		}

		clicked = otherPt;
	}
}
void vimeMapGrid::onRightDouble(wxMouseEvent& evt) {
	save("C:\\Users\\3green\\Dropbox\\Programming\\Python\\VIME\\testMap.christian");
}