APP = pcoordagi

SOURCES = ReadData.cpp ConstraintSolver2D.cpp TSP.cpp MatrixView.cpp  AGI.cpp PCP.cpp MyFrame.cpp 

../bin/$(APP): $(SOURCES)
	clang++ -g -w  -std=c++0x  -o $@ $^ `wx-config --libs --cxxflags --gl-libs` -llbfgs -framework GLUT -framework OpenGL

clean:
	rm -f *.o ../bin/pcoordagi


#myapp: protoagi.o myframe.o pcp.o ctl.o
#	g++  -o myapp protoagi.o myframe.o pcp.o ctl.o `wx-config --libs --cxxflags --gl-libs`
#myframe.o: MyFrame.cpp
#	g++ MyFrame.cpp -o myframe `wx-config --libs --cxxflags --gl-libs`
#protoagi.o: ProtoAGI.cpp
#	g++ ProtoAGI.cpp -o protoagi `wx-config --libs --cxxflags --gl-libs` -c
#pcp.o: PCP.cpp
#	g++ PCP.cpp -o pcp `wx-config --libs --cxxflags --gl-libs` -c
#ctl.o: ControlPanel.cpp
#	g++ ControlPanel.cpp -o ctl `wx-config --libs --cxxflags --gl-libs` -c


       
