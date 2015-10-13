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
 * @file fragment.glsl
 * @date 2015. 9. 18.
 * @author dlarudgus20
 * @copyright The BSD (2-Clause) License
 */

#version 330 core

struct Material
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;
};

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define POINTLIGHT_COUNT 4

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float innerCutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPos;

out vec4 color;

uniform DirectionalLight dirLight;
uniform PointLight ptLights[POINTLIGHT_COUNT];
uniform SpotLight spLight;

uniform Material material;

uniform mat3 NormalMatrix;

vec3 calcDirectionalLight();
vec3 calcPointLight(int idx);
vec3 calcSpotLight();

void main()
{
	vec3 result = calcDirectionalLight();

	for (int i = 0; i < POINTLIGHT_COUNT; i++)
		result += calcPointLight(i);

	result += calcSpotLight();

	color = vec4(result, 1.0f);
}

vec3 calcDirectionalLight()
{
	vec3 diffmap = texture(material.diffuseMap, fragTexCoord).xyz;
	vec3 specmap = texture(material.specularMap, fragTexCoord).xyz;

	vec3 norm = NormalMatrix * normalize(fragNormal);
	vec3 lightDir = normalize(-dirLight.direction);

	vec3 ambient = diffmap * dirLight.ambient;

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * diffmap * dirLight.diffuse;

	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = spec * specmap * dirLight.specular;
	
	return ambient + diffuse + specular;
}

vec3 calcPointLight(int idx)
{
	vec3 diffmap = texture(material.diffuseMap, fragTexCoord).xyz;
	vec3 specmap = texture(material.specularMap, fragTexCoord).xyz;

	vec3 norm = NormalMatrix * normalize(fragNormal);
	vec3 lightDir = normalize(ptLights[idx].position - fragPos);

	float dist = length(ptLights[idx].position - fragPos);
	float constant = ptLights[idx].constant;
	float linear = ptLights[idx].linear;
	float quadratic = ptLights[idx].quadratic;
	float attenuation = 1.0f / (constant + linear * dist + quadratic * (dist * dist));

	vec3 ambient = diffmap * ptLights[idx].ambient;

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * diffmap * ptLights[idx].diffuse;

	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = spec * specmap * ptLights[idx].specular;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 calcSpotLight()
{
	vec3 diffmap = texture(material.diffuseMap, fragTexCoord).xyz;
	vec3 specmap = texture(material.specularMap, fragTexCoord).xyz;

	vec3 norm = NormalMatrix * normalize(fragNormal);
	vec3 lightDir = normalize(spLight.position - fragPos);

	float c_theta = dot(lightDir, normalize(-spLight.direction));
	float epsilon = spLight.innerCutOff - spLight.outerCutOff;
	float intensity = clamp((c_theta - spLight.outerCutOff) / epsilon, 0.0f, 1.0f);

	vec3 ambient = diffmap * spLight.ambient;

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * diffmap * spLight.diffuse;

	vec3 viewDir = normalize(-fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = spec * specmap * spLight.specular;

	return (ambient + diffuse + specular) * intensity;
}
