#include <gl/gl.h>

typedef GLboolean WINAPI type_glIsProgram(GLuint);
typedef void WINAPI type_glGetProgramiv(GLuint, GLenum, GLint *);
typedef void WINAPI type_glGetProgramInfoLog(GLuint, GLsizei, GLsizei *, GLchar *);
typedef GLboolean WINAPI type_glIsShader(GLuint);
typedef void WINAPI type_glGetShaderiv(GLuint, GLenum, GLint *);
typedef void WINAPI type_glGetShaderInfoLog(GLuint, GLsizei, GLsizei *, GLchar *);
typedef GLuint WINAPI type_glCreateShader(GLenum);
typedef void WINAPI type_glShaderSource(GLuint, GLsizei, const GLchar **, const GLint *);
typedef void WINAPI type_glCompileShader(GLuint);
typedef GLuint WINAPI type_glCreateProgram(void);
typedef void WINAPI type_glAttachShader(GLuint, GLuint);
typedef void WINAPI type_glLinkProgram(GLuint);
typedef void WINAPI type_glGenVertexArrays(GLsizei, GLuint *);
typedef void WINAPI type_glGenBuffers(GLsizei, GLuint *);
typedef void WINAPI type_glBindVertexArray(GLuint);
typedef void WINAPI type_glBindBuffer(GLenum, GLuint);
typedef void WINAPI type_glBufferData(GLenum, GLsizeiptr, const GLvoid *, GLenum);
typedef void WINAPI type_glVertexAttribPointer(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);
typedef void WINAPI type_glEnableVertexAttribArray(GLuint);
typedef void WINAPI type_glGenerateMipmap(GLenum);
typedef void WINAPI type_glUseProgram(GLuint);
typedef void WINAPI type_glDeleteProgram(GLuint);

#define declare_glfuncptr(name) type_##name *name = nullptr
declare_glfuncptr(glIsProgram);
declare_glfuncptr(glGetProgramiv);
declare_glfuncptr(glGetProgramInfoLog);
declare_glfuncptr(glIsShader);
declare_glfuncptr(glGetShaderiv);
declare_glfuncptr(glGetShaderInfoLog);
declare_glfuncptr(glCreateShader);
declare_glfuncptr(glShaderSource);
declare_glfuncptr(glCompileShader);
declare_glfuncptr(glCreateProgram);
declare_glfuncptr(glAttachShader);
declare_glfuncptr(glLinkProgram);
declare_glfuncptr(glGenVertexArrays);
declare_glfuncptr(glGenBuffers);
declare_glfuncptr(glBindVertexArray);
declare_glfuncptr(glBindBuffer);
declare_glfuncptr(glBufferData);
declare_glfuncptr(glVertexAttribPointer);
declare_glfuncptr(glEnableVertexAttribArray);
declare_glfuncptr(glGenerateMipmap);
declare_glfuncptr(glUseProgram);
declare_glfuncptr(glDeleteProgram);

#define get_glfuncptr(name) name = (type_##name *)wglGetProcAddress(#name)
void load_opengl_extensions() {
    get_glfuncptr(glIsProgram);
    get_glfuncptr(glGetProgramiv);
    get_glfuncptr(glGetProgramInfoLog);
    get_glfuncptr(glIsShader);
    get_glfuncptr(glGetShaderiv);
    get_glfuncptr(glGetShaderInfoLog);
    get_glfuncptr(glCreateShader);
    get_glfuncptr(glShaderSource);
    get_glfuncptr(glCompileShader);
    get_glfuncptr(glCreateProgram);
    get_glfuncptr(glAttachShader);
    get_glfuncptr(glLinkProgram);
    get_glfuncptr(glGenVertexArrays);
    get_glfuncptr(glGenBuffers);
    get_glfuncptr(glBindVertexArray);
    get_glfuncptr(glBindBuffer);
    get_glfuncptr(glBufferData);
    get_glfuncptr(glVertexAttribPointer);
    get_glfuncptr(glEnableVertexAttribArray);
    get_glfuncptr(glGenerateMipmap);
    get_glfuncptr(glUseProgram);
    get_glfuncptr(glDeleteProgram);
}