#version 150

uniform sampler2D tex0;

in vec4 colorVarying;
in vec2 texCoordVarying;

out vec4 fragColor;

void main (void) {
	fragColor = texture(tex0, texCoordVarying) * colorVarying;
}
