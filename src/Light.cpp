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
 * @file Light.cpp
 * @date 2015. 9. 25.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "Light.h"
#include "Shader.h"

Light::Light()
{
}

Light::~Light()
{
}

const glm::vec3 &Light::getPosition() const
{
	return m_position;
}

void Light::initialize()
{
	m_position = glm::vec3(2.0f, 1.0f, 1.2f);
	m_ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Light::applyToCurrentShader(const glm::mat4 &viewMatrix)
{
	glm::vec3 pos = glm::vec3(viewMatrix * glm::vec4(m_position, 1.0f));
	Shader *pShader = Shader::getCurrentShader();
	pShader->setUniform3f("light.position", pos);
	pShader->setUniform3f("light.ambient", m_ambient);
	pShader->setUniform3f("light.diffuse", m_diffuse);
	pShader->setUniform3f("light.specular", m_specular);
}
