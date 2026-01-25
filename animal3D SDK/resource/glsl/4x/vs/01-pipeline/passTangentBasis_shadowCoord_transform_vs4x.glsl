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
	
	passTangentBasis_shadowCoord_transform_vs4x.glsl
	Calculate and pass tangent basis, and shadow coordinate.
*/

#version 450

layout (location = 0) in vec4 aPosition;
layout (location = 2) in vec4 aNormal;
layout (location = 8) in vec4 aTexcoord;
layout (location = 10) in vec4 aTangent;
layout (location = 11) in vec4 aBitangent;

//uniform mat4 uP;
//uniform mat4 uMV, uMV_nrm;
//uniform mat4 uAtlas;
//uniform mat4 uMVPB_other;

struct sModelMatrixStack
{
	mat4 modelMat;
	mat4 modelMatInverse;
	mat4 modelMatInverseTranspose;
	mat4 modelViewMat;
	mat4 modelViewMatInverse;
	mat4 modelViewMatInverseTranspose;
	mat4 modelViewProjectionMat;
	mat4 atlasMat;
};

struct sProjectorMatrixStack
{
	mat4 projectionMat;
	mat4 projectionMatInverse;
	mat4 projectionBiasMat;
	mat4 projectionBiasMatInverse;
	mat4 viewProjectionMat;
	mat4 viewProjectionMatInverse;
	mat4 viewProjectionBiasMat;
	mat4 viewProjectionBiasMatInverse;
};

uniform ubTransformStack
{
	sProjectorMatrixStack uProjectorCamera, uProjectorLight;
	sModelMatrixStack uModel[64];
};

uniform int uIndex;

out vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
	vec4 vShadowCoord;
};

flat out int vVertexID;
flat out int vInstanceID;

void main()
{
	// DUMMY OUTPUT: directly assign input position to output position
//	gl_Position = aPosition;

	vTangentBasis_view = uModel[uIndex].modelViewMatInverseTranspose * mat4(aTangent, aBitangent, aNormal, vec4(0.0));
	vTangentBasis_view[3] = uModel[uIndex].modelViewMat * aPosition;
	gl_Position = uProjectorCamera.projectionMat * vTangentBasis_view[3];
	
	vTexcoord_atlas = uModel[uIndex].atlasMat * aTexcoord;
	
	vShadowCoord = uProjectorLight.viewProjectionBiasMat * uModel[uIndex].modelMat * aPosition;

	vVertexID = gl_VertexID;
	vInstanceID = gl_InstanceID;
}
