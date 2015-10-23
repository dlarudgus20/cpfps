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
 * @file Texture.h
 * @date 2015. 9. 27.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

class Texture : private ext::noncopyable
{
private:
	GLuint m_texture;

public:
	static void bind(int idx, const Texture *pTexture);
	static void bind(const Texture *pTexture);

	class Parameter
	{
	private:
		std::map<GLenum, GLint> m_params;
	public:
		static Parameter getDefault();

		struct Functor
		{
			Parameter *thiz;
			GLenum idx;
			Parameter &operator ()(GLint par) const
			{
				thiz->m_params[idx] = par;
				return *thiz;
			}
		};
		Functor operator [](GLenum idx)
		{
			return Functor { this, idx };
		}
		void apply() const;
	};

	Texture();
	explicit Texture(const char *file, const Parameter &params);
	explicit Texture(GLint level, GLint internalformat,
		GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels,
		bool bGenMipmap, const Parameter &params);
	~Texture();

	Texture(Texture &&other);
	Texture &operator =(Texture &&other);
	void swap(Texture &other);

	GLuint get() const;

	class LoadError : public std::runtime_error
	{
	public:
		explicit LoadError(const std::string &msg)
			: std::runtime_error(msg) { }
	};
};

#endif /* TEXTURE_H_ */
