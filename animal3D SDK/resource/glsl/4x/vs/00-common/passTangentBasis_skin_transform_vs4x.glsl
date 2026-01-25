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
	
	passTangentBasis_skin_transform_vs4x.glsl
	Calculate and pass tangent basis with skinning.
*/

#version 450

layout (location = 10) in vec4 aTangent;
layout (location = 11) in vec4 aBitangent;
layout (location = 2) in vec4 aNormal;
layout (location = 0) in vec4 aPosition;
layout (location = 8) in vec4 aTexcoord;
layout (location = 1) in vec4 aBlendWeight;
layout (location = 7) in ivec4 aBlendIndex;

#define MAX_NODES 128

#define quat vec4
#define dquat mat2x4

uniform ubTransformBlend {
	mat4 uObjectSpaceBindToCurrent[MAX_NODES];
	dquat uObjectSpaceBindToCurrentDQ[MAX_NODES];
};

uniform mat4 uP;
uniform mat4 uMV, uMV_nrm;
uniform mat4 uAtlas;

out vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
};

flat out int vVertexID;
flat out int vInstanceID;


vec4 skinVector_linear(in vec4 v, in vec4 weight, in ivec4 index)
{
	vec4 result = vec4(0.0);
	result += (uObjectSpaceBindToCurrent[index[0]] * v) * weight[0];
	result += (uObjectSpaceBindToCurrent[index[1]] * v) * weight[1];
	result += (uObjectSpaceBindToCurrent[index[2]] * v) * weight[2];
	result += (uObjectSpaceBindToCurrent[index[3]] * v) * weight[3];
	return result;
}


mat4 dq2mat(in dquat dq)
{
	vec4 r = dq[0], d = dq[1];
	mat4 result = mat4(
		1.0 - 2.0*(r.y*r.y + r.z*r.z),
		2.0*(r.x*r.y + r.z*r.w),
		2.0*(r.x*r.z - r.y*r.w),
		0.0,
		
		2.0*(r.x*r.y - r.z*r.w),
		1.0 - 2.0*(r.z*r.z + r.x*r.x),
		2.0*(r.y*r.z + r.x*r.w),
		0.0,
		
		2.0*(r.x*r.z + r.y*r.w),
		2.0*(r.y*r.z - r.x*r.w),
		1.0 - 2.0*(r.x*r.x + r.y*r.y),
		0.0,
		
		2.0*(-d.w*r.x + d.x*r.w - d.y*r.z + d.z*r.y),
		2.0*(-d.w*r.y + d.x*r.z + d.y*r.w - d.z*r.x),
		2.0*(-d.w*r.z - d.x*r.y + d.y*r.x + d.z*r.w),
		1.0
	);	
	return result;
}

mat4 calcSkinMat_dualquat(in vec4 weight, in ivec4 index)
{
	dquat dq = dquat(0.0);
	dq += uObjectSpaceBindToCurrentDQ[index[0]] * weight[0];
	dq += uObjectSpaceBindToCurrentDQ[index[1]] * weight[1];
	dq += uObjectSpaceBindToCurrentDQ[index[2]] * weight[2];
	dq += uObjectSpaceBindToCurrentDQ[index[3]] * weight[3];
	dq /= length(dq[0]);
	return dq2mat(dq);
}


void main()
{
	// DUMMY OUTPUT: directly assign input position to output position
//	gl_Position = aPosition;

	mat4 dqm = calcSkinMat_dualquat(aBlendWeight, aBlendIndex);

	vTangentBasis_view = uMV_nrm * 
		dqm * mat4(vec4(aTangent.xyz, 0.0), vec4(aBitangent.xyz, 0.0), vec4(aNormal.xyz, 0.0), vec4(0.0));
		//mat4(
		//	normalize(skinVector_linear(vec4(aTangent.xyz, 0.0), aBlendWeight, aBlendIndex)), 
		//	normalize(skinVector_linear(vec4(aBitangent.xyz, 0.0), aBlendWeight, aBlendIndex)), 
		//	normalize(skinVector_linear(vec4(aNormal.xyz, 0.0), aBlendWeight, aBlendIndex)), 
		//	vec4(0.0)
		//);
	vTangentBasis_view[3] = uMV * 
		dqm * aPosition;
		//skinVector_linear(aPosition, aBlendWeight, aBlendIndex);
	gl_Position = uP * vTangentBasis_view[3];
	
	vTexcoord_atlas = uAtlas * aTexcoord;

	vVertexID = gl_VertexID;
	vInstanceID = gl_InstanceID;
}
