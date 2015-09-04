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
	int getWidth();   //master
	int getHeight();  //master
	void solveTSP(float** v, int atr);   //master
	void solveAngle(float** v, int atr); //master
	void setIndex(int i);  //master 
	void setRate();        //master 
	void refine(float** v);
	typedef std::pair<int,float*> ipair;
	// events
	private:
	int index;
	float* rate; 
	//float* length;
	float sumlength;
};

class PCPSub: public wxGLCanvas 
{
	wxGLContext* m_context;
	ReadData* data;
public:
	 PCPSub(wxWindow* parent, int num, ReadData* d, int size);
	//~ PCPSub();
	int getWidth();   
	int getHeight();  
	void setRate(int l, int r, float left, float right);
	void setLength(float p,float l);
	void setSumLength(float l, float w);
	void setIndex(int i);
	void render(wxPaintEvent& evt);  //sub
	void draw(int i);   //sub
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);  //sub
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
	int leftindex;
	int rightindex;
	float length;
	float prelength;
	float sumlength;
	float rrate;
	float lrate;
	int index;
};





/*class PCPPane: public wxGLCanvas
{
    wxGLContext*	m_context;
    ReadData* data;
public:
	PCPPane(wxWindow* parent, int* args, ReadData* d);
	virtual ~PCPPane();
	int getWidth();   //master
	int getHeight();  //master
	void render(wxPaintEvent& evt);  //sub
	void draw(int i);   //sub
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);  //sub
	void refine(float** v); //master
	void solveTSP(float** v,int atr);   //master
	void solveAngle(float** v, int atr); //master
	void setIndex(int i);  //master 
	void setRate();        //master 
	typedef std::pair<int,float*> ipair;
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
private:
	int index;
	float* rate; 
	float* length;
	float sumlength;
};
*/
#endif


// PCPPaneと PCPsubにわける　masterが全体のパネル subが軸と軸の間を担当