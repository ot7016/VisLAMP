#include <stdio.h>
#include <vector>
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

	vPanel = new wxPanel(this,wxID_ANY);
	int atr = data->getatr();
	vsizer = new wxGridSizer(2,atr,20,20); 
	for(int i = 0;i< atr;i++){
		wxStaticText* label =  new wxStaticText(vPanel,wxID_ANY,data->getAtrName(i));
		vsizer->Add(label);
	}
	//wxwidget はobjectのコピーが出来ない 配列も作れない 
	//sizerのメソッドをうまく使えばobjectの追加削除も可能
	for(int i = 0;i<atr;i++){
		wxStaticText* value = new wxStaticText(vPanel,wxID_ANY,"");
		vsizer->Add(value);
	}
	vPanel->SetSizer(vsizer);
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(nPanel);
	sizer->Add(vPanel);
	SetSizer(sizer);
	SetAutoLayout(true);

}

MatrixView::~MatrixView(){

}

void MatrixView::setText(int index){
	int atr = data->getatr();
	//wxWindowList wlist = GetChildren();
	//wxWindowList::iterator it = wlist.begin(); 
	name->SetLabelText(data->getName(index));

	wxWindowList & children = vPanel->GetChildren();
	int j = 0;
	for ( wxWindowList::Node *node = children.GetFirst(); node; node = node->GetNext() ){
		wxStaticText *current = (wxStaticText *)node->GetData();
		if (j<atr){
			int o = data->getOrder(j);
			current->SetLabelText(data->getAtrName(o));
		}
		else{
			int o = data->getOrder(j-atr);
			current->SetLabelText(std::to_string(data->getD(index,o)));
		}
		j++;
	}

     // .. do something with current
	SetAutoLayout(true);
	Show();

}
