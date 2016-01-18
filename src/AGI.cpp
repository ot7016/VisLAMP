#include "Agi.hpp"
#include <GLUT/glut.h>
//#include <GL/glew.h>

//高次元ベクトルをMDSでもとめる→　その必要はとりあえずない
//与えられた高次元配置を表す行列から計算
// 必要な固有値は B = XX^T のもの　　その後固有値の大きさでソートする

//コンストラクタ
Agi::Agi(ReadData* d){
	data = d;
	//delta = 0.5;
	calprj();
	cal2Mtr();
	writeagicood();
	
}

Agi::~Agi(){
	delete[] B;
	delete[] v;
}
void Agi::ReCreate(){
	while(!prjstack.empty()){
		prjstack.pop();
	}
	ee.clear();
	writenum = 0;
	delta = 0.5;
	calprj();
	cal2Mtr();

}

double Agi::getB(int i,int j){
	return B[2*  i +j];
}

//in  固有値 evalue 高次元配置ベクトル Aij  out 初期射影行列     
void Agi::calprj(){
    const int m = data->dim;
	double  f1[m], f2[m];
	double distf1 = 0, distf2 = 0;

	//二次元平面上への射影の場合
	
	for (int i = 0; i < m; i++) {
		if (i%2 == 0) {
			f1[i] = pow(data->evalue[i],delta) ;// 固有値を代入   現在はとりあえず projection factor 0.5とする
			f2[i] = 0;
			distf1 = distf1 + pow(f1[i], 2);
		} else {
			f2[i] = pow(data->evalue[i],delta);
			f1[i] = 0;
			distf2 = distf2 + pow(f2[i], 2);
		}
	}
	
	distf1 = sqrt(distf1);
	distf2 = sqrt(distf2);
	//射影行列確認 
	//stackのことを考えると vectorがいいが　 blas使うなら配列　
	for (int i =0; i< m; i++) {
		ee.push_back(prj(f1[i]/distf1,f2[i]/distf2));
	}
	prjstack.push(ee);
}
//行列の計算   とりあえず実装　最適化とかなし　最終的にはBlasを使う
void Agi::cal2Mtr() {
	const int n = data->aginum;
	const int m = data->dim;
	const int d = 2;
	double e[m*d];
	for(int i = 0; i < m; i++){
		e [d* i] = ee.at(i).first;
		e[d* i +1] = ee.at(i).second;
	}
	 delete[] B;
	 B = new double[n*d];

	// BLAS で計算　B[n,2] = A[n,m] * e[m,2]  　コピーはしたくないがここでは1次元配列にしなければならないのでどうしようもない
	cblas_dgemm(CblasRowMajor, CblasNoTrans ,CblasNoTrans, n, 2, m, 1, data->A, m, e, d, 0 , B, d);

	double prexmax = B[0];
	double preymax = B[1];
	double prexmin = B[0];
	double preymin = B[1]; 

	for (int i = 0; i < n; i++) {
		// 最大値を求める
		if (B[d * i] > prexmax)
			prexmax = B[d * i];
		if (B[d* i] < prexmin)
			prexmin = B[d*i];
		if (B[d* i + 1] > preymax)
			preymax = B[d*i +1];
		if (B[d* i+ 1] < preymin)
			preymin = B[d* i +1];
	}
	xmin = prexmin;
	ymin = preymin;
	xmax = prexmax ;
	ymax = preymax ;
	if(data->isPCA){
		//BLASで 属性の射影V も計算 PCAの場合   V[m * 2] = evector[m * m] * e[m,2]
		delete[] v;
    	v = new double[m * d];
    	cblas_dgemm(CblasRowMajor, CblasNoTrans ,CblasNoTrans, m, d, m, 1, data->evector, m, e, d, 0 , v, d);
	}

}
//射影の更新
//mdsの場合はこのままでも大丈夫 PCAの属性に対応すればOK
int Agi::refine(double* _pre, double* _new, int index) {
		//まずはe3を求める
	const int n = data->aginum;
	const int m = data->dim;
	double pi[m];
	double powpinorm = 0;
	//PCAで属性軸が選択されたとき
	if(data->isPCA && index >= n){
		int atrindex = index -n ;
		for(int i = 0; i< m; i++){
			pi[i] = data->getevector(atrindex,i);
		 powpinorm = pow(pi[i], 2)+powpinorm;
		}
	}
	else{
		for(int i = 0; i<m; i++){
			pi[i] = data->getA(index, i);
			powpinorm = pow(pi[i], 2)+powpinorm;
		}
	}
	double pinorm = sqrt(powpinorm);
	double powprenorm = pow(_pre[0], 2)+pow(_pre[1], 2);
	double prenorm = sqrt(powprenorm);
	double newnorm = sqrt(pow(_new[0], 2)+pow(_new[1], 2));
 	if(pinorm <= prenorm) {
 		std::cerr << "X error pinorm " << std::endl;
 		std::cerr << pinorm << std::endl;
 		std::cerr << "prenorm" << std::endl;
 		std::cerr << prenorm << std::endl;
 		return -1;
 	} 
 	if (pinorm <= newnorm){
 		double gamma = 0.001;
 		if(newnorm > (1- gamma)*pinorm){
 			double _new2[2];
 			_new2[0] = (1-gamma) * pinorm * _new[0] / newnorm ;
 			_new2[1] = (1-gamma) * pinorm * _new[1] / newnorm ;
 			refine(_pre, _new2,index);
 		}
 		else {
 			std::cerr << "X2 error" << std::endl;
 			return -2;
 		}
 	}
	double f3[m];
 	double f3norm = 0;
 	for(int i = 0; i<m; i++) {
 		f3[i] = pi[i] -_pre[0]*ee.at(i).first-_pre[1]*ee.at(i).second;
	}
	f3norm = sqrt(powpinorm- powprenorm );
	for(int i = 0; i<m; i++){
		f3[i] = f3[i]/f3norm;
	}	
	//初期値設定
	const int _N = 6;
	double init[_N];
	for (int i = 0; i < _N; i++) {
      	init[i] = ConstSolve2D::defaultInit[i];
     }
	// 後は制約式を解く
	double* ans = solver2D( _pre, _new, f3norm, init);
	double e1[m],e2[m];
	double a3 = (_new[0] -_pre[0]*ans[0] -_pre[1]*ans[1])/f3norm;
	double b3 = (_new[1] -_pre[0]*ans[2] -_pre[1]*ans[3])/f3norm;
	for(int i = 0; i < m; i++){
		double ei0 = ee.at(i).first;
		double ei1 = ee.at(i).second;
		e1[i] = ans[0] * ei0 + ans[1]* ei1 + a3 *  f3[i];
		e2[i] = ans[2] * ei0 + ans[3]* ei1 + b3 *  f3[i];
	}
	ee.clear();
	for(int i = 0; i < m; i++){
		ee.push_back(prj(e1[i],e2[i]));
	}
	
	cal2Mtr();
	return 0;
}

