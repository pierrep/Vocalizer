uniform sampler2D tex0;


void main (void) {

	vec2 texcoords = gl_TexCoord[0].st;

	gl_FragColor = texture2D(tex0, texcoords) * gl_Color;
}
