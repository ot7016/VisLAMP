#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <exception>
#include <math.h>
#include <sstream>
#include <stack>
#include <array>
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
//#include <vecLib/vecLib.h>

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
		void calprj();
		void cal2Mtr() ;
		int refine(float* _pre, float* _new,int index);
		float getB(int i,int j);
		float getXMax();
		float getYMax();
		float getXMin();
		float getYMin();
		void setdelta(float d);
	private:
		ReadData* data;
		std::stack<float**> pjstack; //射影行列のスタック 
		float delta;
		float** e;   //射影行列
		//std::array< std::array<>>
		float** B; 
		float xmax ,xmin;  //初期値を適切に定めるのはあとで確認
 		float ymax ,ymin;

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
	int getindex(float x, float y);
	void setRate();
	void setdelta(float d);
	void calcagain(float x, float y);
	void undo();
 

	DECLARE_EVENT_TABLE()
private:
	float xrate ;
	float yrate ;
	int nowindex;
	float* _pre;
	float* _new;
	bool isMoved;
	bool isDrug;
};
#endif