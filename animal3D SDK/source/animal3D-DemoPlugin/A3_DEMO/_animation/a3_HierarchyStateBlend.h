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
	
	a3_HierarchyStateBlend.h
	Hierarchy blend operations.
*/

#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#define __ANIMAL3D_HIERARCHYSTATEBLEND_H


#include "a3_HierarchyState.h"

#include "a3_Kinematics.h"


#ifdef __cplusplus
extern "C"
{
#endif	// __cplusplus


//-----------------------------------------------------------------------------

// operation template for any real vector/matrix/array type
typedef a3real* (*a3realOp)(a3real v_out[], ...);

// forward declare blend operation type
typedef struct a3_BlendOp a3_BlendOp;

// operation template for executing an operation
typedef a3_BlendOp const* (*a3blendOpExecute)(a3_BlendOp const* const blendOp);

// limits
enum
{
	a3blendOpLimitControl = 16,
	a3blendOpLimitInput = 8,
};

// replicable data structure for vector operations
typedef struct a3_BlendOp
{
	a3blendOpExecute exec;							// execution function
	a3realOp op;									// blend operation function
	a3real* v_out;									// output value
	a3real const* v_ctrl[a3blendOpLimitControl];	// control value set (maxed at 16 to pre-allocate array)
	a3real const* u[a3blendOpLimitInput];			// input value set
	a3ui16 vCount, uCount;							// control and input counts
} a3_BlendOp;


// set of operations for spatial pose
typedef struct a3_BlendOpSet
{
	a3blendOpExecute exec;
	a3realOp op_transformMat;
	a3realOp op_transformDQ;
	a3realOp op_rotate;
	a3realOp op_scale;
	a3realOp op_translate;
	a3realOp op_user;
} a3_BlendOpSet;

// init
a3_BlendOp* a3blendOpInit(a3_BlendOp* const blendOp, a3ui16 const controlCount, a3ui16 const inputCount, a3realOp const op, a3real* const v_out, a3real const* const v[a3blendOpLimitControl], a3real4 const* const u[a3blendOpLimitInput]);
a3_BlendOpSet* a3blendOpSetInit(a3_BlendOpSet* const blendOpSet, a3ui16 const controlCount, a3ui16 const inputCount, a3realOp const op_transformMat, a3realOp const op_transformDQ, a3realOp const op_rotate, a3realOp const op_scale, a3realOp const op_translate, a3realOp const op_user);

// execute
a3_BlendOp const* a3blendOpExecute0C(a3_BlendOp const* const blendOp);
a3_BlendOp const* a3blendOpExecute1C(a3_BlendOp const* const blendOp);
a3_BlendOp const* a3blendOpExecute2C(a3_BlendOp const* const blendOp);
a3_BlendOp const* a3blendOpExecute1C1I(a3_BlendOp const* const blendOp);
a3_BlendOp const* a3blendOpExecute2C1I(a3_BlendOp const* const blendOp);

// operation examples (follow templates above)
a3real4r a3blendOpRET4(a3real4 v_out);
a3real4r a3blendOpZERO4(a3real4 v_out);
a3real4r a3blendOpONE4(a3real4 v_out);
a3real4r a3blendOpID4(a3real4 v_out);
a3real4r a3blendOpCOPY4(a3real4 v_out, a3real4 const v);
a3real4r a3blendOpNEGATE4(a3real4 v_out, a3real4 const v);
a3real4r a3blendOpRECIP4(a3real4 v_out, a3real4 const v);
a3real4r a3blendOpCONJQ4(a3real4 v_out, a3real4 const v);
a3real4r a3blendOpADD4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpSUB4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpMUL4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpDIV4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpMULQ4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpMULCONJQ4(a3real4 v_out, a3real4 const v0, a3real4 const v1);
a3real4r a3blendOpSCALE4(a3real4 v_out, a3real4 const v, a3real const u);
a3real4r a3blendOpPOW4(a3real4 v_out, a3real4 const v, a3real const u);
a3real4r a3blendOpNEAR4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u);
a3real4r a3blendOpLERP4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u);
a3real4r a3blendOpNLERP4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u);
a3real4r a3blendOpSLERP4(a3real4 v_out, a3real4 const v0, a3real4 const v1, a3real const u);

