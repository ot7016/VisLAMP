#include "PCP.hpp"
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#include "ReadData.hpp"
#include "TSP.hpp"
#include <GLUT/glut.h>
//PCPを実装する場合 agi描画部分も独立させるべき
//それぞれをパネルに乗っけるのが無難か 大幅な書き換えがいる
using namespace std;
 
 
// some useful events to use
void PCPPane::mouseMoved(wxMouseEvent& event) {}
void PCPPane::mouseDown(wxMouseEvent& event) {}
void PCPPane::mouseWheelMoved(wxMouseEvent& event) {}
void PCPPane::mouseReleased(wxMouseEvent& event) {}
void PCPPane::rightClick(wxMouseEvent& event) {}
void PCPPane::mouseLeftWindow(wxMouseEvent& event) {}
void PCPPane::keyPressed(wxKeyEvent& event) {}
void PCPPane::keyReleased(wxKeyEvent& event) {}
 
PCPPane::PCPPane(wxWindow* parent, int* args,ReadData* d ) :
    wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxSize(800,600), wxFULL_REPAINT_ON_RESIZE)
{
 	m_context = new wxGLContext(this);
    data = d;
    setRate();
    int atr = data->getatr();
    order = new int[atr];
    length = new float[atr];
    index = -1;
    for(int i = 0; i< atr; i++){
        order[i] = i;
        length[i] = 1;
    }
    
    sumlength = atr;
    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}
 
PCPPane::~PCPPane(){
	delete m_context;
    delete[] order;
    delete[] length;
}
 
void PCPPane::resized(wxSizeEvent& evt)
{
//	wxGLCanvas::OnSize(evt);
   // Refresh();
}
 
void PCPPane::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 
    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
 
    gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}
 
 
int PCPPane::getWidth()
{
    return GetSize().x;
}
 
int PCPPane::getHeight()
{
    return GetSize().y;
}

void PCPPane::setRate(){
   //各属性ごとに倍率があるので
    int dim = data->getatr();
    rate = new float[dim];
    for(int i = 0;i< dim;i++ ){
        rate[i] = (getHeight()*3)/(4*(data->getDmax(i)-data->getDmin(i)));
        // cerr << rate[i] << endl;
    }
 //        std::cerr << xrate << std::endl;
   //         std::cerr << yrate << std::endl;
}

/*
int PCPPane::getindex(int x, int y){
    
    
    int index = -1;
    const float min = 25;
    float minnow = min; 
    float d; 
    for(int i = 0;i< data->getdim();i++){
        d = pow( data->getA(i,0)* xrate -x,2)+pow( data->getA(i,1)* yrate - y,2);
        if(d < minnow){
            minnow = d;
            index = i;
        }
    }
    return index;
    
    return -1;
} 
*/

void PCPPane::setIndex(int i){
    index = i;
}
void PCPPane::refine(float** v){
    //TSPを解く
     int atr = data->getatr();
    TSPsolver* ts = new TSPsolver(v,atr);
    ts->solve();
    order = new int[atr];
    for(int i = 0; i< atr;i++){
        order[i] = ts->getorder(i);
     }

    sumlength = 0;
     length = new float[atr];
    for(int i = 0;i < atr-1; i++){
        length[i] = ts->getlength(i+1);
        sumlength += length[i];
    }
    length[atr-1] = 0;
     Refresh();
    // Update();
}



 //本当は再描画のことも考えた関数設計にする
void PCPPane::render(wxPaintEvent& evt)
{
    if(!IsShown()) return;

    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    // ------------- draw some 2D ----------------
    prepare2DViewport(0,0,getWidth(), getHeight());
    glLoadIdentity();
 
    // white background
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0,0,0);
    glVertex3f(getWidth(),0,0);
    glVertex3f(getWidth(),getHeight(),0);
    glVertex3f(0,getHeight(),0);
    glEnd();

     
    glColor4f(0.0f,0.0f,0.0f,1.0f);
    glBegin(GL_LINES);
    int dim = data-> getatr();
    //float xrate = getWidth()/sumlength;
    float len1 = 0;
    float xwidth[dim]; 
    int ydown = getHeight()*7/8;
    int yup = getHeight()/8 ;
    for(int i = 0; i< dim;i++){  //要素数
        xwidth[i] = (len1/sumlength)*getWidth();
        //int x = getWidth() * i / dim;
    	glVertex3f(xwidth[i],ydown,0);
    	glVertex3f(xwidth[i],yup,0);
        len1 = len1 + length[i];  
    }
    glEnd();

    for(int i = 0 ;i< dim;i++){
        glRasterPos2d(xwidth[i],ydown+40);
        string str = data->getAtrName(order[i]);
        int size = (int)str.size();
        for(int j = 0;j< size;j++){
            char ic = str[j];
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,ic);
        }
    }

    

    // 追加部分 点を書く
    glColor4f(0.0f,0.0f,1.0f,1.0f);
    glPointSize(10.0);
    //glBegin(GL_POINTS);

    for(int i = 0; i< data->getnum();i++){
       
        if(i != index)
            draw(i);     
    }
    if(index >=0){
      glColor4f(1.0f,0.0f,0.0f,1.0f); 
      draw(index);
  }

    //glEnd();
    glFlush();
    SwapBuffers();
}

void PCPPane::draw(int i){
    glBegin(GL_LINE_STRIP);
        float len = 0;
    for(int j = 0; j< data->getatr(); j++){
        //if(y<0)
         //  y = data->getDmax(j)-y;
        int index = order[j];
        float l = (len/sumlength)*getWidth();
        glVertex3f(l, (data->getDmax(index) - data->getD(i,index) ) *rate[index] + getHeight()/8 ,0);  
        len = len + length[j];   
    }
     glEnd();
}

