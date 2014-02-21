IDIR =../include
ODIR=../objects
CDIR=..
CPP=g++
CPPFLAGS=-I$(IDIR) -Wall -g

_DEPS = cppn.hpp function.hpp visualator.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o cppn.o function.o visualator.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

code: $(OBJ)
	$(CPP) -o $(CDIR)/$@ $^

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 