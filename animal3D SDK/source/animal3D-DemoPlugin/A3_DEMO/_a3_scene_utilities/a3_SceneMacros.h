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

	a3_SceneMacros.h
	Declarations of helpful macros.

	********************************************
	*** Declarations of utility macros.      ***
	********************************************
*/

#ifndef __ANIMAL3D_SCENEMACROS_H
#define __ANIMAL3D_SCENEMACROS_H


//-----------------------------------------------------------------------------
// helpers for toggling and looping controls

#define a3sceneCtrlToggle(value)											(value = !value)
#define a3sceneCtrlIncLoop(value, maximum)								(value = (value + 1) % maximum)
#define a3sceneCtrlDecLoop(value, maximum)								(value = (value + maximum - 1) % maximum)
#define a3sceneCtrlIncCap(value, maximum)								(value = value < maximum ? value + 1 : value)
#define a3sceneCtrlDecCap(value, minimum)								(value = value > minimum ? value - 1 : value)
#define a3sceneCtrlIncClamp(value, maximum, minimum)						(value = value < maximum && value > minimum ? maximum : value)
#define a3sceneCtrlDecClamp(value, maximum, minimum)						(value = value > minimum && value < maximum ? minimum : value)

#define a3sceneCtrlCaseToggle(value, ctrl)								case ctrl: a3sceneCtrlToggle(value); break
#define a3sceneCtrlCaseIncLoop(value, maximum, ctrl)						case ctrl: a3sceneCtrlIncLoop(value, maximum); break
#define a3sceneCtrlCaseDecLoop(value, maximum, ctrl)						case ctrl: a3sceneCtrlDecLoop(value, maximum); break
#define a3sceneCtrlCaseIncCap(value, maximum, ctrl)						case ctrl: a3sceneCtrlIncCap(value, maximum); break
#define a3sceneCtrlCaseDecCap(value, minimum, ctrl)						case ctrl: a3sceneCtrlDecCap(value, minimum); break

#define a3sceneCtrlCasesLoop(value, maximum, ctrlInc, ctrlDec)			a3sceneCtrlCaseIncLoop(value, maximum, ctrlInc);	a3sceneCtrlCaseDecLoop(value, maximum, ctrlDec)
#define a3sceneCtrlCasesCap(value, maximum, minimum, ctrlInc, ctrlDec)	a3sceneCtrlCaseIncCap(value, maximum, ctrlInc);	a3sceneCtrlCaseDecCap(value, minimum, ctrlDec)


//-----------------------------------------------------------------------------


#endif	// !__ANIMAL3D_SCENEMACROS_H