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
	
	a3_KeyframeAnimationController.c
	Implementation of keyframe animation controller.
*/

#include "../a3_KeyframeAnimationController.h"

#include <string.h>


// macros to help with names
#define A3_CLIPCTRL_DEFAULTNAME		("unnamed clip ctrl")
#define A3_CLIPCTRL_SEARCHNAME		((ctrlName && *ctrlName) ? ctrlName : A3_CLIPCTRL_DEFAULTNAME)


//-----------------------------------------------------------------------------

// initialize clip controller
a3i32 a3clipControllerInit(a3_ClipController* clipCtrl_out, const a3byte ctrlName[a3keyframeAnimation_nameLenMax], const a3_ClipPool* clipPool, const a3ui32 clipIndex_pool, const a3i32 playback_step, const a3f64 playback_stepPerSec)
{
	a3i32 const ret = a3clipControllerSetClip(clipCtrl_out, clipPool, clipIndex_pool, playback_step, playback_stepPerSec);
	if (ret >= 0)
	{
		strncpy(clipCtrl_out->name, A3_CLIPCTRL_SEARCHNAME, a3keyframeAnimation_nameLenMax);
		return ret;
	}
	return -1;
}

// update clip controller
a3i32 a3clipControllerUpdate(a3_ClipController* clipCtrl, a3f64 dt)
{
	if (clipCtrl && clipCtrl->clipPool)
	{
//-----------------------------------------------------------------------------
//****TO-DO-ANIM-PROJECT-1: IMPLEMENT ME
//-----------------------------------------------------------------------------

		// variables
		a3f64 overstep;

		// time step
		dt *= clipCtrl->playback_sec;
		clipCtrl->clipTime_sec += dt;
		clipCtrl->keyframeTime_sec += dt;
		//dt *= clipCtrl->playback_stepPerSec;
		//clipCtrl->clipTime_step += (a3i32)dt;
		//clipCtrl->keyframeTime_step += (a3i32)dt;

		// resolve forward
		while ((overstep = clipCtrl->keyframeTime_sec - clipCtrl->keyframe->duration_sec) >= 0.0)
		{
			// are we passing the forward terminus of the clip
			if (clipCtrl->keyframeIndex == clipCtrl->clip->keyframeIndex_final)
			{
				// handle forward transition

				// default testing behavior: loop with overstep
				clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_first;
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;
				clipCtrl->keyframeTime_sec = overstep;
			}
			// are we simply moving to the next keyframe
			else
			{
				// set keyframe indices
				clipCtrl->keyframeIndex += clipCtrl->clip->keyframeDirection;

				// set keyframe pointers
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;

				// new time is just the overstep
				clipCtrl->keyframeTime_sec = overstep;
			}
		}

		// resolve reverse
		while ((overstep = clipCtrl->keyframeTime_sec) < 0.0)
		{
			// are we passing the reverse terminus of the clip
			if (clipCtrl->keyframeIndex == clipCtrl->clip->keyframeIndex_first)
			{
				// handle reverse transition

				// default testing behavior: loop with overstep
				clipCtrl->keyframeIndex = clipCtrl->clip->keyframeIndex_final;
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;
				clipCtrl->keyframeTime_sec = overstep + clipCtrl->keyframe->duration_sec;
			}
			// are we simply moving to the previous keyframe
			else
			{
				clipCtrl->keyframeIndex -= clipCtrl->clip->keyframeDirection;

				// set keyframe pointers
				clipCtrl->keyframe = clipCtrl->clipPool->keyframe + clipCtrl->keyframeIndex;

				// new time is overstep (negative) from new duration
				clipCtrl->keyframeTime_sec = overstep + clipCtrl->keyframe->duration_sec;
			}
		}

		// normalize
		clipCtrl->keyframeParam = clipCtrl->keyframeTime_sec * clipCtrl->keyframe->durationInv;
		clipCtrl->clipParam = clipCtrl->clipTime_sec * clipCtrl->clip->durationInv;

		// done
		return 1;

//-----------------------------------------------------------------------------
//****END-TO-DO-PROJECT-1
//-----------------------------------------------------------------------------
	}
	return -1;
}


//-----------------------------------------------------------------------------
