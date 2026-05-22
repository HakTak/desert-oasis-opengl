// Autor: Milos Medic
// Opis: Projekat Pejzaz Pustinje

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#include <random>
#include <map>

#define PI 3.141592
#define CRES 150
#define NUM_POINTS 100
#define TRAVA 40

//stb_image.h je header-only biblioteka za ucitavanje tekstura.
//Potrebno je definisati STB_IMAGE_IMPLEMENTATION prije njenog ukljucivanja
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source); 
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath); //Ucitavanje teksture, izdvojeno u funkciju


float getTime();
float getRandomFloat(float min, float max);

float timeFrozen = 0.0;
bool isTimeFrozen = false;

int main(void)
{
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ INICIJALIZACIJA ++++++++++++++++++++++++++++++++++++++++++++++++++++++

    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    //Odredjivanje OpenGL verzije i profila (3.3, programabilni pajplajn)
    //bez ovoga, koristi se najnoviji moguci OpenGL koji hardver podrzava
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Stvaranje prozora
    GLFWwindow* window; //Mjesto u memoriji za prozor
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    unsigned int wWidth = mode -> width;
    unsigned int wHeight = mode -> height;
    const char wTitle[] = "[PUSTINJA]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, monitor, NULL); // Napravi novi prozor fullscreen rezolucije mog monitora

    if (window == NULL) 
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate(); 
        return 2; 
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) 
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ PROMJENLJIVE I BAFERI +++++++++++++++++++++++++++++++++++++++++++++++++


    unsigned int VAO[11];
    glGenVertexArrays(11, VAO);
    unsigned int VBO[11];
    glGenBuffers(11, VBO);

    //sunce i mesec
    float nebeskaTela[] = 
    {
        //Pozicija    |    TEKSTURE 
        //X     Y         S    T
        0.1,  0.13,    1.0, 1.0, 
       -0.1,  0.13,    0.0, 1.0, 
        0.1, -0.13,    1.0, 0.0, 
       -0.1, -0.13,    0.0, 0.0,

        0.06,  0.1,    1.0, 1.0,
       -0.06,  0.1,    0.0, 1.0,
        0.06, -0.1,    1.0, 0.0,
       -0.06, -0.1,    0.0, 0.0
    };
    unsigned int stride = 4 * sizeof(float); 

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nebeskaTela), nebeskaTela, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0); 
    glEnableVertexAttribArray(0); 

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float))); 
    glEnableVertexAttribArray(1);

    //zvezde
    float zvezde[CRES * 2];
    for (int i = 0; i < CRES; i++){
        zvezde[2 * i] = getRandomFloat(0.2f, 1.1f) * cos((i * 360 / CRES) * (PI / 180)); //Xi
        zvezde[2 * i + 1] = getRandomFloat(0.3f, 1.0f) * sin((i * 360 / CRES) * (PI / 180)); //Yi
    }

    stride = 2 * sizeof(float);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zvezde), zvezde, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    //pesak
    float zemlja[(NUM_POINTS + 3) * 2]; //204
    zemlja[0] = -1.0; //X[0]
    zemlja[1] = -1.0; //Y[0]
    zemlja[(NUM_POINTS + 3) * 2 - 2] = 1.0;   //X[NUM_POINTS - 1]
    zemlja[(NUM_POINTS + 3) * 2 - 1] = -1.0;  //Y[NUM_POINTS - 1]
    for (int i = 0; i <= NUM_POINTS; i++) {
        zemlja[2 * i + 2] = i / (NUM_POINTS / 2.0) - 1.0;   //Xi
        zemlja[2 * i + 2 + 1] = 0.1 * sin((i * 360 / CRES) * (PI / 180)) - 0.1; //Yi
    }

    stride = 2 * sizeof(float);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zemlja), zemlja, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    //oaza
    float oaza[CRES * 2];
    oaza[0] = 0.0;
    oaza[1] = 0.0;
    for (int i = 0; i < CRES; i++) {
        oaza[2 * i] = 0.35 * cos((i * 360 / CRES) * (PI / 180)); //Xi
        oaza[2 * i + 1] = 0.2 * sin((i * 360 / CRES) * (PI / 180)); //Yi
    }

    stride = 2 * sizeof(float);

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(oaza), oaza, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    //ribica
    float ribica[] = {
        //Pozicija    |    TEKSTURA 
        //X     Y        S       T
       
       -0.475, -0.63,    1.0,   1.0,
       -0.525, -0.63,    0.0,   1.0,
       -0.475, -0.68,    1.0,   0.0,
       -0.525, -0.68,    0.0,   0.0,
    };

    stride = 4 * sizeof(float);

    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ribica), ribica, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //trava
    float trava[TRAVA * 4 * 4]; //4 koordinata: x,y,  s, t  i  4 tacke za kvadrat puta 20 travki
    for (int i = 0; i < TRAVA; i++) {
        float xi = 0.38 * cos((i * 360 / TRAVA) * (PI / 180)) - 0.035; //Xi
        float yi = 0.22 * sin((i * 360 / TRAVA) * (PI / 180)); //Yi

        for (int j = 0; j < 4; j++) {
            int k = 0;
            float x = xi;
            float y = yi;
            float s = 0.0;
            float t = 0.0;
            switch (j) {
                case 1:
                    x += 0.05;
                    s = 1.0;
                    break;
                case 2:
                    y += 0.1;
                    t = 1.0;
                    break;
                case 3:
                    x += 0.05;
                    y += 0.1;
                    s = 1.0;
                    t = 1.0;
                    break;
                default:
                    break;
            }

            trava[16 * i + 4 * j + k++] = x;
            trava[16 * i + 4 * j + k++] = y;
            trava[16 * i + 4 * j + k++] = s; 
            trava[16 * i + 4 * j + k++] = t; 
        }
    }
    stride = 4 * sizeof(float);

    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trava), trava, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //piramide
    float piramide[] = {
       //leva piramida
      -0.5,   -0.2,    0.4, 0.6, 0.1, 1.0,
      -0.45,  -0.2,    0.4, 0.6, 0.1, 1.0,
      -0.3,    0.2,    0.4, 0.6, 0.1, 1.0,

      -0.45,  -0.2,    0.7, 0.8, 0.2, 1.0,
      -0.3,    0.2,    0.7, 0.8, 0.2, 1.0,
       0.0,   -0.2,    0.7, 0.8, 0.2, 1.0,

      -0.45,  -0.2,    0.0, 0.0, 0.0, 1.0,
      -0.3,    0.2,    0.0, 0.0, 0.0, 1.0,

      //desna piramida
       0.25,  -0.22,    0.4, 0.6, 0.1, 1.0,
       0.33,  -0.22,    0.4, 0.6, 0.1, 1.0,
       0.43,   0.18,    0.4, 0.6, 0.1, 1.0,

       0.33,  -0.22,    0.7, 0.8, 0.2, 1.0,
       0.43,   0.18,    0.7, 0.8, 0.2, 1.0,
       0.7,   -0.22,    0.7, 0.8, 0.2, 1.0,

       0.33,  -0.22,    0.0, 0.0, 0.0, 1.0,
       0.43,   0.18,    0.0, 0.0, 0.0, 1.0,

      //srednja i najveca piramida
      -0.15,   -0.25,    0.4, 0.6, 0.1, 1.0,
      -0.07,   -0.25,    0.4, 0.6, 0.1, 1.0,
       0.05,    0.25,    0.4, 0.6, 0.1, 1.0,

      -0.07,  -0.25,    0.7, 0.8, 0.2, 1.0,
       0.05,   0.25,    0.7, 0.8, 0.2, 1.0,
       0.4,   -0.25,    0.7, 0.8, 0.2, 1.0,

      -0.07,  -0.25,    0.0, 0.0, 0.0, 1.0,
       0.05,   0.25,    0.0, 0.0, 0.0, 1.0,

       0.05,   0.25,    0.0, 0.0, 0.0, 1.0,
       0.4,   -0.25,    0.0, 0.0, 0.0, 1.0,
    };

    stride = 6 * sizeof(float);

    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(piramide), piramide, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //obojena pravougaonik
    float crveniPravougaonik[] = {
        
        -1.25,  -0.25,    1.0, 0.0, 0.0, 0.7,
        -1.25,   0.25,    1.0, 0.0, 0.0, 0.7,
        -0.7,   -0.25,    1.0, 0.0, 0.0, 0.7,
        -0.7,    0.25,    1.0, 0.0, 0.0, 0.7,
        -0.15,   0.25,    0.5, 0.0, 0.0, 0.0,
        -0.7,   -0.25,    1.0, 0.0, 0.0, 0.7,
        -0.15,  -0.25,    0.5, 0.0, 0.0, 0.0
    };

    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crveniPravougaonik), crveniPravougaonik, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //prolazi u piramidu
    float prolazi[] = {
      -0.25,   -0.23,    0.0, 0.0, 0.0,
      -0.29,  -0.23,    0.0, 0.0, 0.0,
      -0.25,   -0.32,    0.0, 0.0, 0.0,
      -0.29,  -0.32,    0.0, 0.0, 0.0,

       0.475,  -0.26,   0.0, 0.0, 0.0,
       0.51,   -0.26,   0.0, 0.0, 0.0,
       0.475,  -0.34,   0.0, 0.0, 0.0,
       0.51,   -0.34,   0.0, 0.0, 0.0,

       0.1,   -0.25,   0.0, 0.0, 0.0,
       0.15,  -0.25,   0.0, 0.0, 0.0,
       0.1,   -0.37,   0.0, 0.0, 0.0,
       0.15,  -0.37,   0.0, 0.0, 0.0
    };
    stride = 5 * sizeof(float);

    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(prolazi), prolazi, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //slova
    char recenica[] = {"Nastavice se na\n3D projektu"};
    int const duzinaRecenice = sizeof(recenica) - 1;
    float slova[duzinaRecenice * 4 * 4];
    float x, y, s, t;
    x = -0.75;
    y = 0.4;
    for (int i = 0; i < duzinaRecenice; i++) {
        if (recenica[i] == ' ') {
            x += 0.1;
            continue;
        }
        if (recenica[i] == '\n') {
            x = -0.55;
            y = 0.15;
            continue;
        }
        for (int j = 0; j < 4; j++) {
            int k = 0;
            s = 0.0;
            t = 0.0;
            switch (j) {
            case 1:
                x += 0.1;
                s = 1.0;
                break;
            case 2:
                x -= 0.1;
                y += 0.2;
                t = 1.0;
                break;
            case 3:
                x += 0.1;
                s = 1.0;
                t = 1.0;
                break;
            default:
                break;
            }

            slova[16 * i + 4 * j + k++] = x;
            slova[16 * i + 4 * j + k++] = y;
            slova[16 * i + 4 * j + k++] = s;
            slova[16 * i + 4 * j + k++] = t;

            //std::cout << x << "\t" << y << "\t" << s << "\t" << t << "\n";
        } 
        y -= 0.2;
    }

    stride = 4 * sizeof(float);

    glBindVertexArray(VAO[9]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(slova), slova, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

   /* float potpis[] = {
       -0.98,    0.8,      0.0,   0.0,
       -0.98,    0.95,     0.0,   1.0,
       -0.6,    0.8,      1.0,   0.0,
       -0.6,    0.95,     1.0,   1.0
    };

    stride = 4 * sizeof(float);

    glBindVertexArray(VAO[10]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(potpis), potpis, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ RENDER LOOP - PETLJA ZA CRTANJE +++++++++++++++++++++++++++++++++++++++++++++++++
    unsigned int basicShader = createShader("basic.vert", "basic.frag");
    unsigned int zvezdeShader = createShader("zvezde.vert", "basic.frag");
    unsigned int zemljaShader = createShader("zemlja.vert", "basic.frag");    
    unsigned int oazaShader = createShader("oaza.vert", "basic.frag");
    unsigned int prolaziShader = createShader("prolazi.vert", "prolazi.frag");
    unsigned int crvenaShader = createShader("basic.vert", "crvena.frag");

    unsigned int uPosLoc = glGetUniformLocation(basicShader, "uPos");
    unsigned int uStepLoc = glGetUniformLocation(zvezdeShader, "uPos");
    unsigned int uColLoc = glGetUniformLocation(zemljaShader, "uCol");
    unsigned int uPosLocOaza = glGetUniformLocation(oazaShader, "uPos");
    unsigned int uColLocOaza = glGetUniformLocation(oazaShader, "uCol");
    unsigned int uYProlazi = glGetUniformLocation(prolaziShader, "uY");
    unsigned int uWLocProlaz = glGetUniformLocation(prolaziShader, "uW");
    unsigned int uHLocProlaz = glGetUniformLocation(prolaziShader, "uH");
    unsigned int uWLocCrvena = glGetUniformLocation(crvenaShader, "uW");
    unsigned int uHLocCrvena = glGetUniformLocation(crvenaShader, "uH");


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ TEKSTURE ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    unsigned int basicTekstureShared = createShader("basicTeksture.vert", "basicTeksture.frag");
    unsigned int ribicaShader = createShader("ribicaTekstura.vert", "basicTeksture.frag");
    unsigned int travaShader = createShader("basicTeksture.vert", "trava.frag");
    unsigned int slovaShader = createShader("slova.vert", "slova.frag");

    unsigned uTexLoc = glGetUniformLocation(basicTekstureShared, "uTex");
    unsigned int uPosLocTexture = glGetUniformLocation(basicTekstureShared, "uPos");
    unsigned uTexLocRibica = glGetUniformLocation(ribicaShader, "uTex");
    unsigned int uPosLocRibica = glGetUniformLocation(ribicaShader, "uPos");
    unsigned int uIsReversedLocRibica = glGetUniformLocation(ribicaShader, "isReversed");
    unsigned uTexLocTrava = glGetUniformLocation(travaShader, "uTex");
    unsigned int uPosLocTrava = glGetUniformLocation(travaShader, "uPos");
    unsigned int uIsInvisibleLocTrava = glGetUniformLocation(travaShader, "isInvisible");
    unsigned uTexLocSlova = glGetUniformLocation(slovaShader, "uTex");
    unsigned int uAlphaLocSlova = glGetUniformLocation(slovaShader, "uAlpha");
        
    //Tekstura  
    unsigned checkerTexture[30];
    checkerTexture[0] = loadImageToTexture("res/ribica.png");
    checkerTexture[1] = loadImageToTexture("res/sunce3.png");
    checkerTexture[2] = loadImageToTexture("res/mesec.png");
    checkerTexture[3] = loadImageToTexture("res/trava2.png");
    checkerTexture[4] = loadImageToTexture("res/N.png");
    checkerTexture[5] = loadImageToTexture("res/a2.png");
    checkerTexture[6] = loadImageToTexture("res/s2.png");
    checkerTexture[7] = loadImageToTexture("res/t2.png");
    checkerTexture[8] = loadImageToTexture("res/v2.png");
    checkerTexture[9] = loadImageToTexture("res/i2.png");
    checkerTexture[10] = loadImageToTexture("res/c2.png");
    checkerTexture[11] = loadImageToTexture("res/e2.png");
    checkerTexture[12] = loadImageToTexture("res/3.png");
    checkerTexture[13] = loadImageToTexture("res/D.png");
    checkerTexture[14] = loadImageToTexture("res/p2.png");
    checkerTexture[15] = loadImageToTexture("res/r2.png");
    checkerTexture[16] = loadImageToTexture("res/o2.png");
    checkerTexture[17] = loadImageToTexture("res/j2.png");
    checkerTexture[18] = loadImageToTexture("res/k2.png");
    checkerTexture[19] = loadImageToTexture("res/u2.png");
    checkerTexture[20] = loadImageToTexture("res/n2.png");
    //checkerTexture[21] = loadImageToTexture("res/potpis.png");/

    std::map<char, int> slovaMap = { {'N', 4}, {'a', 5}, {'s', 6}, {'t', 7}, {'v', 8}, {'i', 9}, {'c', 10}, {'e', 11}, {'3', 12}, {'D', 13},
                                        {'p', 14}, {'r', 15}, {'o', 16}, {'j', 17}, {'k', 18}, {'u', 19}, {'n', 20}};

    for (int i = 0; i < 22; i++) {
        glBindTexture(GL_TEXTURE_2D, checkerTexture[i]); //Podesavamo teksturu
        glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   //GL_NEAREST, GL_LINEAR
        glBindTexture(GL_TEXTURE_2D, i);

        //Odnosi se na glActiveTexture(GL_TEXTURE0) u render petlji
        //Moguce je sabirati indekse, tj GL_TEXTURE5 se moze dobiti sa GL_TEXTURE0 + 5 , sto je korisno za iteriranje kroz petlje
    }
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    float r = 1.03;      
    float rotationSpeed = 0.3; 
    float ribicaSpeed = 0.0;
    bool  isTravaInvisible = false;
    float otvaranjeVrata = 0.0;
    float bojanjeCrveno = 0.0;
    float slovaAlpha = 0.0;
    bool  isReversedAlpha = false;
    bool  isClosing = false;

    //r: 135, g: 206, b: 235 plavo nebo
    float rc = 135.0 / 255.0, gc = 206.0 / 255.0, bc = 235.0 / 255.0;

    const float TARGET_FRAME_TIME = 1.0f / 60.0f; // 60 FPS = 1/60 seconds per frame

    glfwSetTime(0.0);
    float lastFrameTime = glfwGetTime(); // Time at the beginning of the last frame
    int lastMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    float closingTime = 0.0;

    //int brojac = 1;

    int previousKeyState = GLFW_RELEASE;

    while (!glfwWindowShouldClose(window)) 
    {
        if (slovaAlpha <= 0 && isClosing && closingTime <= 0.0) {
            closingTime = glfwGetTime();
        }
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || (glfwGetTime() - closingTime >= 2.0 && slovaAlpha <= 0 && isClosing)) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        int currentKeyState = glfwGetKey(window, GLFW_KEY_P);
        if (currentKeyState == GLFW_PRESS && previousKeyState == GLFW_RELEASE) {
            if (isTimeFrozen) {
                isTimeFrozen = false;
                glfwSetTime(timeFrozen);                    
                lastFrameTime = glfwGetTime();
            }
            else {
                isTimeFrozen = true;
                timeFrozen = glfwGetTime();
            }
        }
        previousKeyState = currentKeyState;
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            isTimeFrozen = false;
            glfwSetTime(0.0);
            lastFrameTime = glfwGetTime();
            //brojac = 0;
            otvaranjeVrata = 0.0;
            bojanjeCrveno = 0.0;
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            isTravaInvisible = true;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            isTravaInvisible = false;
        }
        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
            otvaranjeVrata = 0.001;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            if(bojanjeCrveno < 1.09)
                bojanjeCrveno += 0.01;            
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            if(bojanjeCrveno > 0.01)
                bojanjeCrveno -= 0.01;
        }
        int currentMouseState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (/*state == GLFW_PRESS || */currentMouseState == GLFW_RELEASE && lastMouseState != currentMouseState && !isClosing)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            //std::cout << otvaranjeVrata;
            if (otvaranjeVrata >= 0.12 && slovaAlpha <= 0.0 &&
                (((2 * xpos / wWidth - 1.0) >= -0.29 && (2 * -ypos / wHeight + 1.0) >= -0.2 &&
                 (2 * xpos / wWidth - 1.0) <= -0.25 && (2 * -ypos / wHeight + 1.0) <= -0.08)
                ||
                ((2 * xpos / wWidth - 1.0) >=  0.475 && (2 * -ypos / wHeight + 1.0) >= -0.22 &&
                 (2 * xpos / wWidth - 1.0) <=  0.51 && (2 * -ypos / wHeight + 1.0) <= -0.1)
                ||
                ((2 * xpos / wWidth - 1.0) >=  0.1 && (2 * -ypos / wHeight + 1.0) >= -0.25 &&
                 (2 * xpos / wWidth - 1.0) <=  0.15 && (2 * -ypos / wHeight + 1.0) <= -0.13))) {

                slovaAlpha = 0.002;
                isClosing = true;
            }
        }
        lastMouseState = currentMouseState;

        

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        float svetlost = (sin(getTime() * rotationSpeed) + 1 ) / 2; //Svetlost je 1.0 ako je sunce na vrhu neba, a 0.0 ako je na dnu, tj. ako je mesec na vrhu
        glClearColor(rc * svetlost, gc * svetlost, bc * svetlost, 1.0); //Podesavanje boje pozadine neba (RGBA format); U zavisnosti od polozaja sunca osvetljenje pozadine, neba, se menja

        //Brisanje ekrana
        glClear(GL_COLOR_BUFFER_BIT);

        // [KOD ZA CRTANJE]
        glPointSize(5);

        //nebeska tela
        glUseProgram(zvezdeShader); 
        glBindVertexArray(VAO[1]);

        float zvezdeX = r * cos(getTime() * rotationSpeed);
        float zvezdeY = r * sin(getTime() * rotationSpeed);
        glUniform2f(uPosLoc, zvezdeX, zvezdeY);
        glDrawArrays(GL_POINTS, 0, CRES);

        //sunce i mesec
        glUseProgram(basicTekstureShared);
        glBindVertexArray(VAO[0]);

        glUniform1i(uTexLoc, 1); 
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, checkerTexture[1]);

        float sunceX = r * cos(getTime() * rotationSpeed);
        float sunceY = r / 1.15 * sin(getTime() * rotationSpeed);
        glUniform2f(uPosLocTexture, sunceX, sunceY);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 

        glUniform1i(uTexLoc, 2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, checkerTexture[2]);

        float mesecX = r * cos(getTime() * rotationSpeed + PI);
        float mesecY = r / 1.15 * sin(getTime() * rotationSpeed + PI);
        glUniform2f(uPosLocTexture, mesecX, mesecY);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

        //pustinja
        glUseProgram(zemljaShader);
        glBindVertexArray(VAO[2]);

        //r: 234, g: 206, b: 106 zlatni pesak
        float rz = 234.0 / 255.0, gz = 206.0 / 255.0, bz = 106.0 / 255.0;
        glUniform4f(uColLoc, rz, gz, bz, 1.0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(zemlja) / (2 * sizeof(float)));

        //ribica
        glUseProgram(ribicaShader);
        glBindVertexArray(VAO[4]);

        glUniform1i(uTexLocRibica, 0); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, checkerTexture[0]);

        ribicaSpeed += 0.005;
        float ribicaX = r / 4 * cos(ribicaSpeed + PI/2);
        float ribicaY =  r / 80 * sin(ribicaSpeed * 6);
        glUniform2f(uPosLocRibica, ribicaX, ribicaY);
        if (cos(ribicaSpeed) <= 0)
            glUniform1f(uIsReversedLocRibica, false);
        else
            glUniform1f(uIsReversedLocRibica, true);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        //oaza
        glUseProgram(oazaShader);
        glBindVertexArray(VAO[3]);

        //r: 100, g: 200, b: 255 voda oaze
        float ro = 60.0 / 255.0, go = 100.0 / 255.0, bo = 220.0 / 255.0;
        glUniform4f(uColLocOaza, ro, go, bo, 0.7);
        glUniform2f(uPosLocOaza, -0.5, -0.65);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(oaza) / (2 * sizeof(float)));

        //piramide
        glUseProgram(basicShader);
        glBindVertexArray(VAO[6]);

        glUniform2f(uPosLoc, 0, 0);
        glLineWidth(1);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 3, 3);
        glDrawArrays(GL_LINES, 6, 2);

        glDrawArrays(GL_TRIANGLES, 8, 3);
        glDrawArrays(GL_TRIANGLES, 11, 3);
        glDrawArrays(GL_LINES, 14, 2);

        glDrawArrays(GL_TRIANGLES, 16, 3);
        glDrawArrays(GL_TRIANGLES, 19, 3);
        glDrawArrays(GL_LINES, 22, 2);
        glDrawArrays(GL_LINES, 24, 2);

        //maska piramide
        glUseProgram(crvenaShader);
        glBindVertexArray(VAO[7]);

        glUniform2f(uPosLoc, bojanjeCrveno, 0);
        glUniform1f(uWLocCrvena, wWidth);
        glUniform1f(uHLocCrvena, wHeight);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 3, 4);

        //trava
        glUseProgram(travaShader);
        glBindVertexArray(VAO[5]);

        glUniform1i(uTexLocTrava, 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, checkerTexture[3]);

        glUniform1f(uIsInvisibleLocTrava, isTravaInvisible);
        glUniform2f(uPosLocTrava, -0.5, -0.65);
        for (int i = 0; i < TRAVA; i++)
            glDrawArrays(GL_TRIANGLE_STRIP, 4 * i, 4);

        //prolazi
        glUseProgram(prolaziShader);
        glBindVertexArray(VAO[8]);
        if (otvaranjeVrata > 0 && otvaranjeVrata <= 0.12) {
            otvaranjeVrata += 0.001;
        }

        glUniform1f(uYProlazi, otvaranjeVrata);
        glUniform1f(uWLocProlaz, wWidth);
        glUniform1f(uHLocProlaz, wHeight);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

        //slova
        glUseProgram(slovaShader);
        glBindVertexArray(VAO[9]);

        if (slovaAlpha > 0) {
            int brojacSlova = 0;
            float alphaSabiracRazmaka = 0.0;
            float alphaReversed = 1.0;
            for (int i = 0; i < duzinaRecenice; i++) {
                float alpha = slovaAlpha + alphaSabiracRazmaka - i * 1.0 ;
                if (isReversedAlpha) {
                    alpha = slovaAlpha - alphaSabiracRazmaka + i * 1.0 - duzinaRecenice * 1.0 + 4.0;
                    slovaAlpha -= 0.002;
                }
                if (recenica[i] == ' ' || recenica[i] == '\n') {
                    alphaSabiracRazmaka += 1.0;
                }
                glUniform1f(uAlphaLocSlova, alpha);
                glUniform1i(uTexLocSlova, slovaMap[recenica[i]]);
                glActiveTexture(GL_TEXTURE0 + slovaMap[recenica[i]]);
                glBindTexture(GL_TEXTURE_2D, checkerTexture[slovaMap[recenica[i]]]);
                glDrawArrays(GL_TRIANGLE_STRIP, 4 * brojacSlova++, 4);
                if (slovaAlpha <= duzinaRecenice -3 && !isReversedAlpha)
                    slovaAlpha += 0.002;
                else
                    isReversedAlpha = true;
            }
        }

        //trava
        glUseProgram(travaShader);
        glBindVertexArray(VAO[5]);

        glUniform1i(uTexLocTrava, 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, checkerTexture[3]);

        glUniform1f(uIsInvisibleLocTrava, isTravaInvisible);
        glUniform2f(uPosLocTrava, -0.5, -0.65);
        for (int i = 0; i < TRAVA; i++)
            glDrawArrays(GL_TRIANGLE_STRIP, 4 * i, 4);

        //potpis
        /*glUseProgram(basicTekstureShared);
        glBindVertexArray(VAO[10]);

        glUniform1i(uTexLoc, 21);
        glActiveTexture(GL_TEXTURE21);
        glBindTexture(GL_TEXTURE_2D, checkerTexture[21]);

        glUniform2f(uPosLocTexture, 0, 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);*/

        glBindVertexArray(0);
        glUseProgram(0);

        //Zamjena vidljivog bafera sa pozadinskim
        glfwSwapBuffers(window);

        // Sleep or busy-wait to cap the FPS at exactly 60
        while (glfwGetTime() - lastFrameTime < TARGET_FRAME_TIME) {
            //vrti se dok ne prodje 1/60s, vreme jednog frame-a
        }
        lastFrameTime = glfwGetTime();

        /*std::cout << "Vreme: " << glfwGetTime() << "second and ";
        std::cout << brojac++ << "frame\n";*/

        //Hvatanje dogadjaja koji se ticu okvira prozora (promjena velicine, pomjeranje itd)
        glfwPollEvents();
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ POSPREMANJE +++++++++++++++++++++++++++++++++++++++++++++++++


    //Brisanje bafera i sejdera
    glDeleteTextures(30, checkerTexture);
    glDeleteBuffers(10, VBO);
    glDeleteVertexArrays(10, VAO);
    glDeleteProgram(basicShader);
    glDeleteProgram(zvezdeShader);

    //Sve OK - batali program
    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str(); 

    int shader = glCreateShader(type); 
    
    int success; 
    char infoLog[512]; 
    glShaderSource(shader, 1, &sourceCode, NULL); 
    glCompileShader(shader); 

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{

    unsigned int program; 
    unsigned int vertexShader; 
    unsigned int fragmentShader; 

    program = glCreateProgram(); 

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource); 
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource); 

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program); 
    glValidateProgram(program); 

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success); 
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 2: InternalFormat = GL_RG; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}

float getTime() {
    if (isTimeFrozen)
        return timeFrozen;
    return glfwGetTime();
}

float getRandomFloat(float min, float max) {
    std::random_device rd;  // Obtain a random number from the hardware
    std::mt19937 gen(rd()); // Seed the generator
    std::uniform_real_distribution<> dis(min, max); // Define the range

    return dis(gen); // Generate a random number in the range [min, max]
}