#version 330 core
out vec3 FragColor;  
in vec2 ourColor;

uniform float aspectRatio;

void main()
{
    vec3 ro = vec3(0.0, 0.0, -1.0);
    vec3 rd = normalize(vec3(ourColor.x/aspectRatio, ourColor.y, 0.2)-ro);
    vec3 point = vec3(0.0, 0.0, 0.0);
    float dist = 0.0;
    float t = 0;
    bool use = false;
    for(int i = 0; i < 100; i++){
        point = ro + rd*t;
        dist = length(point)-0.5;
        t += dist;
        if(dist < 0.05){
            use = true;
        }
        if(dist > 10.0){
            use = false;
        }
    }
    if(use){
        FragColor = vec3(1.0, 1.0, 1.0);
    }
    else{
        FragColor = vec3(0.0, 0.0, 0.0);
    }
}