#include <sstream>
// include OpenGL
//#ifdef __WXMAC__
#include <stdio.h>
#include <string>
#include <fstream>
#include <wx/wxprec.h>
#include <array>
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
	PCPBorder(wxWindow* parent,bool b,ReadData* d, int size,int h);
	void setLastIndex(int i);
	void ReCreate();
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
	PCPPane(wxWindow* parent, int* args, ReadData* d,PCPBorder* l,int h);
	virtual ~PCPPane();
	void ReCreate();
	int getWidth();   
	int getHeight();  
	void solveTSP(double** v);   
	void solveAngle(double** v);  
	void setRate();        
	void refine(double** v);
	void reselect();
	void isVisible(bool b);
	void idplus();
	typedef std::pair<int,double*> ipair;
	typedef std::pair<int,double> apair;
	int clickid;
	bool anglemax = false;
	private:
	double* rate; 
	double sumlength;
	PCPBorder* last;
};

class PCPSub: public wxGLCanvas 
{
	wxGLContext* m_context;
	ReadData* data;
public:
	 PCPSub(wxWindow* parent, int l, ReadData* d, int size,int h);
	 virtual ~PCPSub();
	int getWidth();   
	int getHeight();  
	void setRate(int u, int l, double upper, double lower);
	void setLength(double p,double l);
	double getOriginalValue(int x);
	void setSumLength(double l, double w);
	void render(wxPaintEvent& evt);  
	void draw(int i,int height);   
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);  
	// events
	void mouseMoved(wxMouseEvent& event);   
	void mouseDown(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	DECLARE_EVENT_TABLE()
	/* data */
private:
	int upperatr, loweratr;
	double length;
	double prelength;
	double sumlength;
	double urate;
	double lrate;
	int layer;
	bool isclicked, isdruged, iscalc;
	double from;
	bool isUpper;
};

#endif