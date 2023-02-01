in VS_OUT{ 
	vec3 FragPos;
	vec3 NormalVector;
    vec2 UV;
} fs_in; 

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
//layout (location = 2) out vec4 gAlbedoSpec;

void main()
{
    // store the fragment position vector in the first gbuffer texture
    gPosition = vec4(fs_in.FragPos, 1.f);
    // also store the per-fragment normals into the gbuffer
    gNormal = vec4(normalize(fs_in.NormalVector), 1.f);
}