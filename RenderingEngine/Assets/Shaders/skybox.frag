out vec4 FragColor;

in vec3 TexCoords;

uniform sampler2D skybox[6];
void main()
{    
    int index=0;
    vec2 uv=ComputeCubeMapUV(TexCoords, index);
    FragColor = texture(skybox[index], uv);
}