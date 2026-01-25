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
	
	passTangentBasis_morph5_transform_instanced_vs4x.glsl
	Calculate and pass tangent basis with morphing.
*/

#version 450

#define MAX_INSTANCES 64

struct sMorphTarget
{
	vec4 mPosition, mNormal, mTangent;
};
//layout (location = 0) in sMorphTarget aMorphTarget[5];
layout (location =  0) in vec4 aMorphTarget0_mPosition;
layout (location =  1) in vec4 aMorphTarget0_mNormal;
layout (location =  2) in vec4 aMorphTarget0_mTangent;
layout (location =  3) in vec4 aMorphTarget1_mPosition;
layout (location =  4) in vec4 aMorphTarget1_mNormal;
layout (location =  5) in vec4 aMorphTarget1_mTangent;
layout (location =  6) in vec4 aMorphTarget2_mPosition;
layout (location =  7) in vec4 aMorphTarget2_mNormal;
layout (location =  8) in vec4 aMorphTarget2_mTangent;
layout (location =  9) in vec4 aMorphTarget3_mPosition;
layout (location = 10) in vec4 aMorphTarget3_mNormal;
layout (location = 11) in vec4 aMorphTarget3_mTangent;
layout (location = 12) in vec4 aMorphTarget4_mPosition;
layout (location = 13) in vec4 aMorphTarget4_mNormal;
layout (location = 14) in vec4 aMorphTarget4_mTangent;
layout (location = 15) in vec4 aTexcoord;

uniform mat4 uP;
struct sTransformStack {
	mat4 mMV, mMV_nrm, mAtlas;
};
uniform ubTransformStack {
	sTransformStack uTransformStack[MAX_INSTANCES];
};
uniform double uTime;

out vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
};

flat out int vVertexID;
flat out int vInstanceID;


vec4 lerp(in vec4 v0, in vec4 v1, in float u);
vec4 nlerp(in vec4 v0, in vec4 v1, in float u);
vec4 CatmullRom(in vec4 vP, in vec4 v0, in vec4 v1, in vec4 vN, in float u);
vec4 nCatmullRom(in vec4 vP, in vec4 v0, in vec4 v1, in vec4 vN, in float u);

void lerp(out sMorphTarget k, in sMorphTarget k0, in sMorphTarget k1, in float u)
{
	k.mPosition = lerp(k0.mPosition, k1.mPosition, u);
	k.mNormal = nlerp(k0.mNormal, k1.mNormal, u);
	k.mTangent = nlerp(k0.mTangent, k1.mTangent, u);
}

void CatmullRom(out sMorphTarget k, in sMorphTarget kP, in sMorphTarget k0, in sMorphTarget k1, in sMorphTarget kN, in float u)
{
	k.mPosition = CatmullRom(kP.mPosition, k0.mPosition, k1.mPosition, kN.mPosition, u);
	k.mNormal = nCatmullRom(kP.mNormal, k0.mNormal, k1.mNormal, kN.mNormal, u);
	k.mTangent = nCatmullRom(kP.mTangent, k0.mTangent, k1.mTangent, kN.mTangent, u);
}


void makeMorphTarget(out sMorphTarget k, in vec4 position, in vec4 normal, in vec4 tangent)
{
	k.mPosition = position;
	k.mNormal = normal;
	k.mTangent = tangent;
}


void main()
{
	// DUMMY OUTPUT: directly assign input position to output position
//	gl_Position = aPosition;

	float t = float(uTime);
	float u = fract(t);
	int i0 = int(t) % 5;
	int i1 = (i0 + 1) % 5;
	int iN = (i1 + 1) % 5;
	int iP = (i0 + 4) % 5;
	
	sMorphTarget k;
	sMorphTarget aMorphTarget[5];
	makeMorphTarget(aMorphTarget[0], aMorphTarget0_mPosition, aMorphTarget0_mNormal, aMorphTarget0_mTangent);
	makeMorphTarget(aMorphTarget[1], aMorphTarget1_mPosition, aMorphTarget1_mNormal, aMorphTarget1_mTangent);
	makeMorphTarget(aMorphTarget[2], aMorphTarget2_mPosition, aMorphTarget2_mNormal, aMorphTarget2_mTangent);
	makeMorphTarget(aMorphTarget[3], aMorphTarget3_mPosition, aMorphTarget3_mNormal, aMorphTarget3_mTangent);
	makeMorphTarget(aMorphTarget[4], aMorphTarget4_mPosition, aMorphTarget4_mNormal, aMorphTarget4_mTangent);
	//k = aMorphTarget[i0];
	//lerp(k, aMorphTarget[i0], aMorphTarget[i1], u);
	CatmullRom(k, aMorphTarget[iP], aMorphTarget[i0], aMorphTarget[i1], aMorphTarget[iN], u);
	//makeMorphTarget(k, aMorphTarget0_mPosition, aMorphTarget0_mNormal, aMorphTarget0_mTangent);
	vec4 aPosition = k.mPosition;
	vec4 aNormal = k.mNormal;
	vec4 aTangent = k.mTangent;
	vec4 aBitangent = vec4(cross(aNormal.xyz, aTangent.xyz), 0.0);

	sTransformStack tstack = uTransformStack[gl_InstanceID];
	vTangentBasis_view = tstack.mMV_nrm * mat4(aTangent, aBitangent, aNormal, vec4(0.0));
	vTangentBasis_view[3] = tstack.mMV * aPosition;
	gl_Position = uP * vTangentBasis_view[3];
	
	vTexcoord_atlas = tstack.mAtlas * aTexcoord;

	vVertexID = gl_VertexID;
	vInstanceID = gl_InstanceID;
}
