out vec4 FragColor;

in vec3 TexCoords;

uniform sampler2D skybox[6];
void main()
{    
    int index=0;
    float u=0.f;
    float v=0.f;

    vec3 TextureEntity=TexCoords;
    vec3 Abs=abs(TextureEntity);

    //-+x
    if(Abs.x >= Abs.y && Abs.x >= Abs.z)
    {
        //left or right
        if(TextureEntity.x < 0.f)
        {
            u=TextureEntity.z;
            index=0;
        }
        else
        {
            u=-TextureEntity.z;
            index=1;
        } 
        u=u/Abs.x;
        v=TextureEntity.y/Abs.x;
    }

    //-+z
    if(Abs.z >= Abs.x && Abs.z >= Abs.y)
    {
        //back or front
        if(TextureEntity.z < 0.f)
        {
            u=-TextureEntity.x;
            index=2;//front
        }
        else
        {
            u=TextureEntity.x;
            index=3;//back
        }
        u=u/Abs.z;
        v=TextureEntity.y/Abs.z;
    }

    //-+y
    if(Abs.y >= Abs.x && Abs.y >= Abs.z)
    {
        u=TextureEntity.x/Abs.y;

        //bottom or top
        if(TextureEntity.y < 0.f)
        {
            v=TextureEntity.z;
            index=4;//bottom
        }
        else
        {
            v=-TextureEntity.z;
            index=5;
        }
        v=v/Abs.y;
    }

    //-1 to 1 to 0 to 1
    u = 0.5f * (u + 1.0f);
    v = 0.5f * (v + 1.0f);

    FragColor = texture(skybox[index], vec2(u,v));
}