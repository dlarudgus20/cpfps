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
 * @file Texture.cpp
 * @date 2015. 9. 27.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "ext.h"
#include "Texture.h"

void Texture::bind(int idx, const Texture *pTexture)
{
	glActiveTexture(GL_TEXTURE0 + idx);
	bind(pTexture);
}
void Texture::bind(const Texture *pTexture)
{
	if (pTexture != nullptr)
		glBindTexture(GL_TEXTURE_2D, pTexture->m_texture);
	else
		glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture()
	: m_texture(0)
{

}

Texture::Texture(const char *file, const Parameter &params)
{
	init(file, GL_SRGB, GL_RGB, params);
}

Texture::Texture(const char *file, GLint internalformat, GLenum format, const Parameter &params)
{
	init(file, internalformat, format, params);
}

void Texture::init(const char *file, GLint internalformat, GLenum format, const Parameter &params)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	{
		params.apply();

		int texWidth, texHeight;
		unsigned char *image = SOIL_load_image(file, &texWidth, &texHeight, nullptr, SOIL_LOAD_RGB);
		if (image == nullptr)
		{
			throw LoadError("[" + std::string(file) + "] : failed to load");
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		SOIL_free_image_data(image);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(GLint level, GLint internalformat,
	GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels,
	bool bGenMipmap, const Parameter &params)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	params.apply();
	glTexImage2D(GL_TEXTURE_2D, level, internalformat, width, height, border, format, type, pixels);
	if (bGenMipmap)
		glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	if (m_texture != 0)
	{
		glDeleteTextures(1, &m_texture);
	}
}

Texture::Texture(Texture &&other)
	: m_texture(other.m_texture)
{
	other.m_texture = 0;
}
Texture &Texture::operator =(Texture &&other)
{
	Texture().swap(*this);
	m_texture = other.m_texture;
	other.m_texture = 0;
	return *this;
}
void Texture::swap(Texture &other)
{
	std::swap(m_texture, other.m_texture);
}

GLuint Texture::get() const
{
	return m_texture;
}

Texture::Parameter Texture::Parameter::getDefault()
{
	return Parameter()
		[GL_TEXTURE_WRAP_S](GL_REPEAT)
		[GL_TEXTURE_WRAP_T](GL_REPEAT)
		[GL_TEXTURE_MIN_FILTER](GL_LINEAR_MIPMAP_LINEAR)
		[GL_TEXTURE_MAG_FILTER](GL_LINEAR);
}

void Texture::Parameter::apply() const
{
	for (const auto &pr : m_params)
	{
		glTexParameteri(GL_TEXTURE_2D, pr.first, pr.second);
	}
}
