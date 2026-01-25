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

	a3_Scene_Rendering.h
	Demo mode interface: rendering scene.

	********************************************
	*** THIS IS ONE DEMO MODE'S HEADER FILE  ***
	********************************************
*/

#ifndef __ANIMAL3D_SCENE_RENDERING_H
#define __ANIMAL3D_SCENE_RENDERING_H


//-----------------------------------------------------------------------------

#include "_a3_scene_utilities/a3_SceneObject.h"

#include "_animation/a3_KeyframeAnimationController.h"
#include "_animation/a3_HierarchyStateBlend.h"


//-----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif	// __cplusplus


//-----------------------------------------------------------------------------

	// maximum unique objects
	enum
	{
		renderingMaxCount_sceneObject = 24,
		renderingMaxCount_projector = 1,
	};

	// scene object rendering program names
	typedef enum a3_Scene_Rendering_RenderProgramName
	{
		rendering_renderSolid,			// solid color
		rendering_renderTexture,		// textured
		rendering_renderLambert,		// Lambert shading model
		rendering_renderPhong,			// Phong shading model

		rendering_render_max
	} a3_Scene_Rendering_RenderProgramName;

	// final display modes
	typedef enum a3_Scene_Rendering_DisplayProgramName
	{
		rendering_displayTexture,			// display simple texture

		rendering_display_max
	} a3_Scene_Rendering_DisplayProgramName;

	// active camera names
	typedef enum a3_Scene_Rendering_ActiveCameraName
	{
		rendering_cameraSceneViewer,		// scene viewing camera

		rendering_camera_max
	} a3_Scene_Rendering_ActiveCameraName;

	// pipeline names
	typedef enum a3_Scene_Rendering_PipelineName
	{
		rendering_forward,				// forward lighting pipeline

		rendering_pipeline_max
	} a3_Scene_Rendering_PipelineName;

	// render passes
	typedef enum a3_Scene_Rendering_PassName
	{
		rendering_passScene,				// render scene objects
		rendering_passComposite,			// composite layers

		rendering_pass_max
	} a3_Scene_Rendering_PassName;

	// render target names
	typedef enum a3_Scene_Rendering_TargetName
	{
		rendering_scene_finalcolor = 0,	// final display color
		rendering_scene_fragdepth,		// fragment depth
	
		rendering_target_scene_max
	} a3_Scene_Rendering_TargetName;

	// control targets
	typedef enum a3_Scene_Rendering_ControlTarget
	{
		rendering_ctrl_camera,
		rendering_ctrl_teapot,
	
		rendering_ctrl_character,

		rendering_ctrl_character_rig,
		rendering_ctrl_neckLookat,
		rendering_ctrl_wristEffector_r,
		rendering_ctrl_wristConstraint_r,
		rendering_ctrl_wristEffector_l,
		rendering_ctrl_wristConstraint_l,
		rendering_ctrl_ankleEffector_r,
		rendering_ctrl_ankleConstraint_r,
		rendering_ctrl_ankleEffector_l,
		rendering_ctrl_ankleConstraint_l,

		rendering_ctrlmode_max
	} a3_Scene_Rendering_ControlTarget;
	
	// input modes
	typedef enum a3_Scene_Rendering_InputMode
	{
		rendering_input_direct,
		rendering_input_euler,
		rendering_input_kinematic,
		rendering_input_interpolate1,
		rendering_input_interpolate2,
	
		rendering_inputmode_max
	} a3_Scene_Rendering_InputMode;

	// display data for hierarchy drawing
	typedef struct a3_Scene_Rendering_HierarchyDisplayData
	{
		a3mat4 mvp_joint[128], mvp_bone[128], t_skin[128];
		a3dualquat dq_skin[128];
	} a3_Scene_Rendering_HierarchyDisplayData;


