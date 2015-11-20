#include "HistView.hpp"

using namespace std;
HistView::HistView(wxWindow* parent,wxSlider* vc, ReadData* d) :
 wxGLCanvas(parent, wxID_ANY, NULL, wxDefaultPosition, wxSize(300,50), wxFULL_REPAINT_ON_RESIZE){
 	 m_context = new wxGLContext(this);
     vcslider = vc;
    data = d;
    Setting();
}
void HistView::Setting(){
    iscalc = false;
    isdruged = false;
    isclicked = false;
    //distの最大値を求める
    double distmax = 0;
    int num = data->num;
    for(int i = 0; i< num; i++){
        for(int j = i; j< num;j++){
           double d = data->alldist[i*num +j] ;
           if(distmax < d)
            distmax = d; 
        }
    }
    //30のヒストグラムにわける
    histlen = 45;  
    hist = new int[histlen];
    for(int i = 0; i< histlen;i++){
        hist[i] = 0;
    }
    double dist10 = distmax /histlen;
    for(int i = 0; i< num;i++){
        for(int j= i;j< num;j++){
            int h = (int) ceil(data->alldist[i*num + j] / dist10);
            if(h == histlen )
                 h--;
            hist[h]++;
        }

    }
    setValue(data->thr);
    setRate();
}
void HistView::reset(){
    
}
void HistView::setValue(double r2){
    selected.clear();
    notselected.clear();
    for(int i = 0;i< histlen;i++){
        notselected.push_back(i);
    }
    for(int i = 0; i< histlen;i++){
        if(i < r2 ){
            selected.push_back(i);
            notselected.remove(i);
        }
    }
}

void HistView::ReCreate(){
    delete[] hist;
    Setting();
}
void HistView::setRate(){
    xrate =  getWidth() /histlen;
    double max = 0;
    for(int i = 0; i< histlen;i++){
        if(max < hist[i]){
            max =  hist[i];
        }
    }
    yrate = getHeight()/ max;
}
int HistView::getWidth(){
    return GetSize().x;
}
 
int HistView::getHeight(){
    return GetSize().y;
}


void HistView::mouseMoved(wxMouseEvent& event){
    if(isclicked){
        isdruged = true;
        if(!iscalc){
            iscalc =true;
             int x = event.GetX();

             //selected を決める 
             double r2 = x/xrate;
             setValue(r2);
            data->recalEdge(r2); 
            vcslider->SetValue(0);
            auto grandparent = GetGrandParent()->GetParent();
            grandparent->Refresh(); 
            iscalc =false;
        }
    }
}   


void HistView::mouseDown(wxMouseEvent& event){
//あとで整理

    isclicked = true;
}
void HistView::mouseReleased(wxMouseEvent& event){
    isclicked = false;
    isdruged = false;
}  

void HistView::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
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

void HistView::render(wxPaintEvent& evt)
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
    //draw histgram  
    //選択もある
    glColor4f(0.2f, 0.4f, 0.7f, 1.0f);
    for(int i: selected){
        int h = hist[i];
        glBegin(GL_QUADS);
        glVertex3f(xrate* i, 0, 0);
        glVertex3f(xrate*i, h*yrate, 0);
        glVertex3f(xrate* (i+1), h*yrate,0);
        glVertex3f(xrate* (i+1), 0, 0);
        glEnd();
    }
    glColor4f(0.2f, 0.4f, 0.7f, 0.3f);
    for(int i: notselected){
        int h = hist[i];
         glBegin(GL_QUADS);
        glVertex3f(xrate* i, 0, 0);
        glVertex3f(xrate*i, h*yrate, 0);
        glVertex3f(xrate* (i+1), h*yrate,0);
        glVertex3f(xrate* (i+1), 0, 0);
        glEnd();
    }
    

    glFlush();
    SwapBuffers();
}