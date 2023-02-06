#version 330 core

out vec4 color;
in vec3 col;

uniform vec3 lightdir;
in vec3 posEye;
in float pointRadius;
in vec3 posWorld;

void main()
{	
//    vec3 N;
//    N.xy = gl_PointCoord * 2.0 - vec2(1.0);
//    float mag = dot(N.xy,N.xy);
//    if(mag>1.0)
//		discard;
//    N.z = sqrt(1.0 - mag);
//    float diffuse = dot(N,lightdir);
//	if(diffuse < 0.0f)
//		diffuse = -diffuse;
//    color = vec4(col * diffuse, 1);

	 const float shininess = 40.0;

    // calculate normal from texture coordinates
    vec3 n;
    n.xy = gl_PointCoord * vec2(2.0, -2.0) + vec2(-1.0, 1.0);
    float mag = dot(n.xy, n.xy);
    if (mag > 1.0) 
        discard;   // kill pixels outside circle
    n.z = sqrt(1.0 - mag);

    // point on surface of sphere in eye space
    vec3 spherePosEye = posEye + n * pointRadius;

    // calculate lighting
    vec3 lightDir = normalize(vec3(0.0f, 10.0f, 0.0f) - posWorld);
    // directionalLighting

    float diffuse = max(0.0, dot(lightDir, n));
    vec3 v = normalize(-spherePosEye);
    vec3 h = normalize(lightDir + v);
    float specular = pow(max(0.0, dot(n, h)), shininess);

    //float dens = particleDensity/15000;

    vec3 tempColor = diffuse * col + specular;

    //color = vec4(pointLight.color * pointLight.intensity * diffuse + ambLight.color * ambLight.intensity, 1);
    color = vec4(tempColor + 0.2f, 1.0f);
    //color = vec4(col, 1);
}
