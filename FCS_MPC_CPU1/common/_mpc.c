#include "F2837xD_device.h"
#include "F2837xD_Examples.h"
#include "stdint.h"
#include "string.h"
#include "math.h"
#include "fpu_vector.h"
#include "fpu_types.h"
#include "dotp.h"

#include <DSP_setup_gpios.h>
#include <mpc.h>

#pragma DATA_SECTION(iagrid,"CLAData");
#pragma DATA_SECTION(ibgrid,"CLAData");
#pragma DATA_SECTION(icgrid,"CLAData");
#pragma DATA_SECTION(iaconv,"CLAData");
#pragma DATA_SECTION(ibconv,"CLAData");
#pragma DATA_SECTION(icconv,"CLAData");
#pragma DATA_SECTION(vabgrid,"CLAData");
#pragma DATA_SECTION(vbcgrid,"CLAData");
#pragma DATA_SECTION(vcagrid,"CLAData");
#pragma DATA_SECTION(vagrid,"CLAData");
#pragma DATA_SECTION(vbgrid,"CLAData");
#pragma DATA_SECTION(vcgrid,"CLAData");
#pragma DATA_SECTION(vaconv,"CLAData");
#pragma DATA_SECTION(vbconv,"CLAData");
#pragma DATA_SECTION(vcconv,"CLAData");
#pragma DATA_SECTION(vcc,"CLAData");


float iagrid,ibgrid,icgrid;    // Correntes da rede (abc)
float iaconv,ibconv,icconv;    // Correntes do conversor (abc)
float vabgrid,vbcgrid,vcagrid; // Tensões da rede no ponto de conexão (abc)
float vagrid,vbgrid,vcgrid;             // Tensões de fase da rede (após transformação)
float vaconv,vbconv,vcconv;    // Tensão dos capacitores (abc)
float vcc;                           // Tensão do barramento CC
float inv_vcc=1;                        // Inverso da tensão do barramento CC

// Variáveis medidas transformadas
float ialfagrid,ibetagrid;  		// Correntes da rede (\alfa\beta)
float ialfaconv,ibetaconv;  		// Correntes de saída do conversor (alpha\beta)
float valfagrid,vbetagrid;  		// Tensões da rede no ponto de conexão (\alfa\beta)
float valfaconv,vbetaconv;  		// Tensões dos capacitores (\alpha\beta)


float dois_PI = 6.2831853071796;

// -- Inputs
float vabgrid, vbcgrid, iaconv, ibconv, icconv, vaconv, vbconv, vcconv;
float iagrid, ibgrid, icgrid;

// -- Intermediate variables
float vagrid, vbgrid, vcgrid, ialfaconv, ibetaconv, valfaconv;
float vbetaconv, valfagrid, vbetagrid, ialfagrid, ibetagrid;
float wt, pi;

// --  Finite Control Set  MPC Current Controller variables
float ialfaconv_km1, ibetaconv_km1, w, theta, Ial_ref_km, Ibe_ref_km;
float Ial_ref_km2, Ibe_ref_km2, Jmin, e_km1[2], J, ualpha, ubeta;
float E, r, L, T;
int j, index;
float ialfaconv_km2, ibetaconv_km2;

// -- Inverter switching vectors
float ual_s[7], ube_s[7];
int compa[7], compb[7], compc[7];

// -- Output variables
int ua, ub, uc, ua_a, ub_a, uc_a;

// -- Initialization variables
float theta_p, out4;

// -- PLL variables
float wref, fn, wc, wc1, kp_pll, ki_pll, qsi, vdf, vqf, vd, vq;
float wo2, x1_f, x2_f, Ls, freq_r, x1_f_p, x2_f_p, dref, xpll;
float freq_i, dfreq, wn2, wn, norm;

// -- Output control modulator variables
int vags, vbgs, vcgs, vags1, vbgs1, vcgs1, counter, inc, Sa, Sb, Sc, Sa1, Sb1, Sc1;
float Tpwm, TPER, Ts1;

float I_ref = 0;

