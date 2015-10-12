#include <sstream>
#include <string>
#include <vector>
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#include "Agi.hpp"
#include "wx/wx.h"
#include "ConstraintSolver2D.hpp"

//高次元ベクトルをMDSでもとめる→　その必要はとりあえずない
//与えられた高次元配置を表す行列から計算
// 必要な固有値は B = XX^T のもの　　その後固有値の大きさでソートする

//コンストラクタ
Agi::Agi(ReadData* d){
	data = d;
	calprj();
	cal2Mtr();
	delta = 0.5;
}

Agi::~Agi(){
	delete[] B;
}

float Agi::getB(int i,int j){
	return B[i][j];
}

//in  固有値 evalue 高次元配置ベクトル Aij  out 初期射影行列     
void Agi::calprj(){
    int m = data->getdim();
	float  f1[m], f2[m];
	float distf1 = 0, distf2 = 0;

	//二次元平面上への射影の場合
	
	for (int i = 0; i < m; i++) {
		if (i%2 == 0) {
			f1[i] = pow(data->getevalue(i),delta) ;// 固有値を代入   現在はとりあえず projection factor 0.5とする
			f2[i] = 0;
			distf1 = distf1 + pow(f1[i], 2);
		} else {
			f2[i] = pow(data->getevalue(i),delta);
			f1[i] = 0;
			distf2 = distf2 + pow(f2[i], 2);
		}
	}
	
	distf1 = sqrt(distf1);
	distf2 = sqrt(distf2);
	//射影行列確認
	for (int i =0; i< m; i++) {
		ee.push_back(prj(f1[i]/distf1,f2[i]/distf2));
	}
	prjstack.push(ee);
}
//行列の計算   とりあえず実装　最適化とかなし　最終的にはBlasを使う
void Agi::cal2Mtr() {
	float prexmax = -100;
	float preymax = -100;
	float prexmin = 100;
	float preymin = 100; 

	int n = data->getnumatr();
	int m = data->getdim();
	B = new float*[n];      
	for (int i = 0; i < n; i++) {
		B[i] = new float[2];
		float b1 = 0, b2 = 0;
		for(int j = 0; j < m; j++) {
			float  A= data-> getA(i,j);
			b1 += ee.at(j).first * A;
			b2 += ee.at(j).second * A;
		}
		B[i][0] = b1;
		B[i][1] = b2;
		// 最大値を求める
		if(B[i][0]>prexmax)
			prexmax = B[i][0];
		if(B[i][0]< prexmin)
			prexmin = B[i][0];
		if(B[i][1]> preymax)
			preymax = B[i][1];
		if(B[i][1]<preymin)
			preymin = B[i][1];
	}
	xmin = prexmin;
	ymin = preymin;
	xmax = prexmax ;
	ymax = preymax ;
}
//射影の更新


