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
 * @file Tetrahedron.cpp
 * @date 2015. 9. 22.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "Tetrahedron.h"
#include "Shader.h"

namespace
{
	GLfloat vertices[] = {
		// position, normal, color, texture
		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-0.87f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.87f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,	0.5f, 1.0f,

		0.0f, 1.0f, 0.0f,		0.72f, 0.42f, -0.56f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		0.87f, -0.5f, 0.0f,		0.72f, 0.42f, -0.56f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		0.0f, 0.0f, -0.75f,		0.72f, 0.42f, -0.56f,	1.0f, 1.0f, 1.0f,	0.5f, 1.0f,

		-0.87f, -0.5f, 0.0f,	-0.72f, 0.42f, -0.56f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		-0.72f, 0.42f, -0.56f,	1.0f, 0.0f, 0.0f,	1.0f, 0.0f,
		0.0f, 0.0f, -0.75f,		-0.72f, 0.42f, -0.56f,	1.0f, 1.0f, 1.0f,	0.5f, 1.0f,

		0.0f, 0.0f, -0.75f,		0.0f, -0.83f, -0.55f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,
		0.87f, -0.5f, 0.0f,		0.0f, -0.83f, -0.55f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		-0.87f, -0.5f, 0.0f,	0.0f, -0.83f, -0.55f,	0.0f, 1.0f, 0.0f,	0.5f, 1.0f,
	};
	GLsizei strides = 11 * sizeof(GLfloat);
}

Tetrahedron::Tetrahedron()
{
}

Tetrahedron::~Tetrahedron()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteTextures(1, &m_texture);
}

void Tetrahedron::initialize()
{
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int texWidth, texHeight;
		unsigned char *image = SOIL_load_image("res/awesomeface.png", &texWidth, &texHeight, nullptr, SOIL_LOAD_RGB);
		if (image == nullptr)
		{
			throw "failed to load texture\n";
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

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
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, strides, (GLvoid *)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, strides, (GLvoid *)(9 * sizeof(GLfloat)));
		glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);
}

void Tetrahedron::draw()
{
	Shader *pShader = Shader::getCurrentShader();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	pShader->setUniform1i("ourTexture", 0);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / strides);
	glBindVertexArray(0);
}
