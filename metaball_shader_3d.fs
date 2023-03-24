#version 330 core
out vec3 FragColor;  
in vec2 ourColor;

uniform float aspectRatio;
uniform float time;

float radius = 0.3;


float smoothUnion( float d1, float d2, float k ) {
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h);
}

float balls(in vec3 p){
    return smoothUnion(
            smoothUnion(
                smoothUnion(
                length(vec3(p.x+cos(time), p.y+sin(time), p.z+cos(time)))-0.5,
                length(vec3(p.x-cos(time), p.y-sin(time), p.z-cos(time)))-0.5,
                0.3),length(vec3(p.x-cos(time), p.y+sin(time), p.z+cos(time)))-0.5,
            0.3),length(vec3(p.x+cos(time), p.y-sin(time), p.z+cos(time)))-0.5,
        0.3);
}

float scene(in vec3 p){
    vec3 c = vec3(5.0, 5.0, 7.0);
    vec3 q = mod(p+0.5*c,c)-0.5*c;
    return balls(q);
}

vec3 calcNormal( in vec3 p )
{
    const float h = 0.00001;
    const vec2 k = vec2(1,-1);
    return normalize( k.xyy*scene( p + k.xyy*h) + 
                      k.yyx*scene( p + k.yyx*h) + 
                      k.yxy*scene( p + k.yxy*h) + 
                      k.xxx*scene( p + k.xxx*h) );
}

void main()
{
    vec3 ro = vec3(0.0, 0.0, -2.0);
    vec3 rd = normalize(vec3(ourColor.x/aspectRatio, ourColor.y, -1)-ro);
    vec3 point = vec3(0.0, 0.0, 0.0);
    vec3 baseColor = vec3(1.0, 1.0, 1.0);
    float dist = 0.0;
    float t = 0;
    bool use = false;
    for(int i = 0; i < 30; i++){
        point = ro + rd*t;
        dist = scene(point);
        t += dist;
        if(dist < 0.005){
            use = true;
        }
        if(dist > 5.0){
            use = false;
        }
    }

    if(use){
        vec3 normal = calcNormal(ro+rd*t);
        FragColor = normal.xzy*dot(vec3(0.7, 0.5, -1.0), normal);
    }
    else{
        FragColor = vec3(0.0, 0.0, 0.0);
    }
}