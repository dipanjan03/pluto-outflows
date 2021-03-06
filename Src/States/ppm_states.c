/* ///////////////////////////////////////////////////////////////////// */
/*!
  \file
  \brief Piecewise parabolic reconstruction.

  Compute interface states using piecewise parabolic reconstruction 
  inside each zone.
  Reconstruction is performed in primitive variable (when 
   <tt>CHAR_LIMITING == NO</tt>) or characteristic variables 
  (when <tt>CHAR_LIMITING == YES</tt>).
  The reconstruction process follows the following steps:
  - compute unlimited 4-th or 5-th order interface values from within 
    the zone (\f${\rm vp} = v_{i,+},\; {\rm vm} = v_{i,-} \f$).
    
  - interface states must lie between adjacent cell averages. 
    This is achieved by working on the increments,
    \f[
      \left\{\begin{array}{lll}
           \min(v_{i+1},v_i) \le v_{i,+} \le \max(v_{i+1},v_i)
           & \qquad \Longrightarrow\qquad &
           \delta v_{i,+} \to {\rm minmod}(\delta v_{i,+}, \Delta v_{i+\HALF})
       \\ \noalign{\medskip}
           \min(v_{i-1},v_i) \le v_{i,-} \le \max(v_{i-1},v_i)
           & \qquad \Longrightarrow\qquad &
           \delta v_{i,-} \to {\rm minmod}(\delta v_{i,-}, \Delta v_{i+\HALF})
       \end{array}\right.
    \f]
    where \f$\delta v_{i,\pm} = v_{i,\pm} - v_i\f$.
    This step is equivalent to Eq. [45] of Mignone (JCP, 2014).
    The previous operation replaces, starting with PLUTO 4.1, the 
    conventional van-Leer limiting used in the original PPM formulation.
    
  - the parabola does not take any extrema inside the zone, see 
    Eq. [46] in Mignone (JCP, 2014).

  When interpolation is carried out in characteristic variables, we 
  first compute \c vp and \c vm (unlimited) in primitive variables and 
  then project the increments in characteristic space:
  \f[
      \delta w_{i,\pm} = \vec{l}_i\cdot\delta v_{i,\pm}
  \f]
  At this point, Eq. [45] (written in terms of increments) is 
  imposed on characteristic variables while Eq. [46] can be imposed
  in characteristic variables (<tt>PRIMITIVE_LIM == 0</tt>), 
  primitive (<tt>PRIMITIVE_LIM == 1</tt>) or both 
  (<tt>PRIMITIVE_LIM == 2</tt>).
  
  
  \author A. Mignone (mignone@ph.unito.it)
  \date   May 24, 2014

  \b References
     - "High-order conservative reconstruction schemes for finite
        volume methods in cylindrical and spherical coordinates"
        A. Mignone, JCP (2014), 270, 784.
     - "Accurate monotonicity- and extrema-preserving methods
        through adaptive nonlinear hybridizations"
        Rider, Greenough and Kamm, JCP (2007) 225, 1827
     - "A limiter for PPM that preserved accuracy at smooth extrema"
        Colella and Sekora, JCP (2008) 227, 7069
     
*/
/* ///////////////////////////////////////////////////////////////////// */
#include "pluto.h"

