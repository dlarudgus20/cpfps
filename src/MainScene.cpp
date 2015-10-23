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
 * @file MainScene.cpp
 * @date 2015. 10. 23.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "ext.h"
#include "MainScene.h"
#include "Shader.h"

MainScene::MainScene()
{

}

MainScene::~MainScene()
{

}

void MainScene::render(const glm::mat4 &viewMatrix) const
{
	Shader *pShader = Shader::getCurrentShader();

	glm::mat4 vmMatrix = viewMatrix;
	glm::mat3 normalMatrix;

	auto calcNormalMat = [&] {
		normalMatrix = glm::mat3(glm::transpose(glm::inverse(vmMatrix)));
	};
	calcNormalMat();

	{
		pShader->setUniformMatrix4f("vmMatrix", vmMatrix);
		pShader->setUniformMatrix3f("NormalMatrix", normalMatrix);
		m_woodplane.draw();
	}
	{
		glm::mat4 prevMat = vmMatrix;
		vmMatrix = glm::translate(vmMatrix, { 0.0f, 1.5f, 0.0f });
		calcNormalMat();
		pShader->setUniformMatrix4f("vmMatrix", vmMatrix);
		pShader->setUniformMatrix3f("NormalMatrix", normalMatrix);
		m_container.draw();
		vmMatrix = prevMat;
	}
	{
		glm::mat4 prevMat = vmMatrix;
		vmMatrix = glm::translate(vmMatrix, { 2.0f, 0.0f, 1.0f });
		calcNormalMat();
		pShader->setUniformMatrix4f("vmMatrix", vmMatrix);
		pShader->setUniformMatrix3f("NormalMatrix", normalMatrix);
		m_container.draw();
		vmMatrix = prevMat;
	}
	{
		glm::mat4 prevMat = vmMatrix;
		vmMatrix = glm::translate(vmMatrix, { -1.0f, 0.0f, 2.0f });
		vmMatrix = glm::rotate(vmMatrix, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
		vmMatrix = glm::scale(vmMatrix, glm::vec3(0.5f));
		calcNormalMat();
		pShader->setUniformMatrix4f("vmMatrix", vmMatrix);
		pShader->setUniformMatrix3f("NormalMatrix", normalMatrix);
		m_container.draw();
		vmMatrix = prevMat;
	}
}
