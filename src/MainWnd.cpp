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

std::unique_ptr<MainWnd> MainWnd::createInstance()
{
	return std::unique_ptr<MainWnd>(new MainWnd());
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
		glfwMakeContextCurrent(m_wnd);
		glfwSetWindowUserPointer(m_wnd, this);

		initCallback();
		return true;
	}
	else
	{
		return false;
	}
}

void MainWnd::loop()
{
	while (!glfwWindowShouldClose(m_wnd))
	{
		glfwPollEvents();
		render();
		idle();
		glfwSwapBuffers(m_wnd);
	}
}

void MainWnd::initCallback()
{
#define THIZ static_cast<MainWnd *>(glfwGetWindowUserPointer(wnd))
	glfwSetFramebufferSizeCallback(m_wnd, [] (GLFWwindow *wnd, int width, int height) {
		THIZ->onFrameBufferSize(width, height);
	});
	glfwSetWindowCloseCallback(m_wnd, [](GLFWwindow *wnd) {
		THIZ->onWindowClose();
	});
#undef THIZ
}

bool MainWnd::initialize()
{
	m_lastFrameTime = glfwGetTime();

	int width, height;
	glfwGetFramebufferSize(m_wnd, &width, &height);

	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	calcProjection(width, height);

	glEnable(GL_DEPTH_TEST);

	try
	{
		m_shader.compile("src/vertex.glsl", "src/fragment.glsl");
	}
	catch (Shader::CompileError &e)
	{
		std::cerr << "Shader::CompileError : " << e.what() << std::endl;
		return false;
	}
	m_shader.use();

	m_tetra.initialize();
	m_container.initialize();

	return true;
}

void MainWnd::render()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static glm::vec3 cubePos[] = {
		{ 0.0f,  0.0f,  0.0f },
		{ 2.0f,  5.0f, -15.0f },
		{-1.5f, -2.2f, -2.5f },
		{-3.8f, -2.0f, -12.3f },
		{ 2.4f, -0.4f, -3.5f },
		{-1.7f,  3.0f, -7.5f },
		{ 1.3f, -2.0f, -2.5f },
		{ 1.5f,  2.0f, -2.5f },
		{ 1.5f,  0.2f, -1.5f },
		{-1.3f,  1.0f, -1.5f },
	};

	glm::mat4 pvMatrix = m_projection * m_camera.getMatrix();

	for (std::size_t i = 0; i < sizeof(cubePos) / sizeof(cubePos[0]); ++i)
	{
		float angle = (float)glm::radians(i * 20.0 + glfwGetTime() * 60.0);

		glm::mat4 model = pvMatrix;
		model = glm::translate(model, cubePos[i]);
		model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

		m_shader.setUniformMatrix4f("ourMatrix", model);
		m_container.draw(m_shader);
	}
}

void MainWnd::idle()
{
	float frameTime = glfwGetTime();
	float deltaTime = frameTime - m_lastFrameTime;

	const float unit = 1.5f;

	int front = 0, right = 0;
	front += glfwGetKey(m_wnd, GLFW_KEY_W) == GLFW_PRESS ? 1 : 0;
	front += glfwGetKey(m_wnd, GLFW_KEY_S) == GLFW_PRESS ? -1 : 0;
	right += glfwGetKey(m_wnd, GLFW_KEY_D) == GLFW_PRESS ? 1 : 0;
	right += glfwGetKey(m_wnd, GLFW_KEY_A) == GLFW_PRESS ? -1 : 0;

	m_camera.move(front, right, unit * deltaTime);

	m_lastFrameTime = frameTime;
}

void MainWnd::calcProjection(int width, int height)
{
	float aspect;

	// 최소화 상태일땐 width == height == 0임.
	if (width == 0 || height == 0)
		aspect = 1.0f;
	else
		aspect = (float)width / height;

	m_projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void MainWnd::onFrameBufferSize(int width, int height)
{
	// 최소화 상태일땐 width == height == 0임.
	if (!(width == 0 || height == 0))
	{
		glViewport(0, 0, width, height);
		calcProjection(width, height);
	}
}

void MainWnd::onWindowClose()
{
	// TODO: onWindowClose
}
