#version 120

varying vec4 vColor;
varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vWorldPos;

void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vColor = gl_Color;
    vTexCoord = gl_MultiTexCoord0.xy;
    vNormal = normalize(gl_Normal);
    vWorldPos = gl_Vertex.xyz;
}
