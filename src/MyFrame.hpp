#include <stdio.h>
#include <vector>
#include <string>
#include <fstream>
#include <dirent.h>
#include <exception>
#include <sstream>
#include <iostream>



#include <wx/wxprec.h>
#include <wx/textdlg.h>
//#ifndef WX_PRECOMP
#include "wx/wx.h"
#include "wx/panel.h"
#include "wx/event.h"
#include "wx/sizer.h"
#include "wx/stattext.h"
#include "wx/glcanvas.h"
#include "wx/radiobut.h"
#include "AGI.hpp"
#include "ReadData.hpp"
#include "PCP.hpp"
#include "MatrixView.hpp"
#include "HistView.hpp"
#include "XXView.hpp"





class MyApp: public wxApp{
    virtual bool OnInit();
    void ReCreate();
 
    wxFrame *frame;
    AGIPane *glPane;
    PCPPane *pcPane;
    PCPBorder* lowerborder;
    ReadData *data;
    MatrixView *md;
    HistView* histview ;
    wxSlider* vcslider;
    wxCheckBox* polyselectbox;
    wxCheckBox* pcpvisiblebox;
    int thr100;
    wxRadioButton* rb1;
    wxRadioButton* rb2;
    wxRadioButton* rb3;
    wxRadioButton* rb4;
    wxRadioButton* rb5;
    wxRadioButton* rb6;
    wxMenu* menu3;

    bool iMac = true;
    int up,low;
public:
    void lenVarclicked(wxCommandEvent& event);
    void isTSPclicked(wxCommandEvent& event);
    void angleMaxclicked(wxCommandEvent& event);

    void undobuttonclicked(wxCommandEvent& event);
    void resetbuttonclicked(wxCommandEvent& event);
    void visiblebuttonclicked(wxCommandEvent& event);
    void thrbuttonclicked(wxCommandEvent& event);
    void polyselect(wxCommandEvent& event);
    void coodselect(wxCommandEvent& event);
   // void getslider(wxCommandEvent& event);
    void getthrslider(wxCommandEvent& event);
    void getvcslider(wxCommandEvent& event);
    void openfile(wxCommandEvent& event);
    void savelog(wxCommandEvent& event);
    void makedatabuttonclicked(wxCommandEvent& event);
    void HandleExit( wxCommandEvent & );
    void opentemp(wxCommandEvent& event);
};
 

IMPLEMENT_APP(MyApp);
enum
{
    ID_Hello = 1
};
