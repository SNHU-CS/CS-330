CC = g++
CFLAGS = $(INCLUDE_DIRS) -Wall -Wextra -ansi -pedantic -g -no-pie -std=c++11
CYGWIN_OPTS = -Wl,--enable-auto-import
LDLIBS = -lGL -lGLEW -lglfw
BUILDDIR = ../build
EXECS = tut_03_02 tut_03_03 tut_03_04 tut_03_05

all : $(EXECS) postbuild

tut_03_02 : tut_03_02.cpp
	$(CC) $(CFLAGS) -o tut_03_02 tut_03_02.cpp $(LDLIBS)

tut_03_03 : tut_03_03.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_03_03 tut_03_03.cpp $(LDLIBS)

tut_03_04 : tut_03_04.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_03_04 tut_03_04.cpp $(LDLIBS)

tut_03_05 : tut_03_05.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_03_05 tut_03_05.cpp $(LDLIBS)

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


