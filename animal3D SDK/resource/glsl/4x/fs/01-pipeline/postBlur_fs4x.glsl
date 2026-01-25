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
	
	postBlur_fs4x.glsl
	Gaussian blur.
*/

#version 450

in vec4 vTexcoord_atlas;

uniform vec4 uColor;

uniform vec2 uAxis;

uniform sampler2D uImage00;

layout (location = 0) out vec4 rtFragColor;

vec4 GaussianBlur11(in sampler2D tex, in vec2 uv, in vec2 blurAxis)
{
    vec2 cp = uv + blurAxis * 0.5, cn = cp - blurAxis;
    vec4 col = (texture(tex, cp) + texture(tex, cn)) * 462.0;
    col += (texture(tex, cp += blurAxis) + texture(tex, cn -= blurAxis)) * 330.0;
    col += (texture(tex, cp += blurAxis) + texture(tex, cn -= blurAxis)) * 165.0;
    col += (texture(tex, cp += blurAxis) + texture(tex, cn -= blurAxis)) * 55.0;
    col += (texture(tex, cp += blurAxis) + texture(tex, cn -= blurAxis)) * 11.0;
    col += (texture(tex, cp += blurAxis) + texture(tex, cn -= blurAxis));
    return (col * (1.0 / 2048.0));
}

void main()
{
	// DUMMY OUTPUT: all fragments are OPAQUE YELLOW
//	rtFragColor = vec4(1.0, 1.0, 0.0, 1.0);

	rtFragColor = GaussianBlur11(uImage00, vTexcoord_atlas.xy, uAxis) * uColor;
}
