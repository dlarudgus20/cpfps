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
 * @file Material.cpp
 * @date 2015. 9. 27.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#include "pch.h"
#include "Material.h"
#include "Shader.h"

Material::Material()
{
}

Material::~Material()
{
}

void Material::initialize(std::shared_ptr<Texture> diffuseMap, std::shared_ptr<Texture> specularMap, float shininess)
{
	m_diffuseMap = std::move(diffuseMap);
	m_specularMap = std::move(specularMap);
	m_shininess = shininess;
}

void Material::apply()
{
	Shader *pShader = Shader::getCurrentShader();

	Texture::bind(0, m_diffuseMap.get());
	Texture::bind(1, m_specularMap.get());

	pShader->setUniform1i("material.diffuseMap", 0);
	pShader->setUniform1i("material.specularMap", 1);
	pShader->setUniform1f("material.shininess", m_shininess);
}

void Material::unapply()
{
	Texture::bind(0, nullptr);
	Texture::bind(1, nullptr);
}
