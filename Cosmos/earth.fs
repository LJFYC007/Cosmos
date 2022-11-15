#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

const int NR_POINT_LIGHTS = 1;

struct Material 
{ 
    sampler2D diffuse, diffuse_night, specular, normal;
    float shininess;
};
uniform Material material;

struct PointLight 
{ 
    vec3 position, ambient, diffuse, specular; 
    float constant, linear, quadratic;
};
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(fs_in.TangentLightPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 diffuse = light.diffuse * diff * (vec3(texture(material.diffuse, fs_in.TexCoords)) + vec3(54.0, 57.0, 81.0) / 255.0);
    diffuse += (1 - diff) * light.ambient * (vec3(texture(material.diffuse_night, fs_in.TexCoords)));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords).r);
    return diffuse + specular;
}

void main()
{
    vec3 norm = texture(material.normal, fs_in.TexCoords).rgb, viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    norm = normalize(norm * 2.0 - 1.0);
    vec3 result = CalcPointLight(pointLights[0], norm, fs_in.FragPos, viewDir);
    FragColor = vec4(result, 1.0);
}
