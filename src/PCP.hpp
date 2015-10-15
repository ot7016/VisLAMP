#include <sstream>
// include OpenGL
//#ifdef __WXMAC__
#include <stdio.h>
#include <string>
#include <fstream>
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

class PCPBorder: public wxGLCanvas
{
	wxGLContext* m_context;
	ReadData* data;
public:
	PCPBorder(wxWindow* parent,bool b,ReadData* d, int size);
	void setLastIndex(int i);
	int getWidth();   
	int getHeight(); 
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);  
	void render(wxPaintEvent& evt);
	DECLARE_EVENT_TABLE()
private:
	bool islast;
	int atr;

};

class PCPPane: public wxPanel{
	
	ReadData* data;
public:
	PCPPane(wxWindow* parent, int* args, ReadData* d,PCPBorder* l);
	virtual ~PCPPane();
	int getWidth();   
	int getHeight();  
	void solveTSP(float** v, int atr);   
	void solveAngle(float** v, int atr);  
	void setRate();        
	void refine(float** v);
	void reselect();
	typedef std::pair<int,float*> ipair;
	private:
	float* rate; 
	float sumlength;
	PCPBorder* last;
};

class PCPSub: public wxGLCanvas 
{
	wxGLContext* m_context;
	ReadData* data;
public:
	 PCPSub(wxWindow* parent, int l, ReadData* d, int size);
	int getWidth();   
	int getHeight();  
	void setRate(int u, int l, float upper, float lower);
	void setLength(float p,float l);
	void setFrom(int x, bool l);
	void setSumLength(float l, float w);
	void render(wxPaintEvent& evt);  
	void draw(int i);   
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);  
	// events
	void mouseMoved(wxMouseEvent& event);   
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);	
	DECLARE_EVENT_TABLE()
	/* data */
private:
	int upperatr;
	int loweratr;
	float length;
	float prelength;
	float sumlength;
	float urate;
	float lrate;
	int layer;
	bool isclicked;
	bool isdruged;
	bool iscalc;
	float from;
	bool isUpper;
};

#endif