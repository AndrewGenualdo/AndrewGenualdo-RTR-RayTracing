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
	
	a3_HierarchyStateBlend.inl
	Implementation of inline hierarchical blend operations.
*/


#ifdef __ANIMAL3D_HIERARCHYSTATEBLEND_H
#ifndef __ANIMAL3D_HIERARCHYSTATEBLEND_INL
#define __ANIMAL3D_HIERARCHYSTATEBLEND_INL


//-----------------------------------------------------------------------------

inline a3blendOpExecute a3blendOpExecSelect(a3ui16 const controlCount, a3ui16 const inputCount)
{
	a3blendOpExecute const exec[a3blendOpLimitControl][a3blendOpLimitInput] = {
		{ a3blendOpExecute0C, },
		{ a3blendOpExecute1C, a3blendOpExecute1C1I, },
		{ a3blendOpExecute2C, a3blendOpExecute2C1I, },
	};
	return exec[controlCount][inputCount];
}


//-----------------------------------------------------------------------------

// init
inline a3_BlendOp* a3blendOpInit(a3_BlendOp* const blendOp, a3ui16 const controlCount, a3ui16 const inputCount, a3realOp const op, a3real* const v_out, a3real const* const v[a3blendOpLimitControl], a3real4 const* const u[a3blendOpLimitInput])
{
	if (blendOp && op && v_out && v && u)
	{
		a3real const** itr = blendOp->v_ctrl, * const* const end = itr + a3blendOpLimitControl;
		blendOp->op = op;
		blendOp->v_out = v_out;
		blendOp->vCount = blendOp->uCount = 0;
		while (itr < end)
			*itr = *(v++);
		return blendOp;
	}
	return 0;
}

inline a3_BlendOpSet* a3blendOpSetInit(a3_BlendOpSet* const blendOpSet, a3ui16 const controlCount, a3ui16 const inputCount, a3realOp const op_transformMat, a3realOp const op_transformDQ, a3realOp const op_rotate, a3realOp const op_scale, a3realOp const op_translate, a3realOp const op_user)
{
	if (blendOpSet)
	{
		blendOpSet->exec = a3blendOpExecSelect(controlCount, inputCount);
		blendOpSet->op_transformMat = op_transformMat ? op_transformMat : a3blendOpRET4X4;
		blendOpSet->op_transformDQ = op_transformDQ ? op_transformDQ : a3blendOpRET4X4;
		blendOpSet->op_rotate = /*(channel & a3poseChannel_rotate_xyz) &&*/ op_rotate ? op_rotate : a3blendOpRET4;
		blendOpSet->op_scale = /*(channel & a3poseChannel_scale_xyz) &&*/ op_scale ? op_scale : a3blendOpRET4;
		blendOpSet->op_translate = /*(channel & a3poseChannel_translate_xyz) &&*/ op_translate ? op_translate : a3blendOpRET4;
		blendOpSet->op_user = /*(channel & a3poseChannel_user_xyz) &&*/ op_user ? op_user : a3blendOpRET4;
		return blendOpSet;
	}
	return 0;
}


// execute
inline a3_BlendOp const* a3blendOpExecute0C(a3_BlendOp const* const blendOp)
{
	//if (blendOp && blendOp->op && blendOp->v_out)
	{
		typedef a3real4r(*a3realOp0C)(a3real* v_out);
		((a3realOp0C)blendOp->op)(blendOp->v_out);
	}
	return blendOp;
}

inline a3_BlendOp const* a3blendOpExecute1C(a3_BlendOp const* const blendOp)
{
	//if (blendOp && blendOp->op && blendOp->v_out && blendOp->v_ctrl[0])
	{
		typedef a3real4r(*a3realOp1C)(a3real* v_out, a3real const* const v_ctrl0);
		((a3realOp1C)blendOp->op)(blendOp->v_out, blendOp->v_ctrl[0]);
	}
	return blendOp;
}

inline a3_BlendOp const* a3blendOpExecute2C(a3_BlendOp const* const blendOp)
{
	//if (blendOp && blendOp->op && blendOp->v_out && blendOp->v_ctrl[0] && blendOp->v_ctrl[1])
	{
		typedef a3real4r(*a3realOp2C)(a3real* v_out, a3real const* const v_ctrl0, a3real const* const v_ctrl1);
		((a3realOp2C)blendOp->op)(blendOp->v_out, blendOp->v_ctrl[0], blendOp->v_ctrl[1]);
	}
	return blendOp;
}

inline a3_BlendOp const* a3blendOpExecute1C1I(a3_BlendOp const* const blendOp)
{
	//if (blendOp && blendOp->op && blendOp->v_out && blendOp->v_ctrl[0] && blendOp->u[0])
	{
		typedef a3real4r(*a3realOp1C1I)(a3real* v_out, a3real const* const v_ctrl0, a3real const u0);
		((a3realOp1C1I)blendOp->op)(blendOp->v_out, blendOp->v_ctrl[0], *blendOp->u[0]);
	}
	return blendOp;
}

inline a3_BlendOp const* a3blendOpExecute2C1I(a3_BlendOp const* const blendOp)
{
	//if (blendOp && blendOp->op && blendOp->v_out && blendOp->v_ctrl[0] && blendOp->v_ctrl[1] && blendOp->u[0])
	{
		typedef a3real4r(*a3realOp2C1I)(a3real* v_out, a3real const* const v_ctrl0, a3real const* const v_ctrl1, a3real const u0);
		((a3realOp2C1I)blendOp->op)(blendOp->v_out, blendOp->v_ctrl[0], blendOp->v_ctrl[1], *blendOp->u[0]);
	}
	return blendOp;
}


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_HIERARCHYSTATEBLEND_INL
#endif	// __ANIMAL3D_HIERARCHYSTATEBLEND_H