a3real4r a3blendOpRET4X2(a3real4 m_out);
a3real4r a3blendOpID4X2(a3real4 m_out);
a3real4r a3blendOpCOPY4X2(a3real4 m_out, a3real4 const m);
a3real4r a3blendOpCONJDQ4X2(a3real4 m_out, a3real4 const m);
a3real4r a3blendOpMULDQ4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1);
a3real4r a3blendOpMULCONJDQ4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1);
a3real4r a3blendOpSCALE4X2(a3real4 m_out, a3real4 const m, a3real const u);
a3real4r a3blendOpNEAR4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);
a3real4r a3blendOpLERP4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);
a3real4r a3blendOpNLERP4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);
a3real4r a3blendOpSCLERP4X2(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);

a3real4r a3blendOpRET4X4(a3real4 m_out);
a3real4r a3blendOpID4X4(a3real4 m_out);
a3real4r a3blendOpCOPY4X4(a3real4 m_out, a3real4 const m);
a3real4r a3blendOpINVR4X4(a3real4 m_out, a3real4 const m);
a3real4r a3blendOpMULM4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1);
a3real4r a3blendOpMULINVR4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1);
a3real4r a3blendOpSCALE4X4(a3real4 m_out, a3real4 const m, a3real const u);
a3real4r a3blendOpNEAR4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);
a3real4r a3blendOpLERP4X4(a3real4 m_out, a3real4 const m0, a3real4 const m1, a3real const u);


//-----------------------------------------------------------------------------

typedef struct a3_SpatialPoseBlendNode
{
	const a3_BlendOpSet* blendOpSet;
	a3_SpatialPose* pose_out;
	a3_SpatialPose const* pose_ctrl[a3blendOpLimitControl];
	a3real const* u[a3blendOpLimitInput];
	a3ui16 vCount, uCount;
} a3_SpatialPoseBlendNode;

typedef struct a3_SpatialPoseBlendTree
{
	a3_Hierarchy const* blendTreeDescriptor;

	a3_SpatialPoseBlendNode* nodes;
} a3_SpatialPoseBlendTree;


//-----------------------------------------------------------------------------

// create blend tree
a3ret a3spatialPoseBlendTreeCreate(a3_SpatialPoseBlendTree* blendTree, a3_Hierarchy const* blendTreeDescriptor);

// release blend tree
a3ret a3spatialPoseBlendTreeRelease(a3_SpatialPoseBlendTree* blendTree);

// configure node internally; set pointers
a3ret a3spatialPoseBlendTreeConfigureNode(a3_SpatialPoseBlendTree const* blendTree, a3ui32 const nodeIndex);

// execute tree from leaves to root
a3ret a3spatialPoseBlendTreeExecute(a3_SpatialPoseBlendTree const* blendTree);


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpIdentity(a3_SpatialPose* pose_out);

// pointer-based LERP operation for single spatial pose
a3_SpatialPose* a3spatialPoseOpLERP(a3_SpatialPose* pose_out, a3_SpatialPose const* pose0, a3_SpatialPose const* pose1, a3real const u);


//-----------------------------------------------------------------------------

// pointer-based reset/identity operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpIdentity(a3_HierarchyPose* pose_out);

// pointer-based LERP operation for hierarchical pose
a3_HierarchyPose* a3hierarchyPoseOpLERP(a3_HierarchyPose* pose_out, a3_HierarchyPose const* pose0, a3_HierarchyPose const* pose1, a3real const u);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#include "_inl/a3_HierarchyStateBlend.inl"


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_H