/* 
 * File:   Weapon.h
 * Author: John
 *
 * Created on May 4, 2013, 3:03 AM
 */

#ifndef WEAPON_H
#define	WEAPON_H

#include "Mesh.h"

class Mesh;

class Weapon : public Mesh {
public:
    Weapon(const char *filename, double xpos, double ypos, double zpos,
            double xcam, double ycam, double zcam);
    virtual ~Weapon();
    double xcam;
    double ycam;
    double zcam;
    virtual void draw(double dt);
    double t;
    bool collided;
    bool initCollision;
    virtual void setCollisionTrajectories();
    virtual bool isFirebomb();
private:

};

#endif	/* WEAPON_H */

