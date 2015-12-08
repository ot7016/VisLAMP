#include "XXView.hpp"
//大きさ要修正
XXView::XXView(wxWindow* parent,ReadData* d,Agi* a)
	:wxGLCanvas(parent,wxID_ANY,wxDefaultPosition, wxSize(600,600), wxFULL_REPAINT_ON_RESIZE){
         m_context = new wxGLContext(this);
		data = d;
		ag = a;
}
int XXView::getWidth(){
    return GetSize().x;
}
 
int XXView::getHeight(){
    return GetSize().y;
}

void XXView::setRate(){
	double num = data->num;
	xrate = getWidth()/(num);
	yrate = getHeight()/(num);
}
void XXView::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
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


void XXView::render(wxPaintEvent &event){
    wxGLCanvas::SetCurrent(*m_context);
 
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int num = data->num;
	int dim = data->dim;
    

    int width = getWidth();
    int height = getHeight();
    setRate();
    prepare2DViewport(0,0,width, height);
    glLoadIdentity();
 
    // black background
    glColor4f(0, 0, 0, 1);
    glBegin(GL_QUADS);
    glVertex3f(0,0,0);
    glVertex3f(width,0,0);
    glVertex3f(width,height,0);
    glVertex3f(0,height,0);
    glEnd();

	// x座標でソート 
    std::vector< pair<int,double> > xindex;
    for(int i = 0; i< num;i++){
    	auto xpair = pair<int,double>(i,ag->getB(i,0)); 
    	xindex.push_back(xpair);
    }
    struct 
        { bool operator()(pair<int, double>& a, pair<int,double>& b){
            return a.second < b.second;
        }
    }lesspair;
    sort(xindex.begin(),xindex.end(),lesspair);
    //色はあとで調整
    //色の違いをどのように変えるかが問題 とりあえずdistmaxを三等分で
    double distmax = data->distmax;
     glColor4f(0.2f, 0.4f, 0.7f, 1.0f);
    for(int i = 0; i< num;i++){
    	auto p1 = xindex.at(i);
    	for(int j = 0;j < num;j++){
    		auto p2 = xindex.at(j);
    		double d = data->alldist[num* p1.first + p2.first];
            //ここのしきい値をどうするかが課題
    		if(i != j && d < distmax/5){
                glBegin(GL_QUADS);
                int x1 =(int) (xrate * i);
                int x2 = max((int)( xrate*(i+1)),x1+1);
                int y1 = (int) (yrate*j);
                int y2 = max((int)( yrate*(j+1)),y1+1);
                glVertex3f(x1, y1, 0);
                glVertex3f(x1, y2, 0);
                glVertex3f(x2, y2, 0);
                glVertex3f(x2, y1, 0);
                glEnd();
            }
    	}
    }
 glFlush();
 SwapBuffers();
}