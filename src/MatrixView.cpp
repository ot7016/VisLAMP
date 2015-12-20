//#include <stdio.h>
//#include <vector>
#include "MatrixView.hpp"

MatrixView::MatrixView(wxWindow* parent,ReadData* d):wxPanel(parent,wxID_ANY,wxDefaultPosition, wxSize(400,400), wxFULL_REPAINT_ON_RESIZE){
	data  = d;
	//vPanel = new wxPanel(this,wxID_ANY);
	int atr = data->atr;
    sizer = new wxGridSizer(atr+1,2,20,20); 
	Create();
	//SetScrollbar(wxVERTICAL,0,5,std::max(atr+1,5),true);
	SetSizer(sizer);
	SetAutoLayout(true);

}

MatrixView::~MatrixView(){
}

void MatrixView::Create(){
	int atr = data->atr;
	wxPanel* space = new wxPanel(this,-1);
	sizer->Add(space,1,wxEXPAND);
	name = new wxStaticText(this,-2,"                                        ");
	sizer->Add(name,1,wxEXPAND);
	for(int i = 0;i< atr;i++){
		wxStaticText* label =  new wxStaticText(this,i,data->atrname.at(i));
		sizer->Add(label,1,wxEXPAND);
		wxStaticText* value =  new wxStaticText(this,i+atr,"");
		sizer->Add(value,1,wxEXPAND);
	} 
	
}


void MatrixView::ReCreate(){
	
	DestroyChildren();
	int atr = data->atr;
	sizer->SetRows(atr+1);
	Create();
	Layout();

}

void MatrixView::setText(int index){
	int atr = data->atr;
	//wxWindowList::iterator it = wlist.begin(); 
	//name->SetLabelText(data->name.at(index));

	wxWindowList & children = GetChildren();
	int j = 0;
	for ( wxWindowList::Node *node = children.GetFirst(); node; node = node->GetNext() ){
		if(j != 0 ){
			wxStaticText *current = (wxStaticText *)node->GetData();
			if (j == 1)
				current->SetLabelText(data->name.at(index));
			else if(j %2 == 0){
				int o = data->order[j/2 -1];
				current->SetLabelText(data->atrname.at(o));
			}
			else {
				int o = data->order[j/2-1];
				current->SetLabelText(std::to_string(data->D[index][o]));
			}
		}
		j++;
		
	}
     // .. do something with current
	//Show();

}
