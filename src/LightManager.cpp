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
 * @file LightManager.cpp
 * @date 2015. 10. 27.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "ext.h"
#include "LightManager.h"

LightManager &LightManager::getInstance()
{
	static LightManager obj;
	return obj;
}

void LightManager::initialize()
{
	//m_dirLight.setDirection({ -2.0f, -1.0f, -1.2f });
	//m_dirLight.setAmbient({ 0.1f, 0.1f, 0.1f });
	//m_dirLight.setDiffuse({ 1.0f, 1.0f, 1.0f });
	//m_dirLight.setSpecular({ 1.0f, 1.0f, 1.0f });

	//glm::vec3 ptLightPositions[] = {
	//	{ 2.3f, -3.3f, -4.0f },
	//	{ 0.7f,  0.2f,  2.0f },
	//	{ -4.0f,  2.0f, -12.0f },
	//	{ 0.0f,  0.0f, -3.0f },
	//};
	for (int i = 0; i < POINTLIGHT_COUNT; ++i)
	{
		m_ptLights[i].setIndex(i);
		//m_ptLights[i].setPosition(ptLightPositions[i]);
		//m_ptLights[i].setAmbient({ 0.1f, 0.1f, 0.1f });
		//m_ptLights[i].setDiffuse({ 1.0f, 1.0f, 1.0f });
		//m_ptLights[i].setSpecular({ 1.0f, 1.0f, 1.0f });
		//m_ptLights[i].setAttenuation(1.0f, 0.14f * 0.4, 0.07f * 0.4);
	}
	m_ptLights[0].setPosition({ -2.0f, 4.0f, -1.0f });
	m_ptLights[0].setAmbient(glm::vec3(0.2f));
	m_ptLights[0].setDiffuse(glm::vec3(1.0f));
	m_ptLights[0].setSpecular(glm::vec3(1.0f));
	m_ptLights[0].setAttenuation(1.0f, 0.0f, 0.0f);

	//m_spLight.setPosition({ 2.3f, -3.3f, -4.0f });
	//m_spLight.setDirection({ -2.3f, 3.3f, 4.0f });
	//m_spLight.setAmbient({ 0.1f, 0.1f, 0.1f });
	//m_spLight.setDiffuse({ 1.0f, 1.0f, 1.0f });
	//m_spLight.setSpecular({ 1.0f, 1.0f, 1.0f });
	//m_spLight.setCutOff(std::cos(glm::radians(12.5f)), std::cos(glm::radians(17.5f)));
}

void LightManager::apply(const glm::mat4 &viewMatrix) const
{
	m_dirLight.apply(viewMatrix);
	for (const auto &ptLight : m_ptLights)
		ptLight.apply(viewMatrix);
	m_spLight.apply(viewMatrix);
}
