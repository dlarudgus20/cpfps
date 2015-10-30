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
 * @file ShadowMap.cpp
 * @date 2015. 10. 23.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "ext.h"
#include "ShadowMap.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "LightManager.h"

namespace
{
	const GLsizei SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
}

ShadowMap::ShadowMap()
{
	auto param = Texture::Parameter()
		[GL_TEXTURE_MIN_FILTER](GL_NEAREST)
		[GL_TEXTURE_MAG_FILTER](GL_NEAREST)
		[GL_TEXTURE_WRAP_S](GL_REPEAT)
		[GL_TEXTURE_WRAP_T](GL_REPEAT);
	m_depthMap = Texture(0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr,
		false, param);

	glGenFramebuffers(1, &m_depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthMap.get(), 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_lightView = glm::lookAt({ -2.0f, 4.0f, -1.0f }, glm::vec3(0.0f), glm::vec3(1.0f));
}

void ShadowMap::calcProjection(float fovy, float aspect, float zNear, float zFar)
{
	float y = zFar * std::tan(fovy / 2);
	float x = aspect * y;
	m_lightProjection = glm::ortho(-x, x, -y, y, zNear, zFar);
	//m_lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
}

namespace
{
	GLuint quadVAO = 0, quadVBO;
	void drawQuad()
	{
		if (quadVAO == 0)
		{
			GLfloat vertices[] = {
				// position, texture
				-1.0f, 1.0f, 0.0f,	0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f,	0.0f, 0.0f,
				1.0f, 1.0f, 0.0f,	1.0f, 1.0f,
				1.0f, -1.0f, 0.0f,	1.0f, 0.0f,
			};
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			{
				glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
				glEnableVertexAttribArray(1);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			glBindVertexArray(0);
		}

		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}
}

void ShadowMap::renderScene(Scene *pScene, const glm::mat4 &projMatrix, const glm::mat4 &viewMatrix,
	GLsizei viewportWidth, GLsizei viewportHeight) const
{
	ShaderManager &sm = ShaderManager::getInstance();
	Shader &shadowShader = sm.getShader(ShaderManager::SHADOW);
	Shader &shadowDepthShader = sm.getShader(ShaderManager::SHADOW_DEPTH);
	Shader &shadowDebugShader = sm.getShader(ShaderManager::SHADOW_DEBUG);

	glBindFramebuffer(GL_FRAMEBUFFER, m_depthMapFBO);
	{
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);

		shadowDepthShader.use();
		pScene->render(m_lightProjection, m_lightView, true);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, viewportWidth, viewportHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static bool bShowDebug = false;
	if (bShowDebug)
	{
		shadowDebugShader.use();
		Texture::bind(0, &m_depthMap);
		shadowDebugShader.setUniform1i("depthMap", 0);
		drawQuad();
		Texture::bind(0, nullptr);
	}
	else
	{
		shadowShader.use();
		shadowShader.setUniformMatrix4f("lightSpaceMatrix", m_lightProjection * m_lightView);
		LightManager::getInstance().apply(viewMatrix);

		Texture::bind(5, &m_depthMap);
		shadowShader.setUniform1i("shadowDepthMap", 5);
		pScene->render(projMatrix, viewMatrix, false);
		Texture::bind(5, nullptr);
	}

	static int count = 0;
	if (++count >= 1000)
	{
		count = 0;
		bShowDebug = !bShowDebug;
	}
}
