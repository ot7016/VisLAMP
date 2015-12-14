#include "MyFrame.hpp"
// include OpenGL
//#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"

//#else
//#include <GL/glu.h>
//#include <GL/gl.h>
//#endif

bool MyApp::OnInit()
{
    
    data = new ReadData(); 
    frame = new wxFrame((wxFrame *)NULL, -1, wxT("Pcoordagi"),wxPoint(0,50), wxSize(1200,1000));
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    wxWindow* mainPanel = new wxWindow((wxFrame*) frame,wxID_ANY);
    wxBoxSizer* mainsizer =  new wxBoxSizer(wxHORIZONTAL);
   
    wxWindow* pcPanel = new wxWindow((wxWindow*)mainPanel,wxID_ANY);
    PCPBorder* upper = new PCPBorder((wxWindow*) pcPanel,false,data,25);
    PCPBorder* lower = new PCPBorder((wxWindow*) pcPanel,true,data,25);
    pcPane = new PCPPane( (wxWindow*) pcPanel, args,data,lower);
    wxBoxSizer* pcsizer = new wxBoxSizer(wxVERTICAL);
    pcsizer->Add(upper);
    pcsizer->Add(pcPane);
    pcsizer->Add(lower);
    pcPanel->SetSizer(pcsizer);

    glPane = new AGIPane( (wxWindow*)mainPanel, args,data,pcPane); 
    
    
   
   wxWindow* underPanel = new wxWindow((wxFrame*) frame,wxID_ANY);
   wxBoxSizer* undersizer = new wxBoxSizer(wxHORIZONTAL);
    XXView* xxview = new XXView((wxWindow*) underPanel,data,glPane->ag);
    md  = new MatrixView((wxWindow*) underPanel,data);
    glPane->setMV(md);
    //パネルを生成、場合によっては拡張
    wxPanel* leftctl = new wxPanel((wxWindow*) underPanel,wxID_ANY);
    wxGridSizer* leftsizer = new wxGridSizer(10,1,0,0);
    wxPanel* rightctl = new wxPanel((wxWindow*) underPanel,wxID_ANY);
    wxGridSizer* rightsizer = new wxGridSizer(10,1,0,0);
   

    wxPanel* P1 = new wxPanel((wxPanel*) rightctl,wxID_ANY);
     wxBoxSizer* P1sizer = new wxBoxSizer(wxHORIZONTAL);
     wxStaticText* text1 = new wxStaticText((wxPanel*) P1,wxID_ANY,"軸間の距離");
    rb1 = new wxRadioButton((wxPanel*) P1, -1, wxT("可変"),wxDefaultPosition,wxDefaultSize, wxRB_GROUP);
    rb1->SetValue(true);
    rb2 = new wxRadioButton((wxPanel*) P1, -1, wxT("固定"));
    rb1->Connect(wxEVT_RADIOBUTTON,wxCommandEventHandler(MyApp::radio1clicked),NULL,this);
    rb2->Connect(wxEVT_RADIOBUTTON,wxCommandEventHandler(MyApp::radio2clicked),NULL,this);
    P1sizer->Add(text1);
    P1sizer->Add(rb1);
    P1sizer->Add(rb2);
    P1->SetSizer(P1sizer);
    P1->SetAutoLayout(true);
    wxPanel* P2 = new wxPanel((wxPanel*) rightctl,wxID_ANY);
     wxBoxSizer* P2sizer = new wxBoxSizer(wxHORIZONTAL);
     wxStaticText* text2 = new wxStaticText((wxPanel*) P2,wxID_ANY,"軸間距離");
    rb3 = new wxRadioButton((wxPanel*) P2, -1, wxT("巡回路"),wxDefaultPosition,wxDefaultSize, wxRB_GROUP);
  
    rb4 = new wxRadioButton((wxPanel*) P2, -1, wxT("角度"));
    rb4->SetValue(true);
    rb3->Connect(wxEVT_RADIOBUTTON,wxCommandEventHandler(MyApp::radio3clicked),NULL,this);
    rb4->Connect(wxEVT_RADIOBUTTON,wxCommandEventHandler(MyApp::radio4clicked),NULL,this);
    P2sizer->Add(text2);
    P2sizer->Add(rb3);
    P2sizer->Add(rb4);
    P2->SetSizer(P2sizer);
    P2->SetAutoLayout(true);
    wxButton* undobutton = new wxButton((wxPanel*) leftctl,wxID_ANY,"射影を戻す");
    wxButton* resetbutton1 = new wxButton((wxPanel*) leftctl,wxID_ANY,"選択をリセット");
    wxButton* resetbutton2 = new wxButton((wxPanel*) rightctl,wxID_ANY,"選択をリセット");
    wxCheckBox* polyselectbox = new wxCheckBox((wxPanel*) leftctl,wxID_ANY,"多角形選択");
    polyselectbox-> Connect( wxEVT_CHECKBOX, wxCommandEventHandler(MyApp::polyselect),NULL, this);
    wxCheckBox* coodselectbox = new wxCheckBox((wxPanel*) rightctl,wxID_ANY,"軸選択モード");
    coodselectbox-> Connect( wxEVT_CHECKBOX, wxCommandEventHandler(MyApp::coodselect),NULL, this);
    wxStaticText* pftext;
    pftext = new wxStaticText((wxPanel*) leftctl,wxID_ANY,"Projection Factor");
    slider = new wxSlider((wxPanel*) leftctl,wxID_ANY,50,0,400);

    wxStaticText* thrtext = new wxStaticText((wxPanel*) leftctl,wxID_ANY,"類似度の閾値");
    wxButton* thrbutton = new wxButton((wxPanel*) leftctl,wxID_ANY,"類似度初期化" );
    //thrslider = new wxSlider((wxPanel*) rightctl,wxID_ANY,data->thr*100,0,data->thr*200);
     //thr100 = data->thr*100;
    wxStaticText* vctext = new wxStaticText((wxPanel*) leftctl,wxID_ANY,"V-Centrality");
    int avevc =  data->getEdge().size()*2 / data->num;
    vcslider = new wxSlider((wxPanel*) leftctl,wxID_ANY,0,0,avevc*200);
    histview = new HistView((wxPanel*) leftctl,vcslider,data);
    
    undobutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MyApp::undobuttonclicked),NULL, this);
    
    resetbutton1->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MyApp::resetbuttonclicked),NULL, this);
    resetbutton2->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MyApp::resetbuttonclicked),NULL, this);
    
     thrbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MyApp::thrbuttonclicked),NULL, this);
    slider-> Connect( wxEVT_COMMAND_SLIDER_UPDATED, 
      wxCommandEventHandler(MyApp::getslider),NULL, this);
    /*
    thrslider-> Connect( wxEVT_COMMAND_SLIDER_UPDATED, 
      wxCommandEventHandler(MyApp::getthrslider),NULL, this);
    */
    vcslider-> Connect( wxEVT_COMMAND_SLIDER_UPDATED, 
      wxCommandEventHandler(MyApp::getvcslider),NULL, this);

    leftsizer->Add(polyselectbox,1,wxEXPAND);
    leftsizer->Add(undobutton,1,wxEXPAND);
    leftsizer->Add(resetbutton1,1,wxEXPAND);
    leftsizer->Add(pftext,1,wxEXPAND);
    leftsizer->Add(slider,1,wxEXPAND);
    leftsizer->Add(histview,1,wxEXPAND);
    leftsizer->Add(thrtext,1,wxEXPAND);
    leftsizer->Add(thrbutton,1,wxEXPAND);
    leftsizer->Add(vctext,1,wxEXPAND);
    leftsizer->Add(vcslider,1,wxEXPAND);

    leftctl->SetSizer(leftsizer);
    leftctl->SetAutoLayout(true);
    

    rightsizer->Add(P1,1,wxEXPAND);
    rightsizer->Add(P2,1,wxEXPAND);
    rightsizer->Add(coodselectbox,1,wxEXPAND);
    rightsizer->Add(resetbutton2,1,wxEXPAND);
   // rightsizer->Add(thrslider,1,wxEXPAND);
    rightctl->SetSizer(rightsizer);
    rightctl->SetAutoLayout(true);
   
 
   mainsizer->Add(glPane, 1, wxEXPAND);
   mainsizer->Add(pcPanel,1,wxEXPAND);
   mainPanel->SetSizer(mainsizer);
   mainPanel->SetAutoLayout(true);

   undersizer->Add(leftctl,1,wxEXPAND);
   undersizer->Add(xxview,1,wxEXPAND);
   undersizer->Add(md,1,wxEXPAND);
   underPanel->SetSizer(undersizer);
   undersizer->Add(rightctl,1,wxEXPAND);
   underPanel->SetAutoLayout(true);

   sizer->Add(mainPanel,1,wxEXPAND);
   sizer->Add(underPanel,1,wxEXPAND);
   frame->SetSizer(sizer);
   wxMenuBar* menubar = new wxMenuBar();
   wxMenu* menu1 = new wxMenu();
   int k = 0;
   for(string s:data->dataname){
   menu1->Append(k, s, _T("Opens an existing file"));
   menu1->Bind(wxEVT_COMMAND_MENU_SELECTED,&MyApp::openfile,this,k) ;
   k++;
   }
   menubar->Append(menu1, _T("&Data"));
   frame->SetMenuBar(menubar);
    frame->SetAutoLayout(true);

    frame->Show();
    /*
    cerr << "VENDOR:   " << glGetString(GL_VENDOR)   << endl;
    cerr << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cerr << "VERSION:  " << glGetString(GL_VERSION)  << endl;
    cerr << "GLSL:     " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl << endl;
    */
    return true;

} 
 void MyApp::ReCreate(){

    glPane->ReCreate();
    pcPane->ReCreate();
    md->ReCreate();
    histview->ReCreate();
    //thrslider->SetValue(data->thr*100);
    //thrslider->SetMin(0);
    //thrslider->SetMax(data->thr*200);
    int vclen = vcslider->GetLineSize();
    int avevc =  data->getEdge().size()*2 / data->num;
    vcslider->SetRange(0,avevc);
    vcslider->SetLineSize(vclen);
    vcslider->SetValue(0);
    rb1->SetValue(true);
    rb4->SetValue(true);
    frame->Refresh();
    int j = 0 ;
 }
 
 void MyApp::radio1clicked(wxCommandEvent& event){
   data->isLenVar = true;
   //現在は長さの計算を描画のときにやってないので要対処
   frame->Refresh();
}
void MyApp::radio2clicked(wxCommandEvent& event){
   data->isLenVar = false;
   frame->Refresh();
}
void MyApp::radio3clicked(wxCommandEvent& event){
   data->isTSP = true;
   frame->Refresh();
}
void MyApp::radio4clicked(wxCommandEvent& event){
   data->isTSP = false;
   frame->Refresh();
}


