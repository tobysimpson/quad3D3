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
    float               msh_vlm;            //mesh volume
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
    float               vlm_loc;            //ele volume (for subtotal and debug)
    
    float               vtx_glb[8][3];      //vertex global coords
    float               vtx_sdf[8];         //signed distance function
    
    int                 vtx_int_tot;        //count internal verts
    int                 fac_vtx_int[3][2];  //count internal verts per face (dim [0,1,2],co-ord [0,1])
    
    int                 fac_int_flg;        //internal face search results
    int                 fac_int_dim;
    int                 fac_int_crd;

    int                 fac_ext_flg;        //external face search results
    int                 fac_ext_dim;
    int                 fac_ext_crd;
    
    int                 fac_vtx[4];         //vertex indices for a given face
    
    int                 vtx_idx[4];         //selected vertices for quadrature functions
    
    float               bas_aa[8];          //interpolated basis function coefficients
};

//problem
struct problem
{
    struct  scheme      scm;                //quadrature scheme
    struct  mesh        msh;                //mesh
    struct  element     ele;                //ele
    
//    struct  quadpoint qpt;                //quadrature point

    float               vlm[3];             //volume (with subtotals)

    
    struct  list        lst1;               //lists for debug
    struct  list        lst2;
    struct  list        lst3;
    struct  list        lst4;
};


#endif /* struct_h */
