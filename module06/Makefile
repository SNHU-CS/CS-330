CC = g++
INCLUDE_DIRS = -I../includes/
CFLAGS = $(INCLUDE_DIRS) -Wall -Wextra -ansi -pedantic -g -no-pie -std=c++11
CYGWIN_OPTS = -Wl,--enable-auto-import
LDLIBS = -lGL -lGLEW -lglfw -lglut
BUILDDIR = ../build
EXECS = tut_06_01 tut_06_02 tut_06_03 

all : $(EXECS) postbuild

tut_06_01 : tut_06_01.cpp
	$(CC) $(CFLAGS) -o tut_06_01 tut_06_01.cpp $(LDLIBS)

tut_06_02 : tut_06_02.cpp
	$(CC) $(CFLAGS) -o tut_06_02 tut_06_02.cpp $(LDLIBS)

tut_06_03 : tut_06_03.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_06_03 tut_06_03.cpp $(LDLIBS)

$(BUILDDIR) :
	mkdir $(BUILDDIR)
	mkdir $(BUILDDIR)/linux

postbuild: | $(BUILDDIR)
	mv $(EXECS) $(BUILDDIR)/linux

clean :
	if [ -d $(BUILDDIR) ]; then \
        	cd $(BUILDDIR); \
        	rm $(EXECS); \
    	fi


