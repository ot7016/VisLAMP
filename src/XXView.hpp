#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <dirent.h>
#include <exception>
#include <sstream>

#include <wx/wxprec.h>
//#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
#include "wx/slider.h"
#include "wx/stattext.h"
#include "ReadData.hpp"
#include "AGI.hpp"
#include "wx/glcanvas.h"
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"



#ifndef xview
#define xview
class XXView : public wxGLCanvas{
public:
	XXView(wxWindow* parent,ReadData* d,Agi* a,int h);
	void setRate();
	int getWidth();
	int getHeight();
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void render(wxPaintEvent &event);

	DECLARE_EVENT_TABLE()
private:
	wxGLContext* m_context;
	ReadData* data;
	Agi* ag;
	double xrate,yrate;
};

#endif
