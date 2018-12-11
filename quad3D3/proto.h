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
void float3_emul_int(float *v1,int *v2, float *res);

float float3_eprd(float *v1);   

float float3_nrm(float *v1);
float float3_nrmp(float *v1, float p);

//mesh.c
void msh_init(struct problem *prb);

//element.c
void ele_calc(struct problem *prb);
void ele_vtx_gen(struct problem *prb);

//list.c
void lst_init(struct point_list *lst);
void lst_add(struct point_list *lst, struct point pt);
void lst_write(struct point_list *lst, char file_name[100]);

//geometry
void geo_sdf(struct point *pt);

//quadrature.c
float quad_ele(struct problem *prb);
void quad_fac(struct problem *prb);

#endif /* proto_h */
