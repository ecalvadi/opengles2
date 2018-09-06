/*
 * Author:    Héctor Álvarez
 * Code adapted from Chapter 09: Simple Texture2D from the Book
 * OpenGL(R) ES 2.0 Programming Guide
 */

/*
 * 01_many_textures.c
 *
 *  This is a simple example that draws many textures on screen (1000 textures)
 *  with a resolution of 1280*720. The purpose of this is test the capability of 
 *  rendering many textures in the Raspberry Pi 1B.
 */

#include <stdlib.h>
#include "esUtil.h"

typedef struct
{
  // Handle to a program object
  GLuint programObject;

  // Attribute locations
  GLint  positionLoc;
  GLint  texCoordLoc;

  // Sampler location
  GLint samplerLoc;

  // Texture handle
  GLuint textureId;

} UserData;

/*
 * Create a multiple textures image with four different colors
 */
GLuint CreateSimpleTexture2D( )
{
  // Texture object handle
  GLuint textureId;
   
  // Many images, 3 bytes per pixel (R, G, B)
  GLubyte pixels[1000 * 3] = {};
	for(int i = 0; i < (1000 * 3); i++)
  {
    pixels[i] = rand() % 255;
  }
	

  // Use tightly packed data
  glPixelStorei ( GL_UNPACK_ALIGNMENT, 1 );

  // Generate a texture object
  glGenTextures ( 1, &textureId );

  // Bind the texture object
  glBindTexture ( GL_TEXTURE_2D, textureId );

  // Load the texture
  glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, 40, 25, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );

  // Set the filtering mode
  glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  return textureId;
}


/*
 * Initialize the shader and program object
 */
int Init ( ESContext *esContext )
{
  esContext->userData = malloc(sizeof(UserData));	
  UserData *userData = esContext->userData;
  GLbyte vShaderStr[] =  
    "attribute vec4 a_position;   \n"
    "attribute vec2 a_texCoord;   \n"
    "varying vec2 v_texCoord;     \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = a_position; \n"
    "   v_texCoord = a_texCoord;  \n"
    "}                            \n";
   
  GLbyte fShaderStr[] =  
    "precision mediump float;                            \n"
    "varying vec2 v_texCoord;                            \n"
    "uniform sampler2D s_texture;                        \n"
    "void main()                                         \n"
    "{                                                   \n"
    "  gl_FragColor = texture2D( s_texture, v_texCoord );\n"
    "}                                                   \n";

  // Load the shaders and get a linked program object
  userData->programObject = esLoadProgram ( vShaderStr, fShaderStr );

  // Get the attribute locations
  userData->positionLoc = glGetAttribLocation ( userData->programObject, "a_position" );
  userData->texCoordLoc = glGetAttribLocation ( userData->programObject, "a_texCoord" );
   
  // Get the sampler location
  userData->samplerLoc = glGetUniformLocation ( userData->programObject, "s_texture" );

  // Load the texture
  userData->textureId = CreateSimpleTexture2D ();

  glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
  
  return GL_TRUE;
}

/*
 * Update function
 */
void Update ( ESContext *esContext, float deltaTime)
{
  UserData *userData = (UserData*) esContext->userData;

  // Load the texture
  userData->textureId = CreateSimpleTexture2D ();
}

/*
 * Draw a triangle using the shader pair created in Init()
 */
void Draw ( ESContext *esContext )
{
  UserData *userData = esContext->userData;
  GLfloat vVertices[] = { -1.0f,  1.0f, 0.0f,  // Position 0
                           0.0f,  0.0f,        // TexCoord 0 
                          -1.0f, -1.0f, 0.0f,  // Position 1
                           0.0f,  2.0f,        // TexCoord 1
                           1.0f, -1.0f, 0.0f,  // Position 2
                           2.0f,  2.0f,        // TexCoord 2
                           1.0f,  1.0f, 0.0f,  // Position 3
                           2.0f,  0.0f         // TexCoord 3
                        };
  GLushort indices[] = { 0, 1, 2, 0, 2, 3 };
      
  // Set the viewport
  glViewport ( 0, 0, esContext->width, esContext->height );
   
  // Clear the color buffer
  glClear ( GL_COLOR_BUFFER_BIT );

  // Use the program object
  glUseProgram ( userData->programObject );

  // Load the vertex position
  glVertexAttribPointer ( userData->positionLoc, 3, GL_FLOAT, 
                           GL_FALSE, 5 * sizeof(GLfloat), vVertices );
  // Load the texture coordinate
  glVertexAttribPointer ( userData->texCoordLoc, 2, GL_FLOAT,
                           GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3] );

  glEnableVertexAttribArray ( userData->positionLoc );
  glEnableVertexAttribArray ( userData->texCoordLoc );

  // Bind the texture
  glActiveTexture ( GL_TEXTURE0 );
  glBindTexture ( GL_TEXTURE_2D, userData->textureId );

  // Set the sampler texture unit to 0
  glUniform1i ( userData->samplerLoc, 0 );

  glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );

}

/*
 * Cleanup
 */
void ShutDown ( ESContext *esContext )
{
  UserData *userData = esContext->userData;

  // Delete texture object
  glDeleteTextures ( 1, &userData->textureId );

  // Delete program object
  glDeleteProgram ( userData->programObject );
	
  free(esContext->userData);
}

int main ( int argc, char *argv[] )
{
  ESContext esContext;
  UserData  userData;

  esInitContext ( &esContext );
  esContext.userData = &userData;

  esCreateWindow ( &esContext, "Many Simple 2D Textures", 1280, 720, ES_WINDOW_RGB );

  if ( !Init ( &esContext ) )
    return 0;

  esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );

  esMainLoop ( &esContext );

  ShutDown ( &esContext );
}
