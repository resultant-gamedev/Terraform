/* 
 * File:   Mesh.cpp
 * Author: John
 * 
 * Created on May 1, 2013, 5:43 PM
 */

#include "Mesh.h"

Mesh::Mesh(const char* filename) {
    readModel(filename);
}

Mesh::Mesh(const Mesh& orig) {
}

Mesh::~Mesh() {
}

void Mesh::setMeshData(double *vertices, int *tris, int numVerts, int numTris) {
    for (int i = 0; i < numVerts; i++) {
        this->verts.push_back(vertices[3*i]);
        this->verts.push_back(vertices[3*i+1]);
        this->verts.push_back(vertices[3*i+2]);
    }
    
    for (int i = 0; i < numTris; i++) {
        this->tris.push_back(new MeshTriangle(this, tris[3*i], tris[3*i+1], tris[3*i+2]));
    }
}

void Mesh::readModel(const char* filename) {  // function reading in Blender model
    ifstream in_stream;
    string line;
    in_stream.open(filename);  // open file

    in_stream >> line;
    int nPoints = atoi(line.c_str());
    in_stream >> line;
    int nPolys = atoi(line.c_str());
    
    double vertices[nPoints*3];
    int triangles[nPolys*3];
    
    in_stream >> line;
    if (!line.compare("vertices")) {
        for (int i = 0; i < nPoints*3; i++) {
            in_stream >> line;
            vertices[i] = atof(line.c_str());
        }
    }
    
    in_stream >> line;
    if (!line.compare("triangles")) {
        for (int i = 0; i < nPolys*3; i++) {
            in_stream >> line;
            triangles[i] = atoi(line.c_str());
        }
    }
    
    in_stream.close();
    setMeshData(vertices,triangles,nPoints,nPolys);
}

void Mesh::draw() {
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < this->tris.size(); i++) {
        MeshTriangle *tri = tris.at(i);
        int t1 = tri->v0;
        int t2 = tri->v1;
        int t3 = tri->v2;
        glVertex3f(verts.at(3*t1),verts.at(3*t1+1),verts.at(3*t1+2));
        glVertex3f(verts.at(3*t2),verts.at(3*t2+1),verts.at(3*t2+2));
        glVertex3f(verts.at(3*t3),verts.at(3*t3+1),verts.at(3*t3+2));
    }
    glEnd();
    glPopMatrix();
}

bool Mesh::intersect(double xpos, double ypos, double zpos, double xcam, double ycam, double zcam) {
    for (int i = 0; i < this->tris.size(); i++) {
        if (tris.at(i)->intersect(xpos,ypos,zpos,xcam,ycam,zcam)) {
            return true;
        }
    }
    return false;
}


