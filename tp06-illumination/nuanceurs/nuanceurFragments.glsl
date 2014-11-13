// les étudiants peuvent utiliser l'exemple du cours pour démarrer:
//    http://www.cours.polymtl.ca/inf2705/nuanceurs/exempleIllumination/phong.glsl

// Les paramètres de la source de lumière sont définis ainsi:
// struct gl_LightSourceParameters
// {
//    vec4 ambient;
//    vec4 diffuse;
//    vec4 specular;
//    vec4 position;
//    vec4 halfVector;
//    vec3 spotDirection;
//    float spotExponent;
//    float spotCutoff;          // ( [0.0,90.0], 180.0 )
//    float spotCosCutoff;       // == cos(spotCutoff) ( [1.0,0.0], -1.0 )
//    float constantAttenuation;
//    float linearAttenuation;
//    float quadraticAttenuation;
// };
// uniform gl_LightSourceParameters gl_LightSource[gl_MaxLights];
// bool gl_FrontFacing  // on est en train de tracer la face avant?


//#version 130
//in vec3 normal, lightDir, eyeVec;
varying vec3 normal, lightDir, eyeVec;
uniform bool utiliseBlinn, utiliseDirect;

uniform int utiliseCouleur, texnumero, noirTransparent;
uniform sampler2D laTexture;

void main (void)
{
   vec4 couleur = gl_FrontLightModelProduct.sceneColor +
                  gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

   if (texnumero > 0) {
       couleur += texture2D(laTexture, gl_TexCoord[0].st);
   }

   vec3 N = normalize( normal );
   vec3 L = normalize( lightDir );
   vec3 D = normalize(gl_LightSource[0].spotDirection);

   float NdotL = dot(N, L);
   float LdotD = dot( -L, D );

   // For Direct3D mode
   float cos_cur_angle = dot(-L, D);
   float cos_outer_cone_angle = pow(gl_LightSource[0].spotCosCutoff, gl_LightSource[0].spotExponent + 1.1);
   float cos_inner_cone_angle = gl_LightSource[0].spotCosCutoff;
   float cos_inner_minus_outer_angle = cos_inner_cone_angle - cos_outer_cone_angle;
   float spot = 0.0;
   spot = clamp((cos_cur_angle - cos_outer_cone_angle) /
          cos_inner_minus_outer_angle, 0.0, 1.0);

   if ( LdotD > gl_LightSource[0].spotCosCutoff || utiliseDirect )
   {
        float lambertTerm = max( NdotL, 0.0);
        couleur += gl_LightSource[0].diffuse *
        gl_FrontMaterial.diffuse *
        lambertTerm *
        ( utiliseDirect ? spot : 1. );

       float specular;

       if (utiliseBlinn) {
           vec3 halfAngle = normalize(lightDir + eyeVec);
           float blinnTerm = dot(N, halfAngle);
           blinnTerm = clamp(blinnTerm, 0., 1.);
           blinnTerm = NdotL != 0.0 ? blinnTerm : 0.0;
           blinnTerm = pow(blinnTerm, gl_FrontMaterial.shininess);
           specular = blinnTerm;
       } else {
           vec3 E = normalize(eyeVec);
           vec3 R = reflect(-L, N);
           specular = pow( max(dot(R, E), 0.0),
           gl_FrontMaterial.shininess );
       }
       couleur += gl_LightSource[0].specular *
               gl_FrontMaterial.specular *
               specular *
               ( utiliseDirect ? spot : 1. );
   }

   gl_FragColor = clamp( couleur, 0.0, 1.0 );
}


