/* ///////////////////////////////////////////////////////////////////// */
/*! 
  \file  
  \brief PLUTO header file for function-like macros.

  \author A. Mignone (mignone@ph.unito.it)
  \date   July 31, 2014
*/
/* ///////////////////////////////////////////////////////////////////// */

/* #####################################################################
    1. Macros that can be placed at any point in the code
   ##################################################################### */

/*! \name Spatial loop macros.
    The following macros provide a compact way to perform 1D or multi-D
    loops in selected regions of the (local) computational domain.
    The \c *BEG_LOOP and \c *END_LOOP macros are used to loop in the 
    leftmost or rightmost boundary ghost zones in the corresponding
    direction \c I, \c J or \c K.
    The \c *DOM_LOOP macros are used to loop inside the computational
    domain (boundaries excluded) while the \c *TOT_LOOP macros are
    used to loop across the entire domain (inside+boundary).
*/
/**@{ */

/* -- MACROS -- */


/* Signum function */
#define SGN(a)    ( ((a) < 0) ? -1 : (((a) > 0) ? 1 : 0) )
#define SGNB(a,b) ( ((a) - (b) < 0) ? -1 : (((a) - (b) > 0) ? 1 : 0) )

/* Heaviside function */
#define HS(a)    ( ((a) < 0) ? 0 : (((a) > 0) ? 1 : 0) )
#define HSB(a,b) ( ((a) - (b) < 0) ? 0 : (((a) - (b) > 0) ? 1 : 0) )
//#define HS(a) 0.5*(1 + SGN(a))
//#define HSB(a, b) 0.5*(1 + SGN(a, b))

/* Select between nozzle type, a: jet, b: ufo. */
#if NOZZLE == NOZZLE_JET
  #define NOZZLE_SELECT(a,b) a
#elif NOZZLE == NOZZLE_UFO
  #define NOZZLE_SELECT(a,b) b
#endif


/* Geometry independence. 
 *
 * A FAIRE: 
 *  - Create function version of these
 *  - Create purely aligned versions of these, always calculating all three dimensions
 *  (- Create versions of these with non-existent dimensions set to 0, using SELECT or EXPAND if required)
 *
 * Note, an arbitrary decision has been made between orientations of different geometries 
 * In general, the standard alingment is used (z-coordinate (third) are aligned (and correspond to each other)), 
 * but in 2D the alignement of the cartesian grid to the non-cartesian grid is chosen so as to retain 
 * the uniqueness of the geometry of the non-cartesian grid. 
 *
 * In 2D in Polar coordinates, we assume the 2D Cartesian grid is on the r, phi plane of 2D polars. 
 * In this case, the alignment doesn't change from 2D to 3D.
 *
 * However, in the spherical and cylindrical cases the 2D Cartesian plane lies on the r-theta or r-z planes, 
 * respectively, and in 3D cartesian, the alignment is standard again.
 *
 * This alignment is motivated by simulations of a flow in the astrophysically most relevant direction for my work.
 * This is usually the z direction or r direction.
 *
 * Dimension/component expansion or selection is not performed, unless the axes orientation changes. This means that
 * care needs to be taken when choosing the values for components that don't exist in the target geometry.
 *
 * For < 3D, in cases where the corresponding dimension does not exist in the target geometries, 
 * the coordiante is always calculated as if in 3D and with the standard alignemnt in 3D. This is to allow 
 * more flexibility in the use of these macros in different dimensionality and geometry than the current.
 *
 * Note, that in cylindrical coordinates, we don't have a third dimension (always set it to 0), but
 * we do have 3 components.  
 *
 * */

/* These macros set appropriate dimensions to 0. This is useful when using with the geometry conversion 
 * macros consistently across dimensions.  Note, these macros don't realign axes. This is done
 * directly in the conversion macros.
 *   */

#define D_ZERO_CYL1(a)  a
#define D_ZERO_CYL2(b)  D_SELECT(0, b, 0)
#define D_ZERO_CYL3(c)  0
#define D_ZERO_SPH1(a)  a
#define D_ZERO_SPH2(b)  D_SELECT(CONST_PI/2, b, b)
#define D_ZERO_SPH3(c)  D_SELECT(0, 0, c)
#define D_ZERO_CART1(a) a
#define D_ZERO_CART2(b) D_SELECT(0, b, b)
#define D_ZERO_CART3(c) D_SELECT(0, 0, c)
#define D_ZERO_POL1(a)  a
#define D_ZERO_POL2(b)  D_SELECT(0, b, b)
#define D_ZERO_POL3(c)  D_SELECT(0, 0, c)

#define C_ZERO1(a) a
#define C_ZERO2(b) D_SELECT(0, b, b)
#define C_ZERO3(c) D_SELECT(0, 0, c)

#if GEOMETRY == CARTESIAN
#define D_ZERO1(a) D_ZERO_CART1(a)
#define D_ZERO2(b) D_ZERO_CART2(b)
#define D_ZERO3(c) D_ZERO_CART3(c)

#elif GEOMETRY == CYLINDRICAL
#define D_ZERO1(a) D_ZERO_CYL1(a)
#define D_ZERO2(b) D_ZERO_CYL2(b)
#define D_ZERO3(c) D_ZERO_CYL3(c)

#elif GEOMETRY == SPHERICAL
#define D_ZERO1(a) D_ZERO_SPH1(a)
#define D_ZERO2(b) D_ZERO_SPH2(b)
#define D_ZERO3(c) D_ZERO_SPH3(c)

#elif GEOMETRY == POLAR
#define D_ZERO1(a) D_ZERO_POL1(a)
#define D_ZERO2(b) D_ZERO_POL2(b)
#define D_ZERO3(c) D_ZERO_POL3(c)
#endif



/* CARTESIAN -- POLAR */
/* The coordinate systems are always aligned the same way for all dimensions */
#define CART2POL1(x1, x2, x3)  ( sqrt((x1)*(x1) + (x2)*(x2)) )
#define CART2POL2(x1, x2, x3)  ( 2*CONST_PI*HS(-(x2)) + (SGN(x2))*acos((x1)/(CART2POL1(x1, x2, x3))) ) 
#define CART2POL3(x1, x2, x3)  ( x3 )

#define POL2CART1(x1, x2, x3)  ( (x1)*cos(x2) )
#define POL2CART2(x1, x2, x3)  ( (x1)*sin(x2) )
#define POL2CART3(x1, x2, x3)  ( x3 )

//#define VCART2POL1(x1, x2, x3, v1, v2, v3)  ( ((x1)*(v1) + (x2)*(v2))/CART2POL1(x1, x2, x3) )
#define VCART2POL1(x1, x2, x3, v1, v2, v3)  ( cos(CART2POL2(x1, x2, x3))*(v1) + sin(CART2POL2(x1, x2, x3))*(v2) )
//#define VCART2POL2(x1, x2, x3, v1, v2, v3)  ( (-sin(CART2POL2(x1, x2, x3))*(v1) + cos(CART2POL2(x1, x2, x3))*(v2))/\
//    (CART2POL1(x1, x2, x3)) )
#define VCART2POL2(x1, x2, x3, v1, v2, v3)  (-sin(CART2POL2(x1, x2, x3))*(v1) + cos(CART2POL2(x1, x2, x3))*(v2)) // without /r
#define VCART2POL3(x1, x2, x3, v1, v2, v3)  ( v3 )

//#define VPOL2CART1(x1, x2, x3, v1, v2, v3)  ( cos(x2)*(v1) - sin(x2)*(x1)*(v2) )
//#define VPOL2CART2(x1, x2, x3, v1, v2, v3)  ( sin(x2)*(v1) + cos(x2)*(x1)*(v2) ) 
#define VPOL2CART1(x1, x2, x3, v1, v2, v3)  ( cos(x2)*(v1) - sin(x2)*(v2) ) // without r
#define VPOL2CART2(x1, x2, x3, v1, v2, v3)  ( sin(x2)*(v1) + cos(x2)*(v2) ) // without r
#define VPOL2CART3(x1, x2, x3, v1, v2, v3)  ( v3 )


