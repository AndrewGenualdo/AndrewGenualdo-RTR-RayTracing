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
	
	a3_DemoState-idle-input.c/.cpp
	Demo state function implementations.

	****************************************************
	*** THIS IS ONE OF YOUR DEMO'S MAIN SOURCE FILES ***
	*** Implement your demo logic pertaining to      ***
	***     HANDLING INPUT in this file.             ***
	****************************************************
*/

//-----------------------------------------------------------------------------

#include "../a3_DemoState.h"

#include "../_a3_scene_utilities/a3_SceneMacros.h"


//-----------------------------------------------------------------------------
// INPUT SUB-ROUTINES

void a3demo_input_controlObject(
	a3_DemoState *demoState, a3_SceneObject *object,
	a3f64 const dt, a3real ctrlMoveSpeed, a3real ctrlRotateSpeed)
{
	a3real azimuth = 0.0f;
	a3real elevation = 0.0f;
	a3boolean rotatingObject = 0, movingObject = 0;

	// using Xbox controller
	if (a3XboxControlIsConnected(demoState->xcontrol))
	{
		// move and rotate camera using joysticks
		a3f64 lJoystick[2], rJoystick[2], lTrigger[1], rTrigger[1];
		a3XboxControlGetJoysticks(demoState->xcontrol, lJoystick, rJoystick);
		a3XboxControlGetTriggers(demoState->xcontrol, lTrigger, rTrigger);

		movingObject = a3scene_moveSceneObject(object, 
			ctrlMoveSpeed * (a3real)(dt),
			(a3real)(lJoystick[0]),
			(a3real)(lJoystick[1]),
			(a3real)(*rTrigger - *lTrigger)
		);
		// rotate
		{
			azimuth = (a3real)(-rJoystick[0]);
			elevation = (a3real)(rJoystick[1]);

			// this really defines which way is "up"
			// mouse's Y motion controls pitch, but X can control yaw or roll
			// controlling yaw makes Y axis seem "up", roll makes Z seem "up"
			rotatingObject = a3scene_rotateSceneObject(object,
				ctrlRotateSpeed * (a3real)(dt * 10.0),
				// pitch: vertical tilt
				elevation,
				// yaw/roll depends on "vertical" axis: if y, yaw; if z, roll
				a3real_zero,
				azimuth);
		}
	}

	// using mouse and keyboard
	else
	{
		// move using WASDEQ
		movingObject = a3scene_moveSceneObject(object, 
			ctrlMoveSpeed * (a3real)(dt),
			(a3real)a3keyboardGetDifference(demoState->keyboard, a3key_D, a3key_A),
			(a3real)a3keyboardGetDifference(demoState->keyboard, a3key_W, a3key_S),
			(a3real)a3keyboardGetDifference(demoState->keyboard, a3key_E, a3key_Q)
		);
		if (a3mouseIsHeld(demoState->mouse, a3mouse_left))
		{
			azimuth = -(a3real)a3mouseGetDeltaX(demoState->mouse);
			elevation = -(a3real)a3mouseGetDeltaY(demoState->mouse);

			// this really defines which way is "up"
			// mouse's Y motion controls pitch, but X can control yaw or roll
			// controlling yaw makes Y axis seem "up", roll makes Z seem "up"
			rotatingObject = a3scene_rotateSceneObject(object,
				ctrlRotateSpeed * (a3real)(dt),
				// pitch: vertical tilt
				elevation,
				// yaw/roll depends on "vertical" axis: if y, yaw; if z, roll
				a3real_zero,
				azimuth);
		}
	}
}

