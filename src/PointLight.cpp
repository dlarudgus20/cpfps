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
#include "ext.h"
#include "PointLight.h"
#include "Shader.h"

PointLight::PointLight()
	: m_index(-1)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_attenuationConstant(1.0f), m_attenuationLinear(0.0f), m_attenuationQuadratic(0.0f)
{
}

void PointLight::setIndex(int i)
{
	m_index = i;
}
int PointLight::getIndex() const
{
	return m_index;
}

void PointLight::setPosition(const glm::vec3 &pos)
{
	m_position = pos;
}
const glm::vec3 &PointLight::getPosition() const
{
	return m_position;
}

void PointLight::setAttenuation(float constant, float linear, float quadratic)
{
	m_attenuationConstant = constant;
	m_attenuationLinear = linear;
	m_attenuationQuadratic = quadratic;
}
float PointLight::getAttenuationConstant() const
{
	return m_attenuationConstant;
}
float PointLight::getAttenuationLinear() const
{
	return m_attenuationLinear;
}
float PointLight::getAttenuationQuadratic() const
{
	return m_attenuationQuadratic;
}

void PointLight::apply(const glm::mat4 &viewMatrix) const
{
	assert(0 <= m_index && m_index <= Shader::POINTLIGHT_COUNT);

	assert(m_index <= 9);
	std::string prefix = "ptLights[ ]";
	prefix[9] = '0' + m_index;

	glm::vec3 pos = glm::vec3(viewMatrix * glm::vec4(m_position, 1.0f));
	Shader *pShader = Shader::getCurrentShader();
	pShader->setUniform3f(prefix + ".position", pos);
	pShader->setUniform1f(prefix + ".constant", m_attenuationConstant);
	pShader->setUniform1f(prefix + ".linear", m_attenuationLinear);
	pShader->setUniform1f(prefix + ".quadratic", m_attenuationQuadratic);
	pShader->setUniform3f(prefix + ".ambient", m_ambient);
	pShader->setUniform3f(prefix + ".diffuse", m_diffuse);
	pShader->setUniform3f(prefix + ".specular", m_specular);
}