/* CYLINDRICAL -- POLAR */
/* The coordinate systems are always aligned, and the conversions trivial 
 * The missing spatial dimension is set to 0. Note that dphi/dt = v3 in 
 * Cylindrical where it is v2 in Polar. */
#define CYL2POL1(x1, x2, x3)  ( x1 )
#define CYL2POL2(x1, x2, x3)  (  0 )
#define CYL2POL3(x1, x2, x3)  ( x2 ) 

#define POL2CYL1(x1, x2, x3)  ( x1 )
#define POL2CYL2(x1, x2, x3)  ( x3 )
#define POL2CYL3(x1, x2, x3)  (  0 )

#define VCYL2POL1(x1, x2, x3, v1, v2, v3)  ( v1 )
#define VCYL2POL2(x1, x2, x3, v1, v2, v3)  ( v3 )
#define VCYL2POL3(x1, x2, x3, v1, v2, v3)  ( v2 )

#define VPOL2CYL1(x1, x2, x3, v1, v2, v3)  ( v1 )
#define VPOL2CYL2(x1, x2, x3, v1, v2, v3)  ( v3 )
#define VPOL2CYL3(x1, x2, x3, v1, v2, v3)  ( v2 )



/* CARTESIAN -- CYLINDRICAL */
/* In 2D Cartesian plane is in cylindrical r-z plane. In 3D the non-existent third cylindrical coordinate is set to 0 
 * In 3D the (x1, x2) is assumed to always lie on the cylindrical plane 
 * so that we can always get the cylindrical radius */ 
#define CART2CYL1(x1, x2, x3)  ( D_SELECT(sqrt((x1)*(x1) +(x2)*(x2)), fabs(x1), sqrt((x1)*(x1) +(x2)*(x2))) )
#define CART2CYL2(x1, x2, x3)  ( D_SELECT(x3, x2, x3) )
#define CART2CYL3(x1, x2, x3)  ( 0 )

#define CYL2CART1(x1, x2, x3)  ( x1 )
#define CYL2CART2(x1, x2, x3)  ( D_SELECT(0, x2, 0) )
#define CYL2CART3(x1, x2, x3)  ( D_SELECT(x2, 0, x2) )


/* In 3D the (x1, x2) is assumed to always lie on the cylindrical plane and v3 perpendicular to that plane.
 * so that we can always get the radial speeds */
#define VCART2CYL1(x1, x2, x3, v1, v2, v3)  ( SELECT( \
      ((x1)*(v1) + (x2)*(v2))/(CART2CYL1(x1, x2, x3)),\
      v1,\
      ((x1)*(v1) + (x2)*(v2))/(CART2CYL1(x1, x2, x3)) ) )
#define VCART2CYL2(x1, x2, x3, v1, v2, v3)  ( SELECT(v3, v2, v3) )
//#define VCART2CYL3(x1, x2, x3, v1, v2, v3)  ( SELECT(\
//      (-sin(CART2POL2(x1, x2, x3))*(v1) + cos(CART2POL2(x1, x2, x3))*(v2))/\
//      (CART2CYL1(x1, x2, x3)),\
//      0, \
//      (-sin(CART2POL2(x1, x2, x3))*(v1) + cos(CART2POL2(x1, x2, x3))*(v2))/\
//      (CART2CYL1(x1, x2, x3)) )
#define VCART2CYL3(x1, x2, x3, v1, v2, v3)  ( SELECT(\
      -sin(CART2POL2(x1, x2, x3))*(v1) + cos(CART2POL2(x1, x2, x3))*(v2),\
      0, \
      -sin(CART2POL2(x1, x2, x3))*(v1) + cos(CART2POL2(x1, x2, x3))*(v2) ) // without /r


/* If GEOMETRY = 3D Cartesian, assumes x1, x2, and x3 are Cartesian coordinates, rather than x1, x2 being cylindrical
 * since no conversion makes sense in 2.5D to 2D Cartesian. So use D_SELECT here. */
#define VCYL2CART1(x1, x2, x3, v1, v2, v3)  ( cos(CART2POL2(x1, x2, x3))*(v1) - sin(CART2POL2(x1, x2, x3))*\
    (CYL2CART1(x1, x2, x3))*(v2) )
#define VCYL2CART2(x1, x2, x3, v1, v2, v3)  ( D_SELECT( \
    sin(CART2POL2(x1, x2, x3))*(v1) + cos(CART2POL2(x1, x2, x3))*(CYL2CART1(x1, x2, x3))*(v2),\
    v2, \
    sin(CART2POL2(x1, x2, x3))*(v1) + cos(CART2POL2(x1, x2, x3))*(CYL2CART1(x1, x2, x3))*(v2)) )
#define VCYL2CART3(x1, x2, x3, v1, v2, v3)  ( D_SELECT( v2,  0, v2) )



/* SPHERICAL -- CYLINDRICAL */
    /* Coordinate axes are always aligned - The non-existent third spatial 
     * dimension in cylindrical is set to 0. In 1D, x2 (theta) has to be pi/2 for consistency */
#define SPH2CYL1(x1, x2, x3) ( sin(x2)*(x1) ) // Assumes theta is measured from x2 in 2D
#define SPH2CYL2(x1, x2, x3) ( cos(x2)*(x1) ) // Assumes theta is measured from x2 in 2D
//#define SPH2CYL1(x1, x2, x3) ( SELECT(sin, cos, sin)(x2)*(x1) ) // Assumes theta is measured from x1 in 2D
//#define SPH2CYL2(x1, x2, x3) ( SELECT(cos, sin, cos)(x2)*(x1) ) // Assumes theta is measured from x1 in 2D
#define SPH2CYL3(x1, x2, x3) ( 0 )

#define CYL2SPH1(x1, x2, x3) ( sqrt((x1)*(x1) + (x2)*(x2))       )
#define CYL2SPH2(x1, x2, x3) ( acos((x2)/(CYL2SPH1(x1, x2, x3))) ) // Assumes theta is measured from x2 in 2D
//#define CYL2SPH2(x1, x2, x3) ( acos((SELECT(x2, x1, x2))/(CYL2SPH1(x1, x2, x3))) )  // Assumes theta is measured from x2 in 2D
#define CYL2SPH3(x1, x2, x3) ( 0 )

//#define VSPH2CYL1(x1, x2, x3, v1, v2, v3) ( sin(x2)*(v1) + (x1)*(v2)*cos(x2) )
//#define VSPH2CYL2(x1, x2, x3, v1, v2, v3) ( cos(x2)*(v1) + (x1)*(v2)*sin(x2) )
#define VSPH2CYL1(x1, x2, x3, v1, v2, v3) ( sin(x2)*(v1) + (v2)*cos(x2) ) // without *r, assumes theta is measured from x2
#define VSPH2CYL2(x1, x2, x3, v1, v2, v3) ( cos(x2)*(v1) - (v2)*sin(x2) ) // without *r, assumes theta is measured from x2
#define VSPH2CYL3(x1, x2, x3, v1, v2, v3) ( v3 )

#define VCYL2SPH1(x1, x2, x3, v1, v2, v3) ( sin(CYL2SPH2(x1, x2, x3))*(v1) + cos(CYL2SPH2(x1, x2, x3))*(v2) )
//#define VCYL2SPH2(x1, x2, x3, v1, v2, v3) ( (cos(CYL2SPH2(x1, x2, x3))*(v1) - sin(CYL2SPH2(x1, x2, x3))*(v2))/\
//    (CYL2SPH1(x1, x2, x3) ) )
#define VCYL2SPH2(x1, x2, x3, v1, v2, v3) (cos(CYL2SPH2(x1, x2, x3))*(v1) - sin(CYL2SPH2(x1, x2, x3))*(v2)) // without /r, assumes theta is measured from x2
#define VCYL2SPH3(x1, x2, x3, v1, v2, v3) ( v3 )


