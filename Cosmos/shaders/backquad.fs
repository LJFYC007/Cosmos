#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

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
    float maxLength = length(dir); 
    float res = min(1.0, asin(Solve(pos + dir) / maxLength));
    dir = normalize(dir);
    for ( float t = 0.01; t < maxLength; )
    {
        //float len = Solve(pos + t * dir); 
        float len = texture(map, pos + t * dir + vec3(0.5f)).x; 
        res = min(res, asin(len / t));
        if ( len < 0.0001 ) break ; 
        t += len;
    }
    return res;
    
    res = clamp(res, 0.0, 1.0);
    return res * res * (3.0 - 2.0 * res);
}

void main()
{		
    vec3 envColor = texture(diffuse, TexCoords).rgb;

    envColor = rayMarching(FragPos, lightPos - FragPos) * envColor;
    
    // HDR tonemap and gamma correct
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 

    FragColor = vec4(envColor, 1.0);
}