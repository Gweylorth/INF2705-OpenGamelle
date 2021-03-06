// les étudiants peuvent utiliser l'exemple du cours pour démarrer:
//    http://www.cours.polymtl.ca/inf2705/nuanceurs/exempleIllumination/phong.glsl

//#version 130
//out vec3 normal, lightDir, eyeVec;
varying vec3 normal, lightDir, eyeVec;

void main()
{
   // transformation standard du sommet (ModelView et Projection)
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   // calculer la normale qui sera interpolée pour le nuanceur de fragment
   normal = gl_NormalMatrix * gl_Normal;
   //normal = normalize( gl_NormalMatrix * gl_Normal );

   // calculer la position du sommet dans l'espace de la caméra ("eye-coordinate position")
   vec3 ecPosition = vec3( gl_ModelViewMatrix * gl_Vertex );

   // vecteur de la direction de la lumière
   lightDir = vec3( gl_LightSource[0].position.xyz - ecPosition );
   eyeVec = -ecPosition; // vecteur qui pointe vers le (0,0,0), c'est-à-dire vers l'oeil

   gl_FrontColor = gl_Color;
   gl_BackColor = 1.0 - gl_Color;
   gl_TexCoord[0] = gl_MultiTexCoord0;
}
