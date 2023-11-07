GXX = g++

OBJDIR   = objects
CPPFLAGS = -g -Wall
LNKFLAGS = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
EXENAME  = CMPM80K

OBJECTS  = $(patsubst %, $(OBJDIR)/%, $(subst .cpp,.o,$(wildcard *.cpp)))

all: $(OBJECTS)
	echo $(OBJECTS)
	$(GXX) -o $(EXENAME) $(OBJECTS) $(LNKFLAGS)

clean:
	-rm $(EXENAME)
	-rm $(OBJDIR) -r 
	-rm *.o

objdir:
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o : %.cpp objdir
	$(GXX) $(CPPFLAGS) -o $@ -c $<
