// source: http://www.cours.polymtl.ca/inf2705/nuanceurs/exempleIllumination/phong.glsl

uniform sampler2D colorMap;
uniform int utiliseTexture;

varying vec3 normal, lightDir, eyeVec;

void main( void )
{
    vec4 couleur = gl_FrontLightModelProduct.sceneColor +
                   gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

    if (utiliseTexture != 0) {
        couleur += texture2D(colorMap, gl_TexCoord[0].st);
    }

    vec3 N = normal;
    vec3 L = normalize(lightDir);

    float NdotL = dot(N, L);

    float lambertTerm = max( NdotL, 0.0);
    couleur += gl_LightSource[0].diffuse *
                gl_FrontMaterial.diffuse *
                lambertTerm;

    vec3 halfAngle = normalize(lightDir + eyeVec);
    float blinnTerm = dot(N, halfAngle);
    blinnTerm = clamp(blinnTerm, 0., 1.);
    blinnTerm = NdotL != 0.0 ? blinnTerm : 0.0;
    blinnTerm = pow(blinnTerm, gl_FrontMaterial.shininess);

    couleur += gl_LightSource[0].specular *
               gl_FrontMaterial.specular *
                blinnTerm;

    gl_FragColor = clamp(couleur, 0., 1.);
}