void MyApp::undobuttonclicked(wxCommandEvent& WXUNUSED(event)){
  glPane->undo();
  frame->Show();
}
void MyApp::polyselect(wxCommandEvent& event){
  glPane->isPoly = !glPane->isPoly  ;
}

void MyApp::coodselect(wxCommandEvent& event){
  data->setCoodSelected();
}
void MyApp::resetbuttonclicked(wxCommandEvent& WXUNUSED(event)){
  data->resetselected();
  histview->reset();
 // histview->setValue(data->thr);
 // thrslider->SetValue(thr100);
  vcslider->SetValue(0);
  frame->Refresh();
}

void MyApp::getslider(wxCommandEvent& WXUNUSED(event)){
    float d = slider->GetValue()/100;
    glPane->setdelta(d);

}
void MyApp::thrbuttonclicked(wxCommandEvent& WXUNUSED(event)){
  double thr =  data->pthr;
  data->thr = thr;
  histview->setValue(thr);
  data->recalEdge(thr);
  frame->Refresh();
  }
/*
void MyApp::getthrslider(wxCommandEvent& WXUNUSED(event)){
    double i =  thrslider->GetValue()/100.0;
    data->recalEdge(i); 
    vcslider->SetValue(0); 
    frame->Refresh();
}
*/