void mpc(void){

#ifdef CPU1
    GPIO_WritePin(DEBUG_MPC_CPU1, 1);
#endif



    ialfaconv = sqrt(2./3.) * (iaconv-0.5 * ibconv - 0.5 * icconv);
    ibetaconv = sqrt(2./3.) * sqrt(3.)/2. * (ibconv - icconv);

    /*prediction */
    ialfaconv_km1 = ialfaconv - T/L*r*ialfaconv + T/L*ualpha;
    ibetaconv_km1 = ibetaconv - T/L*r*ibetaconv + T/L*ubeta;

    w=2.*pi*60.;
    theta=theta+w*T;
    if (theta>2.*pi) theta = theta-2.*pi;

    Ial_ref_km = I_ref*cos(theta);
    Ibe_ref_km = I_ref*sin(theta);

    Ial_ref_km2 = Ial_ref_km*cos(2.*w*T) + Ibe_ref_km*sin(2.*w*T);
    Ibe_ref_km2 = -Ial_ref_km*sin(2.*w*T) + Ibe_ref_km*cos(2.*w*T);

    // Search for the optimal Switching Vector
    Jmin=1e10;
    for (j=0; j<7; j++){
        ialfaconv_km2 = ialfaconv_km1 - T/L*r*ialfaconv_km1 + T/L*ual_s[j]*E;
        ibetaconv_km2 = ibetaconv_km1 - T/L*r*ibetaconv_km1 + T/L*ube_s[j]*E;
        e_km1[0]=Ial_ref_km2- ialfaconv_km2;
        e_km1[1]=Ibe_ref_km2-ibetaconv_km2;
        J = (e_km1[0]*e_km1[0]+e_km1[1]*e_km1[1]);
        if (J<Jmin){
            Jmin=J;
            index=j;
        }

    }


    /* Inverter voltages to be used in the next sampling period */
    ualpha=ual_s[index]*E;
    ubeta=ube_s[index]*E;

    /*  optimal switching vector  to be implemented   */
    ua=compa[index];
    ub=compb[index];
    uc=compc[index];

    /* Output quantities */
    out4 = sqrt(2./3.) * Ial_ref_km2;

    vags = ua;
    vbgs = ub;
    vcgs = uc;

    // output control modulator cmpa ou zro

//    EPwm1Regs.CMPA.bit.CMPA = 0.;
//    EPwm2Regs.CMPA.bit.CMPA = 0.;
//    EPwm3Regs.CMPA.bit.CMPA = 0.;
//
//    EPwm1Regs.AQCTLA.bit.CAU = (vags == 1)? 2:1;
//    EPwm2Regs.AQCTLA.bit.CAU = (vbgs == 1)? 2:1;
//    EPwm3Regs.AQCTLA.bit.CAU = (vcgs == 1)? 2:1;


    EPwm1Regs.AQCTLA.bit.ZRO = (vags == 1)? 2:1;
    EPwm2Regs.AQCTLA.bit.ZRO = (vbgs == 1)? 2:1;
    EPwm3Regs.AQCTLA.bit.ZRO = (vcgs == 1)? 2:1;

    #ifdef CPU1
        GPIO_WritePin(DEBUG_MPC_CPU1, 0);
    #endif

}


void start_mpc(){
    iagrid = 0;
    ibgrid = 0;
    icgrid = 0;
    iaconv = 0;
    ibconv = 0;
    icconv = 0;
    vabgrid = 0;
    vbcgrid = 0;
    vcagrid = 0;
    vagrid = 0;
    vbgrid = 0;
    vcgrid = 0;
    vaconv = 0;
    vbconv = 0;
    vcconv = 0;
    vcc = 0;
    inv_vcc = 1;

    ialfagrid = 0;
    ibetagrid = 0;
    ialfaconv = 0;
    ibetaconv = 0;
    valfagrid = 0;
    vbetagrid = 0;
    valfaconv = 0;
    vbetaconv = 0;

    // controller
    w=377;
    pi=3.1415;
    theta=0;
    theta_p=0;
    E=800;
    r=1.;
    L=.3e-3;
    T = 20e-6;

    ua_a=0;
    ub_a=0;
    uc_a=0;

    ualpha=0;
    ubeta=0;

    /*     Normalized Inverter switching vectors in alpha beta coordinate*/
    ual_s[0]=0;         ube_s[0]=0;
    ual_s[1]=0.6667;    ube_s[1]=0;
    ual_s[2]=0.3333;    ube_s[2]=0.577;
    ual_s[3]=-0.3333;   ube_s[3]=0.577;
    ual_s[4]=-0.6667;   ube_s[4]=0;
    ual_s[5]=-0.3333;   ube_s[5]=-0.577;
    ual_s[6]=0.3333;    ube_s[6]=-0.577;


    /*    Normalized  Inverter switching vectors in abc coordinate     */
    compa[0]=0; compb[0]=0; compc[0]=0;
    compa[1]=1; compb[1]=0; compc[1]=0;
    compa[2]=1; compb[2]=1; compc[2]=0;
    compa[3]=0; compb[3]=1; compc[3]=0;
    compa[4]=0; compb[4]=1; compc[4]=1;
    compa[5]=0; compb[5]=0; compc[5]=1;
    compa[6]=1; compb[6]=0; compc[6]=1;



    dois_PI=6.2831853071796;

    wref=377.;
    theta = 0;

    fn=5;
    wc = 0;
    wc1 = 0;
    kp_pll = 0, ki_pll = 0;

    qsi=0.7;            //// Freq.  2st orde filter damping ratio

    ////  Filter on  vd and vq
    //w10=2*pi*10.;    /// 1st LP cut off frequqncy on the vd and vq.;
    vdf=220;
    vqf=0;
    vd = 0, vq = 0;
    ////

    /// Second order filter
    wo2=2.*pi*25.;   //// Freq.  2st orde filter natural frequency

    x1_f=220./(wo2*wo2);  /// 2st order filter initialization
    x2_f=x1_f;    /// 2nd order filter

    /// rater limiter
    Ls=12.;  //  Hz/s
    freq_r=50.;  /// initial contidion for the state of the rater limiter
    x1_f_p = 0, x1_f = 0, x2_f_p = 0, x2_f;
    dref=50;
    xpll = 0, freq_i = 0, freq_r = 0, dfreq = 0;

    /// cut-off frequency of the norm2
    wn2=2*pi*50;
    wn = 0;
    norm = 0;


    /*
    vag vbg vcg       vab   vbc
    0      0      0         0       0
    0      0      1          0      -1
    0      1       0        -1       1
    0      1        1       -1       0
    1      0        0        1       0
    1      0        1         1     -1
    1      1        0         0      1
    1      1        1          0      0    */

    // modulator
    vags1=0;
    vbgs1=0;
    vcgs1=0;

    counter=2;
    inc=1;
    Ts1=20e-6;

    Sa = 0;
    Sb = 0;
    Sc = 0;

}


