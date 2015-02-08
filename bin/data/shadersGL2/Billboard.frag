uniform sampler2D tex0;
varying  float rotAngle;

void main (void) {
	float angle		= rotAngle;
	vec2 offset		= vec2(0.5, 0.5);
	vec2 v2 = gl_TexCoord[0].st;


	float cosX, sinX;
	float cosY, sinY;
	sinX = sin(angle);
	cosX = cos(angle);
	sinY = sin(angle);
	cosY = cos(angle);

	mat2 rotationMatrix = mat2( cosX, -sinX,
							    sinY, cosX);

    vec2 texcoord = gl_TexCoord[0].st;
	vec2 newcoords = ((texcoord-offset) * (rotationMatrix));
	newcoords += offset;

	gl_FragColor = texture2D(tex0, texcoord) * gl_Color;
}
