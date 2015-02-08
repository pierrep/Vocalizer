
attribute float angle;
varying  float rotAngle;

varying vec2 sprite_coord;
varying float texsize;

void main() {
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec4 eyeCoord = gl_ModelViewMatrix * gl_Vertex;
	gl_Position = gl_ProjectionMatrix * eyeCoord;
	float dist = sqrt(eyeCoord.x*eyeCoord.x + eyeCoord.y*eyeCoord.y + eyeCoord.z*eyeCoord.z);
	float att	 = 600.0 / dist;

	gl_PointSize = gl_Normal.x * att;
	gl_FrontColor = gl_Color;

    float sWidth = gl_Normal.y;
    float totalSprites = gl_Normal.y * gl_Normal.y;
    float sPos = gl_Normal.z;

    float x = mod(sPos,sWidth);
    float y = floor(sPos/sWidth);
    float w = 1.0/sWidth;
    sprite_coord = vec2(x * w, y * w);
	texsize = w;

	rotAngle = angle;
}





