#version 130

in vec3 fN;
in vec3 fL;
in vec3 fL2;

in vec3 fE;

out vec4 fColor;

void main ()
{
	vec3 N = normalize ( fN );
	vec3 L = normalize ( fL );
	vec3 L2 = normalize ( fL2 );
	vec3 E = normalize ( -fE );
	vec3 H = normalize ( L + E );
	vec3 H2 = normalize ( L2 + E );

	float diffuse_intensity = max ( dot ( N , L ) , 0.5 );

	//vec4 diffuse_final = diffuse_intensity * vec4 ( 0.1 , 0.8 , 0.1 , 1.0 );
	vec4 diffuse_final = diffuse_intensity * vec4 ( 0.6 , 0.6 , 0.6 , 1.0 );

	float spec_intensity = pow ( max ( dot ( N , H ) , 0.5 ) , 50 );
	float spec_intensity2 = pow ( max ( dot ( N , H2 ) , 0.1 ) , 50 );
	//vec4 spec_final = spec_intensity * vec4 ( 0.7 , 0.9 , 0.7 , 1.0 );

	vec4 spec_final = spec_intensity * vec4 ( 0.5 , 0.5 , 0.5 , 1.0 );
	vec4 spec_final2 = spec_intensity2 * vec4 ( 0.9 , 0.9 , 0.9 , 1.0 );

	fColor = diffuse_final + spec_final + spec_final2;



	//fColor = vec4 ( diffuse_intensity , diffuse_intensity , diffuse_intensity , 0.5 );
	/*if ( diffuse_intensity == 0 )
	{
		fColor = vec4 ( 0.05 , 0.05 , 0.05 , 1.0 );
	}
	else
	{
		fColor = vec4 ( diffuse_intensity , diffuse_intensity , diffuse_intensity , 0.5 );
	}*/
}