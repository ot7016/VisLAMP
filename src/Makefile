APP = vislamp

SOURCES = ReadData.cpp ConstraintSolver2D.cpp TSP.cpp MatrixView.cpp  AGI.cpp PCP.cpp HistView.cpp XXView.cpp MyFrame.cpp 

../bin/$(APP): $(SOURCES)
	clang++ -g -w  -std=c++0x  -o $@ $^ `wx-config --libs --cxxflags --gl-libs` -llbfgs -framework GLUT -framework OpenGL  -framework Accelerate

clean:
	rm -f *.o ../bin/vislamp
       
