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

    a3_Hull.c
    Hull implementation.
*/

#include "../a3_Hull.h"


//-----------------------------------------------------------------------------

a3i32 a3hullReset(a3_Hull* hull)
{
    if (!hull)
        return -1;

    hull->type = hull_type_point;

    return 0;
}


//-----------------------------------------------------------------------------
