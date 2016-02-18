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
		void prjpush();
		void cal2Mtr() ;
		int refine(double* _pre, double* _new,int index);
		void backprj();
		double getB(int i,int j);
		double getV(int i,int j);
		void writeprojection();
		void writeagicood();
		void setdelta(double d);
		typedef std::pair<double,double> prj;
		std::vector<prj> ee; //射影行列(新)
		double xmax ,xmin;  
 		double ymax ,ymin;
	private:

		ReadData* data;
		std::stack<std::vector<prj> > prjstack; //射影行列のスタック 
		double delta = 0.5;
		double* B; 
		
 		double* v; 
 	    int writenum = 0;

	};
	struct NodeLog{
		NodeLog(){}
    	NodeLog( int i_, double x_, double y_ ) : index(i_),x(x_),y(y_){}
    	int index;
    	double x,y;  // [0.0f, 1.0f]
    };


	class AGIPane: public wxGLCanvas{
		wxGLContext* m_context;
		MatrixView* md;
		
		ReadData* data;
		PCPPane* pcp;
	public:
		AGIPane(wxWindow* parent, int* args,ReadData* d, PCPPane* p,int h);
		virtual ~AGIPane();
		void setMV(MatrixView* m);
		void ReCreate();
		void Setting();
		int getWidth();
	void render(wxPaintEvent& evt);
	void prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y);
	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	int getindex(double x, double y);
	void setRate();
	void setdelta(double d);
	void calcagain(double x, double y);
	void calRange(int x2, int y2);
	void calPoly();
	void undo();
	double enlarge(double p);
	double ensmall(double x);
	void drawcoodname(int i, int w);
	void drawaxispca(int index);
	void addLog(int index,int x, int y);
	void saveLog();
	void loadLog();

	Agi* ag;
	bool isPoly = false;
	std::vector<NodeLog > logvector; 
 

	DECLARE_EVENT_TABLE()
private:
	double xyrate;
	int nowindex;
	double* _pre;
	double* _new;
	bool isMoved, isDrug, iscalc;
	bool polystart = false;
	bool rangeselect,polyselect;
	bool logmode;
	int xfrom, yfrom;
	int xto, yto;
	int clickid;
	int logid ;
	vector<pair<int,int> > polyvector;
	pair<int,int>  polynow;
	const int coodrate = 3;  
};
#endif