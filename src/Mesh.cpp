/* 
 * File:   Mesh.cpp
 * Author: John
 * 
 * Created on May 1, 2013, 5:43 PM
 * 
 * General purpose Mesh class serving functions common to all meshes.
 */

#include "Mesh.h"

// set mesh position, read the .msh file, and build the mesh object
Mesh::Mesh(const char* filename, double xpos, double ypos, double zpos) {
    this->xpos = xpos;
    this->ypos = ypos;
    this->zpos = zpos;
    readModel(filename);
}

Mesh::Mesh(const Mesh& orig) {
}

Mesh::~Mesh() {
}

void Mesh::setMeshData(double *vertices, int *tris, int *mtlIndices, 
        double *mtls, int numVerts, int numTris, int numObjs) {
        
    // build vertex list
    for (int i = 0; i < numVerts; i++) {
        this->verts.push_back(vertices[3*i] + this->xpos);
        this->verts.push_back(vertices[3*i+1] + this->ypos);
        this->verts.push_back(vertices[3*i+2] + this->zpos);
    }
    // build triangle list
    for (int i = 0; i < numTris; i++) {
        this->tris.push_back(new MeshTriangle(this, tris[3*i], tris[3*i+1], tris[3*i+2]));
    }
    // build materials list
    for (int i = 0; i < numObjs; i++) {
        this->mtlIndices.insert(pair<int,int>(i,mtlIndices[i]));
        this->mtls.push_back(mtls[3*i]);
        this->mtls.push_back(mtls[3*i+1]);
        this->mtls.push_back(mtls[3*i+2]);
    }
}

void Mesh::readModel(const char* filename) {  // function reading in Blender model
    ifstream in_stream;
    string line;
    in_stream.open(filename);  // open file

    // get points, triangles, and objects
    in_stream >> line;
    int nPoints = atoi(line.c_str());
    in_stream >> line;
    int nPolys = atoi(line.c_str());
    in_stream >> line;
    int nObjects = atoi(line.c_str());
    
    double vertices[nPoints*3];
    int triangles[nPolys*3];
    int mtlIndices[nObjects];
    double mtls[nObjects*3];
    
    // parse vertex list
    in_stream >> line;
    if (!line.compare("vertices")) {
        for (int i = 0; i < nPoints*3; i++) {
            in_stream >> line;
            vertices[i] = atof(line.c_str());
        }
    }
    
    // parse triangle list
    in_stream >> line;
    if (!line.compare("triangles")) {
        for (int i = 0; i < nPolys*3; i++) {
            in_stream >> line;
            triangles[i] = atoi(line.c_str());
        }
    }
    
    // parse indices for changing materials among a single mesh
    in_stream >> line;
    if (!line.compare("highestTris")) {
        for (int i = 1; i < nObjects; i++) {
            in_stream >> line;
            mtlIndices[i] = atoi(line.c_str());
        }
    }
    mtlIndices[0] = 0;
    
    // parse materials list. this list is comprised of the diffuse colors
    // for each part of the mesh
    in_stream >> line;
    if (!line.compare("materials")) {
        for (int i = 0; i < nObjects*3; i++) {
            in_stream >> line;
            mtls[i] = atof(line.c_str());
        }
    }
    
    in_stream.close();
    // build mesh object with parsed data
    setMeshData(vertices,triangles,mtlIndices,mtls,nPoints,nPolys,nObjects);
}

void Mesh::draw(double dt) {
    glPushMatrix();
    // draw the mesh based on the triangles and the blended colors of each
    // point as specified by phong shading
    glEnable(GL_BLEND);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < this->tris.size(); i++) {
        MeshTriangle *tri = tris.at(i);
        // get material color
        int mtlIndex = 0;
        for (int j = 0; j < this->mtlIndices.size(); j++) {
            if (i < this->mtlIndices.at(j)) {
                mtlIndex = j;
                break;
            }
        }
        double r = this->mtls.at(3 * mtlIndex);
        double g = this->mtls.at(3 * mtlIndex + 1);
        double b = this->mtls.at(3 * mtlIndex + 2);
        glColor3d(r,g,b);
        
        
        glVertex3f(tri->x0,tri->y0,tri->z0);
        glVertex3f(tri->x1,tri->y1,tri->z1);
        glVertex3f(tri->x2,tri->y2,tri->z2);
    }
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
}

bool Mesh::intersect(double xpos, double ypos, double zpos,
        double xcam, double ycam, double zcam, vector<Light*> *lights, bool isFire) {
    // check if the mesh intersects another mesh.
    for (int i = 0; i < this->tris.size(); i++) {
        if (tris.at(i)->burnt) continue;
        if (tris.at(i)->intersect(xpos,ypos,zpos,xcam,ycam,zcam,lights,isFire)) {
            return true;
        }
    }
    return false;
}


