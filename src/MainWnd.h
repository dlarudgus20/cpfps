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
 * @file MainWnd.h
 * @date 2015. 9. 17.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#ifndef MAINWND_H_
#define MAINWND_H_

#include "Shader.h"
#include "Tetrahedron.h"
#include "Container.h"

class MainWnd final
{
public:
	static std::unique_ptr<MainWnd> createInstance();

private:
	GLFWwindow *m_wnd;

	Shader m_shader;

	Tetrahedron m_tetra;
	Container m_container;

	MainWnd();
public:
	~MainWnd();

	bool create();

	bool initialize();
	void loop();

private:
	void initCallback();
	void render();

	void onFrameBufferSize(int width, int height);
	void onWindowClose();
};

#endif /* MAINWND_H_ */
