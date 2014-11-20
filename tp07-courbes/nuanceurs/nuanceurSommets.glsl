// source: http://www.cours.polymtl.ca/inf2705/nuanceurs/exempleIllumination/phong.glsl

uniform int localViewer;
uniform int utiliseTexture;

uniform float facteurZ;
uniform sampler2D displacementMap;

varying vec3 normal, lightDir, eyeVec;

float Fonc( float x, float y )
{
   float z;
   z = x*x - y*y;
   // z = x*y;
   // z =  y*sin(2.0*x) * x*cos(2.0*y);
   return (0.005*facteurZ) * z;
}

void main( void )
{
    float epsilon = 0.001;
    if (utiliseTexture == 0) {
        float x = gl_Vertex.x;
        float y = gl_Vertex.y;
        gl_Vertex.z = Fonc(x, y);
        normal.x = -(Fonc(x + epsilon, y) - Fonc(x - epsilon, y)) / (2.*epsilon);
        normal.y = -(Fonc(x, y + epsilon) - Fonc(x, y - epsilon)) / (2.*epsilon);
        normal.z = 1.;
        normal = normalize( normal );
        normal = gl_NormalMatrix * normal;
    }

   // transformation standard du sommet (ModelView et Projection)
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   gl_ClipVertex = gl_ModelViewMatrix * gl_Vertex;
   // (à partir de la version 1.30, c'est plutôt gl_ClipDistance[] qu'il faut utiliser)
   vec3 vsPos = vec3(gl_ModelViewMatrix * gl_Vertex);
   // gl_ClipDistance[0] = dot( u_plane0, vsPos );
   // gl_ClipDistance[1] = dot( u_plane1, vsPos );

   lightDir = vec3( gl_LightSource[0].position.xyz - vsPos );
   eyeVec = -vsPos;

   gl_FrontColor = gl_Color;
   gl_BackColor = 1.0 - gl_Color;
   gl_TexCoord[0] = gl_MultiTexCoord0;
}
