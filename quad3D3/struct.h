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


//list (for debug)
struct list
{
    int                 pp_num;             //number of points stored
    int                 pp_alloc;           //size allocated
    
    float               *pp;                //array of points
};


//element
struct element
{
    int                 idx;                //counter
    int                 pos[3];             //position in the element space (i,j,k)
    
    float               vtx_glb[8][3];      //vertices
    float               vtx_sdf[8];         //signed distance function
    
    int                 vtx_int;            //count internal verts
    int                 fac_vtx_int[3][2];  //count internal verts per face (dim [0,1,2],co-ord [0,1])
};

//problem
struct problem
{
    struct  scheme      scm;                //quadrature scheme
    struct  mesh        msh;                //mesh
    struct  element     ele;                //ele
    
//    struct  quadpoint qpt;                //quadrature point

    float               vlm;                //volume total
    
    struct  list        lst1;               //all verts
    struct  list        lst2;
    struct  list        lst3;
    struct  list        lst4;
};


#endif /* struct_h */
