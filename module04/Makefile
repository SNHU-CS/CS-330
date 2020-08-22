CC = g++
INCLUDE_DIRS = -I../includes/
CFLAGS = $(INCLUDE_DIRS) -Wall -Wextra -ansi -pedantic -g -no-pie -std=c++11
CYGWIN_OPTS = -Wl,--enable-auto-import
LDLIBS = -lGL -lGLEW -lglfw -lglut
BUILDDIR = ../build
EXECS = tut_04_01 tut_04_02 tut_04_03 tut_04_04 tut_04_05

all : $(EXECS) postbuild

tut_04_01 : tut_04_01.cpp
	$(CC) $(CFLAGS) -o tut_04_01 tut_04_01.cpp $(LDLIBS)

tut_04_02 : tut_04_02.cpp
	$(CC) $(CFLAGS) -o tut_04_02 tut_04_02.cpp $(LDLIBS)

tut_04_03 : tut_04_03.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_04_03 tut_04_03.cpp $(LDLIBS)

tut_04_04 : tut_04_04.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_04_04 tut_04_04.cpp $(LDLIBS)

tut_04_05 : tut_04_05.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_04_05 tut_04_05.cpp $(LDLIBS)

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


