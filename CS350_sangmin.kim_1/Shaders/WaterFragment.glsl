#version 400 core
out vec4 fragColor;

in vec4 clipSpace;
in vec2 textureCoords;
in vec3 toCamera;
in vec3 fromLightVector;

const float waveStrength = 0.02;
const float shineDamper = 20.0;
const float reflectivity = 0.6;

uniform sampler2D reflectTexture;
uniform sampler2D refractTexture;
uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform vec3 lightColor;

uniform float moveFactor;


void main(void)
{
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0 + 0.5;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

	float near = 0.1;
	float far = 1000;
	float depth = texture(depthMap, refractTexCoords).r;
	float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));

	depth = gl_FragCoord.z;
	float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
	float waterDepth = floorDistance - waterDistance;


	vec2 distortedTexcoords = texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 0.1;
	distortedTexcoords = textureCoords + vec2(distortedTexcoords.x, distortedTexcoords.y + moveFactor);
	vec2 totalDistortion = (texture(dudvMap, distortedTexcoords).rg * 2.0 - 1.0) * waveStrength;

	refractTexCoords += totalDistortion ;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
	reflectTexCoords += totalDistortion ;
	reflectTexCoords.x = clamp(reflectTexCoords.x , 0.001, 0.999);
	reflectTexCoords.y = clamp(reflectTexCoords.y , -0.999, -0.001);

	vec4 reflectColor = texture(reflectTexture, reflectTexCoords);
	vec4 refractColor = texture(refractTexture, refractTexCoords);

	vec4 normalMapColor = texture(normalMap, distortedTexcoords);
	vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b * 3.0, normalMapColor.g * 2.0 - 1.0);
	normal = normalize(normal);

	vec3 viewVec = normalize(toCamera);
	float refractiveFactor = dot(viewVec, normal);
	refractiveFactor = pow(refractiveFactor, 0.5);
	refractiveFactor = clamp(refractiveFactor, 0.001, 0.999);

	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, viewVec), 0.1);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = lightColor * specular * 0.5;


	fragColor = mix(reflectColor, refractColor, refractiveFactor);
	fragColor = mix(fragColor , vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specularHighlights, 0.0);
	fragColor.a = clamp(waterDepth / 2.0, 0.0, 1.0);
}

