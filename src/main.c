#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

const char* fragmentSrc =
    "precision mediump float;\n"
    "uniform vec3 u_color;\n"
    "void main() { gl_FragColor = vec4(u_color, 1.0); }\n";

const char* vertexSrc =
    "attribute vec4 a_position;\n"
    "void main() { gl_Position = a_position; }\n";

GLuint createShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    // Gerçek bir uygulamada hata kontrolü eklenmelidir.
    return shader;
}

GLuint createProgram() {
    GLuint vs = createShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint fs = createShader(GL_FRAGMENT_SHADER, fragmentSrc);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vs);
    glAttachShader(prog, fs);
    glLinkProgram(prog);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return prog;
}

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
     

    GLFWwindow* win = glfwCreateWindow(600, 480, "Derinlik Testi (glDepthTest)", NULL, NULL);
    if (!win) {
        printf("Pencere oluşturulamadi!\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(win);

    GLuint prog = createProgram();
    GLint posLoc = glGetAttribLocation(prog, "a_position");
    GLint colorLoc = glGetUniformLocation(prog, "u_color");

    // Vertex verileri
    float redTriangle[] = {
        // Kırmızı üçgen (arkada, z = 1.0)
        -0.15f,  0.75f, 1.0f,
        -0.60f, -0.75f, 1.0f,
         0.30f, -0.75f, 1.0f,
    };

    float blueTriangle[] = {
        // Mavi üçgen (ortada, z = 0.0)
         0.15f,  0.75f, 0.0f,
        -0.30f, -0.75f, 0.0f,               
         0.60f, -0.75f, 0.0f,
    };
    float greenTriangle[] = {
        // Mavi üçgen (ortada, z = -1.0)
         0.45f,  0.75f, -1.0f,
         0.00f, -0.75f, -1.0f,               
         0.90f, -0.75f, -1.0f,
    };



    // Buffer'ları oluştur (sadece 2 üçgen için)
    GLuint vbos[3]; // 0: Kırmızı, 1: Mavi 
    glGenBuffers(3, vbos);

    // Kırmızı üçgenin verisini VBO'ya yükle
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(redTriangle), redTriangle, GL_STATIC_DRAW);

    // Mavi üçgenin verisini VBO'ya yükle
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blueTriangle), blueTriangle, GL_STATIC_DRAW);

    //Yeşil üçgenin verisiin VBO'ya yükle
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(greenTriangle), greenTriangle, GL_STATIC_DRAW);
   

    // Vertex attribute'unu etkinleştir
    glEnableVertexAttribArray(posLoc);
        

    glClearDepthf(0.5f);            // Derinlik temizleme değeri 1 olarak ayarlanır

    glEnable(GL_DEPTH_TEST);        // Depth test ayarları
    glDepthFunc(GL_EQUAL);          // Doğru derinlik testi için GL_NEVER ayarlanır
    
    GLenum error ;
    while ((error = glGetError()) != GL_NO_ERROR) {
    printf("Hata = 0x%x\n",error);   
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Arka plan rengi

    while (!glfwWindowShouldClose(win) && !glfwGetKey(win, GLFW_KEY_ESCAPE)) {
        // Her karede renk ve derinlik tamponunu temizle
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
        glUseProgram(prog);


        // 1. Kırmızı Üçgeni Çiz (z=1.0)
        glUniform3f(colorLoc, 1.0f, 0.2f, 0.2f);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 2. Mavi Üçgeni Çiz (z=0.0)
        glUniform3f(colorLoc, 0.2f, 0.2f, 1.0f);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
        glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // 3. Yeşil Üçgeni Çiz (z=-1.0)
        glUniform3f(colorLoc, 0.2f, 1.0f, 0.2f);
        glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
        glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        
        glfwSwapBuffers(win);
        glfwPollEvents();
       
    }

    // Cleanup
    glDeleteBuffers(3, vbos);
    glDeleteProgram(prog);
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}