/* SPHERICAL -- POLAR */
/* Spherical to polar doesn't make all too much sense in 2D as the planes are perpendicular to each other 
 * Re-aligning them doesn't make much physical sense either, so we keep the axes aligned in the same way
 * across all dimensions.
 * In 1D, for SPH to POL, need to assume x2 = pi/2 */
#define SPH2POL1(x1, x2, x3) ( sin(x2)*(x1) ) // Assumes theta is measured from x2 in 2D
//#define SPH2POL1(x1, x2, x3) ( SELECT(sin, cos, sin)(x2)*(x1) ) // Assumes theta is measured from x1 in 2D
#define SPH2POL2(x1, x2, x3) ( x3 ) 
#define SPH2POL3(x1, x2, x3) ( cos(x2)*(x1) ) // Assumes theta is measured from x2 in 2D
//#define SPH2POL3(x1, x2, x3) ( SELECT(cos, sin, cos)(x2)*(x1) ) // Assumes theta is measured from x1 in 2D

#define POL2SPH1(x1, x2, x3) ( sqrt((x1)*(x1) + (x3)*(x3)) )
#define POL2SPH2(x1, x2, x3) ( acos((x3)/(POL2SPH1(x1, x2, x3))) ) // Assumes theta is measured from x3 in 2D
//#define POL2SPH2(x1, x2, x3) ( acos((SELECT(x3, x1, x3))/(POL2SPH1(x1, x2, x3))) )  // Assumes theta is measured from x1 in 2D
#define POL2SPH3(x1, x2, x3) ( x2 )

//#define VSPH2POL1(x1, x2, x3, v1, v2, v3) ( sin(x2)*(v1) + (x1)*(v2)*cos(x2) )
#define VSPH2POL1(x1, x2, x3, v1, v2, v3) ( sin(x2)*(v1) + (v2)*cos(x2) ) //without *r, assumes theta is measured from x3
#define VSPH2POL2(x1, x2, x3, v1, v2, v3) ( v3 )
//#define VSPH2POL3(x1, x2, x3, v1, v2, v3) ( cos(x2)*(v1) + (x1)*(v2)*sin(x2) )
#define VSPH2POL3(x1, x2, x3, v1, v2, v3) ( cos(x2)*(v1) - (v2)*sin(x2) ) //without *r, assumes theta is measured from x3

#define VPOL2SPH1(x1, x2, x3, v1, v2, v3) ( sin(POL2SPH2(x1, x2, x3))*(v1) + cos(POL2SPH2(x1, x2, x3))*(v3) )
//#define VPOL2SPH2(x1, x2, x3, v1, v2, v3) ( (cos(POL2SPH2(x1, x2, x3))*(v1) - sin(POL2SPH2(x1, x2, x3))*(v3))/\
//    (POL2SPH1(x1, x2, x3)) )
#define VPOL2SPH2(x1, x2, x3, v1, v2, v3) ( cos(POL2SPH2(x1, x2, x3))*(v1) - sin(POL2SPH2(x1, x2, x3))*(v3) ) // without /r
#define VPOL2SPH3(x1, x2, x3, v1, v2, v3) ( v2 )


/* CARTESIAN -- SPHERICAL */
/* Assume theta = pi/2 in 1D. Azimuthal angle phi and poloidal angle theta don't exist in 2D and 1D 
 * and cannot be derived, because the axes are differently aligned. Assume also that phi = 0 in 2D alignemnt. */
#define CART2SPH1(x1, x2, x3) ( sqrt((x1)*(x1) + (x2)*(x2) + (x3)*(x3)) )
#define CART2SPH2(x1, x2, x3) ( acos((D_SELECT(x3, x2, x3))/(CART2SPH1(x1, x2, x3))) ) // Assumes theta is measured from x2 in 2D
//#define CART2SPH2(x1, x2, x3) ( acos((D_SELECT(x3, x1, x3))/(CART2SPH1(x1, x2, x3))) ) // Assumes theta is measured from x1 in 2D
#define CART2SPH3(x1, x2, x3) ( D_SELECT(0, CONST_PI*HS(-(x1)), 2*CONST_PI*HS(-(x2)) +\
    (SGN(x2))*acos((x1)/(CART2CYL1(x1, x2, x3)))) )
#define SPH2CART1(x1, x2, x3) ( (SPH2POL1(x1, x2, x3))*cos(x3) )
#define SPH2CART2(x1, x2, x3) ( D_SELECT(\
      SPH2POL1(x1, x2, x3)*sin(x3),\
      SPH2CYL2(x1, x2, x3),\
      SPH2POL1(x1, x2, x3)*sin(x3)) )
#define SPH2CART3(x1, x2, x3) ( SPH2CYL2(x1, x2, x3) )

//#define VCART2SPH1(x1, x2, x3, v1, v2, v3) ( ((x1)*(v1) + (x2)*(v2) + (x3)*(v3))/(CART2SPH1(x1, x2, x3)) )
#define VCART2SPH1(x1, x2, x3, v1, v2, v3) ( SELECT(\
      sin(CART2SPH2(x1, x2, x3))*cos(CART2SPH3(x1, x2, x3))*(v1) +\
      sin(CART2SPH2(x1, x2, x3))*sin(CART2SPH3(x1, x2, x3))*(v2) +\
      cos(CART2SPH2(x1, x2, x3))*(v3), \
      cos(CART2SPH2(x1, x2, x3))*(v1) - sin(CART2SPH2(x1, x2, x3))*(v2), \
      sin(CART2SPH2(x1, x2, x3))*cos(CART2SPH3(x1, x2, x3))*(v1) +\
      sin(CART2SPH2(x1, x2, x3))*sin(CART2SPH3(x1, x2, x3))*(v2) +\
      cos(CART2SPH2(x1, x2, x3))*(v3) \
      ) ) // assuming theta from x1 axis in 2D
//#define VCART2SPH2(x1, x2, x3, v1, v2, v3) ( SELECT(\
//      (cos(CART2SPH2(x1, x2, x3))*cos(CART2SPH3(x1, x2, x3))*(v1) +\
//       cos(CART2SPH2(x1, x2, x3))*sin(CART2SPH3(x1, x2, x3))*(v2) -\
//       sin(CART2SPH2(x1, x2, x3))*(v3))/(CART2SPH1(x1, x2, x3) ),\
//      (cos(CART2SPH2(x1, x2, x3))*(v1) + sin(CART2SPH2(x1, x2, x3))*(v2))/(CART2SPH1(x1, x2, x3) ),\
//      (cos(CART2SPH2(x1, x2, x3))*cos(CART2SPH3(x1, x2, x3))*(v1) +\
//       cos(CART2SPH2(x1, x2, x3))*sin(CART2SPH3(x1, x2, x3))*(v2) -\
//       sin(CART2SPH2(x1, x2, x3))*(v3))/(CART2SPH1(x1, x2, x3) ) ) \
//    )
#define VCART2SPH2(x1, x2, x3, v1, v2, v3) ( SELECT(\
      cos(CART2SPH2(x1, x2, x3))*cos(CART2SPH3(x1, x2, x3))*(v1) +\
      cos(CART2SPH2(x1, x2, x3))*sin(CART2SPH3(x1, x2, x3))*(v2) -\
      sin(CART2SPH2(x1, x2, x3))*(v3),\
      cos(CART2SPH2(x1, x2, x3))*(v1) + sin(CART2SPH2(x1, x2, x3))*(v2),\
      cos(CART2SPH2(x1, x2, x3))*cos(CART2SPH3(x1, x2, x3))*(v1) +\
      cos(CART2SPH2(x1, x2, x3))*sin(CART2SPH3(x1, x2, x3))*(v2) -\
      sin(CART2SPH2(x1, x2, x3))*(v3) \
    ) ) // without /r and  assuming theta from x2 axis in 2D