void a3demo_input_controlProjector(
	a3_DemoState* demoState, a3_SceneProjector* projector,
	a3f64 const dt, a3real ctrlMoveSpeed, a3real ctrlRotateSpeed, a3real ctrlZoomSpeed)
{
	a3demo_input_controlObject(demoState, projector->sceneObject, 
		dt, projector->ctrlMoveSpeed, projector->ctrlRotateSpeed);

	if (a3XboxControlIsConnected(demoState->xcontrol))
	{
		projector->fovy -= ctrlZoomSpeed * (projector->fovy / a3real_oneeighty)
			* (a3real)((a3XboxControlGetState(demoState->xcontrol, a3xbox_leftBumper)
				- a3XboxControlGetState(demoState->xcontrol, a3xbox_rightBumper)) / 256);
	}
	else
	{
		projector->fovy -= ctrlZoomSpeed * (projector->fovy / a3real_oneeighty)
			* (a3real)(a3mouseIsChangedWheel(demoState->mouse));
	}

	projector->fovy = a3clamp(ctrlZoomSpeed, 
		a3real_oneeighty - ctrlZoomSpeed, projector->fovy);
	a3scene_updateProjectorProjectionMat(projector, true);
}


//-----------------------------------------------------------------------------
// CALLBACKS

// ascii key callback
void a3demo_input_keyCharPress(a3_DemoState* demoState, a3i32 const asciiKey)
{
	switch (asciiKey)
	{
		// change pipeline mode
		a3sceneCtrlCasesLoop(demoState->scene, demoState_mode_max, '>', '<');
		a3sceneCtrlCasesLoop(demoState->scene, demoState_mode_max, '.', ',');

		// toggle grid
		a3sceneCtrlCaseToggle(demoState->displayGrid, 'g');

		// toggle skybox
		a3sceneCtrlCaseToggle(demoState->displaySkybox, 'b');

		// toggle hidden volumes
		a3sceneCtrlCaseToggle(demoState->displayHiddenVolumes, 'h');

		// toggle world axes
		a3sceneCtrlCaseToggle(demoState->displayWorldAxes, 'x');

		// toggle object axes
		a3sceneCtrlCaseToggle(demoState->displayObjectAxes, 'z');

		// toggle tangent bases on vertices or other
		a3sceneCtrlCaseToggle(demoState->displayTangentBases, 'B');

		// toggle wireframe overlay
		a3sceneCtrlCaseToggle(demoState->displayWireframe, 'F');

		// update animation
		a3sceneCtrlCaseToggle(demoState->updateAnimation, 'm');

		// toggle stencil test
		a3sceneCtrlCaseToggle(demoState->stencilTest, 'i');

		// toggle stencil test
		a3sceneCtrlCaseToggle(demoState->skipIntermediatePasses, 'I');
	}


	// update current mode pointer
	demoState->sceneCallbacksPtr = demoState->sceneCallbacks + demoState->scene;

	// callbacks for current mode
	if (demoState->sceneCallbacksPtr)
	{
		if (demoState->sceneCallbacksPtr->handleKeyPress)
			demoState->sceneCallbacksPtr->handleKeyPress(demoState,
				demoState->sceneCallbacksPtr->scene, asciiKey, 0);
		if (demoState->sceneCallbacksPtr->handleKeyHold)
			demoState->sceneCallbacksPtr->handleKeyHold(demoState,
				demoState->sceneCallbacksPtr->scene, asciiKey, 0);
	}
}

// ascii key hold callback
void a3demo_input_keyCharHold(a3_DemoState* demoState, a3i32 asciiKey)
{
//	switch (asciiKey)
//	{
//
//	}


	// callback for current mode
	if (demoState->sceneCallbacksPtr->handleKeyHold)
		demoState->sceneCallbacksPtr->handleKeyHold(demoState,
			demoState->sceneCallbacksPtr->scene, asciiKey, 0);
}


//-----------------------------------------------------------------------------
// INPUT

void a3demo_input(a3_DemoState* demoState, a3f64 const dt)
{
	// callback for current mode
	if (demoState->sceneCallbacksPtr->handleInput)
		demoState->sceneCallbacksPtr->handleInput(demoState,
			demoState->sceneCallbacksPtr->scene, dt);
}


//-----------------------------------------------------------------------------
