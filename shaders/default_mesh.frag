#version 120

uniform sampler2D u_mainTexture;
uniform int u_hasTexture;
uniform int numLights;
uniform vec3 lightPositions[8];
uniform vec3 lightColors[8];
uniform float lightIntensities[8];

varying vec4 vColor;
varying vec2 vTexCoord;
varying vec3 vNormal;
varying vec3 vWorldPos;

void main()
{
    vec4 base = vColor;
    if (u_hasTexture == 1)
    {
        base *= texture2D(u_mainTexture, vTexCoord);
    }

    vec3 n = normalize(vNormal);
    vec3 lit = base.rgb * 0.15;

    for (int i = 0; i < 8; ++i)
    {
        if (i >= numLights)
            break;

        vec3 L = lightPositions[i] - vWorldPos;
        float dist = length(L);
        if (dist > 0.0001)
        {
            L /= dist;
            float NdotL = max(dot(n, L), 0.0);
            float attenuation = 1.0 / (1.0 + 0.02 * dist * dist);
            lit += base.rgb * lightColors[i] * (lightIntensities[i] * NdotL * attenuation / 255.0);
        }
    }

    gl_FragColor = vec4(lit, base.a);
}
