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

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;
uniform vec3 lightPos;
uniform vec3 viewPos;

const float PI = 3.14159265359;

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

vec3 fresnelSchlick(float cosTheta, vec3 F0) { return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0); }  

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness, a2 = a * a;
    float NdotH  = max(dot(N, H), 0.0), NdotH2 = NdotH * NdotH;
    float nom = a2, denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0), k = (r * r) / 8.0;
    float nom = NdotV, denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0), NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness), ggx1  = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

void main()
{
    vec3 N = getNormalFromMap(), V = normalize(viewPos - fs_in.FragPos), R = reflect(-V, N);
    vec3 Lo = vec3(0.0), F0 = vec3(0.04); F0 = mix(F0, albedo, metallic);

    for ( int i = 0; i < 1; ++ i )
    {
        vec3 lightColor = vec3(100.0);
        vec3 L = normalize(lightPos - fs_in.FragPos), H = normalize(V + L);
        float distance = length(lightPos - fs_in.FragPos), attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColor * attenuation;

		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
 
        float NDF = DistributionGGX(N, H, roughness), G = GeometrySmith(N, V, L, roughness);      
        vec3 nominator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; 
		vec3 specular = nominator / denominator;  
 
		vec3 kS = F, kD = vec3(1.0) - kS; kD *= 1.0 - metallic;   
        float NdotL = max(dot(N, L), 0.0);        
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    vec3 irradiance = texture(cubemap, N).rgb;
    vec3 diffuse      = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(cubemap2, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdf, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

	vec3 color = ambient + Lo;  

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));  

    FragColor = vec4(color, 1.0);
}
