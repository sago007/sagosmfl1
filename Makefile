CC=$(CROSS)gcc
CXX=$(CROSS)g++
LD=$(CROSS)ld
AR=$(CROSS)ar
PKG_CONFIG=$(CROSS)pkg-config

-include sago/Makefile.sago

BASE_LIBS=-lboost_program_options -lboost_serialization -lcppdb $(SAGO_BASE_LIBS)
BASE_CFLAGS=-c -g -O1 -Wall -std=c++11 -Iinclude



PROGRAMNAME=sagosmfl1

O_FILES=${PROGRAMNAME}.o MainMenu.o Game.o GameConsole.o CommandProcessor.o model/Misc.o model/World.o model/Store.o model/TileManager.o ${SAGO_O_FILES}

total: ${PROGRAMNAME}

clean: 
	rm -f */*.o *.o *.P */*.P ${PROGRAMNAME}

${PROGRAMNAME}: $(O_FILES)
	$(CXX) -O -o ${PROGRAMNAME} $(O_FILES) $(BASE_LIBS)

%.o : %.cpp
	$(CXX) -MD ${BASE_CFLAGS} -o $@ $<
	@mkdir -p `dirname .$(CROSS)deps/$*.P` && cp $*.d .$(CROSS)deps/$*.P; \
             sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
                 -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> .$(CROSS)deps/$*.P; \
             rm -f $*.d

-include .$(CROSS)deps/*.P