void Agi::prjpush(){
	prjstack.push(ee);
}
void Agi::backprj(){
	if(prjstack.size()>1){
		prjstack.pop();
		ee = prjstack.top();
		cal2Mtr();
	}

}

double Agi::getXMax(){
	return xmax;
}
double Agi::getYMax(){
	return ymax;
}
double Agi::getXMin(){
	return xmin;
}
double Agi::getYMin(){
	return ymin;
}
void Agi::setdelta(double d){
	delta = d;
}
double Agi::getV(int i, int j){
	return v[2 *i + j];
}
//評価用
void Agi::writeprojection(){
    //保存場所とファイル名を考慮
     const string d = data->dataname.at(data->dataid);
     string dir = "../data/" + d +"/"+ d+ "-projection/projection_" +to_string(writenum)+  ".dat";
     int dim = data->dim;
     double projection[dim*2];
     int j = 0;
     for (prj p : ee){
     	projection[2*j] = p.first;
     	projection[2*j+1] = p.second;
     	j++;
     }
     ofstream fs2(dir,ios::out | ios::binary);
     for(int i = 0; i< dim *2 ;i++ ){
     fs2.write((char*) &projection[i],sizeof(double));
   }
   fs2.close();
     writenum++;
  }
  void Agi::writeagicood(){
    //保存場所とファイル名を考慮
    const string d = data->dataname.at(data->dataid);
    const string dir = "../data/" + d +"/"+ d+ "-agicood/agicood_" +to_string(writenum)+  ".dat";
    const int num = data->num;
     ofstream fs2(dir,ios::out | ios::binary);
     for(int i = 0; i< num *2 ;i++ ){
     fs2.write((char*) &B[i],sizeof(double));
   }
   fs2.close();
     writenum++;
  }

