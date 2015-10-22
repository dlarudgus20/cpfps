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
 * @file WoodPlane.cpp
 * @date 2015. 10. 22.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "WoodPlane.h"
#include "Shader.h"

namespace
{
	GLfloat vertices[] = {
		// position, normal, texture
		25.0f, -0.5f, 25.0f,	0.0f, 1.0f, 0.0f,	25.0f, 0.0f,
		-25.0f, -0.5f, -25.0f,	0.0f, 1.0f, 0.0f,	0.0f, 25.0f,
		-25.0f, -0.5f, 25.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,

		25.0f, -0.5f, 25.0f,	0.0f, 1.0f, 0.0f,	25.0f, 0.0f,
		25.0f, -0.5f, -25.0f,	0.0f, 1.0f, 0.0f,	25.0f, 25.0f,
		-25.0f, -0.5f, -25.0f,	0.0f, 1.0f, 0.0f,	0.0f, 25.0f
	};
	GLsizei strides = 8 * sizeof(GLfloat);
}

WoodPlane::WoodPlane()
{
	// TODO Auto-generated constructor stub
}

WoodPlane::~WoodPlane()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void WoodPlane::initialize()
{
	auto pTexture = std::make_shared<Texture>("res/wood.png", Texture::Parameter::getDefault());

	m_material.initialize(pTexture, pTexture, 32.0f);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, strides, (GLvoid *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, strides, (GLvoid *)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, strides, (GLvoid *)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
}

void WoodPlane::draw(bool bUseMaterial /* = true */)
{
	if (bUseMaterial)
		m_material.apply();

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / strides);
	glBindVertexArray(0);

	if (bUseMaterial)
		m_material.unapply();
}
