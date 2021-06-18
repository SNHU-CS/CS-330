#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>


create vector - 4 increments
assign files names to textfilename
- location
- ID
- wrap
- filter

number of loops: calculate size vector / 4
data positions: 0 1 2 3
loop through each set

	createtexture (location, ID, wrap, filter)
		declare height, wdith, channels, 
		assign wrapmode = wrap
		assign filermode = filter
		create local variable for image
		flip image (if needed)
		gentext
		bindtext
		set wrapping parameters
		set filtering parameters
		determine compatibility (RGB/A)
		gen mipmap
		free image
		return true
return false if image fails to load

