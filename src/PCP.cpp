#include "PCP.hpp"
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#include <GLUT/glut.h>
//PCPを実装する場合 agi描画部分も独立させるべき
//それぞれをパネルに乗っけるのが無難か 大幅な書き換えがいる
using namespace std;
 
 
PCPPane::PCPPane(wxWindow* parent, int* args,ReadData* d ) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(800,600), wxFULL_REPAINT_ON_RESIZE)
{
    data = d;
    int atr = data->getatr();
    index = -1;
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
  // wxGridSizer* sizer =   new wxGridSizer(1,atr-1,getWidth()/(atr-1),getHeight());
    int len = getWidth()/(atr-1);
    for(int i = 0 ;i< atr-1;i++){
        PCPSub* sub =  new PCPSub(this,i,d,len);
        sub->setSumLength(atr-1,getWidth()-atr+1);
        sizer->Add(sub);
    }
     sumlength = atr-1;
    setRate(); 
    SetSizer(sizer);
    SetAutoLayout(true);
    // To avoid flashing on MSW
}
 
PCPPane::~PCPPane(){
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
    }
    int k = 0;
    for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
        PCPSub *current = (PCPSub *)node->GetData();
        current->setRate(k,k+1,rate[k],rate[k+1]);
        k++;
        }
    

 //        std::cerr << xrate << std::endl;
   //         std::cerr << yrate << std::endl;
}


void PCPPane::setIndex(int i){
    index = i;
    for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
            PCPSub *current = (PCPSub *)node->GetData();
            current->setIndex(i);
        }

}
void PCPPane::refine(float** v){
    //TSPを解く
     int atr = data->getatr();
     bool isTSP = data->isTSP();
     if(isTSP)
        solveTSP(v,atr);
     else
        solveAngle(v,atr);
   // wxWindowList & children = GetChildren();
    int len[atr-1];
    int k = 0;
    for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
        PCPSub *current = (PCPSub *)node->GetData(); 
        current->setSumLength(sumlength,getWidth());
        current->Refresh();
        len[k] = current->getWidth(); 
        k++;
        }
        for(int i = 0;i < atr-1;i++){
            std::cerr << len[i] << std::endl;
        }
         SetAutoLayout(true);
         Show();
}

void PCPPane::solveTSP(float** v,int atr){
    TSPsolver* ts = new TSPsolver(v,atr);
    ts->solve();
    data->setOrder(ts);
    sumlength = 0;
    int k = 0;
    if(data->isLenVar()){ 
        for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
            PCPSub *current = (PCPSub *)node->GetData();
            float l = ts->getlength(k+1);
            current->setLength(sumlength,l);
            sumlength = sumlength + l;
            int lindex = data->getOrder(k);
            int rindex = data->getOrder(k+1);
            current->setRate(lindex,rindex,rate[lindex],rate[rindex]);
            k++;
        }
    }
    else{
         for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
            PCPSub *current = (PCPSub *)node->GetData();
            current->setLength(k,1);
            int lindex = data->getOrder(k);
            int rindex = data->getOrder(k+1);
            current->setRate(lindex,rindex,rate[lindex],rate[rindex]);
            k++;
        }
        sumlength = atr-1;
    }

}
void PCPPane::solveAngle(float** v,int atr){
    float** v2 =new float*[atr] ;
    for(int i = 0;i<atr;i++){
        v2[i] = new float[2];
        float d = sqrt(pow(v[i][0],2)+pow(v[i][1],2));
        v2[i][0] = v[i][0]/d;
        v2[i][1] = v[i][1]/d;
    }
    vector<ipair> c1 ;
    vector<ipair> c2;
    vector<ipair> c3 ;
    vector<ipair> c4;
    //それぞれに場合分け
    for(int i = 0;i < atr;i++){
        if(v2[i][0]>0 && v2[i][1] >0)
            c1.push_back(ipair(i,v2[i]));
        else if(v2[i][0]>0 && v2[i][1] <0)
            c2.push_back(ipair(i,v2[i]));
        else if(v2[i][0]<0 && v2[i][1] <0)
            c3.push_back(ipair(i,v2[i]));
        else 
            c4.push_back(ipair(i,v2[i]));
    }
    struct {
        bool operator()(ipair& a,ipair& b){
            return a.second[0] < b.second[0];
        }
    } xlesspair;
    struct {
        bool operator()(ipair& a,ipair& b){
            return a.second[0] > b.second[0]; 
            }  
    }xgreaterpair;
    struct {
        bool operator()(ipair& a,ipair& b){
            return a.second[1] < b.second[1];   
        }
    }ylesspair;
    struct {
        bool operator()(ipair& a,ipair& b){
            return a.second[1] > b.second[1];       
        }
    }ygreaterpair;



    sort(c1.begin(),c1.end(),ygreaterpair);
    sort(c2.begin(),c2.end(),xgreaterpair);
    sort(c3.begin(),c3.end(),ylesspair);
    sort(c4.begin(),c4.end(),xlesspair);
    copy(c2.begin(),c2.end(),back_inserter(c1));
    copy(c3.begin(),c3.end(),back_inserter(c1));
    copy(c4.begin(),c4.end(),back_inserter(c1));
    int order[atr];
    for(int i = 0;i< atr ;i++){
        order[i] = c1.at(i).first;
    }
    data->setOrder(order);
 //   wxWindowList & children = GetChildren();
    int k = 0;   
    if(data->isLenVar()){
        sumlength = 0;
       // for(int i = 0 ; i <atr-1 ;i++){
           //length[i] = acos(c1.at(i).second[0]* c1.at(i+1).second[0] + c1.at(i).second[1]* c1.at(i+1).second[1]);
            //sumlength = sumlength + length[i];
        //}
  
        for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
            PCPSub *current = (PCPSub *)node->GetData();
            float l = acos(c1.at(k).second[0]* c1.at(k+1).second[0] + c1.at(k).second[1]* c1.at(k+1).second[1]);
            current->setLength(sumlength,l);
            int lindex = c1.at(k).first;
            int rindex = c1.at(k+1).first;
            current->setRate(lindex,rindex,rate[lindex],rate[rindex]);
            sumlength = sumlength + l;
            k++;
        }
    }
    else{
         for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
            PCPSub *current = (PCPSub *)node->GetData();
            current->setLength(k,1);
             int lindex = c1.at(k).first;
            int rindex = c1.at(k+1).first;
            current->setRate(lindex,rindex,rate[lindex],rate[rindex]);
            k++;
        }
        sumlength = atr-1;
    }

}

