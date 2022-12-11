#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 Normal;
} fs_in;

const int NR_POINT_LIGHTS = 1;

struct Material 
{ 
    sampler2D diffuse, diffuse_night, specular, normal, cloud;
    float shininess;
};
uniform Material material;

struct PointLight 
{ 
    vec3 position, ambient, diffuse, specular; 
    float constant, linear, quadratic;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float cloudtrans;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    vec3 diffuse = (vec3(texture(material.diffuse, fs_in.TexCoords)))
                 + cloudtrans * (vec3(0.0) + vec3(texture(material.cloud, fs_in.TexCoords)));
    diffuse = diff * diffuse + (1 - diff) * light.ambient * (vec3(texture(material.diffuse_night, fs_in.TexCoords)));
    diffuse = diffuse * light.diffuse;

    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords).r);
    return diffuse + specular;
}

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(material.normal, fs_in.TexCoords).xyz * 2.0 - 1.0;
    //vec3 tangentNormal = vec3(0.0, 0.0, 1.0);

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

void main()
{
    vec3 norm = getNormalFromMap(), viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 result = CalcPointLight(pointLights[0], norm, fs_in.FragPos, viewDir);
    FragColor = vec4(result, 1.0);
}
