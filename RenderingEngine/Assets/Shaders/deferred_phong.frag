in VS_OUT{ 
    vec2 UV;
} fs_in; 

uniform sampler2D gPosition;
uniform sampler2D gNormal;

uniform vec3 CameraPosition;
uniform MaterialData Material;

uniform int LightNumbers;
uniform LightData Light[MAX_LIGHTS];


uniform vec3 globalAmbient;
uniform AttenuationData Attenuation;

uniform FogData Fog;
out vec4 FragColor;
void main()
{
    vec3 FragPos=texture(gPosition, fs_in.UV).rgb;
    vec3 NormalVector=normalize(texture(gNormal, fs_in.UV).rgb);
    
    vec3 ViewVector=CameraPosition-FragPos;
    float ViewDistance=length(ViewVector);


    vec3 TotalColor=Material.Emissive;
    for(int i=0; i<LightNumbers; ++i)
    {
        switch (Light[i].LightType)
        {
            case 0:
                TotalColor+=ComputePointLight(Light[i], Attenuation, Material, FragPos, NormalVector, ViewVector);
                break;
            case 1:
                TotalColor+=ComputeDirectionLight(Light[i], Attenuation, Material, FragPos, NormalVector, ViewVector);
                break;
            case 2:
                TotalColor+=ComputeSpotLight(Light[i], Attenuation, Material, FragPos, NormalVector, ViewVector);
                break;
        }
    }
    TotalColor=ComputeFog(Fog, TotalColor, ViewDistance);
	FragColor = vec4(globalAmbient+TotalColor, 1.0);
}