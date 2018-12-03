#version 330 core

/**
\file SimpleTexture.glsl

\brief Fragment shader that either passes the vertex color through if useTexture is false
and uses the textual if useTexture is true.

\param [in] color --- vec4 vertex color from memory.

\param [in] tex_coord --- vec2 texture coordinate from memory.

\param [out] fcolor --- vec4 output color to the frame buffer.

\param [uniform] tex1 --- sampler2D texture.

\param [uniform] useTexture --- bool to use a texture or pass through with the fragment color.

*/

in vec4 color;
in vec2 tex_coord;

uniform sampler2D tex1;
uniform bool useTexture;

out vec4 fColor;

void main()
{
    if (useTexture)
        fColor = texture(tex1, tex_coord);
    else
        fColor = color;

    fColor = min(fColor, vec4(1.0));

}
