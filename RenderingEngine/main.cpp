/* Start Header -------------------------------------------------------
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Purpose: entry point for the engine.
Language: C++ MSVC(2022)
Platform: MSVC(2022), GPU needs to support OpenGL 4.6.0, Windows11(x64)
Project: junwoo.seo_cs300_1
Author: Junwoo Seo, junwoo.seo, 0055213
Creation date: Sep 05 2022
End Header --------------------------------------------------------*/
#include"Core/Application.h"
#include"Core/EntryPoint.h"
#include"Core/Scene/Scene.h"
#include"glad.h"
class MyScene : public Scene
{
	GLuint vertex_array_object[1];
	GLuint vertex_buffer[2];

	GLuint shader;
	

	const char* vertexShaderSource = R"(
		#version 460 core
		layout (location = 0) in vec3 aPos;
		void main()
		{
		   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}
		)";
	const char* fragShaderSource = R"(
		#version 460 core
		out vec4 FragColor;

		void main()
		{
			FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		} 
		)";
	virtual void Awake() {};
	virtual void OnEnable() {};
	virtual void Start() 
	{
		float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
		};
		glEnable(GL_DEPTH_TEST);
		glGenVertexArrays(1, vertex_array_object);
		glBindVertexArray(vertex_array_object[0]);
		glGenBuffers(2, vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertex_shader);

		int  success;
		char infoLog[512];
		glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertex_shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}


		GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(frag_shader, 1, &fragShaderSource, NULL);
		glCompileShader(frag_shader);

		glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(frag_shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shader=glCreateProgram();
		glAttachShader(shader, vertex_shader);
		glAttachShader(shader, frag_shader);
		glLinkProgram(shader);
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		}


		glDeleteShader(vertex_shader);
		glDeleteShader(frag_shader);

		

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
	};
	virtual void Update() 
	{
		glUseProgram(shader);
		glBindVertexArray(vertex_array_object[0]);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
	};
	virtual void LateUpdate() {};
	virtual void OnDisable() {};
	virtual void OnDestroy() 
	{
		glUseProgram(0);
		glBindBuffer(GL_VERTEX_ARRAY, 0);
		glBindVertexArray(0);
		glDeleteBuffers(2, vertex_buffer);
		glDeleteVertexArrays(1, vertex_array_object);
		glDeleteProgram(shader);
	};
};

class MyApp : public Application
{
	virtual void UserDefinedInit() override
	{
		m_SceneManager->SetCurrentScene<MyScene>();
	}
};
std::shared_ptr<Application> CoreMain()
{

	return std::make_shared<MyApp>();
}