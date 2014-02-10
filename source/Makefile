IDIR =../include
ODIR=.
CDIR=..
CPP=g++
CPPFLAGS=-I$(IDIR) -Wall

_DEPS = cppn.hpp function.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o cppn.o function.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

code: $(OBJ)
	$(CPP) -o $(CDIR)/$@ $^

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 