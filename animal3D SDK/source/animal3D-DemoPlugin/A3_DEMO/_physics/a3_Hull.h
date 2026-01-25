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

    a3_Hull.h
    Structures for general shapes and hulls.
*/

#ifndef __ANIMAL3D_A3DX_HULL_H
#define __ANIMAL3D_A3DX_HULL_H


#include "animal3D-A3DM/animal3D-A3DM.h"


#ifdef __cplusplus
extern "C"
{
#endif	// __cplusplus


//-----------------------------------------------------------------------------


typedef enum a3_HullType
{
    hull_type_point = -1,
} a3_HullType;


typedef struct a3_Hull
{
    a3_HullType type;
} a3_Hull;


//-----------------------------------------------------------------------------

a3i32 a3hullReset(a3_Hull* hull);


//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif // #ifndef __ANIMAL3D_A3DX_HULL_H