AGIPane::AGIPane(wxWindow* parent, int* args,ReadData* d, PCPPane* p,int h) :
    wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxSize(h*100,h*100), wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);
    data = d;
    ag = new Agi(d);
    pcp = p;
   Setting();
    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}
 
void AGIPane::Setting(){
	_pre = new double[2];
    _new = new double[2];
    nowindex = -1;
    isMoved = false;
    isDrug = false;
    iscalc = false;
    isPoly = false;
    rangeselect =false;
    polyselect = false;
    polystart = false;
    polyvector.clear();
    polynow = pair<int,int> (0,0);
    xfrom = -1;
    xto = -1;
    yfrom = -1;
    yto = -1;
    clickid = 0;
    logvector.clear();
    logmode = false;
    logid = 0;
    setRate();
} 
void AGIPane::setMV(MatrixView* m){
	md = m;
}
AGIPane::~AGIPane(){
    delete m_context;
    delete[] _pre;
    delete[] _new;
}
void AGIPane::ReCreate(){
    ag->ReCreate();
    Setting();
}

void AGIPane::mouseDown(wxMouseEvent& event) {
	std::cerr << "MouseDown " << std::endl;
    //マウスがクリックされたときの処理?
    double x = event.GetX();
    double y = event.GetY();
    if(!isMoved){
    	if(!isPoly){
    		//このxとyが点の2次元配列に含まれるならOK
    		//もちろんある程度の誤差は許容しなければならない
    		nowindex = getindex(x,y);
    		if(nowindex != -1 && nowindex < data->aginum){
    	 		std::cerr << data->name.at(nowindex) << std::endl;
	    		_pre[0] = ag->getB(nowindex, 0);
    			_pre[1] = ag->getB(nowindex, 1);
    			isMoved = true;
    			data->setSIndex(nowindex);
    			data->selectedcoord = -1;
				md->setText(nowindex);
				Refresh();
				pcp->Refresh();
				addLog(nowindex,_pre[0],_pre[1]);
			}
			else if(data->isPCA && nowindex != -1 && nowindex < data->num+data->atr){
				int index = nowindex - data->num;
				data->selectedcoord = index;  
				std::cerr << data->atrname.at(index)<< std::endl;
				_pre[0] = ag->getV(index,0);
    			_pre[1]  =ag->getV(index,1);
    			isMoved = true;
				addLog(nowindex,_pre[0],_pre[1]);
				}
	
			//どのノードも近くないときは複数選択
			else{
				xfrom = x;
				yfrom = y;
				rangeselect = true;
			}
   		}
   		// 投げ縄選択はボタンを押して発動させるようにする
		//2点以上クリックで選んだのち右クリックして終了
		// mouse release後も処理が続くので注意	
		else if(isPoly){
			if(!polystart){
				polyvector.clear();
				polystart = true;
			}
			auto from = pair<int,int>(x,y);
			polyvector.push_back(from);
		}	
	}
}
//属性にも対応させる  calcagainでも大丈夫か?
void AGIPane::mouseMoved(wxMouseEvent& event) {
	if(!isPoly){
	 	if(nowindex != -1 && isMoved && !iscalc){
	 		iscalc = true;
	 		std::cerr << "mouseMoved " << std::endl;
	 		isDrug = true;
	 		calcagain(event.GetX(),event.GetY());
	 		int num = data->num;
	 		if(data->isPCA && nowindex >= num){
	 			_pre[0] = ag->getV(nowindex-num,0);
	 			_pre[1] = ag->getV(nowindex-num,1);
	 		}
	 		else{
	 			_pre[0] = ag->getB(nowindex, 0);
    			_pre[1] = ag->getB(nowindex, 1);
    		}

    		iscalc = false; 
 		}
 		else if(rangeselect && !iscalc){
 			iscalc =true;
 			calRange(event.GetX(),event.GetY());
 			iscalc = false;
 		}
 	}
 	//polyline選択のときマウスを動かしているだけなのでまだ確定していない
 	else{
 		if(polystart&&!iscalc){
 			iscalc = true;
 			int x = event.GetX();
 			int y = event.GetY();
 			//仮に入れる
 		    polynow = pair<int,int>(x,y);
 			if(polyvector.size() >1)
 				calPoly();
 			auto parent = GetParent();
    		parent->Refresh();

 			iscalc = false;
 		}
 	}

}

