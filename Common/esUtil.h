/*
 * Book:      OpenGL(R) ES 2.0 Programming Guide
 * Authors:   Aaftab Munshi, dan Ginsburg, Dave Shreiner
 * ISBN-10:   0321502795
 * ISBN-13:   9780321502797
 * Publisher: Addison-Wesley Professional
 * URLs:      http://safari.informit.com/9780321563835
 *            http://www.opengles-book.com
 */

/*
 * ESUtil.h
 *
 *  A utility library for OpenGL ES. This librarby provides a
 *  basic common framework for the example applications in the
 *  OpenGL ES 2.0 Programming Guide.
 */
#ifndef ESUTIL_H
#define ESUTIL_H

//Includes
#include <GLES2/gl2.h>
#include <EGL/egl.h>

#ifdef __cplusplus

extern "C" {
#endif

//Macros
#define ESUTIL_API
#define ESCALLBACK


// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0
// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1 
// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2 
// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4
// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8

//Types
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

typedef struct
{
  GLfloat   m[4][4];
} ESMatrix;

typedef struct _escontext
{
  // Put your user data here...
  void*       userData;

  // Window width
  GLint       width;

  // Window height
  GLint       height;

  // Window handle
  EGLNativeWindowType  hWnd;

  // EGL display
  EGLDisplay  eglDisplay;
      
  // EGL context
  EGLContext  eglContext;

  // EGL surface
  EGLSurface  eglSurface;

  // Callbacks
  void (ESCALLBACK *drawFunc) ( struct _escontext * );
  void (ESCALLBACK *keyFunc) ( struct _escontext *, unsigned char, int, int );
  void (ESCALLBACK *updateFunc) ( struct _escontext *, float deltaTime );
} ESContext;


/*
 * Public Functions
 */

/*
 * Brief:     Initialize ES framework context. This must be called before calling any other functions.
 * Param:     esContext Application context
 */
void ESUTIL_API esInitContext ( ESContext *esContext );

/*
 * Brief:     Create a window with the specified parameters
 * Param:     esContext Application context
 * Param:     title Name for title bar of window
 * Param:     width Width in pixels of window to create
 * Param:     height Height in pixels of window to create
 * Param:     flags Bitfield for the window creation flags
 *              ES_WINDOW_RGB         - specifies that the color buffer should have R,G,B channels
 *              ES_WINDOW_ALPHA       - specifies that the ccolor buffer should have alpha
 *              ES_WINDOW_DEPTH       - specifies that a depth buffer should be created
 *              ES_WINDOW_STENCIL     - specifies that a stencil buffer should be created
 *              ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
 * Return:    GL_TRUE if window creatiopn is succesful, GL_FALSE otherwise
 */
GLboolean ESUTIL_API esCreateWindow ( ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags );

/*
 * Brief:     Start the main loop for the OpenGL ES application
 * Param:     esContext Application context
 */
void ESUTIL_API esMainLoop ( ESContext *esContext );

/*
 * Brief:     Register a draw callback function to be used to render each frame
 * Param:     esContext Application context
 * Param:     drawFunc Draw callback function that will be used to render the scene
 */
void ESUTIL_API esRegisterDrawFunc ( ESContext *esContext, void (ESCALLBACK *drawFunc) ( ESContext* ) );

/*
 * Brief:     Register an update callback function to be used to update on each time step
 * Param:     esContext Application context
 * Param:     updateFunc Update callback function that will be used to render the scene
 */
void ESUTIL_API esRegisterUpdateFunc ( ESContext *esContext, void (ESCALLBACK *updateFunc) ( ESContext*, float ) );

/*
 * Brief:     Register an keyboard input processing callback function
 * Param:     esContext Application context
 * Param:     keyFunc Key callback function for application processing of keyboard input
 */
void ESUTIL_API esRegisterKeyFunc ( ESContext *esContext, 
                                    void (ESCALLBACK *drawFunc) ( ESContext*, unsigned char, int, int ) );
/*
 * Brief:     Log a message to the debug output for the platform
 * Param:     formatStr Format string for error log.  
 */
void ESUTIL_API esLogMessage ( const char *formatStr, ... );

/*
 * Brief:     Load a shader, check for compile errors, print error messages to output log
 * Param:     type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
 * Param:     shaderSrc Shader source string
 * Return:    A new shader object on success, 0 on failure
 */
GLuint ESUTIL_API esLoadShader ( GLenum type, const char *shaderSrc );

/*
 * Brief:     Load a vertex and fragment shader, create a program object, link program.
 *            Errors output to log.
 * Param:     vertShaderSrc Vertex shader source code
 * Param:     fragShaderSrc Fragment shader source code
 * Return:    A new program object linked with the vertex/fragment shader pair, 0 on failure
 */
GLuint ESUTIL_API esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc );

