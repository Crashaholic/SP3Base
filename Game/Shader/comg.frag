#version 330 core

// Interpolated values from the vertex shaders
in vec3 vertexPosition_cameraspace;
in vec3 fragmentColor;
in vec3 vertexNormal_cameraspace;
in vec2 texCoord;
// Ouput data
out vec4 color;

struct Light {
	int type;
	vec3 position_cameraspace;
	vec3 color;
	float power;
	float kC;
	float kL;
	float kQ;
	vec3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;
};

struct Material {
	vec3 kAmbient;
	vec3 kDiffuse;
	vec3 kSpecular;
	float kShininess;
};

float getAttenuation(Light light, float distance) {
	if(light.type == 1)
		return 1;
	else
		return 1 / max(1, light.kC + light.kL * distance + light.kQ * distance * distance);
}

float getSpotlightEffect(Light light, vec3 lightDirection) {
	vec3 S = normalize(light.spotDirection);
	vec3 L = normalize(lightDirection);
	float cosDirection = dot(L, S);
	//return smoothstep(light.cosCutoff, light.cosInner, cosDirection);
	if(cosDirection < light.cosCutoff)
		return 0;
	else
		return 1; //pow(cosDirection, light.exponent);
}

// Constant values
const int MAX_LIGHTS = 8;
const int MAX_TEXTURES = 8;

// Values that stay constant for the whole mesh.
uniform bool lightEnabled;
uniform Light lights[MAX_LIGHTS];
uniform Material material;
uniform int numLights;
uniform bool colorTextureEnabled[MAX_TEXTURES];
uniform sampler2D colorTexture[MAX_TEXTURES];
uniform bool textEnabled;
uniform vec3 textColor;
//New Code
uniform float transparency;
uniform vec3 coloredTexture[MAX_TEXTURES];
uniform bool colorableTexture[MAX_TEXTURES];

void main()
{
	if(lightEnabled == true)
	{
		// Material properties
		vec4 materialColor;
		int texCount = 0;
		
		for (int i = 0; i < MAX_TEXTURES; ++i)
		{
			if(colorTextureEnabled[i] == true)
			{
				materialColor.rgb += texture2D( colorTexture[i], texCoord ).rgb;
				if(colorableTexture[i] == true)
				{
					if(texture2D( colorTexture[i], texCoord ).a != 0)
						materialColor.rgb *= coloredTexture[i];
				}
				else
				{
					materialColor.rgb *= vec4(1,1,1,1).rgb;
				}
				++texCount;
			}		
		}
		if (texCount > 0)
		{
			materialColor = materialColor/texCount;
		}
		else
			materialColor = vec4( fragmentColor, 1 );
		

		// Vectors
		vec3 eyeDirection_cameraspace = - vertexPosition_cameraspace;
		vec3 E = normalize(eyeDirection_cameraspace);
		vec3 N = normalize( vertexNormal_cameraspace );
		
		// Ambient : simulates indirect lighting
		color = materialColor * vec4(material.kAmbient, 1);
		//color.rgb *= coloredTexture.rgb;//M: multiplies color by coloredTexture to get color blending;

		for(int i = 0; i < numLights; ++i)
		{
			// Light direction
			float spotlightEffect = 1;
			vec3 lightDirection_cameraspace;
			if(lights[i].type == 1) {
				lightDirection_cameraspace = lights[i].position_cameraspace;
			}
			else if(lights[i].type == 2) {
				lightDirection_cameraspace = lights[i].position_cameraspace - vertexPosition_cameraspace;
				spotlightEffect = getSpotlightEffect(lights[i], lightDirection_cameraspace);
			}
			else {
				lightDirection_cameraspace = lights[i].position_cameraspace - vertexPosition_cameraspace;
			}
			// Distance to the light
			float distance = length( lightDirection_cameraspace );
			
			// Light attenuation
			float attenuationFactor = getAttenuation(lights[i], distance);

			vec3 L = normalize( lightDirection_cameraspace );
			float cosTheta = clamp( dot( N, L ), 0, 1 );
			
			vec3 R = reflect(-L, N);
			float cosAlpha = clamp( dot( E, R ), 0, 1 );
			
			color +=
				// Diffuse : "color" of the object
				materialColor * vec4(material.kDiffuse, 1) * vec4(lights[i].color, 1) * lights[i].power * cosTheta * attenuationFactor * spotlightEffect +
				
				// Specular : reflective highlight, like a mirror
				vec4(material.kSpecular, 1) * vec4(lights[i].color, 1) * lights[i].power * pow(cosAlpha, material.kShininess) * attenuationFactor * spotlightEffect;
		}
	}
	else
	{
		if(colorTextureEnabled[0] == true && textEnabled == true)
		{
			color = texture2D(colorTexture[0], texCoord) * vec4(textColor,1);
		}
		else
		{
			int texCount = 0;

			color = vec4(0,0,0,0);
			for(int i = 0;i<MAX_TEXTURES;++i)
			{
				if(colorTextureEnabled[i] == true)
				{
					if(texture2D( colorTexture[i], texCoord ).a > 0.99)
					{
						color.rgb = texture2D( colorTexture[i], texCoord ).rgb*coloredTexture[i];
						//color.a = 1.0;
						color.a = texture2D( colorTexture[i], texCoord ).a;
					}
					if(color.a<=0.99)
						discard;
					//else
					//	color.rgb*=coloredTexture[i];
					++texCount;
				}
			}
			if(texCount==0)
				color = vec4(fragmentColor,1);
		}
		//if(color.a<0.9)
			//discard;
	}
//		if(colorTextureEnabled[0] == true )
//		{
//			if(textEnabled == true)
//				color = texture2D( colorTexture[0], texCoord ) * vec4( textColor, 1 );
//			else
//				color = texture2D( colorTexture[0], texCoord );
//			//color.rgb *= coloredTexture;//M: multiplies color by coloredTexture to get color blending;
//		}
//		else
//			color = vec4( fragmentColor, 1 );
//	}
	if(color.a<=0.1)
		discard;
	color.a *= transparency;//M: multiply alpha by transparency
}