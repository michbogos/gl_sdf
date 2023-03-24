#version 330 core
out vec3 FragColor;  
in vec2 ourColor;

uniform float time;
//uniform float speed;
uniform float s1;
uniform float s2;
uniform float s3;
uniform float s4;
uniform float distortion;

// complex number operations
vec2 cadd( vec2 a, float s ) { return vec2( a.x+s, a.y ); }
vec2 cmul( vec2 a, vec2 b )  { return vec2( a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x ); }
vec2 cdiv( vec2 a, vec2 b )  { float d = dot(b,b); return vec2( dot(a,b), a.y*b.x - a.x*b.y ) / d; }
vec2 csqrt( vec2 z ) { float m = length(z); return sqrt( 0.5*vec2(m+z.x, m-z.x) ) * vec2( 1.0, sign(z.y) ); }
vec2 conj( vec2 z ) { return vec2(z.x,-z.y); }
vec2 cpow( vec2 z, float n ) { float r = length( z ); float a = atan( z.y, z.x ); return pow( r, n )*vec2( cos(a*n), sin(a*n) ); }

float calc( vec2 p, float time )
{
    // non p dependent
	float ltime = 0.5-0.5*cos(time*0.06);
    float zoom = pow( 0.9, 50.0*ltime );
	vec2  cen = vec2( 0.2655,0.301 ) + zoom*0.8*cos(4.0+2.0*ltime);
    
	vec2 c = vec2( -0.745, 0.186 ) - 0.045*zoom*(1.0-ltime*0.5);
	vec2 z = cen + (p-cen)*zoom;
   
#if 0
    // full derivatives version
	vec2 dz = vec2( 1.0, 0.0 );
	for( int i=0; i<256; i++ )
	{
		dz = 2.0*vec2(z.x*dz.x-z.y*dz.y, z.x*dz.y + z.y*dz.x );
        z = vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y ) + c;
		if( dot(z,z)>200.0 ) break;
	}
	float d = sqrt( dot(z,z)/dot(dz,dz) )*log(dot(z,z));

#else
    // only derivative length version
    float ld2 = 1.0;
    float lz2 = dot(z,z);
    for( int i=0; i<256; i++ )
	{
        ld2 *= 4.0*lz2;
        z = vec2( z.x*z.x - z.y*z.y, 2.0*z.x*z.y ) + c;
        lz2 = dot(z,z);
		if( lz2>200.0 ) break;
	}
    float d = sqrt(lz2/ld2)*log(lz2);

#endif
    
	return sqrt( clamp( (150.0/zoom)*d, 0.0, 1.0 ) );
}

float sdCircle( vec2 p, float r )
{
    return length(p) - r;
}


float opSmoothUnion( float d1, float d2, float k ) {
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h);
}

float sdf(in vec2 point){
    return opSmoothUnion(
            sdCircle(point + vec2(cos(time*0.9),-sin(time*1.5))*0.5, s1),
            opSmoothUnion(
            sdCircle(point + vec2(-cos(time*1.2),-sin(time*1.5))*0.5, s2),
            opSmoothUnion(
                sdCircle(point + vec2(-cos(time*0.4),sin(time*1.3))*0.5, s3),
                sdCircle(point + vec2(cos(time*0.7),sin(time*1.5))*0.5, s4), 0.1)
                , 0.3)
            , 0.3);
}

float displacement(in vec2 point){
    return 0.1*cos(distortion*point.x)*sin(distortion*point.y);
}

void main()
{
    if(opSmoothUnion(sdf(ourColor.xy)+displacement(ourColor.xy), calc(ourColor.xy, 0), 0.1) > 0.0){
        FragColor = vec3(0.0, 0.0, 0.0);
    }
    else{
        FragColor = vec3(ourColor*ourColor, 1.0);
    }
}