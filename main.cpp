#include <stdio.h>
#include <kos.h>

#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glkos.h"

#include <iostream>
#include <memory>
#include <thread>

#include "AssetManager.h"
#include "Obj.h"
#include "Material.h"


// KOS_INIT_FLAGS(INIT_DEFAULT);

int check_start() {
    maple_device_t *cont;
    cont_state_t *state;

    cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

    if(cont) {
        state = (cont_state_t *)maple_dev_status(cont);

        if(state)
            return state->buttons & CONT_START;
    }

    return 0;
}


int main(int argc, char **argv)
{
    AssetManager assetManager;
    glKosInit();

    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);		// This Will Clear The Background Color To Black
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LEQUAL);				// The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix

    gluPerspective(45.0f,(GLfloat)640/(GLfloat)480,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);

    while(1) {
        if(check_start()) break;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

        // render model
        auto obj = assetManager.loadObjModel("model.obj");
        glPushMatrix();
        glTranslatef(-2.0f, -1.0f, -10.0f);
        glBegin(GL_TRIANGLES);

        for (size_t x = 0; x < obj->vertexIndices.size(); x += 3) {
            // Sicherstellen, dass der Materialindex gültig ist
            int materialIndex = (x / 3 < obj->materialIndices.size()) ? obj->materialIndices[x / 3] : 0;

            if (materialIndex < 0 || static_cast<size_t>(materialIndex) >= obj->materials.size()) {
                std::cerr << "Warnung: Face " << (x / 3) << " hat ungültigen Materialindex " << materialIndex << std::endl;
                materialIndex = 0; // Setze Standardmaterial
            }

            // get material
            auto currentMaterial = obj->materials[materialIndex];

            // set material
            glMaterialfv(GL_FRONT, GL_AMBIENT, currentMaterial->ambient.data());
            glMaterialfv(GL_FRONT, GL_DIFFUSE, currentMaterial->diffuse.data());
            glMaterialfv(GL_FRONT, GL_SPECULAR, currentMaterial->specular.data());
            glMaterialf(GL_FRONT, GL_SHININESS, currentMaterial->shininess);

            // use colors if lightning is disabled
            if (!glIsEnabled(GL_LIGHTING)) {
                glColor3f(currentMaterial->diffuse[0], currentMaterial->diffuse[1], currentMaterial->diffuse[2]);
            }

            // Optional: Debug-Ausgabe
            /* std::cout << "Material: " << currentMaterial->name
                    << " | Diffuse: (" << currentMaterial->diffuse[0] << ", "
                    << currentMaterial->diffuse[1] << ", "
                    << currentMaterial->diffuse[2] << ")"
                    << " | Shininess: " << currentMaterial->shininess << std::endl; */

            /* std::cout << "Material: " << currentMaterial->name << "\n"
                    << "  Ambient: (" << currentMaterial->ambient[0] << ", "
                    << currentMaterial->ambient[1] << ", "
                    << currentMaterial->ambient[2] << ", "
                    << currentMaterial->ambient[3] << ")\n"
                    << "  Diffuse: (" << currentMaterial->diffuse[0] << ", "
                    << currentMaterial->diffuse[1] << ", "
                    << currentMaterial->diffuse[2] << ", "
                    << currentMaterial->diffuse[3] << ")\n"
                    << "  Specular: (" << currentMaterial->specular[0] << ", "
                    << currentMaterial->specular[1] << ", "
                    << currentMaterial->specular[2] << ", "
                    << currentMaterial->specular[3] << ")\n"
                    << "  Shininess: " << currentMaterial->shininess << "\n"; */

            // render triangles
            for (int i = 0; i < 3; ++i) {
                int vertexIndex = obj->vertexIndices[x + i] - 1;
                int normalIndex = obj->normalIndices[x + i] - 1;
                //int uvIndex = obj->uvIndices[x + i] - 1; // textures stripped for this reduced code

                glNormal3f(obj->normals[normalIndex].x, obj->normals[normalIndex].y, obj->normals[normalIndex].z);
                glVertex3f(obj->vertices[vertexIndex].x, obj->vertices[vertexIndex].y, obj->vertices[vertexIndex].z);
            }
        }
        glEnd();

        // reset material
        auto defaultMat = assetManager.loadMaterial("default.default");
        glMaterialfv(GL_FRONT, GL_AMBIENT, defaultMat->ambient.data());
        glMaterialfv(GL_FRONT, GL_DIFFUSE, defaultMat->diffuse.data());
        glMaterialfv(GL_FRONT, GL_SPECULAR, defaultMat->specular.data());
        glMaterialf(GL_FRONT, GL_SHININESS, defaultMat->shininess);

        // reset colors
        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();


        // -- Try disabling/enabling lightning here --
        // glEnable(GL_LIGHTING);
        // -- Try disabling/enabling lightning here --
        glEnable(GL_LIGHT0);
    
        GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
        GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
        GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    
    
        /*GLfloat globalAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};  // Sanftes Grundlicht
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);*/
    
        float rgba[ 4 ] = { 0, 0, 0 , 1 };
        glLightModelfv( GL_LIGHT_MODEL_AMBIENT, rgba );
        glLightfv( GL_LIGHT0, GL_AMBIENT, rgba );
    
        /* Finish the frame */
        glKosSwapBuffers();
    }

    return 0;
}