#version 460 core
in vec3 Normal;
uniform vec3 LightPos;
in vec3 FragPos;

out vec4 FragColor;
void main()
{
	vec3 LightDir=normalize(LightPos-FragPos);
	float diff=max(dot(Normal,LightDir),0.f);
	vec3 diffuse=diff*vec3(1.f, 1.f, 1.f);
			
	float ambientStrength=0.1;
	vec3 ambient = ambientStrength * vec3(1.f, 1.f, 1.f);
	vec3 result=(ambient+diffuse)*vec3(1.0f, 0.5f, 0.2f);
	FragColor = vec4(result, 1.0f);
} 