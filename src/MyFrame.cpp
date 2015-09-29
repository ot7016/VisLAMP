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
    frame = new wxFrame((wxFrame *)NULL, -1, wxT("Pcoordagi"),wxPoint(0,50), wxSize(1500,700));
 
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    wxWindow* mainPanel = new wxWindow((wxFrame*) frame,wxID_ANY);
    wxBoxSizer* mainsizer = new wxBoxSizer(wxVERTICAL);
    data = new ReadData();
    md  = new MatrixView((wxFrame*) frame,data);
    pcPane = new PCPPane( (wxWindow*)mainPanel, args,data);
    glPane = new AGIPane( (wxWindow*)mainPanel, args,data,pcPane,md); 
    
            
    wxGridSizer* sizer2 = new wxGridSizer(10,1,30,100);

    //パネルを生成、場合によっては拡張
    wxPanel* ctlPanel;
    wxButton* undobutton;

    ctlPanel = new wxPanel((wxPanel*) mainPanel,wxID_ANY);
    button1 = new wxButton((wxPanel*) ctlPanel,wxID_ANY,"軸間の距離:可変");
    button2 = new wxButton((wxPanel*) ctlPanel,wxID_ANY,"軸間距離:角度");
    undobutton = new wxButton((wxPanel*) ctlPanel,wxID_ANY,"Undo");
    wxStaticText* pftext;
    pftext = new wxStaticText((wxPanel*) ctlPanel,wxID_ANY,"Projection Factor");
    slider = new wxSlider((wxPanel*) ctlPanel,wxID_ANY,50,0,400);
    button1->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MyApp::buttonclicked),
        NULL, this);
    button2->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MyApp::buttonclicked2),
        NULL, this);
    undobutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MyApp::undobuttonclicked),
        NULL, this);

    slider-> Connect( wxEVT_COMMAND_SLIDER_UPDATED, 
      wxCommandEventHandler(MyApp::getslider),
        NULL, this);

    sizer2->Add(button1,1,wxEXPAND);
    sizer2->Add(button2,1,wxEXPAND);
    sizer2->Add(undobutton,1,wxEXPAND);
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
  if(data->isLenVar()){
        button1->SetLabelText("軸間距離:可変");
  }
  else {
      button1->SetLabelText("軸間距離:固定");
  }

}
void MyApp::buttonclicked2(wxCommandEvent& WXUNUSED(event)){
  if(data->isTSP()){
    data->setTSP(false);
    button2->SetLabelText("軸間距離:角度");
  }
  else{ 
    data->setTSP(true);
    button2->SetLabelText("軸間距離:巡回路");
  }
  frame->Show();
}
void MyApp::undobuttonclicked(wxCommandEvent& WXUNUSED(event)){
  glPane->undo();
  frame->Show();
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

BEGIN_EVENT_TABLE(PCPSub, wxGLCanvas)
EVT_MOTION(PCPSub::mouseMoved)
EVT_LEFT_DOWN(PCPSub::mouseDown)
EVT_LEFT_UP(PCPSub::mouseReleased)
EVT_RIGHT_DOWN(PCPSub::rightClick)
EVT_LEAVE_WINDOW(PCPSub::mouseLeftWindow)
//EVT_SIZE(PCPPane::resized)
EVT_KEY_DOWN(PCPSub::keyPressed)
EVT_KEY_UP(PCPSub::keyReleased)
EVT_MOUSEWHEEL(PCPSub::mouseWheelMoved)
EVT_PAINT(PCPSub::render)
END_EVENT_TABLE()
 
// some useful events to use 