int AGIPane::getindex(double x, double y){
    int index = -1;
    const double min = 0.05;
    double minnow = min; 
    double d; 
    double x2 = (x - getWidth()/2) /xrate;
    double y2 = (y - getHeight()/2) /yrate;

    for(int i :data->filterindex){
        d = sqrt(pow(ag->getB(i, 0)-x2, 2)+pow(ag->getB(i, 1)-y2, 2));
        if(d < minnow){
            minnow = d;
            index = i;
        }
    }
    if(index== -1 && data->isPCA){
    	minnow = 0.05;
    	x2 = x2/ coodrate;
    	y2 = y2/coodrate;
    	for(int i = 0;i< data->atr;i++){
    		 //直線上どこでも選択できるようにすると移動でマウスによるベクトルの長さが選択できなくなる
    		d = sqrt(pow(ag->getV(i, 0)-x2, 2)+pow(ag->getV(i, 1)-y2, 2));
    		if(d < minnow){
    			minnow = d;
    			index = i+data->num;
    		}
    	}
    }
    return index;
} 

void AGIPane::calcagain(double x,double y){
	_new[0] = (x - getWidth() /2) /xrate;
     _new[1] = (y - getHeight() /2) /yrate;
     int n = data->num;
     if(data->isPCA && nowindex >= n){
     	_new[0] = _new[0]/ coodrate;
     	_new[1] = _new[1]/ coodrate;
     }
     ag->refine(_pre, _new, nowindex) ;
     int atr = data->atr;
     
     double** v = new double*[atr];
     if(data->isPCA){
     		//PCAのときはePをvに入れる
     		for(int i = 0;i< atr;i++){
     			v[i] = new double[2];
     			v[i][0] = ag->getV(i,0);
     			v[i][1] = ag->getV(i,1);
     		}
     	}
     	else{
     		for(int i = 0 ; i< atr; i++){
     			v[i] = new double[2];
   				v[i][0] = ag->getB(n + i, 0);
    			v[i][1] = ag->getB(n + i, 1);
     		}
     	}
     	pcp->refine(v); 
     	if(nowindex < n)
     		md->setText(nowindex);
     	auto parent = GetGrandParent();
     	parent-> Refresh();
    
}

