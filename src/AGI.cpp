
#include <sstream>
#include <string>
#include <vector>
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#include "Agi.hpp"
#include "wx/wx.h"
#include "ConstraintSolver2D.hpp"
//いらないの多分あるのであとで見直し

//高次元ベクトルをMDSでもとめる→　その必要はとりあえずない
//与えられた高次元配置を表す行列から計算
// 必要な固有値は B = XX^T のもの　　その後固有値の大きさでソートする

//コンストラクタ
Agi::Agi(ReadData* d){
	data = d;
	calprj();
	cal2Mtr();
}

Agi::~Agi(){
	delete[] B;
	delete[] e;
}

float Agi::getB(int i,int j){
	return B[i][j];
}

//in  固有値 evalue 高次元配置ベクトル Aij  out 初期射影行列     
void Agi::calprj(){
	// 固有値計算 veclib使用せずとりあえずRで
/*	
int n=3;
int lwork, info;
float *A = new float[n*n]; float *w = new float[n];
//setting A matrix A[0+0*n]=1;A[0+1*n]=2;A[0+2*n]=3; A[1+0*n]=2;A[1+1*n]=5;A[1+2*n]=4; A[2+0*n]=3;A[2+1*n]=4;A[2+2*n]=6;
lwork = -1;
float *work = new float[1];
dsyev_("V", "U", &n, A, &n, w, work, &lwork, &info); lwork = (int)work[0];
lwork = (int)work[0];
delete[]work;
work = new float[std::max((int) 1, lwork)]; 
//get Eigenvalue
dsyev_("V", "U", &n, A, &n, w, work, &lwork, &info);
*/
    int m = data->getdim();
	float  f1[m], f2[m];
	//int f1i = 0, f2i = 0;
	float distf1 = 0, distf2 = 0;
	//k-means変更  今だけ あとでクラスターの要素を読み込む
	/*
	for(int i = 0;i< m/2;i++){
		float e = data->getevalue(i);
		f1[i] = e;
		f2[i] = 0;
		distf1 = distf1 + pow(e, 2);
	}
	for(int i = m/2;i<m;i++){
		float e = data ->getevalue(i);
		f1[i] = 0;
		f2[i] = e;
		distf2 = distf2 + pow(e, 2);
	}
	*/

	//二次元平面上への射影の場合
	
	for (int i = 0; i < m; i++) {
		if (i%2 == 0) {
			f1[i] = data->getevalue(i) ;// 固有値を代入
			f2[i] = 0;
			distf1 = distf1 + pow(f1[i], 2);
		} else {
			f2[i] = data->getevalue(i);
			f1[i] = 0;
			distf2 = distf2 + pow(f2[i], 2);
		}
	}
	
	distf1 = sqrt(distf1);
	distf2 = sqrt(distf2);
	//射影行列確認
	 e = new float*[m];
	for (int i =0; i< m; i++) {
		e[i] = new float[2];
		e[i][0] = f1[i]/distf1;
		e[i][1] = f2[i]/distf2;
	}
	 std::cerr << e[0][0] << std::endl;
}
//行列の計算   とりあえず実装　最適化とかなし　最終的にはBlasを使う
void Agi::cal2Mtr() {
	float prexmax = -100;
	float preymax = -100;
	float prexmin = 100;
	float preymin = 100; 

	int n = data->getnum();
	int m = data->getdim();
	B = new float*[n];      
	for (int i = 0; i < n; i++) {
		B[i] = new float[2];
		float b1 = 0, b2 = 0;
		for(int j = 0; j < m; j++) {
			float  A= data-> getA(i,j);
			b1 += e[j][0] * A;
			b2 += e[j][1] * A;
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


	//6/2追加 最小値が0以下なので 下駄を履かせる
	//　可視空間のほうが大きかった理由これ
	for(int i = 0;i< n;i++){
		B[i][0] = B[i][0];
		B[i][1] = B[i][1];
	}
	xmax = prexmax ;
	ymax = preymax ;
}
//射影の更新


int Agi::refine(float* _pre, float* _new, int index) {
		//まずはe3を求める
	int n = data->getnum();
	int m = data->getdim();
	float pi[m];
	float powpinorm = 0;
	for(int i = 0; i<m; i++){
		pi[i] = data->getA(index,i);
		powpinorm = pow(pi[i],2)+powpinorm;
	}
	float pinorm = sqrt(powpinorm);
	float powprenorm =   pow(_pre[0],2)+pow(_pre[1],2);
	float prenorm = sqrt(powprenorm);
	float newnorm = sqrt(pow(_new[0],2)+pow(_new[1],2));
	
	

 	if(pinorm < prenorm ) {
 		std::cerr << "X error pinorm " << std::endl;
 		std::cerr << pinorm << std::endl;
 		std::cerr << "prenorm" << std::endl;
 		std::cerr << prenorm << std::endl;
 		return -1;
 	} 
 	if (pinorm < newnorm ){
 		std::cerr << "X2 error" << std::endl;
 		return -2;
 	}
 	
	float f3[m];
 	float f3norm = 0;
 	for(int i = 0; i<m; i++) {
 		f3[i] = pi[i] -_pre[0]*e[i][0]-_pre[1]*e[i][1];
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
	float* ans = solver2D( _pre, _new, f3norm,init);
	std::cerr << ans[0] << std::endl;
	std::cerr << ans[1] << std::endl;
	float e1[m],e2[m];
	float a3 = (_new[0]- _pre[0]*ans[0]-_pre[1]*ans[1])/f3norm;
	float b3 = (_new[1]- _pre[0]*ans[2]-_pre[1]*ans[3])/f3norm;
	for(int i = 0; i < m; i++){
		e1[i] = ans[0] * e[i][0] + ans[1]* e[i][1] + a3 *  f3[i];
		e2[i] = ans[2] * e[i][0] + ans[3]* e[i][1] + b3 *  f3[i];
	}
	for(int i = 0; i < m; i++){
		e[i][0] = e1[i];
		e[i][1] = e2[i];
	}
	cal2Mtr();
	return 0;
}
float Agi::getXMax(){
	return xmax;
}
float Agi::getYMax(){
	return ymax;
}

void AGIPane::mouseMoved(wxMouseEvent& event) {
	 if(nowindex != -1){
    //このxとyが点の2次元配列に含まれるならOK
    //もちろんある程度の誤差は許容しなければならない
	 _new[0] = (event.GetX() - getWidth()/2)/xrate;
     _new[1] = (event.GetY()-getHeight()/2)/yrate;
     ag->refine(_pre,_new,nowindex) ;
     	Refresh();
     
    
    _pre[0] = _new[0];
    _pre[1] = _new[1];
  }

}
void AGIPane::mouseDown(wxMouseEvent& event) {
    //マウスがクリックされたときの処理
    float x =  event.GetX();
    float y = event.GetY();
    //このxとyが点の2次元配列に含まれるならOK
    //もちろんある程度の誤差は許容しなければならない
    nowindex = getindex(x,y);
//    std::cerr << nowindex  << std::endl;
    if(nowindex != -1){
    _pre[0] = ag->getB(nowindex,0);
    _pre[1] = ag->getB(nowindex,1);
}

}
void AGIPane::mouseWheelMoved(wxMouseEvent& event) {
	
}
void AGIPane::mouseReleased(wxMouseEvent& event){
  if(nowindex != -1){
    //このxとyが点の2次元配列に含まれるならOK
    //もちろんある程度の誤差は許容しなければならない
   
     // _pre[0] = _new[0];
     // _pre[1] = _new[1];
      nowindex = -1;
  }
}
void AGIPane::rightClick(wxMouseEvent& event) {}
void AGIPane::mouseLeftWindow(wxMouseEvent& event) {}
void AGIPane::keyPressed(wxKeyEvent& event) {}
void AGIPane::keyReleased(wxKeyEvent& event) {}


 
AGIPane::AGIPane(wxFrame* parent, int* args,ReadData* d) :
    wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxSize(800,600), wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);
    data = d;
    ag = new Agi(d);
    _pre = new float[2];
    _new = new float[2];
    nowindex = -1;
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
//  wxGLCanvas::OnSize(evt);
   // Refresh();
}
 
void AGIPane::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
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
   
    float xmax = ag->getXMax();
    float ymax = ag->getYMax();
    xrate = getWidth() /(3*xmax);
    yrate = getHeight()/(3* ymax);
    //     std::cerr << xrate << std::endl;
    //        std::cerr << yrate << std::endl;
}


int AGIPane::getindex(int x, int y){
    int index = -1;
    const float min = 25;
    float minnow = min; 
    float d; 
    float x2 = (x - getWidth()/2)/xrate;
    float y2 = (y - getWidth()/2 )/yrate;

    for(int i = 0;i< data->getnum();i++){
        d = pow( ag->getB(i,0) -x2,2)+pow( ag->getB(i,1)- y2,2);
        if(d < minnow){
            minnow = d;
            index = i;
        }
    }
    return index;
} 


 //本当は再描画のことも考えた関数設計にする
void AGIPane::render( wxPaintEvent& evt )
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
   
    // 追加部分 点を書く
    glColor4f(0.0f,0.0f,1.0f,1.0f);
    glPointSize(10.0);
    glBegin(GL_POINTS);

    //ここで点を描画する  倍率をきめる関数をどこかで定義する必要あり  データの最大値を使うべきだろう
    for(int i = 0; i< data->getnum();i++){
        glVertex3f(ag->getB(i,0)*xrate+getWidth()/2,ag->getB(i,1)*yrate+getHeight()/2,0);
    }
    glEnd();
    glFlush();
    SwapBuffers();
}