int Agi::refine(float* _pre, float* _new, int index) {
		//まずはe3を求める
	int n = data->getnumatr();
	int m = data->getdim();
	float pi[m];
	float powpinorm = 0;
	for(int i = 0; i<m; i++){
		pi[i] = data->getA(index, i);
		powpinorm = pow(pi[i], 2)+powpinorm;
	}
	float pinorm = sqrt(powpinorm);
	float powprenorm = pow(_pre[0], 2)+pow(_pre[1], 2);
	float prenorm = sqrt(powprenorm);
	float newnorm = sqrt(pow(_new[0], 2)+pow(_new[1], 2));
	
	

 	if(pinorm < prenorm) {
 		std::cerr << "X error pinorm " << std::endl;
 		std::cerr << pinorm << std::endl;
 		std::cerr << "prenorm" << std::endl;
 		std::cerr << prenorm << std::endl;
 		return -1;
 	} 
 	if (pinorm < newnorm){
 		std::cerr << "X2 error" << std::endl;
 		return -2;
 	}
 	
	float f3[m];
 	float f3norm = 0;
 	for(int i = 0; i<m; i++) {
 		f3[i] = pi[i] -_pre[0]*ee.at(i).first-_pre[1]*ee.at(i).second;
	}
	f3norm = sqrt(powpinorm- powprenorm );
	for(int i = 0; i<m; i++){
		f3[i] = f3[i]/f3norm;
	}	
	//初期値設定
	const int _N = 6;
	float init[_N];
	for (int i = 0; i < _N; i++) {
      	init[i] = ConstSolve2D::defaultInit[i];
     }
	// 後は制約式を解く
	float* ans = solver2D( _pre, _new, f3norm, init);
	//std::cerr << ans[0] << std::endl;
	float e1[m],e2[m];
	float a3 = (_new[0] -_pre[0]*ans[0] -_pre[1]*ans[1])/f3norm;
	float b3 = (_new[1] -_pre[0]*ans[2] -_pre[1]*ans[3])/f3norm;
	for(int i = 0; i < m; i++){
		float ei0 = ee.at(i).first;
		float ei1 = ee.at(i).second;
		e1[i] = ans[0] * ei0 + ans[1]* ei1 + a3 *  f3[i];
		e2[i] = ans[2] * ei0 + ans[3]* ei1 + b3 *  f3[i];
	}
	ee.clear();
	for(int i = 0; i < m; i++){
		ee.push_back(prj(e1[i],e2[i]));
	}
	prjstack.push(ee);
	cal2Mtr();
	return 0;
}
void Agi::backprj(){
	if(prjstack.size()>1){
		prjstack.pop();
		ee = prjstack.top();
		cal2Mtr();
	}

}

float Agi::getXMax(){
	return xmax;
}
float Agi::getYMax(){
	return ymax;
}
float Agi::getXMin(){
	return xmin;
}
float Agi::getYMin(){
	return ymin;
}
void Agi::setdelta(float d){
	delta = d;
}


void AGIPane::mouseDown(wxMouseEvent& event) {
	std::cerr << "MouseDown " << std::endl;
    //マウスがクリックされたときの処理?
    float x = event.GetX();
    float y = event.GetY();
    //このxとyが点の2次元配列に含まれるならOK
    //もちろんある程度の誤差は許容しなければならない
    nowindex = getindex(x,y);
//    std::cerr << nowindex  << std::endl;
    if(nowindex != -1 && nowindex < data->getnum()){
	    _pre[0] = ag->getB(nowindex, 0);
    	_pre[1] = ag->getB(nowindex, 1);
    	isMoved = true;
    	data->setSIndex(nowindex);
		md->setText(nowindex);
		Refresh();
		pcp->Refresh();
	}
	else if(nowindex != -1 && nowindex<data->getnumatr()){
		int index = nowindex - data->getnum();
		nowindex = -1;
		std::cerr << data->getAtrName( index )<< std::endl;
	}
	//どのノードも近くないときは複数選択
	else {
		xfrom = x;
		yfrom = y;
		rangeselect = true;
		std::cerr << "select"<< std::endl;
	}
}

void AGIPane::mouseMoved(wxMouseEvent& event) {
	 if(nowindex != -1 && isMoved && !iscalc){
	 	iscalc = true;
	 	std::cerr << "mouseMoved " << std::endl;
	 	isDrug = true;
	 	calcagain(event.GetX(),event.GetY());
	 	iscalc = false;
	 	_pre[0] = ag->getB(nowindex, 0);
    	_pre[1] = ag->getB(nowindex, 1);
 	}
 	else if(rangeselect && !iscalc){
 		iscalc =true;
 		calRange(event.GetX(),event.GetY());
 		iscalc = false;
 	}

}
void AGIPane::calRange(int x2, int y2){
	xto = x2;
	yto = y2;
	std::vector<float> x;
    std::vector<float> y;
	x.push_back( (xfrom - getWidth()/2) /xrate);
    y.push_back( (yfrom - getHeight()/2) /yrate);
 	x.push_back( (x2 - getWidth()/2) /xrate);
    y.push_back( (y2 - getHeight()/2) /yrate);
    sort(x.begin(),x.end());
    sort(y.begin(),y.end());
    std::vector<int> selected;
 	for(int i = 0;i< data->getnum();i++){
 		float a = ag->getB(i,0);
 		float b = ag->getB(i,1);
 		if(a > x.at(0) && a < x.at(1)){
 			if(b >y.at(0) && b < y.at(1)){
 				selected.push_back(i);
 			}
 		}
 	}
 	data->setSIndex(selected);
 	auto parent = GetParent();
    parent->Refresh(); 
}

