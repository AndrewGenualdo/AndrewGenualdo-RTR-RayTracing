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
	
	postBlend_fs4x.glsl
	Blending layers, composition.
*/

#version 450

in vec4 vTexcoord_atlas;

uniform vec4 uColor;

uniform sampler2D uImage00, uImage01, uImage02, uImage03;

layout (location = 0) out vec4 rtFragColor;

vec4 screen(in vec4 a, in vec4 b)
{
	return (1.0 - (1.0 - a) * (1.0 - b));
}

vec4 screen(in vec4 a, in vec4 b, in vec4 c, in vec4 d)
{
	return (1.0 - (1.0 - a) * (1.0 - b) * (1.0 - c) * (1.0 - d));
}

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE YELLOW
//	rtFragColor = vec4(1.0, 1.0, 0.0, 1.0);

	vec4 col00 = texture(uImage00, vTexcoord_atlas.xy);
	vec4 col01 = texture(uImage01, vTexcoord_atlas.xy);
	vec4 col02 = texture(uImage02, vTexcoord_atlas.xy);
	vec4 col03 = texture(uImage03, vTexcoord_atlas.xy);
	rtFragColor = screen(col00, col01, col02, col03) * uColor;
}
