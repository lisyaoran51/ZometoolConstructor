#version 130

in vec4 s_vPosition;
in vec4 s_vNormal;

uniform mat4 mM;
uniform mat4 mV;
uniform mat4 mP;

//R�Ψӱ���ҫ�������`���@
uniform mat4 mRotations;

//��V�����I��
uniform vec4 vLight;

//�S���C��
out vec3 fN;
out vec3 fL;
out vec3 fL2;

out vec3 fE;

void main()
{
	vec4 vLight2 = vec4 ( -1.0 , -1.0 , 1.0 , 1.0 );
	fN = ( mRotations * s_vNormal ).xyz;
	//fN = ( s_vNormal ).xyz;
	fL = vLight.xyz;
	fL2 = vLight2.xyz;

	fE = ( mV * mM * s_vPosition ).xyz;
	//fE = ( s_vPosition ).xyz;
	gl_Position = mP*mV*mM*s_vPosition;

}