void AGIPane::calRange(int x2, int y2){
	xto = x2;
	yto = y2;
	std::vector<double> x;
    std::vector<double> y;
	x.push_back( (xfrom - getWidth()/2) /xrate);
    y.push_back( (yfrom - getHeight()/2) /yrate);
 	x.push_back( (x2 - getWidth()/2) /xrate);
    y.push_back( (y2 - getHeight()/2) /yrate);
    sort(x.begin(),x.end());
    sort(y.begin(),y.end());
    std::list<int> selected;
 	for(int i : data->filterindex){
 		double a = ag->getB(i,0);
 		double b = ag->getB(i,1);
 		if(a > x.at(0) && a < x.at(1)){
 			if(b >y.at(0) && b < y.at(1)){
 				selected.push_back(i);
 			}
 		}
 	}
 	data->setSIndex(selected,clickid);
 	auto parent = GetParent();
    parent->Refresh(); 
}
void AGIPane::calPoly(){
	std::list<int> selected;
	const double width = getWidth();
	const double height = getHeight();
	std::vector<pair<int,int> > calpolyvector(polyvector.begin(),polyvector.end());
	calpolyvector.push_back(polynow);
	double xg = 0;
	double yg = 0;
	for(auto v: calpolyvector){
		xg = xg + (double)v.first;
		yg = yg + (double)v.second;
	}
	xg = xg /calpolyvector.size();
	yg = yg /calpolyvector.size();  
	for(int i :data ->filterindex){
		double x = ag->getB(i,0)*xrate + width/2 -xg;
		double y = ag->getB(i,1)*yrate + height/2 -yg;
		for(int j= 0; j< calpolyvector.size();j++){
			pair<int,int> p1 = calpolyvector.at(j);
			pair<int,int> p2;
			if(j == calpolyvector.size()-1)
				p2 = calpolyvector.at(0);
			else p2 = calpolyvector.at(j+1);
			double p1x =(double) p1.first -xg;
			double p1y =(double) p1.second -yg;
			double p2x = (double )p2.first -xg;
			double p2y = (double) p2.second -yg;
			double d = p1x * p2y - p2x * p1y ;
			if(d != 0){
				double a = ( x * p2y - y * p2x) /d;
				double b = -1 *( x * p1y - y * p1x) /d;
				if(a >= 0 && b >= 0 && a+b <= 1){
					selected.push_back(i);
					break;
				}
			}
		}
	}
	data->setSIndex(selected,clickid);
	
}