/*
 * Brief:     Generates geometry for a sphere.  Allocates memory for the vertex data and stores 
 *            the results in the arrays.  Generate index list for a TRIANGLE_STRIP
 * Param:     numSlices The number of slices in the sphere
 * Param:     vertices If not NULL, will contain array of float3 positions
 * Param:     normals If not NULL, will contain array of float3 normals
 * Param:     texCoords If not NULL, will contain array of float2 texCoords
 * param:     indices If not NULL, will contain the array of indices for the triangle strip
 * Return:    The number of indices required for rendering the buffers (the number of indices stored in the indices array
 *            if it is not NULL ) as a GL_TRIANGLE_STRIP
 */
int ESUTIL_API esGenSphere ( int numSlices, float radius, GLfloat **vertices, GLfloat **normals, 
                             GLfloat **texCoords, GLuint **indices );

/*
 * Brief:     Generates geometry for a cube.  Allocates memory for the vertex data and stores 
 *            the results in the arrays.  Generate index list for a TRIANGLES
 * Param:     scale The size of the cube, use 1.0 for a unit cube.
 * Param:     vertices If not NULL, will contain array of float3 positions
 * Param:     normals If not NULL, will contain array of float3 normals
 * Param:     texCoords If not NULL, will contain array of float2 texCoords
 * Param:     indices If not NULL, will contain the array of indices for the triangle strip
 * Return:    The number of indices required for rendering the buffers (the number of indices stored in the indices array
 *            if it is not NULL ) as a GL_TRIANGLES
 */
int ESUTIL_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals, 
                           GLfloat **texCoords, GLubyte **indices );

/*
 * Brief:     Loads a 24-bit TGA image from a file
 * Param:     fileName Name of the file on disk
 * Param:     width Width of loaded image in pixels
 * Param:     height Height of loaded image in pixels
 * Return:    Pointer to loaded image.  NULL on failure. 
 */
char* ESUTIL_API esLoadTGA ( char *fileName, int *width, int *height );

/*
 * Brief:     multiply matrix specified by result with a scaling matrix and return new matrix in result
 * Param:     result Specifies the input matrix.  Scaled matrix is returned in result.
 * Param:     sx, sy, sz Scale factors along the x, y and z axes respectively
 */
void ESUTIL_API esScale(ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz);

/*
 * Brief:     multiply matrix specified by result with a translation matrix and return new matrix in result
 * Param:     result Specifies the input matrix.  Translated matrix is returned in result.
 * Param:     tx, ty, tz Scale factors along the x, y and z axes respectively
 */
void ESUTIL_API esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz);

/*
 * Brief:     multiply matrix specified by result with a rotation matrix and return new matrix in result
 * Param:     result Specifies the input matrix.  Rotated matrix is returned in result.
 * Param:     angle Specifies the angle of rotation, in degrees.
 * Param:     x, y, z Specify the x, y and z coordinates of a vector, respectively
 */
void ESUTIL_API esRotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

/*
 * Brief:     multiply matrix specified by result with a perspective matrix and return new matrix in result
 * Param:     result Specifies the input matrix.  new matrix is returned in result.
 * Param:     left, right Coordinates for the left and right vertical clipping planes
 * Param:     bottom, top Coordinates for the bottom and top horizontal clipping planes
 * Param:     nearZ, farZ Distances to the near and far depth clipping planes.  Both distances must be positive.
 */
void ESUTIL_API esFrustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);

/*
 * Brief:     multiply matrix specified by result with a perspective matrix and return new matrix in result
 * Param:     result Specifies the input matrix.  new matrix is returned in result.
 * Param:     fovy Field of view y angle in degrees
 * Param:     aspect Aspect ratio of screen
 * Param:     nearZ Near plane distance
 * Param:     farZ Far plane distance
 */
void ESUTIL_API esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ);

/*
 * Brief:     multiply matrix specified by result with a perspective matrix and return new matrix in result
 * Param:     result Specifies the input matrix.  new matrix is returned in result.
 * Param:     left, right Coordinates for the left and right vertical clipping planes
 * Param:     bottom, top Coordinates for the bottom and top horizontal clipping planes
 * Param:     nearZ, farZ Distances to the near and far depth clipping planes.  These values are negative if plane is behind the viewer
 */
void ESUTIL_API esOrtho(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);

/*
 * Brief:     perform the following operation - result matrix = srcA matrix * srcB matrix
 * Param:     result Returns multiplied matrix
 * Param:     srcA, srcB Input matrices to be multiplied
 */
void ESUTIL_API esMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB);

/*
 * Brief:     return an indentity matrix 
 * Param:     result returns identity matrix
 */
void ESUTIL_API esMatrixLoadIdentity(ESMatrix *result);

#ifdef __cplusplus
}
#endif

#endif // ESUTIL_H
