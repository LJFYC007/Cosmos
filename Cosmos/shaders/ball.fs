#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
} fs_in;

uniform sampler2D normal;
uniform samplerCube cubemap;
uniform samplerCube cubemap2;
uniform sampler2D brdf;

uniform vec3 Albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normal, fs_in.TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fs_in.FragPos);
    vec3 Q2  = dFdy(fs_in.FragPos);
    vec2 st1 = dFdx(fs_in.TexCoords);
    vec2 st2 = dFdy(fs_in.TexCoords);

    vec3 N   = normalize(fs_in.Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec3 CalcPointLight(vec3 light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64);
    vec3 ambient = 0.05 * pow(Albedo, vec3(2.2));
    vec3 diffuse = 2.0 * diff * pow(Albedo, vec3(2.2));
    vec3 specular = 1.0 * spec * vec3(1.0f);
    return ambient + diffuse + specular;
}

void main()
{
    vec3 norm = getNormalFromMap(), viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 color = CalcPointLight(lightPos, norm, fs_in.FragPos, viewDir);
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  
    FragColor = vec4(color, 1.0);
}