// some useful events to use
void PCPSub::mouseMoved(wxMouseEvent& event) {}
void PCPSub::mouseDown(wxMouseEvent& event) {}
void PCPSub::mouseWheelMoved(wxMouseEvent& event) {}
void PCPSub::mouseReleased(wxMouseEvent& event) {}
void PCPSub::rightClick(wxMouseEvent& event) {}
void PCPSub::mouseLeftWindow(wxMouseEvent& event) {}
void PCPSub::keyPressed(wxKeyEvent& event) {}
void PCPSub::keyReleased(wxKeyEvent& event) {}

PCPSub::PCPSub(wxWindow* parent, int num,ReadData* d,int size) :
    wxGLCanvas(parent, wxID_ANY, NULL, wxPoint(num*size,0), wxSize(size,600), wxFULL_REPAINT_ON_RESIZE) //wxsize あとで変更
{
    m_context = new wxGLContext(this);
    data = d;
    index = -1;
    length = 1; 
    prelength = num; 
    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

void PCPSub::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 
    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(topleft_x, bottomrigth_x, bottomrigth_y, topleft_y);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
}

int PCPSub::getWidth()
{
    return GetSize().x;
}
void PCPSub::setLength(float p,float l){
    prelength = p;
    length = l;
    //SetSize(l,getHeight());  
}
void PCPSub::setSumLength(float l,float w){
    sumlength = l;
    int size = (length/sumlength) *w;
    SetSize(size  ,getHeight());
    SetPosition(wxPoint((prelength/sumlength)*w,0));
}
void PCPSub::setIndex(int i){
    index = i;
}
 
int PCPSub::getHeight()
{
    return GetSize().y;
}
void PCPSub::setRate(int l, int r,float left,float right){
    leftindex = l;
    rightindex = r; 
    lrate = left;
    rrate = right;
}
 
 //本当は再描画のことも考えた関数設計にする
void PCPSub::render(wxPaintEvent& evt)
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
    //int dim = data-> getatr();
    //float len1 = 0;
   //float xwidth[dim]; 
    int ydown = getHeight()*7/8;
    int yup = getHeight()/8 ;
  //  for(int i = 0; i< dim;i++){
        //xwidth[i] = (len1/sumlength)*getWidth();
    	glVertex3f(getWidth(),ydown,0);
    	glVertex3f(getWidth(),yup,0); 
    glEnd();

  //  for(int i = 0 ;i< dim;i++){
        glRasterPos2d(getWidth()*0.5,ydown+40);
        string str = data->getAtrName(data->getOrder(rightindex));
        int size = (int)str.size();
        for(int j = 0;j< size;j++){
            char ic = str[j];
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,ic);
        }


    // 追加部分 点を書く
    glColor4f(0.0f,0.0f,1.0f,1.0f);
    glPointSize(10.0);
    glLineWidth(1);
    for(int i = 0; i< data->getnum();i++){   
        if(i != index)
            draw(i);                  
    }
    if(index >=0){
      glColor4f(1.0f,0.0f,0.0f,1.0f);
      glLineWidth(2); 
      draw(index);
  }
    glFlush();
    SwapBuffers();
}

void PCPSub::draw(int i){
    glBegin(GL_LINE_STRIP);
        float len = 0;
// for(int j = 0; j< data->getatr(); j++){
       // float l = (len/sumlength)*getWidth();  //rateの使用はあとで
        glVertex3f(0, (data->getDmax(leftindex) - data->getD(i,leftindex) ) *lrate + getHeight()/8 ,0); 
        glVertex3f(getWidth(), (data->getDmax(rightindex) - data->getD(i,rightindex) ) *rrate + getHeight()/8 ,0);  
    
     glEnd();
}

