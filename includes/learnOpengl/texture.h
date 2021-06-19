#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vectors>

#include <GL/glew.h>

/*
create vector - 4 increments
assign files names to textfilename
- location
- ID
- wrap
- filter
*create enum

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

*/



vec
/*examples
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
*/
// texture creation for texture_2D. 
/*Generate and load the texture*/
bool create2DTexture()
{
    //char* texFilename = "../../resources/textures/smiley.png";
    /*
    const char* filename;
    GLuint textureID;
    GLint wrapMode;
    GLint filterMode;


    vector<vector<string>> textures;
    textures[] = {
        GL
    };

    ENUM
    GL_REPEAT
        GL_MIRRORED_REPEAT);
        GL_CLAMP_TO_EDGE);
        L_CLAMP_TO_BORDER



    for (, ,) 
    {
        int width, height, channels;


        char* filename = ;
        textureID = ;
        wrap = ;
        filter = ;
*/

        unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

        if (image)
        {
            flipImageVertical(image, width, height, channels);

            glGenTextures(1, &textureId);
            glBindTexture(GL_TEXTURE_2D, textureId);

            // set the texture wrapping parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

            // check RGB/A compatibility
            if (channels == 3)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
            else if (channels == 4)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            else
            {
                cout << "Not implemented to handle image with " << channels << " channels" << endl;
                return false;
            }

            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(image);

            glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

            return true;
        }
    }

    // Error loading the image
    return false;
}