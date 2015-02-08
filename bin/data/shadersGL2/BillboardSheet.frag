uniform sampler2D tex0;

varying float texsize;
varying vec2 sprite_coord;
varying  float rotAngle;

void main (void) {

	float angle		= rotAngle;
	vec2 offset		= vec2(sprite_coord.x + texsize*0.9/2.0 + texsize*0.05, sprite_coord.y + texsize*0.9/2.0 + texsize*0.05);
	vec2 v2 = gl_TexCoord[0].st;


	float cosX, sinX;
	float cosY, sinY;
	sinX = sin(angle);
	cosX = cos(angle);
	sinY = sin(angle);
	cosY = cos(angle);

	mat2 rotationMatrix = mat2( cosX, -sinX,
							    sinY, cosX);

    vec2 texcoord = sprite_coord + gl_TexCoord[0].st * texsize*0.9;
    texcoord += vec2(texsize*0.05,texsize*0.05);
	vec2 newcoords = ((texcoord-offset) * (rotationMatrix));
	newcoords += offset;

	gl_FragColor = texture2D(tex0, newcoords) * gl_Color;

    //vec2 texcoord = sprite_coord + gl_TexCoord[0].st * texsize;
    //gl_FragColor = texture2D(tex0, texcoord) * gl_Color;

	//gl_FragColor = vec4(1,0,0,1);
	//gl_FragColor = gl_Color;
}
