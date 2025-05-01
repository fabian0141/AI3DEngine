#version 330 core
in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 Position_worldspace;

uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform float specularExponent;
uniform vec3 camPosition;
uniform float dissolve;
out vec4 color;

void main(){
    vec3 n = normalize( Normal_cameraspace );
    vec3 l = normalize( LightDirection_cameraspace );

    float distance = distance(LightDirection_cameraspace, Position_worldspace);

    float cosTheta = clamp( dot( n,l ), 0,1 );

    vec3 lightDir = normalize(vec3(150.0, 200.0, 100.0) - Position_worldspace);
    float diff = max(dot(n, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor;

    vec3 viewDir = normalize(camPosition - Position_worldspace);
    vec3 reflectDir = reflect(-lightDir, n);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularExponent);
    vec3 specular = spec * specularColor;  

    color = vec4(vec3(0.05) + diffuse + specular, dissolve); //* 70000 * cosTheta / (distance*distance);
}