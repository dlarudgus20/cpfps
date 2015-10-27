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
 * @file Shader_impl.h
 * @date 2015. 10. 27.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */


#ifndef SHADER_IMPL_H_
#define SHADER_IMPL_H_

inline void Shader::setUniform1f(const char *var, GLfloat f)
{
	glUniform1f(findUniform(var), f);
}
inline void Shader::setUniform1f(const std::string &var, GLfloat f)
{
	setUniform1f(var.c_str(), f);
}

inline void Shader::setUniform3f(const char *var, const glm::vec3 &vec3)
{
	glUniform3fv(findUniform(var), 1, glm::value_ptr(vec3));
}
inline void Shader::setUniform3f(const std::string &var, const glm::vec3 &vec3)
{
	setUniform3f(var.c_str(), vec3);
}

inline void Shader::setUniform4f(const char *var, const glm::vec4 &vec4)
{
	glUniform4fv(findUniform(var), 1, glm::value_ptr(vec4));
}
inline void Shader::setUniform4f(const std::string &var, const glm::vec4 &vec4)
{
	setUniform4f(var.c_str(), vec4);
}

inline void Shader::setUniform1i(const char *var, GLint i)
{
	glUniform1i(findUniform(var), i);
}
inline void Shader::setUniform1i(const std::string &var, GLint i)
{
	setUniform1i(var.c_str(), i);
}

inline void Shader::setUniformMatrix3f(const char *var, const glm::mat3 &mat)
{
	glUniformMatrix3fv(findUniform(var), 1, GL_FALSE, glm::value_ptr(mat));
}
inline void Shader::setUniformMatrix3f(const std::string &var, const glm::mat3 &mat)
{
	setUniformMatrix3f(var.c_str(), mat);
}

inline void Shader::setUniformMatrix4f(const char *var, const glm::mat4 &mat)
{
	glUniformMatrix4fv(findUniform(var), 1, GL_FALSE, glm::value_ptr(mat));
}
inline void Shader::setUniformMatrix4f(const std::string &var, const glm::mat4 &mat)
{
	setUniformMatrix4f(var.c_str(), mat);
}

#endif /* SHADER_IMPL_H_ */
