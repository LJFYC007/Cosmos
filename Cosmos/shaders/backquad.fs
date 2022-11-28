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

float Solve(vec3 pos)
{
    float r = 0.1f, lambda = 1 - sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z) / r;
    vec3 x = pos / (1 - lambda);
    return length(pos - x);
}

float rayMarching(vec3 pos, vec3 dir)
{
    float res = 1.0, maxLength = length(dir); dir = normalize(dir);
    for ( float t = 0; t < maxLength; )
    {
        float len = Solve(pos + t * dir); //texture(map, pos + t * dir + vec3(0.5f)).x; 
        //res = min(res, 2 * len / t);
        res = min(res, atan(len, maxLength - t));
        /*
        float ph = 1e20;
        float x = len * len / (2.0 * ph);
        float d = sqrt(len * len - x * x);
        res = min(res, 2 * d / max(0.0, t - x));
        */

        if ( len < 0.001f ) 
            return res;

        t += len;
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