// les étudiants peuvent utiliser l'exemple du cours pour démarrer:
//    http://www.cours.polymtl.ca/inf2705/nuanceurs/exempleSimple/

#define M_PI_2  (1.57079632679489661923)  // PI/2
uniform float facteurRechauffement;
varying vec3 vecN;

void main( void )
{
   // transformation standard du sommet (ModelView et Projection)
   gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
   //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   gl_FrontColor = gl_Color;
   gl_BackColor = 1.0 - gl_Color;
   vecN = normalize(gl_Normal);

}
