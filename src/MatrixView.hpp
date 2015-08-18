#include <stdio.h>
#include <vector>
#include <string>

#include <wx/wxprec.h>
#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "ReadData.hpp"

#ifndef mview
#define mview
class MatrixView : public wxPanel{
private:
	ReadData* data;
	wxGridSizer* sizer;
public:
	MatrixView(wxFrame* frame, ReadData* d);
	~MatrixView();
	void setText(int index);
};
#endif
