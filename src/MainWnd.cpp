// Copyright (c) 2014, 임경현
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file MainWnd.cpp
 * @date 2015. 9. 17.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "MainWnd.h"

const int MAINWND_WIDTH = 800;
const int MAINWND_HEIGHT = 600;

MainWnd &MainWnd::getInstance()
{
	static MainWnd obj;
	return obj;
}

MainWnd::MainWnd()
	: m_wnd(nullptr)
{
	// TODO Auto-generated constructor stub
}

MainWnd::~MainWnd()
{
	// TODO Auto-generated destructor stub
}

bool MainWnd::create()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	m_wnd = glfwCreateWindow(MAINWND_WIDTH, MAINWND_HEIGHT, "pfps", nullptr, nullptr);
	if (m_wnd != nullptr)
	{
		glViewport(0, 0, MAINWND_WIDTH, MAINWND_HEIGHT); // TODO
		glfwMakeContextCurrent(m_wnd);

		initCallback();
		return true;
	}
	else
	{
		return false;
	}
}

GLuint vao, vbo;
void MainWnd::initialize()
{
	glViewport(0, 0, MAINWND_WIDTH, MAINWND_HEIGHT); // TODO

	try
	{
		m_shader.compile("vertex.vs", "fragment.fs");
	}
	catch (Shader::CompileError &e)
	{
		std::cerr << "Shader::CompileError : " << e.what() << std::endl;
	}

	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
		glEnableVertexAttribArray(0);
	}
	glBindVertexArray(0);
}

void MainWnd::destroy()
{
	glfwSetWindowShouldClose(m_wnd, GL_TRUE);
}

void MainWnd::loop()
{
	while (!glfwWindowShouldClose(m_wnd))
	{
		glfwPollEvents();
		render();
		glfwSwapBuffers(m_wnd);
	}
}

void MainWnd::initCallback()
{
	glfwSetFramebufferSizeCallback(m_wnd, [] (GLFWwindow *, int width, int height) {
		MainWnd::getInstance().onFrameBufferSize(width, height);
	});
	glfwSetWindowCloseCallback(m_wnd, [](GLFWwindow *) {
		MainWnd::getInstance().onWindowClose();
	});
}

void MainWnd::render()
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void MainWnd::onFrameBufferSize(int width, int height)
{
	glViewport(0, 0, width, height);
}

void MainWnd::onWindowClose()
{
	// ShouldClose is set GL_TRUE on destroy()
	glfwSetWindowShouldClose(m_wnd, GL_FALSE);

	destroy();
}
