///
/// @file mb71_msg.c
/// @authors k. headley
/// @date 08 aug 2019

/// MB-System record format 71 (FBT)

/// @sa doxygen-examples.c for more examples of Doxygen markup


/////////////////////////
// Terms of use
/////////////////////////
/*
 Copyright Information
 
 Copyright 2002-2019 MBARI
 Monterey Bay Aquarium Research Institute, all rights reserved.
 
 Terms of Use
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version. You can access the GPLv3 license at
 http://www.gnu.org/licenses/gpl-3.0.html
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details
 (http://www.gnu.org/licenses/gpl-3.0.html)
 
 MBARI provides the documentation and software code "as is", with no warranty,
 express or implied, as to the software, title, non-infringement of third party
 rights, merchantability, or fitness for any particular purpose, the accuracy of
 the code, or the performance or results which you may obtain from its use. You
 assume the entire risk associated with use of the code, and you agree to be
 responsible for the entire cost of repair or servicing of the program with
 which you are using the code.
 
 In no event shall MBARI be liable for any damages, whether general, special,
 incidental or consequential damages, arising out of your use of the software,
 including, but not limited to, the loss or corruption of your data or damages
 of any kind resulting from use of the software, any prohibited use, or your
 inability to use the software. You agree to defend, indemnify and hold harmless
 MBARI and its officers, directors, and employees against any claim, loss,
 liability or expense, including attorneys' fees, resulting from loss of or
 damage to property or the injury to or death of any person arising out of the
 use of the software.
 
 The MBARI software is provided without obligation on the part of the
 Monterey Bay Aquarium Research Institute to assist in its use, correction,
 modification, or enhancement.
 
 MBARI assumes no responsibility or liability for any third party and/or
 commercial software required for the database or applications. Licensee agrees
 to obtain and maintain valid licenses for any additional third party software
 required.
 */

/////////////////////////
// Headers 
/////////////////////////

/////////////////////////
// Macros
/////////////////////////
#define MB171_MSG_NAME "mb71_msg"
#ifndef MB171_MSG_BUILD
/// @def MB171_MSG_BUILD
/// @brief module build date.
/// Sourced from CFLAGS in Makefile
/// w/ -DMBTRN_BUILD=`date`
#define MB171_MSG_BUILD ""VERSION_STRING(APP_BUILD)
#endif

