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
 * @file DirectionalLight.cpp
 * @date 2015. 10. 13.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "ext.h"
#include "DirectionalLight.h"
#include "Shader.h"

DirectionalLight::DirectionalLight()
	: m_direction(1.0f, 0.0f, 0.0f)
{
}

void DirectionalLight::setDirection(const glm::vec3 &dir)
{
	m_direction = dir;
}
const glm::vec3 &DirectionalLight::getDirection() const
{
	return m_direction;
}

void DirectionalLight::apply(const glm::mat4 &viewMatrix) const
{
	glm::vec3 dir = glm::vec3(viewMatrix * glm::vec4(m_direction, 0.0f));
	Shader *pShader = Shader::getCurrentShader();
	pShader->setUniform3f("dirLight.direction", dir);
	pShader->setUniform3f("dirLight.ambient", m_ambient);
	pShader->setUniform3f("dirLight.diffuse", m_diffuse);
	pShader->setUniform3f("dirLight.specular", m_specular);
}