//#define VCART2SPH2(x1, x2, x3, v1, v2, v3) ( SELECT(\
//      cos(CART2SPH2(x1, x2, x3))*cos(CART2SPH3(x1, x2, x3))*(v1) +\
//      cos(CART2SPH2(x1, x2, x3))*sin(CART2SPH3(x1, x2, x3))*(v2) -\
//      sin(CART2SPH2(x1, x2, x3))*(v3),\
//      -sin(CART2SPH2(x1, x2, x3))*(v1) + cos(CART2SPH2(x1, x2, x3))*(v2),\
//      cos(CART2SPH2(x1, x2, x3))*cos(CART2SPH3(x1, x2, x3))*(v1) +\
//      cos(CART2SPH2(x1, x2, x3))*sin(CART2SPH3(x1, x2, x3))*(v2) -\
//      sin(CART2SPH2(x1, x2, x3))*(v3) ) \
//    ) // without /r and  assuming theta from x1 axis in 2D

//#define VCART2SPH3(x1, x2, x3, v1, v2, v3) ( (-sin(CART2SPH3(x1, x2, x3))*(v1) + cos(CART2SPH3(x1, x2, x3))*(v2))/\
//    (CART2POL1(x1, x2, x3)) )
#define VCART2SPH3(x1, x2, x3, v1, v2, v3) ( -sin(CART2SPH3(x1, x2, x3))*(v1) + cos(CART2SPH3(x1, x2, x3))*(v2) )

//#define VSPH2CART1(x1, x2, x3, v1, v2, v3) ( SELECT(\
//      sin(x2)*cos(x3)*(v1) + cos(x2)*cos(x3)*(v2)*(x1) - sin(x2)*(v3)*(SPH2POL1(x1, x2, x3)\
//      sin(x2)*(v1) + cos(x2)*(v2)*(x1),\
//      sin(x2)*cos(x3)*(v1) + cos(x2)*cos(x3)*(v2)*(x1) - sin(x2)*(v3)*(SPH2POL1(x1, x2, x3)\
//      ) ), assumes theta is measured from x2 in 2D
#define VSPH2CART1(x1, x2, x3, v1, v2, v3) ( SELECT(\
      sin(x2)*cos(x3)*(v1) + cos(x2)*cos(x3)*(v2) - sin(x2)*(v3), \
      SGN(SPH2CART1(x1, x2, x3))*(sin(x2)*(v1) + cos(x2)*(v2)),\
      sin(x2)*cos(x3)*(v1) + cos(x2)*cos(x3)*(v2) - sin(x2)*(v3) \
      ) ) // without *r, assumes theta is measured from x2 in 2D

//#define VSPH2CART2(x1, x2, x3, v1, v2, v3) ( SELECT(\
//      sin(x2)*sin(x3)*(v1) + cos(x2)*sin(x3)*(v2)*(x1) + cos(x3)*(v3)*(SPH2POL1(x1, x2, x3)),\
//      cos(x2)*(v1) - sin(x2)*(v2)*(x1),\
//      sin(x2)*sin(x3)*(v1) + cos(x2)*sin(x3)*(v2)*(x1) + cos(x3)*(v3)*(SPH2POL1(x1, x2, x3)))\
//    ) assumes theta is measured from x2 in 2D
#define VSPH2CART2(x1, x2, x3, v1, v2, v3) ( SELECT(\
      sin(x2)*sin(x3)*(v1) + cos(x2)*sin(x3)*(v2) + cos(x3)*(v3),\
      cos(x2)*(v1) - sin(x2)*(v2),\
      sin(x2)*sin(x3)*(v1) + cos(x2)*sin(x3)*(v2) + cos(x3)*(v3))\
    ) // without *r, assumes theta is measured from x2 in 2D

//#define VSPH2CART3(x1, x2, x3, v1, v2, v3) ( cos(x2)*(v1) - sin(x2)*(v2)*(x1) )
#define VSPH2CART3(x1, x2, x3, v1, v2, v3) ( cos(x2)*(v1) - sin(x2)*(v2) ) // without *r



/* Generalization for different geometries. The following macros can be used to make a 
 * problem initialization independent of geometry. The grid alignment properties 
 * are discussed above. Do not zero out missing dimensions if calculating from a 
 * different geometry (macros endign with _{123}, because we might want to take into 
 * account the full geoemtry of the source grid. An example is SPH_3, where in 2D, phi 
 * may be 0 or pi, depending on whether x1 is positive or negative. */


#if GEOMETRY == CARTESIAN
  #define CART1(x1, x2, x3)                ( D_ZERO1(x1) )
  #define CART2(x1, x2, x3)                ( D_ZERO2(x2) )
  #define CART3(x1, x2, x3)                ( D_ZERO3(x3) )
  #define CYL1(x1, x2, x3)                 ( CART2CYL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CYL2(x1, x2, x3)                 ( CART2CYL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define SPH1(x1, x2, x3)                 ( CART2SPH1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define SPH2(x1, x2, x3)                 ( CART2SPH2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define SPH3(x1, x2, x3)                 ( CART2SPH3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define POL1(x1, x2, x3)                 ( CART2POL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define POL2(x1, x2, x3)                 ( CART2POL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define POL3(x1, x2, x3)                 ( CART2POL3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CART_1(x1, x2, x3)               ( x1 )
  #define CART_2(x1, x2, x3)               ( x2 )
  #define CART_3(x1, x2, x3)               ( x3 )
  #define CYL_1(x1, x2, x3)                ( CYL2CART1(x1, x2, x3 ) )
  #define CYL_2(x1, x2, x3)                ( CYL2CART2(x1, x2, x3 ) )
  #define CYL_3(x1, x2, x3)                ( CYL2CART3(x1, x2, x3 ) )
  #define SPH_1(x1, x2, x3)                ( SPH2CART1(x1, x2, x3 ) )
  #define SPH_2(x1, x2, x3)                ( SPH2CART2(x1, x2, x3 ) )
  #define SPH_3(x1, x2, x3)                ( SPH2CART3(x1, x2, x3 ) )
  #define POL_1(x1, x2, x3)                ( POL2CART1(x1, x2, x3 ) )
  #define POL_2(x1, x2, x3)                ( POL2CART2(x1, x2, x3 ) )
  #define POL_3(x1, x2, x3)                ( POL2CART3(x1, x2, x3 ) )
