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


//point  list
struct point_list
{
    int                 pp_num;             //number of points stored
    int                 pp_alloc;           //size allocated
    
    struct point        *pp;                //array
};

//point
struct point
{
    float               x[3];               //coords
    float               v;                  //value
};


//counter (for debug)
struct counter
{
    int                 vtx_int[5][5][2];      //a counter int and path
};

//element
struct element
{
    int                 idx;                //counter
    int                 pos[3];             //position in the element space (i,j,k)
    float               vlm_loc;            //ele volume (for subtotal and debug)
    
    float               vtx_glb[8][3];      //vertex global coords
    float               vtx_sdf[8];         //signed distance function
    
    float               bas_aa[8];          //interpolated basis function coefficients
    
    int                 vtx_int_tot;        //count internal verts
    int                 fac_vtx_int[3][2];  //count internal verts per face (dim [0,1,2],co-ord [0,1])
    
    int                 bf_dim;             //base face dimension for integration
    int                 bf_crd;             //base face coord for integration

    int                 vtx_idx[4];         //selected vertices for quadrature functions
    
    struct counter      ctr;                //counter for logic debug
};


//geometry
struct geometry
{
    float               cc[GEO_NC][3];      //coords
    float               rr[GEO_NC];         //radii
    
    float               dof_sdf[MSH_ELE_DIM_0+1][MSH_ELE_DIM_1+1][MSH_ELE_DIM_2+1]; //sdf data
};

//problem
struct problem
{
    struct  scheme      scm;                //quadrature scheme
    struct  mesh        msh;                //mesh
    struct  element     ele;                //ele
    struct  geometry    geo;                //geometry data


    float               vlm[3];             //volume (with subtotals)
    

    struct  point_list        lst1;               //lists for debug
    struct  point_list        lst2;
    struct  point_list        lst3;
    struct  point_list        lst4;
};


#endif /* struct_h */
