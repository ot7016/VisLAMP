//#include <stdio.h>
//#include <vector>
#include <string>
#include "MatrixView.hpp"

MatrixView::MatrixView(wxFrame* parent,ReadData* d):wxPanel(parent,wxID_ANY){
	data  = d;
	//wxStaticText* title = new wxStaticText((wxPanel*) this,wxID_ANY,"MatrixView");
	wxPanel* nPanel = new wxPanel(this,wxID_ANY);
	wxPanel* space = new wxPanel(nPanel,wxID_ANY);
	name = new wxStaticText(nPanel,wxID_ANY,"                                        ");
	wxGridSizer* nsizer = new wxGridSizer(2,1,20,100);
	nsizer->Add(space);
	nsizer->Add(name);
	nPanel->SetSizer(nsizer);

	//vPanel = new wxPanel(this,wxID_ANY);
	int atr = data->atr;
	wxGridSizer* sizer = new wxGridSizer((atr+10)/10,10,20,20); 
	sizer->Add(nPanel);
	for(int i = 0;i< atr;i++){
		wxPanel* vPanel = new wxPanel(this,wxID_ANY);
		wxGridSizer* vsizer = new wxGridSizer(2,1,20,100);
		wxStaticText* label =  new wxStaticText(vPanel,wxID_ANY,data->atrname.at(i));
		vsizer->Add(label);
		wxStaticText* value = new wxStaticText(vPanel,wxID_ANY,"");
		vsizer->Add(value);
		vPanel->SetSizer(vsizer);
		sizer->Add(vPanel);


	}
	//wxwidget はobjectのコピーが出来ない 配列も作れない 
	//sizerのメソッドをうまく使えばobjectの追加削除も可能
	//for(int i = 0;i<atr;i++){
	//	wxStaticText* value = new wxStaticText(vPanel,wxID_ANY,"");
	//	vsizer->Add(value);
	//}
	//vPanel->SetSizer(vsizer);
	//wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	//sizer->Add(nPanel);
	//sizer->Add(vPanel);
	SetSizer(sizer);
	SetAutoLayout(true);

}

MatrixView::~MatrixView(){

}

void MatrixView::setText(int index){
	int atr = data->atr;
	//wxWindowList wlist = GetChildren();
	//wxWindowList::iterator it = wlist.begin(); 
	name->SetLabelText(data->name.at(index));

	wxWindowList & children = GetChildren();
	int j = 0;
	for ( wxWindowList::Node *node = children.GetFirst(); node; node = node->GetNext() ){
		wxPanel *current = (wxPanel *)node->GetData();
		if(j >1){
			wxWindowList & children =current->GetChildren();
			wxWindowList::Node *pnode = children.GetFirst(); 
			wxStaticText *atrname = (wxStaticText *) pnode->GetData();
			int o = data->order[j-1];
		    atrname->SetLabelText(data->atrname.at(o));
			pnode = pnode->GetNext();
		    wxStaticText* value = (wxStaticText *)pnode->GetData();	
			value->SetLabelText(std::to_string(data->D[index][o]));		
		}
		j++;
	}
     // .. do something with current
	SetAutoLayout(true);
	//Show();

}
