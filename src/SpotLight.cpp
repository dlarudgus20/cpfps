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
 * @file SpotLight.cpp
 * @date 2015. 10. 13.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "ext.h"
#include "SpotLight.h"
#include "Shader.h"

SpotLight::SpotLight()
	: m_position(0.0f, 0.0f, 0.0f), m_direction(1.0f, 0.0f, 0.0f)
	, m_innerCutOff(0.976f /*std::cos(glm::radian(12.5f))*/)
	, m_outerCutOff(0.954f /*std::cos(glm::radian(17.5f))*/)
{
}

void SpotLight::setPosition(const glm::vec3 &pos)
{
	m_position = pos;
}
const glm::vec3 &SpotLight::getPosition() const
{
	return m_position;
}

void SpotLight::setDirection(const glm::vec3 &dir)
{
	m_direction = dir;
}
const glm::vec3 &SpotLight::getDirection() const
{
	return m_direction;
}

void SpotLight::setCutOff(float inner, float outer)
{
	m_innerCutOff = inner;
	m_outerCutOff = outer;
}
float SpotLight::getInnerCutOff() const
{
	return m_innerCutOff;
}
float SpotLight::getOuterCutOff() const
{
	return m_outerCutOff;
}

void SpotLight::apply(const glm::mat4 &viewMatrix) const
{
	glm::vec3 pos = glm::vec3(viewMatrix * glm::vec4(m_position, 1.0f));
	glm::vec3 dir = glm::vec3(viewMatrix * glm::vec4(m_direction, 0.0f));
	Shader *pShader = Shader::getCurrentShader();
	pShader->setUniform3f("spLight.position", pos);
	pShader->setUniform3f("spLight.direction", dir);
	pShader->setUniform1f("spLight.innerCutOff", m_innerCutOff);
	pShader->setUniform1f("spLight.outerCutOff", m_outerCutOff);
	pShader->setUniform3f("spLight.ambient", m_ambient);
	pShader->setUniform3f("spLight.diffuse", m_diffuse);
	pShader->setUniform3f("spLight.specular", m_specular);
}
