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

	a3_Scene_Starter.h
	Demo mode interface: starter scene.

	********************************************
	*** THIS IS ONE DEMO MODE'S HEADER FILE  ***
	********************************************
*/

#ifndef __ANIMAL3D_SCENE_STARTER_H
#define __ANIMAL3D_SCENE_STARTER_H


//-----------------------------------------------------------------------------

#include "_a3_scene_utilities/a3_SceneObject.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// maximum unique objects
	enum
	{
		starterMaxCount_sceneObject = 8,
		starterMaxCount_cameraObject = 1,
		starterMaxCount_projector = 1,
	};

	// scene object rendering program names
	typedef enum a3_Scene_Starter_RenderProgramName
	{
		starter_renderSolid,			// solid color
		starter_renderTexture,			// textured
		starter_renderLambert,			// Lambert shading model
		starter_renderPhong,			// Phong shading model

		starter_render_max
	} a3_Scene_Starter_RenderProgramName;

	// final display modes
	typedef enum a3_Scene_Starter_DisplayProgramName
	{
		starter_displayTexture,			// display simple texture

		starter_display_max
	} a3_Scene_Starter_DisplayProgramName;

	// active camera names
	typedef enum a3_Scene_Starter_ActiveCameraName
	{
		starter_cameraSceneViewer,		// scene viewing camera

		starter_camera_max
	} a3_Scene_Starter_ActiveCameraName;

	// pipeline names
	typedef enum a3_Scene_Starter_PipelineName
	{
		starter_forward,				// forward lighting pipeline

		starter_pipeline_max
	} a3_Scene_Starter_PipelineName;

	// render passes
	typedef enum a3_Scene_Starter_PassName
	{
		starter_passScene,				// render scene objects
		starter_passComposite,			// composite layers

		starter_pass_max
	} a3_Scene_Starter_PassName;

	// render target names
	typedef enum a3_Scene_Starter_TargetName
	{
		starter_scene_finalcolor = 0,	// final display color
		starter_scene_fragdepth,		// fragment depth
	
		starter_target_scene_max
	} a3_Scene_Starter_TargetName;


//-----------------------------------------------------------------------------

	// demo mode for basic shading
	typedef struct a3_Scene_Starter
	{
		a3_Scene_Starter_RenderProgramName render;
		a3_Scene_Starter_DisplayProgramName display;
		a3_Scene_Starter_ActiveCameraName activeCamera;

		a3_Scene_Starter_PipelineName pipeline;
		a3_Scene_Starter_PassName pass;
		a3_Scene_Starter_TargetName targetIndex[starter_pass_max], targetCount[starter_pass_max];

		// objects
		union {
			a3_SceneObject object_scene[starterMaxCount_sceneObject];
			struct {
				a3_SceneObject
					obj_skybox[1];
				a3_SceneObject
					obj_plane[1],
					obj_box[1],
					obj_sphere[1],
					obj_cylinder[1],
					obj_capsule[1],
					obj_torus[1];
				a3_SceneObject
					obj_teapot[1];
			};
		};
		union {
			a3_SceneObject object_camera[starterMaxCount_cameraObject];
			struct {
				a3_SceneObject
					obj_camera_main[1];
			};
		};
		union {
			a3_SceneProjector projector[starterMaxCount_projector];
			struct {
				a3_SceneProjector
					proj_camera_main[1];
			};
		};
	} a3_Scene_Starter;


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_SCENE_STARTER_H