//#define CART_1(x1, x2, x3)               ( D_ZERO_CART1(x1) )
//#define CART_2(x1, x2, x3)               ( D_ZERO_CART2(x2) )
//#define CART_3(x1, x2, x3)               ( D_ZERO_CART3(x3) )
//#define CYL_1(x1, x2, x3)                ( CYL2CART1(D_ZERO_CYL1(x1), D_ZERO_CYL2(x2), D_ZERO_CYL3(x3) ) )
//#define CYL_2(x1, x2, x3)                ( CYL2CART2(D_ZERO_CYL1(x1), D_ZERO_CYL2(x2), D_ZERO_CYL3(x3) ) )
//#define CYL_3(x1, x2, x3)                ( CYL2CART3(D_ZERO_CYL1(x1), D_ZERO_CYL2(x2), D_ZERO_CYL3(x3) ) )
//#define SPH_1(x1, x2, x3)                ( SPH2CART1(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3) ) )
//#define SPH_2(x1, x2, x3)                ( SPH2CART2(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3) ) )
//#define SPH_3(x1, x2, x3)                ( SPH2CART3(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3) ) )
//#define POL_1(x1, x2, x3)                ( POL2CART1(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3) ) )
//#define POL_2(x1, x2, x3)                ( POL2CART2(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3) ) )
//#define POL_3(x1, x2, x3)                ( POL2CART3(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3) ) )
  #define VCART1(x1, x2, x3, v1, v2, v3)   ( C_ZERO1(v1) )
  #define VCART2(x1, x2, x3, v1, v2, v3)   ( C_ZERO2(v2) )
  #define VCART3(x1, x2, x3, v1, v2, v3)   ( C_ZERO3(v3) )
  #define VCYL1(x1, x2, x3, v1, v2, v3)    ( VCART2CYL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
  #define VCYL2(x1, x2, x3, v1, v2, v3)    ( VCART2CYL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
  #define VSPH1(x1, x2, x3, v1, v2, v3)    ( VCART2SPH1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
  #define VSPH2(x1, x2, x3, v1, v2, v3)    ( VCART2SPH2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
  #define VSPH3(x1, x2, x3, v1, v2, v3)    ( VCART2SPH3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
  #define VPOL1(x1, x2, x3, v1, v2, v3)    ( VCART2POL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
  #define VPOL2(x1, x2, x3, v1, v2, v3)    ( VCART2POL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
  #define VPOL3(x1, x2, x3, v1, v2, v3)    ( VCART2POL3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
//#define VCART_1(x1, x2, x3, v1, v2, v3)  ( C_ZERO1(v1) )
//#define VCART_2(x1, x2, x3, v1, v2, v3)  ( C_ZERO2(v2) )
//#define VCART_3(x1, x2, x3, v1, v2, v3)  ( C_ZERO3(v3) )
//#define VCYL_1(x1, x2, x3, v1, v2, v3)   ( VCYL2CART1(D_ZERO_CYL1(x1), D_ZERO_CYL2(x2), D_ZERO_CYL3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
//#define VCYL_2(x1, x2, x3, v1, v2, v3)   ( VCYL2CART2(D_ZERO_CYL1(x1), D_ZERO_CYL2(x2), D_ZERO_CYL3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
//#define VCYL_3(x1, x2, x3, v1, v2, v3)   ( VCYL2CART3(D_ZERO_CYL1(x1), D_ZERO_CYL2(x2), D_ZERO_CYL3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
//#define VSPH_1(x1, x2, x3, v1, v2, v3)   ( VSPH2CART1(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
//#define VSPH_2(x1, x2, x3, v1, v2, v3)   ( VSPH2CART2(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
//#define VSPH_3(x1, x2, x3, v1, v2, v3)   ( VSPH2CART3(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
//#define VPOL_1(x1, x2, x3, v1, v2, v3)   ( VPOL2CART1(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
//#define VPOL_2(x1, x2, x3, v1, v2, v3)   ( VPOL2CART2(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
//#define VPOL_3(x1, x2, x3, v1, v2, v3)   ( VPOL2CART3(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3) ) )
  #define VCART_1(x1, x2, x3, v1, v2, v3)  ( v1 )
  #define VCART_2(x1, x2, x3, v1, v2, v3)  ( v2 )
  #define VCART_3(x1, x2, x3, v1, v2, v3)  ( v3 )
  #define VCYL_1(x1, x2, x3, v1, v2, v3)   ( VCYL2CART1(x1, x2, x3, v1, v2, v3 ) )
  #define VCYL_2(x1, x2, x3, v1, v2, v3)   ( VCYL2CART2(x1, x2, x3, v1, v2, v3 ) )
  #define VCYL_3(x1, x2, x3, v1, v2, v3)   ( VCYL2CART3(x1, x2, x3, v1, v2, v3 ) )
  #define VSPH_1(x1, x2, x3, v1, v2, v3)   ( VSPH2CART1(x1, x2, x3, v1, v2, v3 ) )
  #define VSPH_2(x1, x2, x3, v1, v2, v3)   ( VSPH2CART2(x1, x2, x3, v1, v2, v3 ) )
  #define VSPH_3(x1, x2, x3, v1, v2, v3)   ( VSPH2CART3(x1, x2, x3, v1, v2, v3 ) )
  #define VPOL_1(x1, x2, x3, v1, v2, v3)   ( VPOL2CART1(x1, x2, x3, v1, v2, v3 ) )
  #define VPOL_2(x1, x2, x3, v1, v2, v3)   ( VPOL2CART2(x1, x2, x3, v1, v2, v3 ) )
  #define VPOL_3(x1, x2, x3, v1, v2, v3)   ( VPOL2CART3(x1, x2, x3, v1, v2, v3 ) )

/* Elif here guards against other geometries in case Chombo is used 
   The geometries CARTESIAN, CYLINDRICAL, etc are not defined when compiled with Chombo */
#elif GEOMETRY == CYLINDRICAL
  #define CART1(x1, x2, x3)                ( CYL2CART1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CART2(x1, x2, x3)                ( CYL2CART2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CART3(x1, x2, x3)                ( CYL2CART3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CYL1(x1, x2, x3)                 ( D_ZERO1(x1) )
  #define CYL2(x1, x2, x3)                 ( D_ZERO2(x2) )
  #define SPH1(x1, x2, x3)                 ( CYL2SPH1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define SPH2(x1, x2, x3)                 ( CYL2SPH2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define SPH3(x1, x2, x3)                 ( CYL2SPH3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define POL1(x1, x2, x3)                 ( CYL2POL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define POL2(x1, x2, x3)                 ( CYL2POL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define POL3(x1, x2, x3)                 ( CYL2POL3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
//#define CART_1(x1, x2, x3)               ( CART2CYL1(D_ZERO_CART1(x1), D_ZERO_CART2(x2), D_ZERO_CART3(x3)) )
//#define CART_2(x1, x2, x3)               ( CART2CYL2(D_ZERO_CART1(x1), D_ZERO_CART2(x2), D_ZERO_CART3(x3)) )
//#define CART_3(x1, x2, x3)               ( CART2CYL3(D_ZERO_CART1(x1), D_ZERO_CART2(x2), D_ZERO_CART3(x3)) )
//#define CYL_1(x1, x2, x3)                ( D_ZERO_CYL1(x1) )
//#define CYL_2(x1, x2, x3)                ( D_ZERO_CYL2(x2) )
//#define CYL_3(x1, x2, x3)                ( D_ZERO_CYL3(x3) )
//#define SPH_1(x1, x2, x3)                ( SPH2CYL1(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3)) )
//#define SPH_2(x1, x2, x3)                ( SPH2CYL2(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3)) )
//#define SPH_3(x1, x2, x3)                ( SPH2CYL3(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3)) )
//#define POL_1(x1, x2, x3)                ( POL2CYL1(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3)) )
//#define POL_2(x1, x2, x3)                ( POL2CYL2(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3)) )
//#define POL_3(x1, x2, x3)                ( POL2CYL3(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3)) )
  #define CART_1(x1, x2, x3)               ( CART2CYL1(x1, x2, x3) )
  #define CART_2(x1, x2, x3)               ( CART2CYL2(x1, x2, x3) )
  #define CART_3(x1, x2, x3)               ( CART2CYL3(x1, x2, x3) )
  #define CYL_1(x1, x2, x3)                ( x1 )
  #define CYL_2(x1, x2, x3)                ( x2 )
  #define CYL_3(x1, x2, x3)                ( x3 )
  #define SPH_1(x1, x2, x3)                ( SPH2CYL1(x1, x2, x3) )
  #define SPH_2(x1, x2, x3)                ( SPH2CYL2(x1, x2, x3) )
  #define SPH_3(x1, x2, x3)                ( SPH2CYL3(x1, x2, x3) )
  #define POL_1(x1, x2, x3)                ( POL2CYL1(x1, x2, x3) )
  #define POL_2(x1, x2, x3)                ( POL2CYL2(x1, x2, x3) )
  #define POL_3(x1, x2, x3)                ( POL2CYL3(x1, x2, x3) )
  #define VCART1(x1, x2, x3, v1, v2, v3)   ( VCYL2CART1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCART2(x1, x2, x3, v1, v2, v3)   ( VCYL2CART2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCART3(x1, x2, x3, v1, v2, v3)   ( VCYL2CART3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCYL1(x1, x2, x3, v1, v2, v3)    ( C_ZERO1(v1) )
  #define VCYL2(x1, x2, x3, v1, v2, v3)    ( C_ZERO2(v2) )
  #define VSPH1(x1, x2, x3, v1, v2, v3)    ( VCYL2SPH1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VSPH2(x1, x2, x3, v1, v2, v3)    ( VCYL2SPH2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VSPH3(x1, x2, x3, v1, v2, v3)    ( VCYL2SPH3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VPOL1(x1, x2, x3, v1, v2, v3)    ( VCYL2POL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VPOL2(x1, x2, x3, v1, v2, v3)    ( VCYL2POL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VPOL3(x1, x2, x3, v1, v2, v3)    ( VCYL2POL3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
