CXXFLAGS=-I/opt/local/include
LDFLAGS=-L/opt/local/lib

all: test_all

test_all: test_all.o test_GTP.o ../GTPEngine.o ../GoBoard.o ../GoPoint.o ../GoGame.o ../GoMove.o ../Exception.o
	g++ -o $@ $^ $(LDFLAGS) -lboost_unit_test_framework 