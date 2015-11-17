#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <exception>
#include <math.h>
#include <sstream>
#include <stack>
#include <tuple>
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
 #include <Accelerate/Accelerate.h>

#include <wx/wxprec.h>
//#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/glcanvas.h"
#include "ReadData.hpp"
#include "PCP.hpp"
#include "ConstraintSolver2D.hpp"
#include "MatrixView.hpp"
//#else
#ifndef agi 
#define agi 
	class Agi{
	public:
		Agi(ReadData* d);
		virtual ~Agi();
		void ReCreate();
		void calprj();
		void cal2Mtr() ;
		int refine(double* _pre, double* _new,int index);
		void backprj();
		double getB(int i,int j);
		double getXMax();
		double getYMax();
		double getXMin();
		double getYMin();
		double getV(int i,int j);
		void setdelta(double d);
		typedef std::pair<double,double> prj;
		std::vector<prj> ee; //射影行列(新)
	private:

		ReadData* data;
		std::stack<std::vector<prj> > prjstack; //射影行列のスタック 
		double delta;
		double* B; 
		double xmax ,xmin;  
 		double ymax ,ymin;
 		double* v;

	};


	class AGIPane: public wxGLCanvas{
		wxGLContext* m_context;
		MatrixView* md;
		Agi* ag;
		ReadData* data;
		PCPPane* pcp;
	public:
		AGIPane(wxWindow* parent, int* args,ReadData* d, PCPPane* p,MatrixView* m);
		virtual ~AGIPane();
		void ReCreate();
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
	int getindex(double x, double y);
	void setRate();
	void setdelta(double d);
	void calcagain(double x, double y);
	void calRange(int x2, int y2);
	void undo();
	void drawcoodname(int i, int w, int h);
 

	DECLARE_EVENT_TABLE()
private:
	double xrate ;
	double yrate ;
	int nowindex;
	double* _pre;
	double* _new;
	bool isMoved;
	bool isDrug;
	bool iscalc;
	bool rangeselect;
	int xfrom;
	int yfrom;
	int xto;
	int yto;
	int clickid;
};
#endif