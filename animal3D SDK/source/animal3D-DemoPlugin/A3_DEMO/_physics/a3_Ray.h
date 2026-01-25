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
    animal3D SDK: Minimal 3D Animation Framework Extended (A3DX)
    By Daniel S. Buckstein

    a3_Ray.h
    Structures for raycasts and handling.
*/

#ifndef __ANIMAL3D_A3DX_RAY_H
#define __ANIMAL3D_A3DX_RAY_H


#include "a3_Hull.h"


#ifdef __cplusplus
extern "C"
{
#endif	// __cplusplus


//-----------------------------------------------------------------------------

typedef struct a3_Ray
{
    a3vec4 p_origin;
    a3vec4 v_direction;
} a3_Ray;


typedef struct a3_RayHit
{
    a3vec4 p_hit;
} a3_RayHit;


//-----------------------------------------------------------------------------

a3real3r a3rayComputePos(a3real3 p_out, a3real3 const p_origin, a3real3 const v_direction, a3real const param);


//-----------------------------------------------------------------------------

a3i32 a3rayReset(a3_Ray* ray);
a3i32 a3rayInit(a3_Ray* ray, a3real3 const p_origin, a3real3 const v_direction);
a3i32 a3rayInitTarget(a3_Ray* ray, a3real3 const p_origin, a3real3 const p_target);
a3i32 a3rayInitUnit(a3_Ray* ray, a3real3 const p_origin, a3real3 const v_direction);
a3i32 a3rayInitTargetUnit(a3_Ray* ray, a3real3 const p_origin, a3real3 const p_target);


//-----------------------------------------------------------------------------

a3i32 a3rayHitReset(a3_RayHit* ray_hit);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif // #ifndef __ANIMAL3D_A3DX_RAY_H