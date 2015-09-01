#include <sstream>
#include <iostream>
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
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame((wxFrame *)NULL, -1, wxT("Pcoordagi"),wxPoint(0,50), wxSize(1900,700));
 
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    wxWindow* mainPanel = new wxWindow((wxFrame*) frame,wxID_ANY);
    wxBoxSizer* mainsizer = new wxBoxSizer(wxVERTICAL);
    data = new ReadData();
    md  = new MatrixView((wxFrame*) frame,data);
    pcPane = new PCPPane( (wxWindow*)mainPanel, args,data);
    glPane = new AGIPane( (wxWindow*)mainPanel, args,data,pcPane,md); 
    
            
    wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);

    //パネルを生成、場合によっては拡張
   wxPanel* ctlPanel;
  
   wxButton* button2; 

    ctlPanel = new wxPanel((wxPanel*) mainPanel,wxID_ANY);
    button1 = new wxButton((wxPanel*) ctlPanel,wxID_ANY,"軸間の距離変更の有無");
    button2 = new wxButton((wxPanel*) ctlPanel,wxID_ANY,"ボタンのテスト2");
    wxStaticText* pftext;
    pftext = new wxStaticText((wxPanel*) ctlPanel,wxID_ANY,"Projection Factor");
    slider = new wxSlider((wxPanel*) ctlPanel,wxID_ANY,50,0,400);
    button1->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MyApp::buttonclicked),
        NULL, this);
    slider-> Connect( wxEVT_COMMAND_SLIDER_UPDATED, 
      wxCommandEventHandler(MyApp::getslider),
        NULL, this);

    sizer2->Add(button1,1,wxEXPAND);
    sizer2->Add(button2,1,wxEXPAND);
    sizer2->Add(pftext,1,wxEXPAND);
    sizer2->Add(slider,1,wxEXPAND);
    ctlPanel->SetSizer(sizer2);
    ctlPanel->SetAutoLayout(true);

   sizer->Add(glPane, 1, wxEXPAND);
   sizer->Add(pcPane,1,wxEXPAND);
   sizer->Add(ctlPanel,1,wxEXPAND);
   mainPanel->SetSizer(sizer);
   mainPanel->SetAutoLayout(true);

   mainsizer->Add(mainPanel,1,wxEXPAND);
   mainsizer->Add(md,1,wxEXPAND);
   frame->SetSizer(mainsizer);

    frame->SetAutoLayout(true);
    frame->Show();

    return true;
} 

void MyApp::buttonclicked(wxCommandEvent& WXUNUSED(event)){
  data-> turnLenVar();
  if(data->isLenVar())
    std::cerr << "軸間距離: 可変" << std::endl;
  else 
    std::cerr << "軸間距離: 固定" << std::endl;

}
void MyApp::getslider(wxCommandEvent& WXUNUSED(event)){
    float d = slider->GetValue()/100;
    glPane->setdelta(d);

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

BEGIN_EVENT_TABLE(PCPPane, wxGLCanvas)
EVT_MOTION(PCPPane::mouseMoved)
EVT_LEFT_DOWN(PCPPane::mouseDown)
EVT_LEFT_UP(PCPPane::mouseReleased)
EVT_RIGHT_DOWN(PCPPane::rightClick)
EVT_LEAVE_WINDOW(PCPPane::mouseLeftWindow)
EVT_SIZE(PCPPane::resized)
EVT_KEY_DOWN(PCPPane::keyPressed)
EVT_KEY_UP(PCPPane::keyReleased)
EVT_MOUSEWHEEL(PCPPane::mouseWheelMoved)
EVT_PAINT(PCPPane::render)
END_EVENT_TABLE()
 
// some useful events to use 