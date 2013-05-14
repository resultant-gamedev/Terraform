/* 
 * File:   Mesh.h
 * Author: John
 *
 * Created on May 1, 2013, 5:43 PM
 */

#ifndef MESH_H
#define	MESH_H

#include <GL/glew.h>
#include <GLUT/glut.h>
#include "MeshTriangle.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "ParticleMachine.h"

using namespace std;

class MeshTriangle;

class ParticleMachine;

class Mesh {
public:
    Mesh(const char* filename, double xpos, double ypos, double zpos);
    Mesh(const Mesh& orig);
    virtual ~Mesh();
    void readModel(const char* filename);
    void setMeshData(double *vertices, int *triangles, int numVerts, int numTris);
    vector<double> verts;
    vector<MeshTriangle*> tris;
    vector<ParticleMachine*> particles;
    virtual void draw(double dt);
    bool intersect(double xpos, double ypos, double zpos, double xcam, double ycam, double zcam);
    double xpos;
    double ypos;
    double zpos;
private:

};

#endif	/* MESH_H */

