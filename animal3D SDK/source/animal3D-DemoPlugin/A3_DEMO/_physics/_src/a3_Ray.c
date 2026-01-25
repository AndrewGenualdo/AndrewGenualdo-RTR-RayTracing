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

    a3_Ray.c
    Ray implementation.
*/

#include "../a3_Ray.h"


//-----------------------------------------------------------------------------

a3real3r a3rayComputePos(a3real3 p_out, a3real3 const p_origin, a3real3 const v_direction, a3real const param)
{
    if (!p_out || !p_origin || !v_direction)
        return 0;

    // P(t) = P(0) + v*t
    a3real3ProductS(p_out, v_direction, param);
    a3real3Add(p_out, p_origin);

    return p_out;
}


//-----------------------------------------------------------------------------

a3i32 a3rayReset(a3_Ray* ray)
{
    if (!ray)
        return -1;

    ray->p_origin    = a3vec4_zero;
    ray->v_direction = a3vec4_zero;

    return 0;
}

a3i32 a3rayInit(a3_Ray* ray, a3real3 const p_origin, a3real3 const v_direction)
{
    if (!ray || !p_origin || !v_direction)
        return -1;

    a3real4SetReal3W(ray->p_origin.v,    p_origin,    a3real_one);
    a3real4SetReal3W(ray->v_direction.v, v_direction, a3real_zero);

    return 0;
}

a3i32 a3rayInitTarget(a3_Ray* ray, a3real3 const p_origin, a3real3 const p_target)
{
    if (!ray || !p_origin || !p_target)
        return -1;

    a3real4SetReal3W(ray->p_origin.v, p_origin, a3real_one);
    a3real3Diff(ray->v_direction.v, p_target, p_origin);
    ray->v_direction.w = a3real_zero;

    return 0;
}

a3i32 a3rayInitUnit(a3_Ray* ray, a3real3 const p_origin, a3real3 const v_direction)
{
    if (!ray || !p_origin || !v_direction)
        return -1;
    
    a3rayInit(ray, p_origin, v_direction);
    a3real3Normalize(ray->v_direction.v);

    return 0;
}

a3i32 a3rayInitTargetUnit(a3_Ray* ray, a3real3 const p_origin, a3real3 const p_target)
{
    if (!ray || !p_origin || !p_target)
        return -1;

    a3rayInitTarget(ray, p_origin, p_target);
    a3real3Normalize(ray->v_direction.v);

    return 0;
}


//-----------------------------------------------------------------------------

a3i32 a3rayHitReset(a3_RayHit* ray_hit)
{
    if (!ray_hit)
        return -1;

    ray_hit->p_hit = a3vec4_zero;

    return 0;
}


//-----------------------------------------------------------------------------
