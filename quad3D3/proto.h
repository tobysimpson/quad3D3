//
//  proto.h
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#ifndef proto_h
#define proto_h

/*
 ===============================
 funciton prototypes
 ===============================
 */

//scheme.c
void scm_init(struct problem *prb);

//float3.c
void float3_eadd(float *v1,float *v2, float *res);
void float3_esub(float *v1,float *v2, float *res);

void float3_emul(float *v1,float *v2, float *res);
void float3_emul_int3(float *v1,int *v2, float *res);

float float3_eprd(float *v1);   

float float3_nrm(float *v1);
float float3_nrmp(float *v1, float p);

//mesh.c
void msh_init(struct problem *prb);

//element.c
void ele_calc(struct problem *prb);
void fac_get_vtx(struct problem *prb, int fac_dim, int fac_crd, int vtx_intext, int vtx_num);
int  vtx_adj(struct problem *prb);

//list.c
void lst_init(struct list *lst);
void lst_add(struct list *lst, const float x[3], const float s);
void lst_add_ele(struct list *lst, struct problem *prb);
void lst_write(struct list *lst, char file_name[20]);

//geometry
void geo_init(struct problem *prb);
float geo_sdf(struct problem *prb, float x[3]);

//quadrature.c
float quad_ele(struct problem *prb);
float quad_vtx1(struct problem *prb);
float quad_vtx2(struct problem *prb);
float quad_vtx3(struct problem *prb);
float quad_vtx4(struct problem *prb);

float bas_root(struct problem *prb, int dim_idx, float vtx_loc[3]);

//path
int ele_pth_test(struct problem *prb);
int pth_calc(struct problem *prb, int vtx_idx);
void pth_rec(struct problem *prb, int vtx_idx1, int *pth_vtx, int *pth_num);

//write
void write_dof_raw(struct problem *prb);

#endif /* proto_h */
