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
    sampler2D diffuse;
};
uniform Material material;

struct PointLight 
{ 
    vec3 position, ambient, diffuse;
    float constant, linear, quadratic;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    float dis = length(lightPos - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * (dis * dis));
    return (ambient + diffuse) * attenuation;
}

void main()
{
    vec3 norm = normalize(fs_in.Normal), viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 result = CalcPointLight(pointLights[0], norm, fs_in.FragPos, viewDir);
    FragColor = vec4(result.x, result.x, result.x, 1.0);
}
