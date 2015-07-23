#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <dirent.h>
#include <exception>
#include <math.h>
#include <sstream>
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
//#include <vecLib/vecLib.h>

#include <wx/wxprec.h>
//#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
//#include "wx/slider.h"
#include "wx/stattext.h"
#include "wx/glcanvas.h"
#include "ReadData.hpp"
#include "PCP.hpp"
#include "ConstraintSolver2D.hpp"
//#else
#ifndef agi 
#define agi 

//#include <vecLib/vecLib.h>
	class Agi{
	public:
		Agi(ReadData* d);
		virtual ~Agi();
		void calprj();
		void cal2Mtr() ;
		int refine(float* _pre, float* _new,int index);
		float getB(int i,int j);
		float getXMax();
		float getYMax();
	private:
		ReadData* data;
		float *evalue;
		float** e;
		float** B; 
		float xmax ,xmin;  //初期値を適切に定めるのはあとで確認
 		float ymax ,ymin;

	};


	class AGIPane: public wxGLCanvas{
		wxGLContext* m_context;
		Agi* ag;
		ReadData* data;
		PCPPane* pcp;
	public:
		AGIPane(wxFrame* parent, int* args,ReadData* d, PCPPane* p);
		virtual ~AGIPane();
		void resized(wxSizeEvent& evt);
		int getWidth();
		int getHeight();
	void render(wxPaintEvent& evt);
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
	int getindex(int x, int y);
	void setRate();
 

	DECLARE_EVENT_TABLE()
private:
	int xrate ;
	int yrate ;
	int nowindex;
	float* _pre;
	float* _new;
};
#endif