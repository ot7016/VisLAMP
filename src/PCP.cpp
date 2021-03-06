#include "PCP.hpp"
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#include <GLUT/glut.h>
//#include <FTGL/ftgl.h>
//PCPを実装する場合 agi描画部分も独立させるべき
using namespace std;
 
 //できればサイズをmacbook用とiMac用の両方を用意
PCPPane::PCPPane(wxWindow* parent, int* args,ReadData* d,PCPBorder* l,int h) :
    wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(100*h,100*(h-1)+50), wxFULL_REPAINT_ON_RESIZE)
{
    data = d;
    last = l;
    clickid = 1;
    const int atr = data->atr;
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sumlength = atr-1;
   const int len = getHeight()/(atr-1);
    
    for(int i = 0 ;i< atr-1;i++){
        PCPSub* sub =  new PCPSub(this,i,d,len,h);
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
　
//dataを読み込んだあとの再描画
void PCPPane::ReCreate(){
    DestroyChildren();
    const int width = getWidth();
    const int height = getHeight();
    clickid = 1;
    const int atr = data->atr;
    wxSizer* sizer = GetSizer();
    sumlength = atr-1;
    anglemax = false;
    const int len = height/(atr-1);
    
    for(int i = 0 ;i< atr-1;i++){
        PCPSub* sub =  new PCPSub(this,i,data,len,width/100);
        sub->setSumLength(sumlength,height);
        sizer->Add(sub);
    }
    setRate(); 
    last->setLastIndex(atr-1);
}
int PCPPane::getWidth()
{
    return GetSize().x;
}
 
int PCPPane::getHeight()
{
    return GetSize().y;
}

void PCPPane::idplus(){
    clickid = clickid+2;
}
void PCPPane::setRate(){
   //各属性ごとに倍率があるので
    const int atr = data->atr;
    rate = new double[atr];
    for(int i = 0;i< atr;i++ ){
        rate[i] = (getWidth()*3)/(4*(data->Dmax[i]-data->Dmin[i]));
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
     if(data->isTSP)
        solveTSP(v);
     else
        solveAngle(v);
    for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
        PCPSub *current = (PCPSub *)node->GetData(); 
        current->setSumLength(sumlength,getHeight());
    }
    Refresh();
}
void PCPPane::reselect(){
    auto parent = GetGrandParent();
    parent->Refresh(); 
}

void PCPPane::isVisible(bool b){
    for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
        PCPSub *current = (PCPSub *)node->GetData();
        current->Show(b);
    }
}

void PCPPane::solveTSP(double** v){
    const int atr = data->atr;
    TSPsolver* ts = new TSPsolver(v,atr);
    ts->solve();
    data->setOrder(ts);
    sumlength = 0;
    int k = 0;
    if(data->isLenVar){ 
        for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
            PCPSub *current = (PCPSub *)node->GetData();
            double l = ts->getlength(k+1);
            current->setLength(sumlength,l);
            sumlength = sumlength + l;
            int lindex = data->order[k];
            int rindex = data->order[k+1];
            current->setRate(lindex,rindex,rate[lindex],rate[rindex]);
            
            k++;
        }
          last->setLastIndex(data->order[atr-1]);
    }
    else{
         for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
            PCPSub *current = (PCPSub *)node->GetData();
            current->setLength(k,1);
            int lindex = data->order[k];
            int rindex = data->order[k+1];
            current->setRate(lindex,rindex,rate[lindex],rate[rindex]);
            k++;
        }
          last->setLastIndex(data->order[atr-1]);
        sumlength = atr-1;

    }

}
void PCPPane::solveAngle(double** v){
    // 多分全面的に書き換え
    const int atr = data->atr;
    const double pi2 = 2* M_PI;
    vector< apair > angle;
    for(int i = 0 ; i < atr; i++){
       const double r2 = pow(v[i][0],2)+ pow(v[i][1],2);
       const double a = acos(v[i][0] / sqrt(r2));
        if(v[i][1] < 0 )
            angle.push_back ( apair (i, pi2 - a) );
        else 
            angle.push_back(apair (i, a ) );
    }
    struct 
        { bool operator()(apair& a, apair& b){
            return a.second < b.second;
        }
    }lesspair;
    //これで角度でソートする
    sort(angle.begin(),angle.end(),lesspair);

    double angledif[atr];
    for(int i = 0; i< atr-1;i++){
        angledif[i] = angle.at(i+1).second - angle.at(i).second;
    } 
    angledif[atr-1] = pi2 + angle.at(0).second  - angle.at(atr-1).second ;
    int maxindex = 0;
    double max = angledif[0];
    if(anglemax){
        for(int i = 0 ;i < atr;i++){
            if(angledif[i] > max){
                maxindex = i;
                max = angledif[i];
            }    
        }
    }
    else{
    //応急処置的に最も幅が近いところで切る方法も
        for(int i = 0 ;i < atr;i++){
            if(angledif[i] < max){
                maxindex = i;
                max = angledif[i];
            }       
        }
    }

    int order[atr];
    double angledif2[atr];
    int j = 0;
    for(int i = maxindex+1; i < atr; i++ ){
        order[j] = angle.at(i).first;
        angledif2[j] = angledif[i]; 
        j++;
    }
    for(int i = 0;i < maxindex+1 ;i++){
        order[j] = angle.at(i).first;
        angledif2[j] = angledif[i];
        j++;
    }
    data->setOrder(order);

    int k = 0;   // 0 <= k < m -1  
    if(data->isLenVar){
        sumlength = 0;
        for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
            PCPSub *current = (PCPSub *)node->GetData();
            //double l = acos(c1.at(k).second[0]* c1.at(k+1).second[0] + c1.at(k).second[1]* c1.at(k+1).second[1]);
            const double l =  angledif2[k];
            current->setLength(sumlength,l);
            const int lindex = data->order[k];
            const int rindex = data->order[k+1];
            current->setRate(lindex,rindex,rate[lindex],rate[rindex]);
            sumlength = sumlength + l;
            k++;
        }
        last->setLastIndex(data->order[atr-1]);
    }
    else{
         for ( wxWindowList::Node *node = m_children.GetFirst(); node; node = node->GetNext() ){
            PCPSub *current = (PCPSub *)node->GetData();
            current->setLength(k,1);
            const int lindex = data->order[k];
            const int rindex = data->order[k+1];
            current->setRate(lindex,rindex,rate[lindex],rate[rindex]);
            k++;
        }
        last->setLastIndex(data->order[atr-1]);
        sumlength = atr-1;
    }
}