//#define VCART_1(x1, x2, x3, v1, v2, v3)  ( VCART2CYL1(D_ZERO_CART1(x1), D_ZERO_CART2(x2), D_ZERO_CART3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
//#define VCART_2(x1, x2, x3, v1, v2, v3)  ( VCART2CYL2(D_ZERO_CART1(x1), D_ZERO_CART2(x2), D_ZERO_CART3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
//#define VCART_3(x1, x2, x3, v1, v2, v3)  ( VCART2CYL3(D_ZERO_CART1(x1), D_ZERO_CART2(x2), D_ZERO_CART3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
//#define VCYL_1(x1, x2, x3, v1, v2, v3)   ( C_ZERO1(v1) )
//#define VCYL_2(x1, x2, x3, v1, v2, v3)   ( C_ZERO2(v2) )
//#define VCYL_3(x1, x2, x3, v1, v2, v3)   ( C_ZERO3(v3) )
//#define VSPH_1(x1, x2, x3, v1, v2, v3)   ( VSPH2CYL1(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
//#define VSPH_2(x1, x2, x3, v1, v2, v3)   ( VSPH2CYL2(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
//#define VSPH_3(x1, x2, x3, v1, v2, v3)   ( VSPH2CYL3(D_ZERO_SPH1(x1), D_ZERO_SPH2(x2), D_ZERO_SPH3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
//#define VPOL_1(x1, x2, x3, v1, v2, v3)   ( VPOL2CYL1(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
//#define VPOL_2(x1, x2, x3, v1, v2, v3)   ( VPOL2CYL2(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
//#define VPOL_3(x1, x2, x3, v1, v2, v3)   ( VPOL2CYL3(D_ZERO_POL1(x1), D_ZERO_POL2(x2), D_ZERO_POL3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCART_1(x1, x2, x3, v1, v2, v3)  ( VCART2CYL1(x1, x2, x3, v1, v2, v3) )
  #define VCART_2(x1, x2, x3, v1, v2, v3)  ( VCART2CYL2(x1, x2, x3, v1, v2, v3) )
  #define VCART_3(x1, x2, x3, v1, v2, v3)  ( VCART2CYL3(x1, x2, x3, v1, v2, v3) )
  #define VCYL_1(x1, x2, x3, v1, v2, v3)   ( v1 )
  #define VCYL_2(x1, x2, x3, v1, v2, v3)   ( v2 )
  #define VCYL_3(x1, x2, x3, v1, v2, v3)   ( v3 )
  #define VSPH_1(x1, x2, x3, v1, v2, v3)   ( VSPH2CYL1(x1, x2, x3, v1, v2, v3) )
  #define VSPH_2(x1, x2, x3, v1, v2, v3)   ( VSPH2CYL2(x1, x2, x3, v1, v2, v3) )
  #define VSPH_3(x1, x2, x3, v1, v2, v3)   ( VSPH2CYL3(x1, x2, x3, v1, v2, v3) )
  #define VPOL_1(x1, x2, x3, v1, v2, v3)   ( VPOL2CYL1(x1, x2, x3, v1, v2, v3) )
  #define VPOL_2(x1, x2, x3, v1, v2, v3)   ( VPOL2CYL2(x1, x2, x3, v1, v2, v3) )
  #define VPOL_3(x1, x2, x3, v1, v2, v3)   ( VPOL2CYL3(x1, x2, x3, v1, v2, v3) )

#elif GEOMETRY == SPHERICAL
  #define CART1(x1, x2, x3)                ( SPH2CART1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CART2(x1, x2, x3)                ( SPH2CART2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CART3(x1, x2, x3)                ( SPH2CART3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CYL1(x1, x2, x3)                 ( SPH2CYL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CYL2(x1, x2, x3)                 ( SPH2CYL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define SPH1(x1, x2, x3)                 ( D_ZERO1(x1) )
  #define SPH2(x1, x2, x3)                 ( D_ZERO2(x2) )
  #define SPH3(x1, x2, x3)                 ( D_ZERO3(x3) )
  #define POL1(x1, x2, x3)                 ( SPH2POL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define POL2(x1, x2, x3)                 ( SPH2POL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define POL3(x1, x2, x3)                 ( SPH2POL3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CART_1(x1, x2, x3)               ( CART2SPH1(x1, x2, x3) )
  #define CART_2(x1, x2, x3)               ( CART2SPH2(x1, x2, x3) )
  #define CART_3(x1, x2, x3)               ( CART2SPH3(x1, x2, x3) )
  #define CYL_1(x1, x2, x3)                ( CYL2SPH1(x1, x2, x3) )
  #define CYL_2(x1, x2, x3)                ( CYL2SPH2(x1, x2, x3) )
  #define CYL_3(x1, x2, x3)                ( CYL2SPH3(x1, x2, x3) )
  #define SPH_1(x1, x2, x3)                ( x1 )
  #define SPH_2(x1, x2, x3)                ( x2 )
  #define SPH_3(x1, x2, x3)                ( x3 )
  #define POL_1(x1, x2, x3)                ( POL2SPH1(x1, x2, x3) )
  #define POL_2(x1, x2, x3)                ( POL2SPH2(x1, x2, x3) )
  #define POL_3(x1, x2, x3)                ( POL2SPH3(x1, x2, x3) )
  #define VCART1(x1, x2, x3, v1, v2, v3)   ( VSPH2CART1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCART2(x1, x2, x3, v1, v2, v3)   ( VSPH2CART2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCART3(x1, x2, x3, v1, v2, v3)   ( VSPH2CART3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCYL1(x1, x2, x3, v1, v2, v3)    ( VSPH2CYL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCYL2(x1, x2, x3, v1, v2, v3)    ( VSPH2CYL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VSPH1(x1, x2, x3, v1, v2, v3)    ( C_ZERO1(v1) )
  #define VSPH2(x1, x2, x3, v1, v2, v3)    ( C_ZERO2(v2) )
  #define VSPH3(x1, x2, x3, v1, v2, v3)    ( C_ZERO3(v3) )
  #define VPOL1(x1, x2, x3, v1, v2, v3)    ( VSPH2POL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VPOL2(x1, x2, x3, v1, v2, v3)    ( VSPH2POL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VPOL3(x1, x2, x3, v1, v2, v3)    ( VSPH2POL3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCART_1(x1, x2, x3, v1, v2, v3)  ( VCART2SPH1(x1, x2, x3, v1, v2, v3) ) 
  #define VCART_2(x1, x2, x3, v1, v2, v3)  ( VCART2SPH2(x1, x2, x3, v1, v2, v3) ) 
  #define VCART_3(x1, x2, x3, v1, v2, v3)  ( VCART2SPH3(x1, x2, x3, v1, v2, v3) ) 
  #define VCYL_1(x1, x2, x3, v1, v2, v3)   ( VCYL2SPH1(x1, x2, x3, v1, v2, v3) )
  #define VCYL_2(x1, x2, x3, v1, v2, v3)   ( VCYL2SPH2(x1, x2, x3, v1, v2, v3) )
  #define VCYL_3(x1, x2, x3, v1, v2, v3)   ( VCYL2SPH3(x1, x2, x3, v1, v2, v3) )
  #define VSPH_1(x1, x2, x3, v1, v2, v3)   ( v1 )
  #define VSPH_2(x1, x2, x3, v1, v2, v3)   ( v2 )
  #define VSPH_3(x1, x2, x3, v1, v2, v3)   ( v3 )
  #define VPOL_1(x1, x2, x3, v1, v2, v3)   ( VPOL2SPH1(x1, x2, x3, v1, v2, v3 ) )
  #define VPOL_2(x1, x2, x3, v1, v2, v3)   ( VPOL2SPH2(x1, x2, x3, v1, v2, v3 ) )
  #define VPOL_3(x1, x2, x3, v1, v2, v3)   ( VPOL2SPH3(x1, x2, x3, v1, v2, v3 ) )

