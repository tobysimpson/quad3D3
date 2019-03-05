//
//  const.h
//  quad3D3
//
//  Created by Toby Simpson on 11.12.18.
//  Copyright © 2018 Toby Simpson. All rights reserved.
//

#ifndef const_h
#define const_h

/*
 ===============================
 constants
 ===============================
 */


#define                     MIN(a,b) (((a)<(b))?(a):(b))                    //max and min functions
#define                     MAX(a,b) (((a)>(b))?(a):(b))

//various
#define SCM_NP              3                                               //quad points per scheme
#define GEO_NC              100                                             //number of centres for geometry
#define FILE_ROOT           "/Users/toby/Downloads"                         //write path

//mesh dims
#define MSH_XMIN_0          -1                                              //mesh range
#define MSH_XMIN_1          -1
#define MSH_XMIN_2          -1

#define MSH_XMAX_0          +1
#define MSH_XMAX_1          +1
#define MSH_XMAX_2          +1

//num elements
#define MSH_ELE_DIM         64

#define MSH_ELE_DIM_0       MSH_ELE_DIM                                     //mesh elements per dimension
#define MSH_ELE_DIM_1       MSH_ELE_DIM
#define MSH_ELE_DIM_2       MSH_ELE_DIM

#define MSH_ELE_TOT         (MSH_ELE_DIM_0*MSH_ELE_DIM_1*MSH_ELE_DIM_2)     //total num elements


#endif /* const_h */
