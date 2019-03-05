//
//  float3.c
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#include "master.h"


//scalar add
void float3_sadd(float *v1, float s, float *res)
{
    for(int i=0; i<3; i++)
    {
        res[i] = v1[i] + s;
    }
    return;
}

//scalar multiply
void float3_smul(float *v1, float s, float *res)
{
    for(int i=0; i<3; i++)
    {
        res[i] = v1[i] * s;
    }
    return;
}

//scalar divide
void float3_sdiv(float *v1, float s, float *res)
{
    for(int i=0; i<3; i++)
    {
        res[i] = v1[i] / s;
    }
    return;
}

//elementwise add
void float3_eadd(float *v1,float *v2, float *res)
{
    for(int i=0; i<3; i++)
    {
        res[i] = v1[i] + v2[i];
    }
    return;
}

//elementwise subtract
void float3_esub(float *v1,float *v2, float *res)
{
    for(int i=0; i<3; i++)
    {
        res[i] = v1[i] - v2[i];
    }
    return;
}


//elementwise multiply
void float3_emul(float *v1,float *v2, float *res)
{
    for(int i=0; i<3; i++)
    {
        res[i] = v1[i] * v2[i];
    }
    return;
}

//elementwise multiply by integers
void float3_emul_int3(float *v1,int *v2, float *res)
{
    for(int i=0; i<3; i++)
    {
        res[i] = v1[i] * v2[i];
    }
    return;
}


//elementwise divide
void float3_ediv(float *v1,float *v2, float *res)
{
    for(int i=0; i<3; i++)
    {
        res[i] = v1[i] / v2[i];
    }
    return;
}


//element sum
float float3_esum(float *v1)
{
    float s = 0;
    
    for(int i=0; i<3; i++)
    {
        s += v1[i];
    }
    return s;
}


//element product
float float3_eprd(float *v1)
{
    float p = 1;
    
    for(int i=0; i<3; i++)
    {
        p *= v1[i];
    }
    return p;
}

//dot product
float float3_dot(float *v1,float *v2)
{
    float s = 0;
    
    for(int i=0; i<3; i++)
    {
        s += v1[i] * v2[i];
    }
    return s;
}

//2-norm
float float3_nrm(float *v1)
{
    float s = 0;
    
    for(int i=0; i<3; i++)
    {
        s += v1[i] * v1[i];
    }
    return sqrtf(s);
}

//p-norm
float float3_nrmp(float *v1, float p)
{
    float s = 0;
    
    for(int i=0; i<3; i++)
    {
        s += powf(fabs(v1[i]),p);
    }
    return powf(s,1e0/p);
}


