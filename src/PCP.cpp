#include "PCP.hpp"
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#include <GLUT/glut.h>
//PCPを実装する場合 agi描画部分も独立させるべき
//それぞれをパネルに乗っけるのが無難か 大幅な書き換えがいる
using namespace std;
 
 
PCPPane::PCPPane(wxWindow* parent, int* args,ReadData* d,PCPBorder* l) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(600,550), wxFULL_REPAINT_ON_RESIZE)
{
    data = d;
    last = l;
    int atr = data->getatr();
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sumlength = atr-1;
    int len = getHeight()/(atr-1);
    
    for(int i = 0 ;i< atr-1;i++){
        PCPSub* sub =  new PCPSub(this,i,d,len);
        sub->setSumLength(sumlength,getHeight());
        sizer->Add(sub);
    }
    setRate(); 
    last->setLastIndex(atr-1);
    SetSizer(sizer);
    SetAutoLayout(true);
    // To avoid flashing on MSW
}
 
PCPPane::~PCPPane(){
    delete[] rate;
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
    rate = new double[dim];
    for(int i = 0;i< dim;i++ ){
        rate[i] = (getWidth()*3)/(4*(data->getDmax(i)-data->getDmin(i)));
    }
    int k = 0;
    for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
            PCPSub *current = (PCPSub *)node->GetData();
            current->setRate(k,k+1,rate[k],rate[k+1]);
        
        k++;
        }
}

void PCPPane::refine(double** v){
    //TSPを解く
     int atr = data->getatr();
     bool isTSP = data->isTSP();
     if(isTSP)
        solveTSP(v,atr);
     else
        solveAngle(v,atr);
    for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
        PCPSub *current = (PCPSub *)node->GetData(); 
        current->setSumLength(sumlength,getHeight());
        }
         SetAutoLayout(true);
         Refresh();
}
void PCPPane::reselect(){
    auto parent = GetGrandParent();
    parent->Refresh(); 
}

void PCPPane::solveTSP(double** v,int atr){
    TSPsolver* ts = new TSPsolver(v,atr);
    ts->solve();
    data->setOrder(ts);
    sumlength = 0;
    int k = 0;
    if(data->isLenVar()){ 
        for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){

                PCPSub *current = (PCPSub *)node->GetData();
                double l = ts->getlength(k+1);
                current->setLength(sumlength,l);
                sumlength = sumlength + l;
                int lindex = data->getOrder(k);
                int rindex = data->getOrder(k+1);
                current->setRate(lindex,rindex,rate[lindex],rate[rindex]);
            
            k++;
        }
          last->setLastIndex(data->getOrder(atr-1));
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
          last->setLastIndex(data->getOrder(atr-1));
        sumlength = atr-1;

    }

}
void PCPPane::solveAngle(double** v,int atr){
    double** v2 =new double*[atr] ;
    for(int i = 0;i<atr;i++){
        v2[i] = new double[2];
        double d = sqrt(pow(v[i][0],2)+pow(v[i][1],2));
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
        for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
                PCPSub *current = (PCPSub *)node->GetData();
                double l = acos(c1.at(k).second[0]* c1.at(k+1).second[0] + c1.at(k).second[1]* c1.at(k+1).second[1]);
                current->setLength(sumlength,l);
                int lindex = data->getOrder(k);
                int rindex = data->getOrder(k+1);
                current->setRate(lindex,rindex,rate[lindex],rate[rindex]);
                sumlength = sumlength + l;
            k++;
        }
        last->setLastIndex(data->getOrder(atr-1));
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
              last->setLastIndex(data->getOrder(atr-1));
        
        sumlength = atr-1;
    }
}

// some useful events to use
void PCPSub::mouseMoved(wxMouseEvent& event) {
    if(isclicked){
        isdruged = true;
        if(!iscalc){
            iscalc =true;
            int x = event.GetX();
            int atr;
            double rate;
            if(isUpper){
                atr = upperatr;
                rate =urate;
            }
            else{
                atr = loweratr;
                rate =lrate;
            }
            double to =  data->getDmax(atr) - x/rate;
            vector<double> v;
            v.push_back(from);
            v.push_back(to);
            data->setSIndex(atr,v);
            auto parent = GetGrandParent()->GetParent();
            parent->Refresh(); 
            iscalc =false;
        }
    }
}
void PCPSub::mouseDown(wxMouseEvent& event) {
    //あとで整理
    data->clearSIndex();
    auto parent = GetGrandParent();
    parent->Refresh();
    int y = event.GetY();
    if(y<getHeight()/2) {     //上がクリックされたとき
        setFrom(event.GetX(),true);
    } 
    else {  //下がクリックされたとき
        setFrom(event.GetX(),false);
    }
}

void PCPSub::setFrom(int x,bool u){
    int atr;
    double rate;
    isUpper = u;
    if(u){
        atr = upperatr;
        rate = urate;
    }
    else{
        atr = loweratr;
        rate = lrate;
    }
    from =  data->getDmax(atr) - x/rate;
    isclicked = true;
}



