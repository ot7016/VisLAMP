#include <stdio.h>
#include <vector>
#include <string>

#include <wx/wxprec.h>
#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
#include <wx/grid.h>
#include "wx/stattext.h"
#include "ReadData.hpp"

#ifndef mview
#define mview
class MatrixView : public wxGrid{
private:
	ReadData* data;
	wxStaticText* name;
	wxGridSizer* sizer ;
	int nowrow;
public:
	MatrixView(wxWindow* parent, ReadData* d);
	~MatrixView();
	void Create();
	void ReCreate();
	void setText(int index);
};
#endif
