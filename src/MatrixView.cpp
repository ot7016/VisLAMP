//#include <stdio.h>
//#include <vector>
#include "MatrixView.hpp"

MatrixView::MatrixView(wxFrame* parent,ReadData* d):wxPanel(parent,wxID_ANY,wxDefaultPosition, wxSize(1000,300), wxFULL_REPAINT_ON_RESIZE){
	data  = d;
	//wxStaticText* title = new wxStaticText((wxPanel*) this,wxID_ANY,"MatrixView");
	

	//vPanel = new wxPanel(this,wxID_ANY);
	int atr = data->atr;
	wxGridSizer* sizer = new wxGridSizer((atr+10)/10,10,20,20); 
	Create(sizer);
	
	SetSizerAndFit(sizer);
	SetAutoLayout(true);

}

MatrixView::~MatrixView(){

}

void MatrixView::Create(wxGridSizer* sizer){
	wxPanel* nPanel = new wxPanel(this,0);
	wxPanel* space = new wxPanel(nPanel,wxID_ANY);
	name = new wxStaticText(nPanel,wxID_ANY,"                                        ");
	wxGridSizer* nsizer = new wxGridSizer(2,1,20,100);
	nsizer->Add(space,1,wxEXPAND);
	nsizer->Add(name,1,wxEXPAND);
	nPanel->SetSizer(nsizer);
	sizer->Add(nPanel,1,wxEXPAND);
	int atr = data->atr;
	for(int i = 0;i< atr;i++){
		wxPanel* vPanel = new wxPanel(this,i+1,wxDefaultPosition, wxSize(100,50), wxFULL_REPAINT_ON_RESIZE);
		wxGridSizer* vsizer = new wxGridSizer(2,1,20,100);
		wxStaticText* label =  new wxStaticText(vPanel,wxID_ANY,data->atrname.at(i));
		vsizer->Add(label,1,wxEXPAND);
		wxStaticText* value = new wxStaticText(vPanel,wxID_ANY,"");
		vsizer->Add(value,1,wxEXPAND);
		vPanel->SetAutoLayout(true);
		vPanel->SetSizer(vsizer);
		sizer->Add(vPanel,1,wxEXPAND);
	}
}


void MatrixView::ReCreate(){
	wxWindowList & children = GetChildren();
	int j = 0;
	for ( wxWindowList::Node *node = children.GetFirst(); node; node = node->GetNext() ){
		wxPanel *current = (wxPanel *)node->GetData();
		current->DestroyChildren();
	}
	DestroyChildren();
	int atr = data->atr;
	wxGridSizer* sizer =(wxGridSizer*) GetSizer();

	sizer->SetCols((atr+10)/10);
	Create(sizer);
}

void MatrixView::setText(int index){
	int atr = data->atr;
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
	//Show();

}