// These macros should only be defined for 
// application main files rather than general C files
/*
 /// @def PRODUCT
 /// @brief header software product name
 #define PRODUCT "TBD_PRODUCT"
 
 /// @def COPYRIGHT
 /// @brief header software copyright info
 #define COPYRIGHT "Copyright 2002-2019 MBARI Monterey Bay Aquarium Research Institute, all rights reserved."
 /// @def NOWARRANTY
 /// @brief header software terms of use
 #define NOWARRANTY  \
 "This program is distributed in the hope that it will be useful,\n"\
 "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"\
 "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"\
 "GNU General Public License for more details (http://www.gnu.org/licenses/gpl-3.0.html)\n"
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <memory.h>

#include "mb71_msg.h"
#include "mswap.h"

/////////////////////////
// Declarations 
/////////////////////////


/////////////////////////
// Imports
/////////////////////////

/////////////////////////
// Module Global Variables
/////////////////////////

/////////////////////////
// Function Definitions
/////////////////////////

void mb71v5_show(mb71v5_t *self, bool verbose, uint16_t indent)
{
    if (NULL != self) {
        fprintf(stderr,"%*s[self           %10p]\n",indent,(indent>0?" ":""), self);
        fprintf(stderr,"%*s[recordtype     %6s%04X]\n",indent,(indent>0?" ":"")," ", self->recordtype);
        fprintf(stderr,"%*s[time_d     %10.3lf]\n",indent,(indent>0?" ":""), self->time_d);
        fprintf(stderr,"%*s[longitude      %10.3lf]\n",indent,(indent>0?" ":""), self->longitude);
        fprintf(stderr,"%*s[latitude       %10.3lf]\n",indent,(indent>0?" ":""), self->latitude);
        fprintf(stderr,"%*s[sonardepth     %10.3lf]\n",indent,(indent>0?" ":""), self->sonardepth);
        fprintf(stderr,"%*s[altitude       %10.3lf]\n",indent,(indent>0?" ":""), self->altitude);
        fprintf(stderr,"%*s[heading        %10.3lf]\n",indent,(indent>0?" ":""), self->heading);
        fprintf(stderr,"%*s[speed          %10.3lf]\n",indent,(indent>0?" ":""), self->speed);
        fprintf(stderr,"%*s[roll           %10.3lf]\n",indent,(indent>0?" ":""), self->roll);
        fprintf(stderr,"%*s[pitch          %10.3lf]\n",indent,(indent>0?" ":""), self->pitch);
        fprintf(stderr,"%*s[heave          %10.3lf]\n",indent,(indent>0?" ":""), self->heave);
        fprintf(stderr,"%*s[beam_xwidth    %10.3lf]\n",indent,(indent>0?" ":""), self->beam_xwidth);
        fprintf(stderr,"%*s[beam_lwidth    %10.3lf]\n",indent,(indent>0?" ":""), self->beam_lwidth);
        fprintf(stderr,"%*s[beams_bath     %10d]\n",indent,(indent>0?" ":""), self->beams_bath);
        fprintf(stderr,"%*s[beams_amp      %10d]\n",indent,(indent>0?" ":""), self->beams_amp);
        fprintf(stderr,"%*s[pixels_ss      %10d]\n",indent,(indent>0?" ":""), self->pixels_ss);
        fprintf(stderr,"%*s[spare1         %10d]\n",indent,(indent>0?" ":""), self->spare1);
        fprintf(stderr,"%*s[depth_scale    %10.6lf]\n",indent,(indent>0?" ":""), self->depth_scale);
        fprintf(stderr,"%*s[distance_scale %10.6lf]\n",indent,(indent>0?" ":""), self->distance_scale);
        fprintf(stderr,"%*s[ss_scalepower  %8s%02X]\n",indent,(indent>0?" ":""), " ",self->ss_scalepower);
        fprintf(stderr,"%*s[ss_type        %8s%02X]\n",indent,(indent>0?" ":""), " ",self->ss_type);
        fprintf(stderr,"%*s[imagery_type   %8s%02X]\n",indent,(indent>0?" ":""), " ",self->imagery_type);
        fprintf(stderr,"%*s[topo_type      %8s%02X]\n",indent,(indent>0?" ":""), " ",self->topo_type);
        int nbeams =self->beams_bath;
        if(nbeams>0){
        unsigned char *bf = MB71_PBF(self,nbeams);
        short *bz = MB71_PBZ(self,nbeams);
        short *by = MB71_PBY(self,nbeams);
        short *bx = MB71_PBX(self,nbeams);

        fprintf(stderr,"%*s[ n   flags vert    cross      along]\n",indent+3,(indent>0?" ":""));

        for(int i=0;i<self->beams_bath;i++){
            fprintf(stderr,"%*s[%3d  %02X,%8hd,%8hd,%8hd ]\n",indent+3,(indent>0?" ":""), i, bf[i],bz[i],by[i],bx[i]);
        }
        }

    }
}
// End function mb71v5_show

int mb71v5_bswap(mb71v5_t *dest, mb71v5_t *src)
{
    int retval=-1;
    
    if(NULL!=src  && src->beams_bath>0){
        // save nbeams before it's swapped
        int nbeams=src->beams_bath;
        mb71v5_t *out = (NULL==dest ? src : dest);
//        uint64_t u64[2]={0};
//        uint32_t u32[2]={0};
//        uint16_t u16[2]={0};
        
        out->recordtype = mswap_16(src->recordtype);
        if(out==src){
            // swap in place
            mswap_bytes(&out->time_d,8);
            mswap_bytes(&out->longitude,8);
            mswap_bytes(&out->latitude,8);
            mswap_bytes(&out->sonardepth,8);
            mswap_bytes(&out->altitude,8);
            
            mswap_bytes(&out->heading,4);
            mswap_bytes(&out->speed,4);
            mswap_bytes(&out->roll,4);
            mswap_bytes(&out->pitch,4);
            mswap_bytes(&out->heave,4);
            mswap_bytes(&out->beam_xwidth,4);
            mswap_bytes(&out->beam_lwidth,4);
       }else{
           // swap into memory (don't change source)
            mswap_bytes_mem(&out->time_d,    &src->time_d,8);
            mswap_bytes_mem(&out->longitude, &src->longitude,8);
            mswap_bytes_mem(&out->latitude,  &src->latitude,8);
            mswap_bytes_mem(&out->sonardepth,&src->sonardepth,8);
            mswap_bytes_mem(&out->altitude,  &src->altitude,8);
            
            mswap_bytes_mem(&out->heading,&src->heading,4);
            mswap_bytes_mem(&out->speed,  &src->speed,4);
            mswap_bytes_mem(&out->roll,   &src->roll,4);
            mswap_bytes_mem(&out->pitch,  &src->pitch,4);
            mswap_bytes_mem(&out->heave,  &src->heave,4);
            mswap_bytes_mem(&out->beam_xwidth,&src->beam_xwidth,4);
            mswap_bytes_mem(&out->beam_lwidth,&src->beam_lwidth,4);
        }

        out->beams_bath = mswap_32(src->beams_bath);
        out->beams_amp  = mswap_32(src->beams_amp);
        out->pixels_ss  = mswap_32(src->pixels_ss);
        out->spare1     = mswap_32(src->spare1);

        out->depth_scale = mswap_16(src->depth_scale);
        out->distance_scale  = mswap_16(src->distance_scale);
        
        // no swap ss_scalepower (1 byte)
        // no swap ss_type (1 byte)
        // no swap imagery_type (1 byte)
        // no swap topo_type (1 byte)
        
        // no swap flags (1 byte)

        short *bz = MB71_PBZ(src,nbeams);
        short *by = MB71_PBY(src,nbeams);
        short *bx = MB71_PBX(src,nbeams);
        short *obz = MB71_PBZ(out,nbeams);
        short *oby = MB71_PBY(out,nbeams);
        short *obx = MB71_PBX(out,nbeams);

        int i=0;
		for(i=0;i<nbeams;i++){
            obz[i] = mswap_16(bz[i]);
            oby[i] = mswap_16(by[i]);
            obx[i] = mswap_16(bx[i]);
        }
        retval=0;
    }
    return retval;
}
