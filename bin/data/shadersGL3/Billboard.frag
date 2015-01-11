#version 150

uniform sampler2D tex0;

in vec4 colorVarying;

out vec4 fragColor;

void main (void) {
	fragColor = texture(tex0, gl_PointCoord) * colorVarying;
}
