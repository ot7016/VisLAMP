#include <stdio.h>
#include <vector>
#include <string>
#include "MatrixView.hpp"

MatrixView::MatrixView(wxFrame* parent,ReadData* d):wxPanel(parent,wxID_ANY){
	data  = d;
	//wxStaticText* title = new wxStaticText((wxPanel*) this,wxID_ANY,"MatrixView");

	int atr = data->getatr();
	sizer = new wxGridSizer(2,atr,20,20); 
	for(int i = 0;i< atr;i++){
		wxStaticText* label =  new wxStaticText(this,wxID_ANY,data->getAtrName(i));
		sizer->Add(label);
	}
	//wxwidget はobjectのコピーが出来ない 配列も作れない 
	//sizerのメソッドをうまく使えばobjectの追加削除も可能
	//value = new std::vector<wxStaticText>(atr);
	// value = new wxStaticText[atr];
	// wxStaticText value[atr];
	for(int i = 0;i<atr;i++){
		wxStaticText* value = new wxStaticText(this,wxID_ANY,"");
		//value.push_back(v);
		//value[i].SetSize(20,50);
		//value[i].SetLabelText("");
		sizer->Add(value);
	}
	SetSizer(sizer);
	SetAutoLayout(true);

}

MatrixView::~MatrixView(){

}
//wxStaticText MatrixView::getMVText(){
//	return tx;
//}
void MatrixView::setText(int index){
	int atr = data->getatr();
	wxWindowList wlist = GetChildren();
	wxWindowList::iterator it = wlist.begin(); 
	for(int i = 0;i<atr;i++){
		++it;
	}
	//while(it != wlist.end()){
	//	RemoveChild(*it);
		//}
	wxWindowList & children = GetChildren();
	int j = 0;
	for ( wxWindowList::Node *node = children.GetFirst(); node; node = node->GetNext() ){
		if(j>=atr){
			wxStaticText *current = (wxStaticText *)node->GetData();
			current->SetLabelText(std::to_string(data->getD(index,atr-j)));
		}
		j++;
	}

     // .. do something with current




	//for(int i = 0;i< atr;i++){
	//	wxStaticText* value = new wxStaticText(this,wxID_ANY,std::to_string(data->getD(index,i)));
	//	sizer->Add(value);
	//	}
	SetAutoLayout(true);
	Show();

}