void PCPSub::mouseWheelMoved(wxMouseEvent& event) {}
void PCPSub::mouseReleased(wxMouseEvent& event) {
    isclicked = false;
    isdruged = false;
}
void PCPSub::rightClick(wxMouseEvent& event) {}
void PCPSub::mouseLeftWindow(wxMouseEvent& event) {}

PCPSub::PCPSub(wxWindow* parent,int l, ReadData* d, int size) :
    wxGLCanvas(parent, wxID_ANY, NULL, wxPoint(0,l*size), wxSize(590,size), wxFULL_REPAINT_ON_RESIZE) //wxsize あとで変更
{
    m_context = new wxGLContext(this);
    data = d;
    length = 1; 
    prelength = l;
    layer = l; 
    iscalc =false;
    isclicked = false;
    isdruged = false;
    from = -999;
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
void PCPSub::setLength(double p,double l){
    prelength = p;
    length = l; 
}
void PCPSub::setSumLength(double l,double w){
    sumlength = l;
    int size = (length/sumlength) *w;
    SetSize(getWidth(),size);
    SetPosition(wxPoint(0,(prelength/sumlength)*w));
}

int PCPSub::getHeight()
{
    return GetSize().y;
}
void PCPSub::setRate(int u, int l,double upper,double lower){
    upperatr = u;
    loweratr = l; 
    urate = upper;
    lrate = lower;
}
 
 //本当は再描画のことも考えた関数設計にする
void PCPSub::render(wxPaintEvent& evt)
{   
    if(!IsShown() || getHeight()<2 ) return;
   
    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int width = getWidth();
    int height = getHeight();
    // ------------- draw some 2D ----------------
    prepare2DViewport(0,0,width, height);
    glLoadIdentity();
 
    // white background
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0,0,0);
    glVertex3f(width,0,0);
    glVertex3f(width,height,0);
    glVertex3f(0,height,0);
    glEnd();

    glColor4f(0.0f,0.0f,0.0f,1.0f);
    glBegin(GL_LINES);
    int xright = width*3/4;
    int xleft = 0 ;
    glVertex3f(xleft, 0,0);
    glVertex3f(xright,0,0); 
    glEnd();
    glRasterPos2d(xright+10,10);
    string str = data->getAtrName(upperatr);
    int size = (int)str.size();
    for(int j = 0;j< size;j++){
        char ic = str[j];
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,ic);
    }
    vector<int> selected = data->getSIndex();
    vector<int> notselected = data->getNSIndex();
    

    glColor4f(0.5f,0.6f,0.9f,1.0f);
    glLineWidth(1);
     glBegin(GL_LINES);
    for(int i: notselected ) {   
        draw(i,height);                  
    }
    glEnd();
    if(!selected.empty()){
      glColor4f(0.8f,0.3f,0.6f,1.0f);
      glLineWidth(2);
      glBegin(GL_LINES);
      for(int i: selected) {
        draw(i,height);
    }
     glEnd();

  }
    glFlush();
    SwapBuffers();
 
}

void PCPSub::draw(int i,int height){
    
    glVertex3f( (data->getDmax(upperatr) - data->getD(i,upperatr) ) *urate,0 ,0); 
    glVertex3f( (data->getDmax(loweratr) - data->getD(i,loweratr) ) *lrate,height,0);      
    }
PCPBorder::PCPBorder(wxWindow* parent,bool b,ReadData* d, int size) :
wxGLCanvas(parent, wxID_ANY, NULL, wxPoint(0,0), wxSize(590,24), wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);
    data = d;
    islast = b;
    atr = d->getatr()-1;
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}
void PCPBorder::setLastIndex(int i){
    atr  = i;
}
int PCPBorder::getWidth(){
    return GetSize().x;
}
 
int PCPBorder::getHeight(){
    return GetSize().y;
}

void PCPBorder::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
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

void PCPBorder::render(wxPaintEvent& evt)
{
    if(!IsShown()) {
        return;
    }
    wxGLCanvas::SetCurrent(*m_context);
 
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    // ------------- draw some 2D ----------------
    int width = getWidth();
    int height = getHeight();
    prepare2DViewport(0,0,width, height);
    glLoadIdentity();
 
    // white background
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0,0,0);
    glVertex3f(width,0,0);
    glVertex3f(width,height,0);
    glVertex3f(0,height,0);
    glEnd();

    if(islast){
        glColor4f(0.0f,0.0f,0.0f,1.0f);
        glBegin(GL_LINES);
        int xright = width*3/4;
        int xleft = 0 ;
        glVertex3f(xleft,0,0);
        glVertex3f(xright,0,0); 
        glEnd();
        glRasterPos2d(xright+10,12);
        string str = data->getAtrName(atr);
            std::cerr << str<< std::endl;
        int size = (int)str.size();
        for(int j = 0;j< size;j++){
            char ic = str[j];
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,ic);
        }
    }
    glFlush();
    SwapBuffers();
}