void MyApp::getvcslider(wxCommandEvent& WXUNUSED(event)){
    double i =  vcslider->GetValue()/100.0;
    data->calDgCentrality(i);  
    frame->Refresh();
}

void MyApp::openfile(wxCommandEvent& event){
  //id がdatasetの番号を表す　あらかじめどれが何かは読み込んである
  int id = event.GetId();
   cerr<< id <<endl;
   data->clearall();
   data->read(id);
   ReCreate();
   frame->Refresh();
  
}


BEGIN_EVENT_TABLE(AGIPane, wxGLCanvas)
EVT_MOTION(AGIPane::mouseMoved)
EVT_LEFT_DOWN(AGIPane::mouseDown)
EVT_LEFT_UP(AGIPane::mouseReleased)
EVT_RIGHT_DOWN(AGIPane::rightClick)
EVT_LEAVE_WINDOW(AGIPane::mouseLeftWindow)
EVT_SIZE(AGIPane::resized)
EVT_KEY_DOWN(AGIPane::keyPressed)
EVT_KEY_UP(AGIPane::keyReleased)
EVT_MOUSEWHEEL(AGIPane::mouseWheelMoved)
EVT_PAINT(AGIPane::render)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(PCPSub, wxGLCanvas)
EVT_MOTION(PCPSub::mouseMoved)
EVT_LEFT_DOWN(PCPSub::mouseDown)
EVT_LEFT_UP(PCPSub::mouseReleased)
EVT_RIGHT_DOWN(PCPSub::rightClick)
EVT_LEAVE_WINDOW(PCPSub::mouseLeftWindow)
EVT_MOUSEWHEEL(PCPSub::mouseWheelMoved)
EVT_PAINT(PCPSub::render)
END_EVENT_TABLE()
 

BEGIN_EVENT_TABLE(PCPBorder, wxGLCanvas)
EVT_PAINT(PCPBorder::render)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(HistView, wxGLCanvas)
EVT_MOTION(HistView::mouseMoved)
EVT_LEFT_DOWN(HistView::mouseDown)
EVT_LEFT_UP(HistView::mouseReleased)
EVT_PAINT(HistView::render)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(XXView, wxGLCanvas)
EVT_PAINT(XXView::render)
END_EVENT_TABLE()

// some useful events to use 