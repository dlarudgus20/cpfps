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
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_TEXTURE_2D, m_depthMap.get(), 0);
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
}

void ShadowMap::renderScene(Scene *pScene, const glm::mat4 &projMatrix, const glm::mat4 &viewMatrix,
	GLsizei viewportWidth, GLsizei viewportHeight) const
{
	auto &shadowShader = ShaderManager::getInstance().getShadowShader();
	auto &shadowDepthShader = ShaderManager::getInstance().getShadowDepthShader();

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

	shadowShader.use();
	shadowShader.setUniformMatrix4f("lightSpaceMatrix", m_lightProjection * m_lightView);
	LightManager::getInstance().apply(viewMatrix);

	Texture::bind(&m_depthMap);
	pScene->render(projMatrix, viewMatrix, false);
}
