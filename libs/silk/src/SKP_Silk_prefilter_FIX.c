

/***********************************************************************

Copyright (c) 2006-2010, Skype Limited. All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, (subject to the limitations in the disclaimer below)
are permitted provided that the following conditions are met:
- Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
- Neither the name of Skype Limited, nor the names of specific
contributors, may be used to endorse or promote products derived from
this software without specific prior written permission.
NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED
BY THIS LICENSE.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

***********************************************************************/

#include "SKP_Silk_main_FIX.h"
#include "SKP_Silk_perceptual_parameters_FIX.h"

/* SKP_Silk_prefilter. Prefilter for finding Quantizer input signal */
SKP_INLINE void SKP_Silk_prefilt_FIX(
    SKP_Silk_prefilter_state_FIX *P,                    /* I/O state                          */
    SKP_int32   st_res_Q12[],                           /* I short term residual signal       */
    SKP_int16   xw[],                                   /* O prefiltered signal               */
    SKP_int32   HarmShapeFIRPacked_Q12,                 /* I Harmonic shaping coeficients     */
    SKP_int     Tilt_Q14,                               /* I Tilt shaping coeficient          */
    SKP_int32   LF_shp_Q14,                             /* I Low-frequancy shaping coeficients*/
    SKP_int     lag,                                    /* I Lag for harmonic shaping         */
    SKP_int     length                                  /* I Length of signals                */
);

void SKP_Silk_prefilter_FIX(





    SKP_Silk_encoder_state_FIX          *psEnc,         /* I/O  Encoder state FIX                           */
    const SKP_Silk_encoder_control_FIX  *psEncCtrl,     /* I    Encoder control FIX                         */
    SKP_int16                           xw[],           /* O    Weighted signal                             */
    const SKP_int16                     x[]             /* I    Speech signal                               */
)
{
    SKP_Silk_prefilter_state_FIX *P = &psEnc->sPrefilt;
    SKP_int   j, k, lag;
    SKP_int32 tmp_32, B_Q12;
    const SKP_int16 *AR1_shp_Q13;
    const SKP_int16 *px;
    SKP_int16 *pxw, *pst_res;
    SKP_int   HarmShapeGain_Q12, Tilt_Q14, LF_shp_Q14;
    SKP_int32 HarmShapeFIRPacked_Q12;
    SKP_int32 x_filt_Q12[ MAX_FRAME_LENGTH / NB_SUBFR ], filterState[ MAX_LPC_ORDER ];
    SKP_int16 st_res[ ( MAX_FRAME_LENGTH / NB_SUBFR ) + MAX_LPC_ORDER ];

    /* Setup pointers */
    px  = x;
    pxw = xw;
    lag = P->lagPrev;
    for( k = 0; k < NB_SUBFR; k++ ) {
        /* Update Variables that change per sub frame */
        if( psEncCtrl->sCmn.sigtype == SIG_TYPE_VOICED ) {
            lag = psEncCtrl->sCmn.pitchL[ k ];
        }

        /* Noise shape parameters */
        HarmShapeGain_Q12 = SKP_SMULWB( psEncCtrl->HarmShapeGain_Q14[ k ], 16384 - psEncCtrl->HarmBoost_Q14[ k ] );
        SKP_assert( HarmShapeGain_Q12 >= 0 );
        HarmShapeFIRPacked_Q12  =                        SKP_RSHIFT( HarmShapeGain_Q12, 2 );
        HarmShapeFIRPacked_Q12 |= SKP_LSHIFT( (SKP_int32)SKP_RSHIFT( HarmShapeGain_Q12, 1 ), 16 );
        Tilt_Q14    = psEncCtrl->Tilt_Q14[   k ];
        LF_shp_Q14  = psEncCtrl->LF_shp_Q14[ k ];
        AR1_shp_Q13 = &psEncCtrl->AR1_Q13[   k * SHAPE_LPC_ORDER_MAX ];

        /* Short term FIR filtering*/
        SKP_memset( filterState, 0, psEnc->sCmn.shapingLPCOrder * sizeof( SKP_int32 ) );
        SKP_Silk_MA_Prediction_Q13( px - psEnc->sCmn.shapingLPCOrder, AR1_shp_Q13, filterState,
            st_res, psEnc->sCmn.subfr_length + psEnc->sCmn.shapingLPCOrder, psEnc->sCmn.shapingLPCOrder );

        pst_res = st_res + psEnc->sCmn.shapingLPCOrder; /* Point to first sample */

        /* reduce (mainly) low frequencies during harmonic emphasis */
        B_Q12 = SKP_RSHIFT_ROUND( psEncCtrl->GainsPre_Q14[ k ], 2 );
        tmp_32 = SKP_SMLABB( INPUT_TILT_Q26, psEncCtrl->HarmBoost_Q14[ k ], HarmShapeGain_Q12 ); /* Q26 */
        tmp_32 = SKP_SMLABB( tmp_32, psEncCtrl->coding_quality_Q14, HIGH_RATE_INPUT_TILT_Q12 );  /* Q26 */
        tmp_32 = SKP_SMULWB( tmp_32, -psEncCtrl->GainsPre_Q14[ k ] );                            /* Q24 */





        tmp_32 = SKP_RSHIFT_ROUND( tmp_32, 12 );                                                 /* Q12 */
        B_Q12 |= SKP_LSHIFT( SKP_SAT16( tmp_32 ), 16 );

        /* NOTE: the code below loads two int16 values in an int32, and multiplies each using the   */
        /* SMLABB and SMLABT instructions. On a big-endian CPU the two int16 variables would be     */
        /* loaded in reverse order and the code will give the wrong result. In that case swapping   */
        /* the SMLABB and SMLABT instructions should solve the problem.                             */
        x_filt_Q12[ 0 ] = SKP_SMLABT( SKP_SMULBB( pst_res[ 0 ], B_Q12 ), P->sHarmHP, B_Q12 );
        for( j = 1; j < psEnc->sCmn.subfr_length; j++ ) {
            x_filt_Q12[ j ] = SKP_SMLABT( SKP_SMULBB( pst_res[ j ], B_Q12 ), pst_res[ j - 1 ], B_Q12 );
        }
        P->sHarmHP = pst_res[ psEnc->sCmn.subfr_length - 1 ];

        SKP_Silk_prefilt_FIX( P, x_filt_Q12, pxw, HarmShapeFIRPacked_Q12, Tilt_Q14,
            LF_shp_Q14, lag, psEnc->sCmn.subfr_length );

        px  += psEnc->sCmn.subfr_length;
        pxw += psEnc->sCmn.subfr_length;
    }

    P->lagPrev = psEncCtrl->sCmn.pitchL[ NB_SUBFR - 1 ];
}

