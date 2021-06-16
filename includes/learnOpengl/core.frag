# FRAGMENT SHADER SOURCE CODE
// remove comments if experiencing issues


// FRAG SHADER FOR COLOR AND TEXTURE
const GLchar * fragmentShaderSource = GLSL(440,
    in vec2 vertexTextureCoordinate; //hold incoming color data from vertex shader

    out vec4 fragmentColor;

    uniform sampler2D uTexture;
    uniform vec2 uvScale;

    void main()
    {
        fragmentColor = texture(uTexture, vertexTextureCoordinate * uvScale);
    }
);


/* FRAG SHADER FOR ONLY COLOR (NO TEXTURE)
const GLchar * FragmentShaderSource = GLSL(440,

    out vec4 fragmentColor; // For outgoing lamp color (smaller cube) to the GPU

    void main()
    {
        fragmentColor = vec4(1.0f); // Set color to white (1.0f,1.0f,1.0f) with alpha 1.0
    }
);