void AGIPane::mouseWheelMoved(wxMouseEvent& event) {
	
}
void AGIPane::mouseReleased(wxMouseEvent& event){
	std::cerr << "MouseReleased " << std::endl;
  if(nowindex != -1 && isDrug){
  	calcagain(event.GetX(),event.GetY());
    //このxとyが点の2次元配列に含まれるならOK
    //もちろんある程度の誤差は許容しなければならない
      isMoved = false;
  }
  else if(rangeselect){
  	rangeselect = false;
  }
   isDrug = false;
}
void AGIPane::rightClick(wxMouseEvent& event) {
	std::cerr << "Right click " << std::endl;
	float x = event.GetX();
	float y = event.GetY();
	//ドラッグ中に右クリックされると間違いなくバグるのであとで対処
	nowindex = getindex(x,y);
	if(nowindex !=  -1 && nowindex < data->getnum()){
		data->setSIndex(nowindex);
		pcp->Refresh();
		md->setText(nowindex);
		Refresh();
	}
	else if(nowindex !=-1  && nowindex < data->getnumatr()){
		 int index = nowindex - data->getnum();
		 nowindex = -1;
		 std::cerr << data->getAtrName( index )<< std::endl;
	} 
}
void AGIPane::mouseLeftWindow(wxMouseEvent& event) {}
void AGIPane::keyPressed(wxKeyEvent& event) {}
void AGIPane::keyReleased(wxKeyEvent& event) {}

void AGIPane::calcagain(float x,float y){
	_new[0] = (x - getWidth() /2) /xrate;
     _new[1] = (y - getHeight() /2) /yrate;
  
     	ag->refine(_pre, _new, nowindex) ;
     	Refresh();
     	int atr = data->getatr();
     	int n = data->getnum();
     	float** v = new float*[atr];
     	for(int i = 0 ; i< atr; i++){
     		v[i] = new float[2];
   			v[i][0] = ag->getB(n + i, 0);
    		v[i][1] = ag->getB(n + i, 1);
     	}
     	pcp->refine(v); 
     	md->setText(nowindex);
    
}


 
AGIPane::AGIPane(wxWindow* parent, int* args,ReadData* d, PCPPane* p, MatrixView* m) :
    wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxSize(600,600), wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);
    md = m;
    data = d;
    ag = new Agi(d);
    pcp = p;
    _pre = new float[2];
    _new = new float[2];
    nowindex = -1;
    isMoved = false;
    isDrug = false;
    iscalc = false;
    rangeselect =false;
    xfrom = -1;
    xto = -1;
    yfrom = -1;
    yto = -1;
    setRate();

    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}
 
AGIPane::~AGIPane(){
    delete m_context;
    delete ag;
    delete[] _pre;
}
 
void AGIPane::resized(wxSizeEvent& evt)
{
}
 
void AGIPane::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glEnable(GL_TEXTURE_2D);   // textures
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LINE_SMOOTH);
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
 
 
int AGIPane::getWidth()
{
    return GetSize().x;
}
 
int AGIPane::getHeight()
{
    return GetSize().y;
}

//呼び出すのは最初の一度だけ

void AGIPane::setRate(){
   
    float xabs = std::max(ag->getXMax(),-(ag->getXMin()));
    float yabs = std::max(ag->getYMax(),-(ag->getYMin()));

    xrate = getWidth()  / (2 * xabs);
    yrate = getHeight() /(2 * yabs);
    //     std::cerr << xrate << std::endl;       
  }

 void AGIPane::setdelta(float d){
 	ag->setdelta(d);
 	//不十分　バグあり　ノード移動による射影の変更が反映されない
 	ag->calprj();
 	ag->cal2Mtr();
 	Refresh();
 }


