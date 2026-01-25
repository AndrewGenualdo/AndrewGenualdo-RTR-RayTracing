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
	
	a3_SceneObject.h
	Example of scene utility header file.
*/

#ifndef __ANIMAL3D_SCENEOBJECT_H
#define __ANIMAL3D_SCENEOBJECT_H


// math library
#include "animal3D-A3DM/animal3D-A3DM.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif	// __cplusplus

	
//-----------------------------------------------------------------------------

	// matrix stack for a single object
	typedef struct a3_SceneModelMatrixStack
	{
		a3mat4 modelMat;						// model matrix (object -> world)
		a3mat4 modelMatInverse;					// model inverse matrix (world -> object)
		a3mat4 modelMatInverseTranspose;		// model inverse-transpose matrix (object -> world skewed)
		a3mat4 modelViewMat;					// model-view matrix (object -> viewer)
		a3mat4 modelViewMatInverse;				// model-view inverse matrix (viewer -> object)
		a3mat4 modelViewMatInverseTranspose;	// model-view inverse transpose matrix (object -> viewer skewed)
		a3mat4 modelViewProjectionMat;			// model-view-projection matrix (object -> clip)
		a3mat4 atlasMat;						// atlas matrix (texture -> cell)
	} a3_SceneModelMatrixStack;

	// matrix stack for a viewer object
	typedef struct a3_SceneViewerMatrixStack
	{
		a3mat4 projectionMat;					// projection matrix (viewer -> clip)
		a3mat4 projectionMatInverse;			// projection inverse matrix (clip -> viewer)
		a3mat4 projectionBiasMat;				// projection-bias matrix (viewer -> biased clip)
		a3mat4 projectionBiasMatInverse;		// projection-bias inverse matrix (biased clip -> viewer)
		a3mat4 viewProjectionMat;				// view-projection matrix (world -> clip)
		a3mat4 viewProjectionMatInverse;		// view-projection inverse matrix (clip -> world)
		a3mat4 viewProjectionBiasMat;			// view projection-bias matrix (world -> biased clip)
		a3mat4 viewProjectionBiasMatInverse;	// view-projection-bias inverse matrix (biased clip -> world)
	} a3_SceneViewerMatrixStack;

	// general scene objects
	typedef struct a3_SceneObject
	{
		a3mat4 modelMat;		// model matrix: transform relative to scene
		a3mat4 modelMatInv;		// inverse model matrix: scene relative to this
		a3vec3 euler;			// euler angles for direct rotation control
		a3vec3 position;		// scene position for direct control
		a3vec3 scale;			// scale (not accounted for in update)
		a3ui32 scaleMode;		// 0 = off; 1 = uniform; other = non-uniform (nightmare)
		a3ui32 sceneGraphIndex;	// index in scene graph
	} a3_SceneObject;

	// projector/camera/viewer
	typedef struct a3_SceneProjector
	{
		a3_SceneObject *sceneObject;		// pointer to scene object
		a3mat4 projectionMat;				// projection matrix
		a3mat4 projectionMatInv;			// inverse projection matrix
		a3mat4 viewProjectionMat;			// concatenation of view-projection
        a3mat4 viewProjectionMatInv;        // inverse concatenation of view-projection
		a3boolean perspective;				// perspective or orthographic
		a3real fovy;						// persp: vert field of view/ortho: vert size
		a3real aspect;						// aspect ratio
		a3real znear, zfar;					// near and far clipping planes
		a3real ctrlMoveSpeed;				// how fast controlled camera moves
		a3real ctrlRotateSpeed;				// control rotate speed (degrees)
		a3real ctrlZoomSpeed;				// control zoom speed (degrees)
	} a3_SceneProjector;

	// simple point light
	typedef struct a3_ScenePointLight
	{
		a3vec4 worldPos;					// position in world space
		a3vec4 viewPos;						// position in viewer space
		a3vec4 color;						// RGB color with padding
		a3real radius;						// radius (distance of effect from center)
		a3real radiusSq;					// radius squared (if needed)
		a3real radiusInv;					// radius inverse (also used for attenuation sometimes)
		a3real radiusInvSq;					// radius inverse squared (attenuation factor)
	}a3_ScenePointLight;


//-----------------------------------------------------------------------------

	// scene object initializers and updates
	inline void a3scene_initSceneObject(a3_SceneObject *sceneObject);
	inline void a3scene_updateSceneObject(a3_SceneObject *sceneObject, const a3boolean useZYX);
	inline a3i32 a3scene_rotateSceneObject(a3_SceneObject *sceneObject, const a3real speed, const a3real deltaX, const a3real deltaY, const a3real deltaZ);
	inline a3i32 a3scene_moveSceneObject(a3_SceneObject *sceneObject, const a3real speed, const a3real deltaX, const a3real deltaY, const a3real deltaZ);

	inline void a3scene_setProjectorSceneObject(a3_SceneProjector *projector, a3_SceneObject *sceneObject);
	inline void a3scene_initProjector(a3_SceneProjector *projector);
	inline void a3scene_updateProjectorProjectionMat(a3_SceneProjector *projector, const a3boolean z_up);
	inline void a3scene_updateProjectorViewProjectionMat(a3_SceneProjector *projector);

	inline void a3scene_resetModelMatrixStack(a3_SceneModelMatrixStack* model);
	inline void a3scene_resetViewerMatrixStack(a3_SceneViewerMatrixStack* viewer);
	inline void a3scene_updateModelMatrixStack(a3_SceneModelMatrixStack* model, a3real4x4p const projectionMat_viewer, a3real4x4p const modelMat_viewer, a3real4x4p const modelMatInv_viewer, a3real4x4p const modelMat, a3real4x4p const atlasMat);
	inline void a3scene_updateViewerMatrixStack(a3_SceneViewerMatrixStack* viewer, a3real4x4p const modelMat_viewer, a3real4x4p const modelMatInv_viewer, a3real4x4p const projectionMat, a3real4x4p const projectionMatInv, a3real4x4p const biasMat, a3real4x4p const biasMatInv);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_SCENEOBJECT_H