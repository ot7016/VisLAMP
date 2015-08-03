#include <sstream>
// include OpenGL
//#ifdef __WXMAC__
#include <stdio.h>
#include <string>
#include <fstream>
#include <dirent.h>
#include <exception>
#include <sstream>
//#include <vecLib/vecLib.h>

#include <wx/wxprec.h>
#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
//#include "wx/slider.h"
#include "wx/stattext.h"
#include "wx/glcanvas.h"
#include "ReadData.hpp"
#ifndef pc 
#define pc

class PCPPane: public wxGLCanvas
{
    wxGLContext*	m_context;
    ReadData* data;
public:
	PCPPane(wxFrame* parent, int* args, ReadData* d);
	virtual ~PCPPane();
	void resized(wxSizeEvent& evt);
	int getWidth();
	int getHeight();
	void render(wxPaintEvent& evt);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void refine(float** v);
	int solveTSP(float** v);
	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
	void setIndex(int i);
	//int getindex(int x, int y);
	void setRate();
 
	DECLARE_EVENT_TABLE()
private:
	int index;
	float* rate; 
	int* order;
	float* length;
	float sumlength;
};
#endif