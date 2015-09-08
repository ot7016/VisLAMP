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




class PCPPane: public wxPanel{
	
	ReadData* data;
public:
	PCPPane(wxWindow* parent, int* args, ReadData* d);
	virtual ~PCPPane();
	int getWidth();   
	int getHeight();  
	void solveTSP(float** v, int atr);   
	void solveAngle(float** v, int atr); 
	void setIndex(int i);  
	void setRate();        
	void refine(float** v);
	typedef std::pair<int,float*> ipair;
	// events
	private:
	int index;
	float* rate; 
	float sumlength;
};

class PCPSub: public wxGLCanvas 
{
	wxGLContext* m_context;
	ReadData* data;
public:
	 PCPSub(wxWindow* parent, int l, ReadData* d, int size);
	int getWidth();   
	int getHeight();  
	void setRate(int l, int r, float left, float right);
	void setLength(float p,float l);
	void setSumLength(float l, float w);
	void setIndex(int i);
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
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
	
	DECLARE_EVENT_TABLE()
	/* data */
private:
	int leftatr;
	int rightatr;
	float length;
	float prelength;
	float sumlength;
	float rrate;
	float lrate;
	int index;
	int layer;
};

/*
class PCPLast: public wxGLCanvas{
	wxGLContext* m_context;
	ReadData* data;
public:
	PCPLast(wxWindow* parent,int anum,ReadData d, int size);
	void render(wxPaintEvent& evt); 
};
*/

// 最後のだけ処理が変わるので　継承クラスを作ったほうがよさそう 継承元の関数に virtual をつけることで　元のキャストでも継承先の関数が利用可能

#endif