/* SKP_Silk_prefilter. Prefilter for finding Quantizer input signal                           */
SKP_INLINE void SKP_Silk_prefilt_FIX(
    SKP_Silk_prefilter_state_FIX *P,                    /* I/O state                          */
    SKP_int32   st_res_Q12[],                           /* I short term residual signal       */
    SKP_int16   xw[],                                   /* O prefiltered signal               */
    SKP_int32   HarmShapeFIRPacked_Q12,                 /* I Harmonic shaping coeficients     */
    SKP_int     Tilt_Q14,                               /* I Tilt shaping coeficient          */
    SKP_int32   LF_shp_Q14,                             /* I Low-frequancy shaping coeficients*/
    SKP_int     lag,                                    /* I Lag for harmonic shaping         */
    SKP_int     length                                  /* I Length of signals                */
)
{
    SKP_int   i, idx, LTP_shp_buf_idx;
    SKP_int32 n_LTP_Q12, n_Tilt_Q10, n_LF_Q10;
    SKP_int32 sLF_MA_shp_Q12, sLF_AR_shp_Q12;
    SKP_int16 *LTP_shp_buf;

    /* To speed up use temp variables instead of using the struct */
    LTP_shp_buf     = P->sLTP_shp1;
    LTP_shp_buf_idx = P->sLTP_shp_buf_idx1;
    sLF_AR_shp_Q12  = P->sLF_AR_shp1_Q12;
    sLF_MA_shp_Q12  = P->sLF_MA_shp1_Q12;

    for( i = 0; i < length; i++ ) {
        if( lag > 0 ) {





            /* unrolled loop */
            SKP_assert( HARM_SHAPE_FIR_TAPS == 3 );
            idx = lag + LTP_shp_buf_idx;
            n_LTP_Q12 = SKP_SMULBB(            LTP_shp_buf[ ( idx - HARM_SHAPE_FIR_TAPS / 2 - 1) & LTP_MASK ], HarmShapeFIRPacked_Q12 );
            n_LTP_Q12 = SKP_SMLABT( n_LTP_Q12, LTP_shp_buf[ ( idx - HARM_SHAPE_FIR_TAPS / 2    ) & LTP_MASK ], HarmShapeFIRPacked_Q12 );
            n_LTP_Q12 = SKP_SMLABB( n_LTP_Q12, LTP_shp_buf[ ( idx - HARM_SHAPE_FIR_TAPS / 2 + 1) & LTP_MASK ], HarmShapeFIRPacked_Q12 );
        } else {
            n_LTP_Q12 = 0;
        }

        n_LF_Q10   = SKP_SMLAWB( SKP_SMULWT( sLF_AR_shp_Q12, LF_shp_Q14 ), sLF_MA_shp_Q12, LF_shp_Q14 );
        n_Tilt_Q10 = SKP_SMULWB( sLF_AR_shp_Q12, Tilt_Q14 );

        sLF_AR_shp_Q12 = SKP_SUB32( st_res_Q12[ i ], SKP_LSHIFT( n_Tilt_Q10, 2 ) );
        sLF_MA_shp_Q12 = SKP_SUB32( sLF_AR_shp_Q12,  SKP_LSHIFT( n_LF_Q10,   2 ) );

        LTP_shp_buf_idx                = ( LTP_shp_buf_idx - 1 ) & LTP_MASK;
        LTP_shp_buf[ LTP_shp_buf_idx ] = (SKP_int16)SKP_SAT16( SKP_RSHIFT_ROUND( sLF_MA_shp_Q12, 12 ) );

        xw[i] = (SKP_int16)SKP_SAT16( SKP_RSHIFT_ROUND( SKP_SUB32( sLF_MA_shp_Q12, n_LTP_Q12 ), 12 ) );
    }

    /* Copy temp variable back to state */
    P->sLF_AR_shp1_Q12   = sLF_AR_shp_Q12;
    P->sLF_MA_shp1_Q12   = sLF_MA_shp_Q12;
    P->sLTP_shp_buf_idx1 = LTP_shp_buf_idx;
}