#elif GEOMETRY == POLAR
  #define CART1(x1, x2, x3)                ( POL2CART1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CART2(x1, x2, x3)                ( POL2CART2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CART3(x1, x2, x3)                ( POL2CART3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define CYL1(x1, x2, x3)                 ( POL2CYL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) ) 
  #define CYL2(x1, x2, x3)                 ( POL2CYL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) )
  #define SPH1(x1, x2, x3)                 ( POL2SPH1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) ) 
  #define SPH2(x1, x2, x3)                 ( POL2SPH2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) ) 
  #define SPH3(x1, x2, x3)                 ( POL2SPH3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3)) ) 
  #define POL1(x1, x2, x3)                 ( D_ZERO1(x1) ) 
  #define POL2(x1, x2, x3)                 ( D_ZERO2(x2) ) 
  #define POL3(x1, x2, x3)                 ( D_ZERO3(x3) ) 
  #define CART_1(x1, x2, x3)               ( CART2POL1(x1, x2, x3) )
  #define CART_2(x1, x2, x3)               ( CART2POL2(x1, x2, x3) )
  #define CART_3(x1, x2, x3)               ( CART2POL3(x1, x2, x3) )
  #define CYL_1(x1, x2, x3)                ( CYL2POL1(x1, x2, x3) )
  #define CYL_2(x1, x2, x3)                ( CYL2POL2(x1, x2, x3) )
  #define CYL_3(x1, x2, x3)                ( CYL2POL3(x1, x2, x3) )
  #define SPH_1(x1, x2, x3)                ( SPH2POL1(x1, x2, x3) )
  #define SPH_2(x1, x2, x3)                ( SPH2POL2(x1, x2, x3) )
  #define SPH_3(x1, x2, x3)                ( SPH2POL3(x1, x2, x3) )
  #define POL_1(x1, x2, x3)                ( x1 )
  #define POL_2(x1, x2, x3)                ( x2 )
  #define POL_3(x1, x2, x3)                ( x3 )
  #define VCART1(x1, x2, x3, v1, v2, v3)   ( VPOL2CART1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCART2(x1, x2, x3, v1, v2, v3)   ( VPOL2CART2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCART3(x1, x2, x3, v1, v2, v3)   ( VPOL2CART3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCYL1(x1, x2, x3, v1, v2, v3)    ( VPOL2CYL1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VCYL2(x1, x2, x3, v1, v2, v3)    ( VPOL2CYL2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VSPH1(x1, x2, x3, v1, v2, v3)    ( VPOL2SPH1(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VSPH2(x1, x2, x3, v1, v2, v3)    ( VPOL2SPH2(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VSPH3(x1, x2, x3, v1, v2, v3)    ( VPOL2SPH3(D_ZERO1(x1), D_ZERO2(x2), D_ZERO3(x3), C_ZERO1(v1), C_ZERO2(v2), C_ZERO3(v3)) )
  #define VPOL1(x1, x2, x3, v1, v2, v3)    ( C_ZERO1(v1) )
  #define VPOL2(x1, x2, x3, v1, v2, v3)    ( C_ZERO2(v2) )
  #define VPOL3(x1, x2, x3, v1, v2, v3)    ( C_ZERO3(v3) )
  #define VCART_1(x1, x2, x3, v1, v2, v3)  ( VCART2POL1(x1, x2, x3, v1, v2, v3) )
  #define VCART_2(x1, x2, x3, v1, v2, v3)  ( VCART2POL2(x1, x2, x3, v1, v2, v3) )
  #define VCART_3(x1, x2, x3, v1, v2, v3)  ( VCART2POL3(x1, x2, x3, v1, v2, v3) )
  #define VCYL_1(x1, x2, x3, v1, v2, v3)   ( VCYL2POL1(x1, x2, x3, v1, v2, v3 ) )
  #define VCYL_2(x1, x2, x3, v1, v2, v3)   ( VCYL2POL2(x1, x2, x3, v1, v2, v3 ) )
  #define VCYL_3(x1, x2, x3, v1, v2, v3)   ( VCYL2POL3(x1, x2, x3, v1, v2, v3 ) )
  #define VSPH_1(x1, x2, x3, v1, v2, v3)   ( VSPH2POL1(x1, x2, x3, v1, v2, v3 ) )
  #define VSPH_2(x1, x2, x3, v1, v2, v3)   ( VSPH2POL2(x1, x2, x3, v1, v2, v3 ) )
  #define VSPH_3(x1, x2, x3, v1, v2, v3)   ( VSPH2POL3(x1, x2, x3, v1, v2, v3 ) )
  #define VPOL_1(x1, x2, x3, v1, v2, v3)   ( v1 )
  #define VPOL_2(x1, x2, x3, v1, v2, v3)   ( v2 )
  #define VPOL_3(x1, x2, x3, v1, v2, v3)   ( v3 )
#endif



/* WARNING, The following macros output a comma separated list. 
 * The commas are not preserved down to higher macro layers. */
#define ARGCART(x1, x2, x3) CART1(x1, x2, x3), CART2(x1, x2, x3), CART3(x1, x2, x3)
#define ARGCART_(x1, x2, x3) CART_1(x1, x2, x3), CART_2(x1, x2, x3), CART_3(x1, x2, x3)
#define ARGVCART(x1, x2, x3, v1, v2, v3) VCART1(x1, x2, x3, v1, v2, v3), VCART2(x1, x2, x3, v1, v2, v3), VCART3(x1, x2, x3, v1, v2, v3)
#define ARGVCART_(x1, x2, x3, v1, v2, v3) VCART_1(x1, x2, x3, v1, v2, v3), VCART_2(x1, x2, x3, v1, v2, v3), VCART_3(x1, x2, x3, v1, v2, v3)

/* Magnitudes */
#define RMAG2(x1, x2, x3) ( EXPAND(pow(CART1(x1, x2, x3), 2), + pow(CART2(x1, x2, x3), 2), + pow(CART3(x1, x2, x3), 2)) )
#define RMAG(x1, x2, x3) ( sqrt( RMAG2(x1, x2, x3) ) )
#define VMAG2(x1, x2, x3, v1, v2, v3) ( EXPAND(pow(VCART1(x1, x2, x3, v1, v2, v3), 2), + pow(VCART2(x1, x2, x3, v1, v2, v3), 2), + pow(VCART3(x1, x2, x3, v1, v2, v3), 2)) ) 
#define VMAG(x1, x2, x3, v1, v2, v3) ( sqrt( VMAG2(x1, x2, x3, v1, v2, v3) ) )


