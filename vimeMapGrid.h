#pragma once
#include "wx/wx.h"
#include "vimeMap.h"

class vimeMapGrid : public wxPanel {
private:
	vimeMap* map;
	int mapZoom;
	wxPoint desloc;

	vimeWall selectedWall;
	bool isWallSelected0, isWallSelected1;

	vimePoint selectedPoint;
	bool isPointSelected_;


	wxPoint clicked;

	wxMemoryDC memDC;

public:
	vimeMapGrid(wxWindow* parent, wxWindowID winid = wxID_ANY);
	~vimeMapGrid();

private:
	wxPoint toWxPoint(const vimePoint& p);
	vimePoint toVimePoint(const wxPoint& p);
	wxPoint firstGridPoint();

	void updateGrid(wxBitmap& bitmap);
	void updateWalls(wxBitmap& bitmap);

public:
	void redraw();
	void selectPoint(wxPoint selection);
	void deselectAll();
	void save(std::string filePath);
	void createSelectedWall();
	void destroySelectedWall();
	void setTexturePathSelectedWall(std::string path);

	bool isWallSelected();
	bool isPointSelected();

	void onPaint(wxPaintEvent& evt);
	void onDoubleClick(wxMouseEvent&);
	void onRightClick(wxMouseEvent& evt);
	void onDown(wxMouseEvent&);
	void onMotion(wxMouseEvent&);
	void onRightDouble(wxMouseEvent& evt);
};

class nothing_selected : public std::exception {
public:
	nothing_selected() noexcept : std::exception("nothing selected", 1) {}

private:
	nothing_selected(char const* const _Message) noexcept : exception(_Message, 1) {}
};