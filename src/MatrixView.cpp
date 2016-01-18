//#include <stdio.h>
//#include <vector>
#include "MatrixView.hpp"

MatrixView::MatrixView(wxWindow* parent,ReadData* d,int l):wxGrid(parent,wxID_ANY,wxDefaultPosition, wxSize(l*100,(d->atr+1)*50), wxFULL_REPAINT_ON_RESIZE){
	data  = d;
	//vPanel = new wxPanel(this,wxID_ANY);
	const int atr = data->atr;
    //sizer = new wxGridSizer(atr+1,2,20,20); 
    nowrow = atr;
    CreateGrid(atr+1,2);
	Create();
	SetAutoLayout(true);

}

MatrixView::~MatrixView(){
}

void MatrixView::Create(){
	const int atr = data->atr;
	//wxGridで実装してみる
	for(int i = 0 ;i< atr;i++){
		SetRowSize(i,30);
		SetColSize(i,150);
	}
	SetCellValue(0,0,"");
	SetCellValue(0,1,"");
	for(int i = 0; i< atr;i++){
		SetCellValue(i+1,0,data->atrname.at(i));
		SetCellValue(i+1,1,"");
	}
	/*
	wxPanel* space = new wxPanel(this,-1);
	sizer->Add(space,1,wxEXPAND);
	name = new wxStaticText(this,-2,"                                        ");
	sizer->Add(name,1,wxEXPAND);
	for(int i = 0;i< atr;i++){
		wxStaticText* label =  new wxStaticText(this,i,data->atrname.at(i));
		label->SetMinSize(wxSize(100,50));
		sizer->Add(label,0,0);
		wxStaticText* value =  new wxStaticText(this,i+atr,"");
		value->SetMinSize(wxSize(100,50));
		sizer->Add(value,0,0);
	} 
	*/
	SetScrollbar(wxVERTICAL,0,10,std::max(atr+1,10),true);
	AlwaysShowScrollbars(false,true);
	SetSizer(sizer);
	if( !CanScroll(wxVERTICAL) )
		std::cerr << "スクロール不可" << std::endl;

	
}


void MatrixView::ReCreate(){
	const int atr = data->atr;
	//sizer->SetRows(atr+1);
	while(nowrow != atr){
		if(nowrow > atr){
			DeleteRows();
			nowrow--;
		}
		else{
			AppendRows();
			nowrow++;
		} 
	}
	Create();
	Layout();

}

void MatrixView::setText(int index){
	const int atr = data->atr;
	/*
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
	*/

	SetCellValue(0,1,data->name.at(index));
	for(int i = 0;i<atr;i++){
		int o = data->order[i];
		SetCellValue(i+1,0,data->atrname.at(o));
		SetCellValue(i+1,1,std::to_string(data->D[index][o]));
	}
     // .. do something with current
	
	//Show();

}