void AGIPane::mouseReleased(wxMouseEvent& event){
	std::cerr << "MouseReleased " << std::endl;
	if(!isPoly){	
		if(nowindex != -1 && isDrug){
  			//calcagain(event.GetX(),event.GetY());
    		//このxとyが点の2次元配列に含まれるならOK
    		//もちろんある程度の誤差は許容しなければならない
    		ag->prjpush();
    		addLog(nowindex,_pre[0],_pre[1]);
  		}
  		else if(rangeselect){
	  		rangeselect = false;
  		} 
   		clickid  = clickid +2;
   		isMoved = false;
   		isDrug = false;
   		//ag -> writeagicood();
 	}
 	//poly選択の際にはこのイベントがあっても続く
 	else{
 		polyselect = false;
 	}
}
void AGIPane::rightClick(wxMouseEvent& event) {
	std::cerr << "Right click " << std::endl;
	const double x = event.GetX();
	const double y = event.GetY();
	//ドラッグ中に右クリックされると間違いなくバグるのであとで対処
	if(!isPoly){
		nowindex = getindex(x,y);
		if(nowindex !=  -1 && nowindex < data->num){
			data->setSIndex(nowindex);
			pcp->Refresh();
			md->setText(nowindex);
			Refresh();
		}
		else if(nowindex !=-1  && nowindex < data->aginum){
			int index = nowindex - data->num;
			 nowindex = -1;
			 std::cerr << data->atrname.at(index)<< std::endl;
		} 
	}
	else if(polystart){
		//ここで終了処理
		calPoly();
		polystart = false;
		clickid = clickid+2;
	}
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
 
 
int AGIPane::getWidth(){
    return GetSize().x;
}
 
int AGIPane::getHeight(){
    return GetSize().y;
}

//呼び出すのは最初の一度だけ

void AGIPane::setRate(){
   
    double xabs = std::max(ag->getXMax(),-(ag->getXMin()));
    double yabs = std::max(ag->getYMax(),-(ag->getYMin()));

    xrate = getWidth()  / (2 * xabs);
    yrate = getHeight() /(2 * yabs);
    //     std::cerr << xrate << std::endl;       
  }

 void AGIPane::setdelta(double d){
 	ag->setdelta(d);
 	//不十分　バグあり　ノード移動による射影の変更が反映されない
 	ag->calprj();
 	ag->cal2Mtr();
 	Refresh();
 }



void AGIPane::undo(){
	ag->backprj();
	Refresh();
	int atr = data->atr;
     int n = data->num;
     double** v = new double*[atr];
     for(int i = 0 ; i< atr; i++){
     	v[i] = new double[2];
     	if(data->isPCA){
     		v[i][0] = ag->getV(i,0);
     		v[i][1] = ag->getV(i,1);
     	}
     	else{
   			v[i][0] = ag->getB(n + i, 0);
    		v[i][1] = ag->getB(n + i, 1);
    	}
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
    int width = getWidth();
    int height = getHeight();
    int atr = data->atr;
    std::list<int> notselected = data->getNSIndex();
    int num = data->num;
    // ------------- draw some 2D ----------------
    prepare2DViewport(0, 0, width, height);
    glLoadIdentity();
 
    //white background
    glColor4f(1, 1, 1, 1);
    glBegin(GL_QUADS);
    glVertex3f(0, 0, 0);
    glVertex3f(width, 0, 0);
    glVertex3f(width, height, 0);
    glVertex3f(0, height, 0);
    glEnd();
   
    //辺を描く
    glColor4f(0.2f, 0.4f, 0.7f, 0.3f);
    glLineWidth(1);
	glBegin(GL_LINES);
	std::vector<std::pair<int,int> > edge = data->filteredge;
	for(int i = 0;i< edge.size();i++){
		int n1 = edge.at(i).first;
		int n2 = edge.at(i).second;
		glVertex3f(ag->getB(n1,0)*xrate + width/2, ag->getB(n1,1)*yrate + height/2,0);
		glVertex3f(ag->getB(n2,0)*xrate + width/2, ag->getB(n2,1)*yrate + height/2,0);
	}
    glEnd();
     glColor4f(0.2f, 0.4f, 0.7f, 1.0f);
    glPointSize(5.0);
    glBegin(GL_POINTS);
    //選択されていない点を書く
  	for(int i: notselected){
       	glVertex3f(ag->getB(i,0)*xrate + width/2, ag->getB(i,1)*yrate + height/2,0);
    }
    glEnd();
    glPointSize(8.0); 
    glBegin(GL_POINTS);
    //選択されている点を描く
    for(auto c:data->getCluster()){
    	RGB rgb = c.rgb; 
    	glColor4f(rgb.r, rgb.g, rgb.b, 1.0f);
    	for(int i :c.index ){ 
       		glVertex3f(ag->getB(i,0)*xrate + width/2, ag->getB(i,1)*yrate + height/2, 0);
    	}
	}
	glEnd();
	if(data->isPCA){
		//元の軸を描く
		glLineWidth(2.0);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		for(int i = 0;i< atr;i++){
			glVertex3f(width/2,height/2,0);
			glVertex3f(ag->getV(i,0)*xrate * coodrate + width/2, ag->getV(i,1)*yrate * coodrate+ height/2, 0);
		}
		glEnd();
		//PCPが軸選択モードのとき
		if(data->isCoord){
			int o = data->selectedorder;
			glColor4f(0.8f, 0.1f, 0.1f, 1.0f);
			glLineWidth(2.0);
			glBegin(GL_LINES);	
			glVertex3f(width/2,height/2,0);
			glVertex3f(ag->getV(data->order[o],0)*xrate * 2 + width/2, ag->getV(data->order[o],1)*yrate * 2+ height/2, 0);
			glEnd();
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			int from = std::max(o-1,0);
			int to = std::min(o+2,atr);
			for(int i = from;i< to;i++){
				int k = data->order[i];
				drawcoodname(k,width,height);
			}		
		}
		else{
			for(int i = 0;i< atr;i++)
				drawcoodname(i,width,height);
    	}
    	//agiで軸選択がされているとき
    	int nowcoord = data->selectedcoord;
    	if(data->selectedcoord != -1){
    		glColor4f(0.8f, 0.1f, 0.1f, 1.0f);
			glBegin(GL_LINES);	
			glVertex3f(width/2,height/2,0);
			glVertex3f(ag->getV(nowcoord,0)*xrate * coodrate + width/2, ag->getV(nowcoord,1)*yrate * coodrate+ height/2, 0);
			glEnd();
			drawcoodname(nowcoord,width,height);
    	}
    	glLineWidth(1);
	}
	//pcaを使っていないとき
	else{
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glPointSize(5.0);
		glLineWidth(1);
		for(int i = 0; i<data->atr ;i++){
			glBegin(GL_POINTS);
			int x = ag->getB(i+num,0)*xrate + width/2;
			int y = ag->getB(i+num,1)*yrate + height/2;
			glVertex3f(x, y, 0);
			glEnd();
			
			glRasterPos2d(x+5, y+5);
    		std::string str = data->atrname.at(i);
   			int size = (int)str.size();
    		for(int j = 0;j< size;j++){
       			char ic = str[j];
        		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,ic);
    		} 
    		glBegin(GL_LINES);
    		glVertex3f(width/2,height/2,0);
    		glVertex3f(x,y,0);
    		glEnd();  		
		}
		glLineWidth(1);
	}
	//範囲選択がされているとき
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
    if(polystart){
    	glColor4f(0.0f,0.0f,0.0f,1.0f);
    	glBegin(GL_LINE_LOOP);
    	for(auto p:polyvector)
    		glVertex3f(p.first,p.second,0);
    	glVertex3f(polynow.first,polynow.second,0);
    	glEnd();
    }
    glFlush();
    SwapBuffers();

}
void AGIPane::drawcoodname(int i, int w, int h){
	glRasterPos2d(ag->getV(i,0)*xrate * coodrate + w/2, ag->getV(i,1)*yrate *coodrate+ h/2);
    std::string str = data->atrname.at(i);
   	int size = (int)str.size();
    for(int j = 0;j< size;j++){
       	char ic = str[j];
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,ic);
    }   		
}