// some useful events to use
void PCPSub::mouseMoved(wxMouseEvent& event) {
    if(isclicked){
        isdruged = true;
        if(!iscalc){
            iscalc =true;
            const int atr = isUpper ? upperatr:loweratr;

            double to = getOriginalValue(event.GetX());
            vector<double> v;
            v.push_back(from);
            v.push_back(to);
            PCPPane* parent = (PCPPane* ) GetParent();  
            data->setSIndex(atr,v,parent->clickid);
            auto grandparent = GetGrandParent()->GetParent();
            grandparent->Refresh(); 
            iscalc =false;
        }
    }
}
void PCPSub::mouseDown(wxMouseEvent& event) {
    //あとで整理
    const int y = event.GetY();
    const int height = getHeight();
    if(data->isCoord) {
        if( y < height/4 )
            data->setCood(layer,true);
        else if(y > height* 3 /4)
            data->setCood(layer,false);
    }
    else{
        isUpper = y < height/2 ;  //値の選択 
        from = getOriginalValue(event.GetX());  
    }
    isclicked =true;
    auto parent = GetParent()->GetGrandParent();
    parent->Refresh();
}

double PCPSub::getOriginalValue(int x){
    const int atr = isUpper ? upperatr:loweratr;
    const double rate = isUpper ? urate:lrate;

    return  x/rate +data->Dmin[atr];
}

void PCPSub::mouseReleased(wxMouseEvent& event) {
    isclicked = false;
    isdruged = false;
    PCPPane* parent = (PCPPane* )GetParent();
    parent->idplus();
}

PCPSub::PCPSub(wxWindow* parent,int l, ReadData* d, int size,int h) :
    wxGLCanvas(parent, wxID_ANY, NULL, wxPoint(0,l*size), wxSize(h*100 -10,size), wxFULL_REPAINT_ON_RESIZE) //wxsize あとで変更
{
    m_context = new wxGLContext(this);
   //  GLenum err = glewInit();
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
PCPSub::~PCPSub(){
    delete m_context;
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
    const int size = (length/sumlength) *w;
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
    const int width = getWidth();
    const int height = getHeight();
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
    if(upperatr == data->selectedcoord)
        glColor4f(0.8f,0.1f,0.1f,1.0f);  
    else
        glColor4f(0.0f,0.0f,0.0f,1.0f);
    glLineWidth(3.0);
    glBegin(GL_LINES);
    const int xright = width*3/4;
    const int xleft = 0 ;
    glVertex3f(xleft, 0,0);
    glVertex3f(xright,0,0); 
    glEnd();
    glRasterPos2d(xright+10,20);
    //選んだ軸だけでなく上下も表示したい
    if(!data->isCoord || data->containSelectedCood(upperatr) ){
        const string str = data->atrname.at(upperatr);
        const int size = (int)str.size();
        for(int j = 0;j< size;j++){
            char ic = str[j];
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,ic); 

        }
    }
    vector< S_Cluster > cluster = data->getCluster();
    list<int> notselected = data->getNSIndex();
    

    glColor4f(0.5f,0.6f,0.9f,1.0f);
    glLineWidth(1);
     glBegin(GL_LINES);
    for(int i: notselected ) {   
        draw(i,height);                  
    }
    glEnd();
    if(!cluster.empty()){
        glLineWidth(2);
        glBegin(GL_LINES);
        for(auto ls:cluster){
            //色は後で調整　クラスターごとに変えたい  
            RGB rgb = ls.rgb;  
            glColor4f(rgb.r,rgb.g,rgb.b,1.0f);
            for(int i: ls.index) {
                draw(i,height);
            }
        }
        glEnd();
    }
    glFlush();
    SwapBuffers();
 
}

void PCPSub::draw(int i,int height){
    
    glVertex3f( ( data->D[i][upperatr] - data->Dmin[upperatr]) *urate,0 ,0); 
    glVertex3f( ( data->D[i][loweratr] -data->Dmin[loweratr]) *lrate,height,0);      
    }
PCPBorder::PCPBorder(wxWindow* parent,bool b,ReadData* d, int size,int h) :
wxGLCanvas(parent, wxID_ANY, NULL, wxPoint(0,0), wxSize(100*h,24), wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);
    data = d;
    islast = b;
    atr = d->atr-1;
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
    const int width = getWidth();
    const int height = getHeight();
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
        glLineWidth(2.0);
        glBegin(GL_LINES);
        const int xright = width*3/4;
        const int xleft = 0 ;
        glVertex3f(xleft,0,0);
        glVertex3f(xright,0,0); 
        glEnd();
        glRasterPos2d(xright+10,18);
        const string str = data->atrname.at(atr);
        const int size = (int)str.size();
        for(int j = 0;j< size;j++){
            char ic = str[j];
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,ic);
        }
    }
    glFlush();
    SwapBuffers();
}