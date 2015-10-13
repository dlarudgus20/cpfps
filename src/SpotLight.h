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
 * @file SpotLight.h
 * @date 2015. 10. 13.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#ifndef SPOTLIGHT_H_
#define SPOTLIGHT_H_

#include "Light.h"

class SpotLight : public Light
{
private:
	glm::vec3 m_position, m_direction;
	float m_innerCutOff, m_outerCutOff;

public:
	SpotLight();

	void setPosition(const glm::vec3 &pos);
	const glm::vec3 &getPosition() const;

	void setDirection(const glm::vec3 &dir);
	const glm::vec3 &getDirection() const;

	void setCutOff(float inner, float outer);
	float getInnerCutOff() const;
	float getOuterCutOff() const;

	virtual void apply(const glm::mat4 &viewMatrix) const override;
};

#endif /* SPOTLIGHT_H_ */
