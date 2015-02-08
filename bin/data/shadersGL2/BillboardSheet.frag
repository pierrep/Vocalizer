uniform sampler2D tex0;

float texsize = 0.5;
varying vec2 sprite_coord;


void main (void) {
	vec2 texcoord = sprite_coord + gl_TexCoord[0].st * texsize;
	gl_FragColor = texture2D(tex0, texcoord) * gl_Color;
	//gl_FragColor = vec4(1,0,0,1);
	//gl_FragColor = gl_Color;
}