#if CHAR_LIMITING == NO
/* ********************************************************************** */
void States (const State_1D *state, int beg, int end, Grid *grid)
/*!
 * 
 * \param [in]      state pointer to State_1D structure
 * \param [in]      beg   initial index of computation
 * \param [in]      end   final   index of computation
 * \param [in]      grid  pointer to an array of Grid structures
 *
 ************************************************************************ */
{
  int   i, nv;
  double dv, **v;
  double dvp, cp, *wp, *hp, **vp;
  double dvm, cm, *wm, *hm, **vm;
  PPM_Coeffs ppm_coeffs;
  PLM_Coeffs plm_coeffs;
  
/* ---------------------------------------------------------
   1. Set pointers, compute geometrical coefficients
   --------------------------------------------------------- */

  v  = state->v;
  vm = state->vm;
  vp = state->vp;

  PPM_CoefficientsGet(&ppm_coeffs, g_dir);
  #if SHOCK_FLATTENING == MULTID
   PLM_CoefficientsGet(&plm_coeffs, g_dir);
  #endif

  hp = ppm_coeffs.hp;
  hm = ppm_coeffs.hm;

/* ---------------------------------------------------------
   2. Define unlimited left and right interface values and
      make sure they lie between adjacent cell averages.
   --------------------------------------------------------- */

  #if PPM_VERSION == PPM3 || PPM_VERSION == PPM5
   for (i = beg; i <= end; i++) {
     wp = ppm_coeffs.wp[i];
     wm = ppm_coeffs.wm[i];
     VAR_LOOP(nv){
       #if PPM_VERSION == PPM3
        vp[i][nv] = wp[-1]*v[i-1][nv] + wp[0]*v[i][nv] + wp[1]*v[i+1][nv];
        vm[i][nv] = wm[-1]*v[i-1][nv] + wm[0]*v[i][nv] + wm[1]*v[i+1][nv];
       #elif PPM_VERSION == PPM5
        vp[i][nv] = wp[-2]*v[i-2][nv] + wp[-1]*v[i-1][nv] +  
                    wp[ 0]*v[i][nv]   + wp[ 1]*v[i+1][nv] + wp[ 2]*v[i+2][nv]; 

        vm[i][nv] =  wm[-2]*v[i-2][nv] + wm[-1]*v[i-1][nv] + wm[0]*v[i][nv]
                   + wm[ 1]*v[i+1][nv] + wm[ 2]*v[i+2][nv];
       #endif
       dvp = vp[i][nv] - v[i][nv];
       dvm = vm[i][nv] - v[i][nv];

       dv  = v[i+1][nv] - v[i][nv];
       vp[i][nv] = v[i][nv] + MINMOD(dvp, dv);

       dv  = v[i][nv] - v[i-1][nv];
       vm[i][nv] = v[i][nv] + MINMOD(dvm, -dv);
     }
   }
  #elif PPM_VERSION == PPM4  /* -- set a unique interface value -- */
   for (i = beg-1; i <= end; i++) {
     wp = ppm_coeffs.wp[i];
     VAR_LOOP(nv){
       vp[i][nv] =  wp[-1]*v[i-1][nv] + wp[0]*v[i][nv]
                  + wp[ 1]*v[i+1][nv] + wp[2]*v[i+2][nv];

       dv  = v[i+1][nv] - v[i][nv];
       dvp = vp[i][nv] - v[i][nv];
       vp[i][nv] = v[i][nv] + MINMOD(dvp, dv);
     }
   }
   for (i = beg; i <= end; i++) VAR_LOOP(nv) vm[i][nv] = vp[i-1][nv];
  #endif

/* ---------------------------------------------------------
   3. Apply parabolic limiter: no new extrema should appear
      in the parabola defined by vp, vm and v.
   --------------------------------------------------------- */

  for (i = beg; i <= end; i++) {
    #if SHOCK_FLATTENING == MULTID    
     if (CheckZone (i, FLAG_MINMOD)) {
       wp = plm_coeffs.wp;
       wm = plm_coeffs.wm;
       VAR_LOOP(nv) {
         dvp = (v[i+1][nv] - v[i][nv])*wp[i];
         dvm = (v[i][nv] - v[i-1][nv])*wm[i];
         dv  = MINMOD(dvp, dvm);
         vp[i][nv] = v[i][nv] + dv*plm_coeffs.dp[i];
         vm[i][nv] = v[i][nv] - dv*plm_coeffs.dm[i];
       }
       #if PHYSICS == RHD || PHYSICS == RMHD
        VelocityLimiter (v[i], vp[i], vm[i]);
       #endif
       continue;
     }
    #endif

    #if PPM_VERSION == PPM0
     cm = cp = 2.0;
    #else
     cm = (hm[i] + 1.0)/(hp[i] - 1.0);
     cp = (hp[i] + 1.0)/(hm[i] - 1.0);
    #endif

    for (nv = 0; nv < NVAR; nv++){
      dvp = vp[i][nv] - v[i][nv];
      dvm = vm[i][nv] - v[i][nv];

      if (dvp*dvm >= 0.0) dvp = dvm = 0.0;
      else{
        if      (fabs(dvp) >= cm*fabs(dvm)) dvp = -cm*dvm;
        else if (fabs(dvm) >= cp*fabs(dvp)) dvm = -cp*dvp;
      }
      vp[i][nv] = v[i][nv] + dvp; 
      vm[i][nv] = v[i][nv] + dvm;
    }
    #if PHYSICS == RHD || PHYSICS == RMHD
     VelocityLimiter (v[i], vp[i], vm[i]);
    #endif
  }

/* --------------------------------------------------------
          1D shock flattening
   -------------------------------------------------------- */

  #if SHOCK_FLATTENING == YES
   Flatten (state, beg, end, grid);
  #endif

/*  -------------------------------------------
      Assign face-centered magnetic field
    -------------------------------------------  */

  #ifdef STAGGERED_MHD
   for (i = beg-1; i <= end; i++) {
     state->vR[i][BXn] = state->vL[i][BXn] = state->bn[i];
   }
  #endif

  #if TIME_STEPPING == CHARACTERISTIC_TRACING
   CharTracingStep(state, beg, end, grid);
  #endif

/* -------------------------------------------
    compute states in conservative variables
   ------------------------------------------- */

  PrimToCons (state->vp, state->up, beg, end);
  PrimToCons (state->vm, state->um, beg, end);
}
#endif /* CHAR_LIMITING == NO */



