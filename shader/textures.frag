#version 330 core
out vec4 FragColor;
 
in vec2 TexCoord;
 
uniform sampler2D texture1;
uniform sampler2D texture2;
 
void main()
{
    FragColor = mix(texture2D(texture1, TexCoord), texture2D(texture2, TexCoord), 0.2f);
}