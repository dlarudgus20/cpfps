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
 * @file shadow.fs
 * @date 2015. 10. 26.
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

in VS_OUT
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoord;
	vec4 fragPosLightSpace;
} vs_out;

out vec4 color;

uniform DirectionalLight dirLight;
uniform PointLight ptLights[POINTLIGHT_COUNT];
uniform SpotLight spLight;

uniform Material material;

uniform sampler2D shadowDepthMap;

vec3 calcDirectionalLight();
vec3 calcPointLight(int idx);
vec3 calcSpotLight();

vec3 calcBlinnPhong(vec3 lightDir, vec3 ambientLight, vec3 diffuseLight, vec3 specularLight);
float calcShadow();

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
	vec3 lightDir = normalize(-dirLight.direction);
	
	return calcBlinnPhong(lightDir, dirLight.ambient, dirLight.diffuse, dirLight.specular);
}

vec3 calcPointLight(int idx)
{
	vec3 lightDir = normalize(ptLights[idx].position - vs_out.fragPos);

	float dist = length(ptLights[idx].position - vs_out.fragPos);
	float constant = ptLights[idx].constant;
	float linear = ptLights[idx].linear;
	float quadratic = ptLights[idx].quadratic;
	float attenuation = 1.0f / (constant + linear * dist + quadratic * (dist * dist));
	
	return calcBlinnPhong(lightDir, ptLights[idx].ambient, ptLights[idx].diffuse, ptLights[idx].specular) * attenuation;
}

vec3 calcSpotLight()
{
	vec3 lightDir = normalize(spLight.position - vs_out.fragPos);

	float c_theta = dot(lightDir, normalize(-spLight.direction));
	float epsilon = spLight.innerCutOff - spLight.outerCutOff;
	float intensity = clamp((c_theta - spLight.outerCutOff) / epsilon, 0.0f, 1.0f);
	
	return calcBlinnPhong(lightDir, spLight.ambient, spLight.diffuse, spLight.specular) * intensity;
}

vec3 calcBlinnPhong(vec3 lightDir, vec3 ambientLight, vec3 diffuseLight, vec3 specularLight)
{
	vec3 diffmap = texture(material.diffuseMap, vs_out.texCoord).xyz;
	vec3 specmap = texture(material.specularMap, vs_out.texCoord).xyz;

	vec3 norm = normalize(vs_out.normal);

	vec3 ambient = diffmap * ambientLight;

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * diffmap * diffuseLight;

	vec3 viewDir = normalize(-vs_out.fragPos);
	// phong
	//vec3 reflectDir = reflect(-lightDir, norm);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	// blinn-phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0f), material.shininess);
	vec3 specular = spec * specmap * specularLight;

	float shadow = calcShadow();
	return ambient + (1.0f - shadow) * (diffuse + specular);
}

float calcShadow()
{
	vec3 projCoord = vs_out.fragPosLightSpace.xyz / vs_out.fragPosLightSpace.w;

	// convert [-1,1] NDC to [0,1] of depth map
	projCoord = projCoord * 0.5 + 0.5;

	float closestDepth = texture(shadowDepthMap, projCoord.xy).r;
	float currentDepth = projCoord.z;

	return currentDepth > closestDepth ? 1.0f : 0.0f;
}
