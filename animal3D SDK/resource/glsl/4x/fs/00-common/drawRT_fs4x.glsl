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
	
	drawRT_fs4x.glsl
	Output ray-tracing.
*/

#version 450

in vbVertexData {
	mat4 vTangentBasis_view;
	vec4 vTexcoord_atlas;
};

#define MAX_MODELS 24
#define MAX_VIEWERS 1
struct sModelStack
{
	mat4 modelMat;						// model matrix (object -> world)
	mat4 modelMatInverse;				// model inverse matrix (world -> object)
	mat4 modelMatInverseTranspose;		// model inverse-transpose matrix (object -> world skewed)
	mat4 modelViewMat;					// model-view matrix (object -> viewer)
	mat4 modelViewMatInverse;			// model-view inverse matrix (viewer -> object)
	mat4 modelViewMatInverseTranspose;	// model-view inverse transpose matrix (object -> viewer skewed)
	mat4 modelViewProjectionMat;		// model-view-projection matrix (object -> clip)
	mat4 atlasMat;						// atlas matrix (texture -> cell)
};
struct sViewerStack
{
	mat4 projectionMat;					// projection matrix (viewer -> clip)
	mat4 projectionMatInverse;			// projection inverse matrix (clip -> viewer)
	mat4 projectionBiasMat;				// projection-bias matrix (viewer -> biased clip)
	mat4 projectionBiasMatInverse;		// projection-bias inverse matrix (biased clip -> viewer)
	mat4 viewProjectionMat;				// view-projection matrix (world -> clip)
	mat4 viewProjectionMatInverse;		// view-projection inverse matrix (clip -> world)
	mat4 viewProjectionBiasMat;			// view projection-bias matrix (world -> biased clip)
	mat4 viewProjectionBiasMatInverse;	// view-projection-bias inverse matrix (biased clip -> world)
};
uniform ubTransformStack {
	sViewerStack viewer_stack[MAX_VIEWERS];
	sModelStack model_stack[MAX_MODELS];
};


#define SPHERE 0
#define CUBE 1

#define IDX_MODEL_CUBE0 0
#define IDX_MODEL_CUBE1 1
#define IDX_MODEL_SPHERE0 2
#define IDX_MODEL_SPHERE1 3
#define IDX_MODEL_LIGHT0 4
#define IDX_MODEL_WALLS 5

const float radius_sphere1 = 1.0f;
const float radius_sphere2 = 2.0f;
const float halfsize_cube0 = 0.5f;
const float halfsize_cube1 = 0.5f;
const float radius_sphere0 = 2.0f;
const float halfsize_walls = 2.28571428571f;

uniform mat4 uP;
uniform mat4 uPB;

uniform vec4 uColor;

uniform sampler2D uTex_dm;

layout (location = 0) out vec4 rtFragColor;

float raySphereHit(vec3 p0, vec3 p, vec3 Q, float r) {
	vec3 s = Q - p0;

	float b = dot(p, s);
	float c = dot(s,s) - r * r;
	float d = b * b - c;
	if(d < 0.0f) return -1.0f;
	return b - sqrt(d);
}

//based on https://en.wikipedia.org/wiki/Slab_method
float rayCubeHit(vec3 p0, vec3 p, int modelIndex, float halfSize) {

	mat4 invModel = model_stack[modelIndex].modelViewMatInverse;
    vec3 ro = (invModel * vec4(p0, 1.0)).xyz; //ray origin
    vec3 rd = (invModel * vec4(p, 0.0)).xyz; //ray direction

    vec3 tMin = (-halfSize - ro) / rd; //t values hitting the -halfSize planes
    vec3 tMax = ( halfSize - ro) / rd; //t values hitting the +halfSize planes

    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);

    float tNear = max(max(t1.x, t1.y), t1.z); //enter
    float tFar  = min(min(t2.x, t2.y), t2.z); //exit

    if(tNear > tFar || tFar < 0.0) return -1.0;
    return tNear >= 0.0 ? tNear : tFar;
}



float random(float randomSeed, float min, float max) {
	
	uint x = uint(randomSeed);
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;

	float res = float(x) / 2295.0f;
	res = mod(res, 1.0f);
	res *= (max - min);
	res += min;
	return res;
}

float random(float min, float max) {
	return random(dot(gl_FragCoord.x * gl_FragCoord.y, length(vTangentBasis_view[3].xyz)), min, max);
}

vec3 randomUnitVector() {
	float r1 = random(dot(gl_FragCoord.x * gl_FragCoord.y, length(vTangentBasis_view[3].xyz)), -1.0f, 1.0f);
	float r2 = random(r1 * 1000.0f, -1.0f, 1.0f);
	float r3 = random(r2 * 100000.0f, -1.0f, 1.0f);

	vec3 p = vec3(r1, r2, r3);
	return normalize(p);
}

//call this 16 times per bounce?
vec3 randomOnHemisphere(vec3 normal) {

	vec3 onUnitSphere = randomUnitVector();
	if(dot(onUnitSphere, normal) <= 0.0f) onUnitSphere = -onUnitSphere;
	onUnitSphere += normal;

	return normalize(onUnitSphere);
}

void main()
{
	vec3 p0 = vec3(0.0f);
	vec3 P_target = vTangentBasis_view[3].xyz;
	vec3 p = normalize(P_target - p0);

	//infinity :D
	float closestDist = 1.0 / 0.0;
	int closestIndex = -1;

	int types[6] = int[](CUBE, CUBE, SPHERE, SPHERE, SPHERE, CUBE);
	int models[6] = int[](IDX_MODEL_CUBE0, IDX_MODEL_CUBE1, IDX_MODEL_SPHERE0, IDX_MODEL_SPHERE1, IDX_MODEL_LIGHT0, IDX_MODEL_WALLS);
	float sizes[6] = float[](halfsize_cube0, halfsize_cube1,radius_sphere0, radius_sphere1, radius_sphere2, halfsize_walls);

	for(int i = 0; i <= 5; i++) {
		float d = 0.0f;
		if(types[i] == CUBE) d = rayCubeHit(p0, p, models[i], sizes[i]);
		else if(types[i] == SPHERE) d = raySphereHit(p0, p, model_stack[models[i]].modelViewMat[3].xyz, sizes[i]);

		if(d >= 0.0f && d < closestDist) {
			closestIndex = i;
			closestDist = d;
		}
	}


	if(closestIndex == -1) {
		vec4 sample_dm = texture(uTex_dm, vTexcoord_atlas.xy);
		//rtFragColor = sample_dm * uColor;
		rtFragColor = vec4(0.0f);
		rtFragColor.a = sample_dm.a;
		return;
	}

	vec3 hit_pos = p0 + closestDist * p;
	vec4 hit_pos_view = vec4(hit_pos, 1.0f);
	vec4 hit_pos_bias = viewer_stack[0].projectionBiasMat * hit_pos_view;

	gl_FragDepth = hit_pos_bias.z / hit_pos_bias.w;
	vec3 hit_normal;
	if(types[closestIndex] == SPHERE) hit_normal = normalize(hit_pos - model_stack[models[closestIndex]].modelViewMat[3].xyz);
	else if(types[closestIndex] == CUBE) hit_normal = normalize(hit_pos - model_stack[models[closestIndex]].modelViewMat[3].xyz);

	//debug to see normals
	rtFragColor.rgb = hit_normal * 0.5f + 0.5f;
	rtFragColor.a = 1.0f;

	//debug to see random
	//rtFragColor.rgb = vec3(random(0.0f, 1.0f));

	rtFragColor.rgb = randomOnHemisphere(hit_normal) * 0.5f + 0.5f;
}
