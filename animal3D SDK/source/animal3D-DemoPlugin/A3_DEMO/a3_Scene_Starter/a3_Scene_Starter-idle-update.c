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
	
	a3_Scene_Starter-idle-update.c/.cpp
	Demo mode implementations: starter scene.

	********************************************
	*** UPDATE FOR STARTER SCENE MODE        ***
	********************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_Scene_Starter.h"

//typedef struct a3_DemoState a3_DemoState;
#include "../a3_DemoState.h"

#include "../_a3_scene_utilities/a3_SceneMacros.h"


//-----------------------------------------------------------------------------
// UPDATE

void a3demo_update_objects(a3f64 const dt, a3_SceneObject* sceneObjectBase,
	a3ui32 count, a3boolean useZYX, a3boolean applyScale);
void a3demo_update_defaultAnimation(a3_DemoState* demoState, a3f64 const dt,
	a3_SceneObject* sceneObjectBase, a3ui32 count, a3ui32 axis);
void a3demo_update_bindSkybox(a3_SceneObject* obj_camera, a3_SceneObject* obj_skybox);
void a3demo_update_pointLight(a3_SceneObject* obj_camera, a3_ScenePointLight* pointLightBase, a3ui32 count);

void a3demo_applyScale_internal(a3_SceneObject* sceneObject, a3real4x4p s);

void a3starter_update(a3_DemoState* demoState, a3_Scene_Starter* scene, a3f64 const dt)
{
	a3ui32 i;
	a3_SceneModelMatrixStack matrixStack[starterMaxCount_sceneObject];

	// active camera
	a3_SceneProjector const* activeCamera = scene->projector + scene->activeCamera;
	a3_SceneObject const* activeCameraObject = activeCamera->sceneObject;

	// temp scale mat
	a3mat4 scaleMat = a3mat4_identity;

	a3demo_update_objects(dt, scene->object_scene,
		starterMaxCount_sceneObject, 0, 0);
	a3demo_update_objects(dt, scene->object_camera,
		starterMaxCount_cameraObject, 1, 0);

	a3scene_updateProjectorViewProjectionMat(scene->proj_camera_main);

	a3demo_update_defaultAnimation(demoState, dt, scene->obj_box, 6, 2);

	// apply scales to objects
	for (i = 0; i < starterMaxCount_sceneObject; ++i)
		a3demo_applyScale_internal(scene->object_scene + i, scaleMat.m);

	// update skybox
	a3demo_update_bindSkybox(scene->obj_camera_main, scene->obj_skybox);

	// update matrix stack data
	for (i = 0; i < starterMaxCount_sceneObject; ++i)
	{
		a3scene_updateModelMatrixStack(matrixStack + i,
			activeCamera->projectionMat.m, activeCameraObject->modelMat.m, activeCameraObject->modelMatInv.m,
			scene->object_scene[i].modelMat.m, a3mat4_identity.m);
	}
}


//-----------------------------------------------------------------------------
