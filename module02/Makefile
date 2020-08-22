CC = g++
CFLAGS = $(INCLUDE_DIRS) -Wall -Wextra -ansi -pedantic -g -no-pie -std=c++11
CYGWIN_OPTS = -Wl,--enable-auto-import
LDLIBS = -lGL -lGLEW -lglfw
BUILDDIR = ../build
EXECS = tut_02_02 tut_02_03 tut_02_04 tut_02_05 tut_02_06 tut_02_07 tut_02_08

all : $(EXECS) postbuild

tut_02_02 : tut_02_02.cpp
	$(CC) $(CFLAGS) -o tut_02_02 tut_02_02.cpp $(LDLIBS)

tut_02_03 : tut_02_03.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_02_03 tut_02_03.cpp $(LDLIBS)

tut_02_04 : tut_02_04.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_02_04 tut_02_04.cpp $(LDLIBS)

tut_02_05 : tut_02_05.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_02_05 tut_02_05.cpp $(LDLIBS)

tut_02_06 : tut_02_06.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_02_06 tut_02_06.cpp $(LDLIBS)

tut_02_07 : tut_02_07.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_02_07 tut_02_07.cpp $(LDLIBS)

tut_02_08 : tut_02_08.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o tut_02_08 tut_02_08.cpp $(LDLIBS)

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


