#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <dirent.h>
#include <exception>
#include <sstream>
#include <list>


#include <wx/wxprec.h>
//#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
#include "wx/slider.h"
#include "wx/stattext.h"
#include "wx/glcanvas.h"
#include "ReadData.hpp"
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"

#ifndef histv
#define histv
using namespace std;
class HistView :public wxGLCanvas 
{
	wxGLContext* m_context;
	ReadData* data;
	wxSlider* vcslider ;
	int* hist;
	list<int> selected;
	list<int> notselected;
	double xrate;
	double yrate;
	double distrate;
	int histlen;
	bool iscalc,isdruged,isclicked;
public:
	HistView(wxWindow* parent, wxSlider* vc, ReadData* d);
	void setRate();
	void Setting();
	void setValue(double r2);
	void reset();
	void ReCreate();
	int getWidth();
	int getHeight();
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);  
	void mouseMoved(wxMouseEvent& event);   
	void mouseDown(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void render(wxPaintEvent& evt);  
	DECLARE_EVENT_TABLE()

};
#endif