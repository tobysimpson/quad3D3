//
//  struct.h
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#ifndef struct_h
#define struct_h

/*
 ===============================
 structures
 ===============================
 */

//quadrature scheme
struct scheme
{
    int                 np;                 //num points
    float               pp[5];              //quadrature points (max is 5)
    float               ww[5];              //quadrature weights
};

//mesh
struct mesh
{
    float               xmin[3];            //mesh coord range
    float               xmax[3];
    
    float               msh_h[3];           //mesh coordinate dimensions
    float               ele_h[3];           //element coordinate dimensions
    float               ele_vlm;            //element volume
};

//point
struct point
{
    float               x[3];               //coords
    float               v;                  //scalar value
};

//point  list (for debug)
struct point_list
{
    int                 pp_num;             //number of points stored
    int                 pp_alloc;           //size allocated
    
    struct point        *pp;                //array of points
};

//point
struct quadpoint
{
    struct point        loc;                //local coords and weight
    struct point        glb;                //global coords
};

//element
struct element
{
    int                 idx;                //counter
    int                 pos[3];             //position in the element space (i,j,k)
    
    struct point        vtx[2][2][2];       //vertices
    int                 vtx_int;            //internal verts
};

//problem
struct problem
{
    struct  scheme      scm;                //quadrature scheme
    struct  mesh        msh;                //mesh
    struct  element     ele;                //ele
    
    struct  quadpoint   qpt;                //quadrature point

    float               vlm;                //volume total
    
    struct  point_list  lst1;               //verts
    struct  point_list  lst2;               //qpts
};


#endif /* struct_h */