//-----------------------------------------------------------------------------

	// demo mode for basic shading
	typedef struct a3_Scene_Rendering
	{
		a3_Scene_Rendering_RenderProgramName render;
		a3_Scene_Rendering_DisplayProgramName display;
		a3_Scene_Rendering_ActiveCameraName activeCamera;

		a3_Scene_Rendering_PipelineName pipeline;
		a3_Scene_Rendering_PassName pass;
		a3_Scene_Rendering_TargetName targetIndex[rendering_pass_max], targetCount[rendering_pass_max];

		// scene graph
		a3_Hierarchy sceneGraph[1];
		a3_HierarchyState sceneGraphState[1];
		a3_SceneModelMatrixStack matrixStack[renderingMaxCount_sceneObject];
		
		// skeletal animation controllers
		union {
			a3_ClipController clipCtrl[3];
			struct {
				a3_ClipController
					clipCtrl_idle_p[1],
					clipCtrl_idle_m[1],
					clipCtrl_idle_f[1];
			};
		};

		// other animation controllers
		a3_ClipController clipCtrl_morph[1];
		a3f64 morph_time;

		// complete clip/keyframe/sample collection
		a3_ClipPool clipPool[1];
		
		// skeletal animation
		union {
			a3_HierarchyState hierarchyState_skel[4];
			struct {
				a3_HierarchyState
					hierarchyState_skel_ik[1],
					hierarchyState_skel_fk[1],
					hierarchyState_skel_final[1],
					hierarchyState_skel_base[1];
			};
		};
		a3_Hierarchy hierarchy_skel[1];
		a3_HierarchyPoseGroup hierarchyPoseGroup_skel[1];
		a3_Scene_Rendering_HierarchyDisplayData display_main;
		
		// blend tree
		a3_Hierarchy blendTree[1];
		union {
			a3_HierarchyState hierarchyState_skel_blend[5];
			struct {
				a3_HierarchyState
					//blend control nodes
					hierarchyState_skel_blend_idle_p[1],
					hierarchyState_skel_blend_idle_m[1],
					hierarchyState_skel_blend_idle_f[1],
					//blend result nodes
					hierarchyState_skel_blend_idle_fm_blend[1],
					hierarchyState_skel_blend_result[1];
			};
		};
		a3_Scene_Rendering_HierarchyDisplayData display_tree[5];
		
		// blend operations
		a3_BlendOpSet blendOpID[1], blendOpRET[1], blendOpCOPY[1], blendOpINV[1],
			blendOpCONCAT[1], blendOpDECONCAT[1], blendOpSCALE[1],
			blendOpNEAR[1], blendOpLERP[1];
		// input axes & integration variables
		a3f64 axis_l[2], axis_r[2];
		a3vec2 pos, vel, acc;
		a3real rot, velr, accr;
		a3_Scene_Rendering_InputMode ctrl_position, ctrl_rotation;

		// control modes
		a3_Scene_Rendering_ControlTarget ctrl_target;

		// objects
		union {
			a3_SceneObject object_scene[renderingMaxCount_sceneObject];
			struct {
				a3_SceneObject
					obj_world_root[1];
				a3_SceneObject
					obj_camera_main[1];
				a3_SceneObject
					obj_light_main[1];
				a3_SceneObject
					obj_skybox[1];
				a3_SceneObject
					obj_teapot[1];
				
				a3_SceneObject
					obj_skeleton_ctrl[1],
					obj_skeleton_rig[1];
				a3_SceneObject
					obj_skeleton_neckLookat_ctrl[1],
					obj_skeleton_wristEffector_r_ctrl[1],
					obj_skeleton_wristConstraint_r_ctrl[1],
					obj_skeleton_wristEffector_l_ctrl[1],
					obj_skeleton_wristConstraint_l_ctrl[1],
					obj_skeleton_ankleEffector_r_ctrl[1],
					obj_skeleton_ankleConstraint_r_ctrl[1],
					obj_skeleton_ankleEffector_l_ctrl[1],
					obj_skeleton_ankleConstraint_l_ctrl[1],
					obj_skeleton[1];
			};
		};
		union {
			a3_SceneProjector projector[renderingMaxCount_projector];
			struct {
				a3_SceneProjector
					proj_camera_main[1];
			};
		};
	} a3_Scene_Rendering;


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !__ANIMAL3D_SCENE_RENDERING_H