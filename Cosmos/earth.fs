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
    sampler2D diffuse, specular, normal;
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
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.TexCoords));
    float dis = length(fs_in.TangentViewPos - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * (dis * dis));
    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    vec3 norm = texture(material.normal, fs_in.TexCoords).rgb, viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    norm = normalize(norm * 2.0 - 1.0);
    vec3 result = CalcPointLight(pointLights[0], norm, fs_in.FragPos, viewDir);
    FragColor = vec4(result, 1.0);
}
