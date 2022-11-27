#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 Pos;

uniform sampler2D diffuse;
uniform sampler3D map;
uniform vec3 lightPos;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = vec3(0.0, 0.0, 1.0);

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N = normalize(Normal);
    vec3 T = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float rayMarching(vec3 pos, vec3 dir)
{
    return texture(map, Pos + vec3(0.5f)).x;
    float res = 1.0, maxLength = 0.9 * length(dir); 
    int cnt = 0;
    for ( float t = 0; t < maxLength; )
    {
        float len = texture(map, pos + t * dir).x; 
        if ( len < 0.001f ) 
            return 0.0f;
        // res = min(res, 2 * len / t);
        t += len;
        ++ cnt; if ( cnt > 20 ) break ; 
    }
    return res;
}

void main()
{		
    vec3 envColor = texture(diffuse, TexCoords).rgb;

    envColor = rayMarching(lightPos, Pos - lightPos) * envColor;
    
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 

    FragColor = vec4(envColor, 1.0);
}