/*
	Copyright 2011-2026 Daniel S. Buckstein

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

/*
	animal3D SDK: Minimal 3D Animation Framework
	By Daniel S. Buckstein
	
	drawPhong_shadow_fs4x.glsl
	Output Phong shading with shadow mapping.
*/

#version 450

in vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
	vec4 vShadowCoord;
};

struct sPointLight
{
	vec4 viewPos, worldPos, color, radiusInfo;
};

uniform ubLight
{
	sPointLight uPointLight[8];
};

uniform int uCount;

uniform vec4 uColor;

uniform sampler2D uTex_dm, uTex_sm, uTex_shadow;

const vec4 kEyePos = vec4(0.0, 0.0, 0.0, 1.0);

layout (location = 0) out vec4 rtFragColor;

void initDefaultPoint(out sPointLight light);
void calcPhongPoint(out vec4 diffuseColor, out vec4 specularColor, in vec4 eyeVec,
	in vec4 fragPos, in vec4 fragNrm, in vec4 fragColor,
	in vec4 lightPos, in vec4 lightRadiusInfo, in vec4 lightColor);

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE GREEN
//	rtFragColor = vec4(0.0, 1.0, 0.0, 1.0);

	vec4 diffuseColor = vec4(0.0), specularColor = diffuseColor, dd, ds;
	vec4 viewVec = kEyePos - vTangentBasis_view[3];
	int i;
	
	for (i = 0; i < uCount; ++i)
	{
		calcPhongPoint(dd, ds, normalize(viewVec), vTangentBasis_view[3], normalize(vTangentBasis_view[2]), uColor, uPointLight[i].viewPos, uPointLight[i].radiusInfo, uPointLight[i].color);
		diffuseColor += dd;
		specularColor += ds;
	}

	vec4 sample_dm = texture(uTex_dm, vTexcoord_atlas.xy);
	vec4 sample_sm = texture(uTex_sm, vTexcoord_atlas.xy);
	rtFragColor = sample_dm * diffuseColor + sample_sm * specularColor;
	
	vec4 shadowCoord = vShadowCoord / vShadowCoord.w;
	vec4 shadowSample = texture(uTex_shadow, shadowCoord.xy);
	vec4 shadowColor = sample_dm * 0.1;
	float shadowTest = float(shadowSample.r > shadowCoord.z - 0.0001);
	
	rtFragColor = mix(shadowColor, rtFragColor, shadowTest) + uColor * 0.0625;
	rtFragColor.a = sample_dm.a;
	
	// DEBUGGING
	//rtFragColor = vec4(shadowSample.r > shadowCoord.z - 0.0001);
	//rtFragColor.a = 1.0;
}
