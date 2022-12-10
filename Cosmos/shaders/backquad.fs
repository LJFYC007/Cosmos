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

float QueryDistance(vec3 v)
{
	bool inx = -0.1 <= v.x && v.x <= 0.1;
	bool iny = -0.1 <= v.y && v.y <= 0.1;
	bool inz = -0.1 <= v.z && v.z <= 0.1;
	float dx = 0, dy = 0, dz = 0,ans=100;
	for(int i=0;i<=1;++i)
		for(int j=0;j<=1;++j)
			for (int k = 0;k <= 1;++k) {
                float x = i == 0 ? -0.1 : 0.1;
                float y = j == 0 ? -0.1 : 0.1;
                float z = k == 0 ? -0.1 : 0.1;
				if (!inx) dx = (v.x - x) * (v.x - x);
				if (!iny) dy = (v.y - y) * (v.y - y);
				if (!inz) dz = (v.z - z) * (v.z - z);
				ans = min(ans, dx + dy + dz);
			}
	return sqrt(ans); 
}

vec3 QueryDistanceVec(vec3 v)
{
	bool inx = -0.1 <= v.x && v.x <= 0.1;
	bool iny = -0.1 <= v.y && v.y <= 0.1;
	bool inz = -0.1 <= v.z && v.z <= 0.1;
	vec3 ans;
	float dx = 0, dy = 0, dz = 0, dis = 100;
	for (int i = 0;i <= 1;++i)
		for (int j = 0;j <= 1;++j)
			for (int k = 0;k <= 1;++k) {
                float x = i == 0 ? -0.1 : 0.1;
                float y = j == 0 ? -0.1 : 0.1;
                float z = k == 0 ? -0.1 : 0.1;
				if (!inx) dx = (v.x - x) * (v.x - x);
				if (!iny) dy = (v.y - y) * (v.y - y);
				if (!inz) dz = (v.z - z) * (v.z - z);
				if (dis > dx + dy + dz)dis = dx + dy + dz,ans=vec3(x,y,z);
			}
	if (inx)ans.x = v.x;
	if (iny)ans.y = v.y;
	if (inz)ans.z = v.z;
	return ans;
}

float Solve(vec3 pos)
{
    /*
    float r = 0.1f, lambda = 1 - sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z) / r;
    vec3 x = pos / (1 - lambda);
	return length(pos - x);
    */

    if ( abs(pos.x) <= 0.1f && abs(pos.y) <= 0.1f && abs(pos.z) <= 0.1f )
        return texture(map, pos * 5 + vec3(0.5f)).x;

    float len = QueryDistance(pos);
    vec3 x = QueryDistanceVec(pos);
    return texture(map, x * 5 + vec3(0.5f)).x + len;
}

float rayMarching(vec3 pos, vec3 dir)
{
    float maxLength = length(dir); 
    float res = min(1.0, Solve(pos + dir) / maxLength);
    // float res = min(1.0, asin(Solve(pos + dir) / maxLength));
    dir = normalize(dir);
    int cnt = 0;
    for ( float t = 0.00001; t < maxLength; )
    {
        float len = Solve(pos + t * dir); 
        // float len = length(texture(map, pos + t * dir + vec3(0.5f)).rgb); 
        // res = min(res, asin(len / t));
        res = min(res, len / t);
        if ( len < 0.000001 ) return 0.0f ; 
        t += len;
        ++ cnt;
        if ( cnt > 32 ) break ; 
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