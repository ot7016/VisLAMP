#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <dirent.h>
#include <exception>
#include <sstream>
#include <iostream>

//#include <vecLib/vecLib.h>

#include <wx/wxprec.h>
//#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
//#include "wx/slider.h"
#include "wx/stattext.h"
#include "wx/glcanvas.h"
#include "AGI.hpp"
#include "ReadData.hpp"
#include "PCP.hpp"
#include "MatrixView.hpp"

/* //エラーが出るので一旦停止
class MyFrame: public wxFrame{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
 //   void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
  //  void OnAbout(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();

};
*/

class MyApp: public wxApp{
    virtual bool OnInit();
 
    wxFrame *frame;
    AGIPane *glPane;
    PCPPane *pcPane;
    ReadData *data;
    wxButton* button1;
    wxButton* button2; 
    wxSlider *slider;
    MatrixView *md;
public:
    void buttonclicked(wxCommandEvent& event);
    void buttonclicked2(wxCommandEvent& event);
    void undobuttonclicked(wxCommandEvent& event);
    void getslider(wxCommandEvent& event);
};

IMPLEMENT_APP(MyApp);
enum
{
    ID_Hello = 1
};