int AGIPane::getindex(float x, float y){
    int index = -1;
    const float min = 0.05;
    float minnow = min; 
    float d; 
    float x2 = (x - getWidth()/2) /xrate;
    float y2 = (y - getHeight()/2) /yrate;

    for(int i = 0;i< data->getnumatr();i++){
        d = sqrt(pow(ag->getB(i, 0)-x2, 2)+pow(ag->getB(i, 1)-y2, 2));
        if(d < minnow){
            minnow = d;
            index = i;
        }
    }
     std::cerr << data->getName(index) << std::endl;
    return index;
} 

void AGIPane::undo(){
	ag->backprj();
	Refresh();
	int atr = data->getatr();
     int n = data->getnum();
     float** v = new float*[atr];
     for(int i = 0 ; i< atr; i++){
     	v[i] = new float[2];
   		v[i][0] = ag->getB(n + i, 0);
    	v[i][1] = ag->getB(n + i, 1);
     }
     pcp->refine(v); 
}


 //本当は再描画のことも考えた関数設計にする
void AGIPane::render(wxPaintEvent& evt)
{
	std::cerr << "AGIRender" << std::endl;
    if(!IsShown()) return;
 
    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    // ------------- draw some 2D ----------------
    prepare2DViewport(0, 0, getWidth(), getHeight());
    glLoadIdentity();
 
    // white background
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(getWidth(), 0, 0);
    glVertex3f(getWidth(), getHeight(), 0);
    glVertex3f(0, getHeight(), 0);
    glEnd();
   
    // 追加部分 点を書く
    glColor4f(0.2f, 0.4f, 0.7f, 1.0f);
    glPointSize(5.0);
    glBegin(GL_POINTS);

    //ここで点を描画する  倍率をきめる関数をどこかで定義する必要あり  データの最大値を使うべきだろう
	std::vector<int> selected = data->getSIndex();
    std::vector<int> notselected = data->getNSIndex();
    int num = data->getnum();
    
  	for(int i: notselected){
       	glVertex3f(ag->getB(i,0)*xrate + getWidth()/2, ag->getB(i,1)*yrate + getHeight()/2,0);
    }
    glEnd();
    glPointSize(8.0);
    glBegin(GL_POINTS);
    glColor4f(0.7f, 0.3f, 0.4f, 1.0f);

    for(int i : selected){
       	glVertex3f(ag->getB(i,0)*xrate + getWidth()/2, ag->getB(i,1)*yrate + getHeight()/2,0);
    }
	glEnd();
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	for(int i = 0; i<data->getatr() ;i++){
		glVertex3f(ag->getB(i+num,0)*xrate + getWidth()/2, ag->getB(i+num,1)*yrate + getHeight()/2,0);
	}
	glColor4f(0.2f, 0.4f, 0.7f, 0.3f);
	glEnd();
	glBegin(GL_LINES);
	glLineWidth(1);
	std::vector<std::pair<int,int> > edge = data->getEdge();
	for(int i = 0;i< edge.size();i++){
		int n1 = edge.at(i).first;
		int n2 = edge.at(i).second;
		glVertex3f(ag->getB(n1,0)*xrate + getWidth()/2, ag->getB(n1,1)*yrate + getHeight()/2,0);
		glVertex3f(ag->getB(n2,0)*xrate + getWidth()/2, ag->getB(n2,1)*yrate + getHeight()/2,0);
	}
    glEnd();
    if(rangeselect){
    	glColor4f(0.0f,0.0f,0.0f,1.0f);
    	glBegin(GL_LINE_STRIP);
    	glVertex3f(xfrom,yfrom,0);
    	glVertex3f(xfrom,yto,0);
    	glVertex3f(xto,yto,0);
    	glVertex3f(xto,yfrom,0);
    	glVertex3f(xfrom,yfrom,0);
    	glEnd();
    }
    glFlush();
    SwapBuffers();
}