void AGIPane::keyPressed(wxKeyEvent& event){
	   std::cerr << "keyPressed" << std::endl; 
	   if(logmode && logid < logvector.size()){
	   	//nodeを読み込み計算する	
	   	auto nodelog = logvector.at(logid);
	   	if(logid %2 == 0){
	   		nowindex = nodelog.index;
	   		_pre[0] = nodelog.x;
	   		_pre[1] = nodelog.y;
	   		if(nowindex < data->num)
	   			data->setSIndex(nowindex);
	   		auto parent = GetGrandParent();
	   		parent->Refresh();
	   	}
	   	else {
	   		_new[0] = nodelog.x;
	   		_new[1] = nodelog.y;
	   		calcagain(nodelog.x*xrate + getWidth()/2, nodelog.y *yrate + getHeight()/2);

	   	}
	   	logid++;
	   	if(logid == logvector.size()){
	   	 std::cerr << " DONE ALL" << std::endl; 
	   	logmode = false;
	   }
	   }
	   
}
void AGIPane::addLog(int index,int x, int y){
	auto l = NodeLog(index,x,y);
	logvector.push_back(l);
}
void AGIPane::saveLog(){
	string d = data->dataname.at(data->dataid);
    string dir = "../data/" + d +"/"+ d+ "-log.dat";

    ofstream fs2(dir,ios::out | ios::binary);
    for(auto node: logvector ){
     	fs2.write((char*) &node.index,sizeof(int));
     	fs2.write((char*) &node.x,sizeof(double));
     	fs2.write((char*) &node.y,sizeof(double));
   }
   fs2.close();
    std::cerr << "SAVE LOG" << std::endl; 
}
void AGIPane::loadLog(){
  ifstream ifs;
  int status = STATUS_OK;
  string d = data->dataname.at(data->dataid);
  string dir = "../data/" + d +"/"+ d+ "-log.dat";
  ifs.open(dir,ios::binary);
  //src_size = ifs.tellg();
  logvector.clear();
  string str;
  if (ifs.fail()){
        cerr << "log読み込み失敗" << endl;
        exit(1);
  }
    
  ifs.seekg(0);
  while( (status == STATUS_OK) && (!ifs.eof())) {
  	//このindexが本当に大丈夫か確認必要あり
  	int index[1] ;
    ifs.read((char* ) index, sizeof(int) );
    double xy[2]; 
    ifs.read((char* ) xy, sizeof(double)*2 );
    auto l = NodeLog(index[0],xy[0],xy[1]);
    logvector.push_back(l);
  }
  ifs.close();
   std::cerr << " LOAD COMPLETE" << std::endl; 
  logmode = true;
}