#if CHAR_LIMITING == YES

/* ---------------------------------------------------------
    Set PARABOLIC_LIM to 0,1,2 to apply the parabolic
    limiter of PPM to either characteristic (0),
    primitive (1) or both variables (2). 
   --------------------------------------------------------- */

#define PARABOLIC_LIM  1
/* *************************************************************************** */
void States (const State_1D *state, int beg, int end, Grid *grid)
/*
 *************************************************************************** */
{
  int    i, j, k, nv, S=1;
  double dtdx, dx, dx2;
  double dp, cp, dvp[NVAR], dwp[NVAR], dwp1[NVAR], *wp, *hp, *vp;
  double dm, cm, dvm[NVAR], dwm[NVAR], dwm1[NVAR], *wm, *hm, *vm;
  double dv, *vc, **v, **L, **R, *lambda;
  double tau, a0, a1, w0, w1;
  static double  **dvF, **vppm4;
  PPM_Coeffs ppm_coeffs;
  PLM_Coeffs plm_coeffs;

/* --------------------------------------------
       local array memory allocation
   -------------------------------------------- */

  if (dvF == NULL){
    dvF = ARRAY_2D(NMAX_POINT, NVAR, double);
    vppm4 = ARRAY_2D(NMAX_POINT,NVAR,double);
  } 
  v = state->v;

  PPM_CoefficientsGet(&ppm_coeffs, g_dir);
  #if SHOCK_FLATTENING == MULTID
   PLM_CoefficientsGet(&plm_coeffs, g_dir);
  #endif

  hp = ppm_coeffs.hp;
  hm = ppm_coeffs.hm;

/* ---------------------------------------------
    define some useful quantities, compute
    source term and undivided differences
   --------------------------------------------- */

  #if INTERPOLATION == PARABOLIC
   S = 2;
  #endif
  SoundSpeed2 (v, state->a2, state->h, beg, end, CELL_CENTER, grid);

  for (i = beg-S; i <= end+S-1; i++){    
    for (nv = NVAR; nv--;   ) dvF[i][nv] = v[i+1][nv] - v[i][nv];
  }

/* -----------------------------------------------------------
    Compute unlimited interface values
   ----------------------------------------------------------- */

  for (i = beg-1; i <= end; i++) {
    wp = ppm_coeffs.wp[i];
    VAR_LOOP(nv){
      vppm4[i][nv] =  wp[-1]*v[i-1][nv] + wp[0]*v[i][nv]
                    + wp[ 1]*v[i+1][nv] + wp[2]*v[i+2][nv];
    } 
  }

/* --------------------------------------------------------------
                    main spatial loop
   -------------------------------------------------------------- */

  for (i = beg; i <= end; i++){    

    dx   = grid[g_dir].dx[beg];
    dx2  = dx*dx;
    dtdx = g_dt/dx;

    vc     = state->v[i]; 
    vp     = state->vp[i];
    vm     = state->vm[i];
    L      = state->Lp[i];
    R      = state->Rp[i];
    lambda = state->lambda[i];

    PrimEigenvectors(vc, state->a2[i], state->h[i], lambda, L, R);
    #if NVAR != NFLX
     for (k = NFLX; k < NVAR; k++) lambda[k] = vc[VXn]; 
    #endif

    #if SHOCK_FLATTENING == MULTID    
     if (CheckZone (i, FLAG_MINMOD)) {
       for (nv = 0; nv < NVAR; nv++){
         dp = dvF[i][nv]  *plm_coeffs.wp[i];
         dm = dvF[i-1][nv]*plm_coeffs.wm[i];
         dv     = MINMOD(dp, dm);
         vp[nv] = vc[nv] + dv*plm_coeffs.dp[i];
         vm[nv] = vc[nv] - dv*plm_coeffs.dm[i];
       }
       #if PHYSICS == RHD || PHYSICS == RMHD
        VelocityLimiter (vc, vp, vm);
       #endif
       continue;
     }
    #endif  /* SHOCK_FLATTENING == MULTID */

  /* ------------------------------------------------------------------
     1. Project undivided difference of primitive variables on
        along characteristics.
        Compute limited characteristic increments dwp and dwm
        by suitable reconstruction and such that 

          wp = w + dwp
          wm = w + dwm
     ------------------------------------------------------------------ */

    #if INTERPOLATION == WENO3

   /* -- compute undivided differences and 
         reconstruct characteristic fields -- */

     PrimToChar(L, dvF[i-1], dwm1);
     PrimToChar(L, dvF[i  ], dwp1);
     for (k = 0; k < NVAR; k++){
       tau = (dwp1[k] - dwm1[k]); 
       tau = tau*tau;

       a0 = 1.0 + tau/(dx2 + dwp1[k]*dwp1[k]);
       a1 = 1.0 + tau/(dx2 + dwm1[k]*dwm1[k]);

       dwp[k] =  (a0*dwp1[k] + 0.5*a1*dwm1[k])/(2.0*a0 + a1);
       dwm[k] = -(a1*dwm1[k] + 0.5*a0*dwp1[k])/(2.0*a1 + a0);
     }

    #endif     /* INTERPOLATION == WENO3 */

    #if INTERPOLATION == PARABOLIC
    
  /* -- write states in terms of differences -- */

     VAR_LOOP(nv) {
       dvp[nv] = vppm4[i][nv]   - v[i][nv];  
       dvm[nv] = vppm4[i-1][nv] - v[i][nv];
     }

  /* -- Project to characteristic spcae -- */

     PrimToChar(L, dvp, dwp);
     PrimToChar(L, dvm, dwm);

     PrimToChar(L, dvF[i-1], dwm1);
     PrimToChar(L, dvF[i  ], dwp1);

    /* -- Enforce monotonicity by constraining interface
          values to lie between adjacent cell averages   -- */

     for (k = 0; k < NVAR; k++){
       dwp[k] = MINMOD(dwp[k],  dwp1[k]);
       dwm[k] = MINMOD(dwm[k], -dwm1[k]);
     }

    /* -- cm and cp are coefficients used in the parabolic limiter -- */

     cm = (hm[i] + 1.0)/(hp[i] - 1.0);
     cp = (hp[i] + 1.0)/(hm[i] - 1.0);

    /* -- parabolic limiter using characteristics or primitive -- */

     #if PARABOLIC_LIM == 0 || PARABOLIC_LIM == 2
      for (k = 0; k < NVAR; k++){
        if (dwp[k]*dwm[k] >= 0.0) dwm[k] = dwp[k] = 0.0;
        else{
          if      (fabs(dwp[k]) >= cm*fabs(dwm[k])) dwp[k] = -cm*dwm[k];
          else if (fabs(dwm[k]) >= cp*fabs(dwp[k])) dwm[k] = -cp*dwp[k];
        }
      }
     #endif

    #endif   /* INTERPOLATION == PARABOLIC */

  /* -------------------------------------------------------------------
     2. Project limited differences in characteristic variable on right
        eigenvectors to obtain the corresponding primitive quantities,
 
        dv = \sum dw.R
     ------------------------------------------------------------------- */

    for (nv = 0; nv < NFLX; nv++) {
      dp = dm = 0.0;
      for (k = 0; k < NFLX; k++){
        dp += dwp[k]*R[nv][k];
        dm += dwm[k]*R[nv][k];
      }
      dvp[nv] = dp;
      dvm[nv] = dm;
    }
    #if NVAR != NFLX
     for (nv = NFLX; nv < NVAR; nv++){
       dvp[nv] = dwp[nv];
       dvm[nv] = dwm[nv];
     }
    #endif 

  /* --------------------------------------------------------------------
     3. Build L/R states in primitive variables and apply parabolic
        limiter to primitive variables if required.
     -------------------------------------------------------------------- */

    for (nv = 0; nv < NVAR; nv++) {
      #if PARABOLIC_LIM == 1 || PARABOLIC_LIM == 2
       if (dvp[nv]*dvm[nv] >= 0.0) dvp[nv] = dvm[nv] = 0.0;
       else {
         if      (fabs(dvp[nv]) >= cm*fabs(dvm[nv])) dvp[nv] = -cm*dvm[nv];
         else if (fabs(dvm[nv]) >= cp*fabs(dvp[nv])) dvm[nv] = -cp*dvp[nv];
       }
      #endif
      vp[nv] = vc[nv] + dvp[nv];
      vm[nv] = vc[nv] + dvm[nv];
    }

  /* ------------------------------------------------------------------
     4. Make sure that left and right states at time t^n are
        physically admissible. If not, use linear reconstruction on
        density and pressure.
     ------------------------------------------------------------------ */

    if (vp[RHO] < 0.0 || vm[RHO] < 0.0) {
      dvp[RHO] = 0.5*(MINMOD(dvF[i][RHO], dvF[i-1][RHO]));
      dvm[RHO] = - dvp[RHO]; 
      vp[RHO] = vc[RHO] + dvp[RHO];
      vm[RHO] = vc[RHO] + dvm[RHO];
    }
    #if HAVE_PRESSURE
     if (vp[PRS] < 0.0 || vm[PRS] < 0.0) {
       dvp[PRS] = 0.5*(MINMOD(dvF[i][PRS], dvF[i-1][PRS]));
       dvm[PRS] = - dvp[PRS];       
       vp[PRS] = vc[PRS] + dvp[PRS];
       vm[PRS] = vc[PRS] + dvm[PRS];
     }
    #endif

    #if PHYSICS == RHD || PHYSICS == RMHD
     VelocityLimiter (vc, vp, vm);
    #endif

  }  /* -- end main loop on grid points -- */

/*  -------------------------------------------
      Assign face-centered magnetic field
    -------------------------------------------  */

  #ifdef STAGGERED_MHD
   for (i = beg-1; i <= end; i++) {
     state->vR[i][BXn] = state->vL[i][BXn] = state->bn[i];
   }
  #endif

  #if TIME_STEPPING == CHARACTERISTIC_TRACING
   CharTracingStep(state, beg, end, grid);
  #endif

/* -------------------------------------------
    compute states in conservative variables
   ------------------------------------------- */

  PrimToCons (state->vp, state->up, beg, end);
  PrimToCons (state->vm, state->um, beg, end);
}
#undef PARABOLIC_LIM
#endif /* CHAR_LIMITING == YES */