/* GEOMETRY and DIMENSION independent determination of flow axis 
 * Note that for jets/outflows, CYL 1D and 3D, and POL 1D, 2D  are not really applicable. 
 * But for other problems may exist where this makes sense.
 * - The Macro FLOWAXIS{123} outputs a if this is the flow axis, or b if it's not the flow axis.
 * - The macro FLOWAXIS outputs the flowaxis'th element. This is currently only used in 
 *   USERDEF_BOUNDARY and has limited use and is complicated.
 * - Currently the flow axis is the axis along which the outflow is directed. One is not necessarily
 *   restricted to this definition, though. */
#if   GEOMETRY == CARTESIAN
  #if   DIMENSIONS == 1
    #define FLOWAXIS(a, b, c) a
    #define FLOWAXIS1(a, b) a
    #define FLOWAXIS2(a, b) b
    #define FLOWAXIS3(a, b) b
  #elif DIMENSIONS == 2
    #define FLOWAXIS(a, b, c) b
    #define FLOWAXIS1(a, b) b
    #define FLOWAXIS2(a, b) a
    #define FLOWAXIS3(a, b) b
  #elif DIMENSIONS == 3
    #define FLOWAXIS(a, b, c) c
    #define FLOWAXIS1(a, b) b
    #define FLOWAXIS2(a, b) b
    #define FLOWAXIS3(a, b) a
  #endif
#elif GEOMETRY == CYLINDRICAL
  #if   DIMENSIONS == 1
    #define FLOWAXIS(a, b, c) a
    #define FLOWAXIS1(a, b) a
    #define FLOWAXIS2(a, b) b
    #define FLOWAXIS3(a, b) b
  #elif DIMENSIONS == 2
    #define FLOWAXIS(a, b, c) b
    #define FLOWAXIS1(a, b) b
    #define FLOWAXIS2(a, b) a
    #define FLOWAXIS3(a, b) b
  #elif DIMENSIONS == 3
    #define FLOWAXIS(a, b, c) b
    #define FLOWAXIS1(a, b) b
    #define FLOWAXIS2(a, b) a
    #define FLOWAXIS3(a, b) b
  #endif
#elif GEOMETRY == SPHERICAL
  #if   DIMENSIONS == 1
    #define FLOWAXIS(a, b, c) a
    #define FLOWAXIS1(a, b) a
    #define FLOWAXIS2(a, b) b
    #define FLOWAXIS3(a, b) b
  #elif DIMENSIONS == 2
    #define FLOWAXIS(a, b, c) a
    #define FLOWAXIS1(a, b) a
    #define FLOWAXIS2(a, b) b
    #define FLOWAXIS3(a, b) b
  #elif DIMENSIONS == 3
    #define FLOWAXIS(a, b, c) a
    #define FLOWAXIS1(a, b) a
    #define FLOWAXIS2(a, b) b
    #define FLOWAXIS3(a, b) b
  #endif
#elif GEOMETRY == POLAR
  #if   DIMENSIONS == 1
    #define FLOWAXIS(a, b, c) a
    #define FLOWAXIS1(a, b) a
    #define FLOWAXIS2(a, b) b
    #define FLOWAXIS3(a, b) b
  #elif DIMENSIONS == 2
    #define FLOWAXIS(a, b, c) a
    #define FLOWAXIS1(a, b) a
    #define FLOWAXIS2(a, b) b
    #define FLOWAXIS3(a, b) b
  #elif DIMENSIONS == 3
    #define FLOWAXIS(a, b, c) c
    #define FLOWAXIS1(a, b) b
    #define FLOWAXIS2(a, b) b
    #define FLOWAXIS3(a, b) a
  #endif
#endif


/* GEOMETRY and dimension dependent dimensions of a dimension. 
 * a is output if dimension is non-dimensional (an angle)
 * b        -               has dimensions of length   
 * */
#define DIMDIM1(a, b) b
#if   GEOMETRY == SPHERICAL
  #define DIMDIM2(a, b) a
  #define DIMDIM3(a, b) a
#elif GEOMETRY == POLAR
  #define DIMDIM2(a, b) a
  #define DIMDIM3(a, b) b
#elif GEOMETRY == CARTESIAN
  #define DIMDIM2(a, b) a
  #define DIMDIM3(a, b) b
#elif GEOMETRY == CYLINDRICAL
  #define DIMDIM2(a, b) b
  #define DIMDIM3(a, b) a
#endif



/* Add normal transverse loops. Cf TRANSVERSE_LOOP in pluto.h */
#define KTDOM_LOOP(in,k,j,i) k = in; JDOM_LOOP(j) IDOM_LOOP(i)
#define JTDOM_LOOP(in,k,j,i) j = in; KDOM_LOOP(k) IDOM_LOOP(i)
#define ITDOM_LOOP(in,k,j,i) i = in; KDOM_LOOP(k) JDOM_LOOP(j)

/* Loop over all cell-centered points of cells on the surface of the domain */
#define DOM_SURF_LOOP(k,j,i,command) \
KTDOM_LOOP(KBEG,k,j,i) {command} KTDOM_LOOP(KEND,k,j,i) {command} \
JTDOM_LOOP(JBEG,k,j,i) {command} JTDOM_LOOP(JEND,k,j,i) {command} \
ITDOM_LOOP(IBEG,k,j,i) {command} ITDOM_LOOP(IEND,k,j,i) {command} 

/* The Box surface loop, adapted from BOX_LOOP */
#define BOX_SURF_LOOP(B,k,j,i,command) \
 k = (B)->kb;\
 for ((B)->dj = ((j=(B)->jb) <= (B)->je ? 1:-1); j != (B)->je+(B)->dj; j += (B)->dj)\
 for ((B)->di = ((i=(B)->ib) <= (B)->ie ? 1:-1); i != (B)->ie+(B)->di; i += (B)->di)\
 {command}\
 k = (B)->ke;\
 for ((B)->dj = ((j=(B)->jb) <= (B)->je ? 1:-1); j != (B)->je+(B)->dj; j += (B)->dj)\
 for ((B)->di = ((i=(B)->ib) <= (B)->ie ? 1:-1); i != (B)->ie+(B)->di; i += (B)->di)\
 {command}\
 j = (B)->jb;\
 for ((B)->dk = ((k=(B)->kb) <= (B)->ke ? 1:-1); k != (B)->ke+(B)->dk; k += (B)->dk)\
 for ((B)->di = ((i=(B)->ib) <= (B)->ie ? 1:-1); i != (B)->ie+(B)->di; i += (B)->di)\
 {command}\
 j = (B)->je;\
 for ((B)->dk = ((k=(B)->kb) <= (B)->ke ? 1:-1); k != (B)->ke+(B)->dk; k += (B)->dk)\
 for ((B)->di = ((i=(B)->ib) <= (B)->ie ? 1:-1); i != (B)->ie+(B)->di; i += (B)->di)\
 {command}\
 i = (B)->ib;\
 for ((B)->dk = ((k=(B)->kb) <= (B)->ke ? 1:-1); k != (B)->ke+(B)->dk; k += (B)->dk)\
 for ((B)->dj = ((j=(B)->jb) <= (B)->je ? 1:-1); j != (B)->je+(B)->dj; j += (B)->dj)\
 {command}\
 i = (B)->ie;\
 for ((B)->dk = ((k=(B)->kb) <= (B)->ke ? 1:-1); k != (B)->ke+(B)->dk; k += (B)->dk)\
 for ((B)->dj = ((j=(B)->jb) <= (B)->je ? 1:-1); j != (B)->je+(B)->dj; j += (B)->dj)\
 {command}



