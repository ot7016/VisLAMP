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
#include "wx/radiobut.h"
#include "wx/filedlg.h"
#include <wx/wfstream.h>
#include "AGI.hpp"
#include "ReadData.hpp"
#include "PCP.hpp"
#include "MatrixView.hpp"


class MyApp: public wxApp{
    virtual bool OnInit();
    void ReCreate();
 
    wxFrame *frame;
    AGIPane *glPane;
    PCPPane *pcPane;
    ReadData *data;
    wxSlider *slider;
    MatrixView *md;
     wxSlider* thrslider ;
     wxSlider* vcslider;
     int thr100;
public:
    void radio1clicked(wxCommandEvent& event);
    void radio2clicked(wxCommandEvent& event);
    void radio3clicked(wxCommandEvent& event);
    void radio4clicked(wxCommandEvent& event);

    void undobuttonclicked(wxCommandEvent& event);
    void resetbuttonclicked(wxCommandEvent& event);
    void coodselect(wxCommandEvent& event);
    void getslider(wxCommandEvent& event);
    void getthrslider(wxCommandEvent& event);
    void getvcslider(wxCommandEvent& event);
    void openfile(wxCommandEvent& event);
};
 

IMPLEMENT_APP(MyApp);
enum
{
    ID_Hello = 1
};
