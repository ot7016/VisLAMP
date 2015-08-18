#include <sstream>
// include OpenGL
//#ifdef __WXMAC__
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <wx/wxprec.h>
#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/glcanvas.h"
#include "ReadData.hpp"
#include "TSP.hpp"
#ifndef pc 
#define pc

class PCPPane: public wxGLCanvas
{
    wxGLContext*	m_context;
    ReadData* data;
public:
	PCPPane(wxWindow* parent, int* args, ReadData* d);
	virtual ~PCPPane();
	void resized(wxSizeEvent& evt);
	int getWidth();
	int getHeight();
	void render(wxPaintEvent& evt);
	void draw(int i);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	void refine(float** v);
	void solveTSP(float** v,int atr);
	void solveAngle(float** v, int atr);
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
	typedef std::pair<int,float*> ipair;
 
	DECLARE_EVENT_TABLE()
private:
	int index;
	float* rate; 
	float* length;
	float sumlength;
};
#endif