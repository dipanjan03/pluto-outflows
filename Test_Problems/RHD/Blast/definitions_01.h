#define  PHYSICS                 RHD
#define  DIMENSIONS              1
#define  COMPONENTS              1
#define  GEOMETRY                CARTESIAN
#define  BODY_FORCE              NO
#define  COOLING                 NO
#define  INTERPOLATION           PARABOLIC
#define  TIME_STEPPING           CHARACTERISTIC_TRACING
#define  DIMENSIONAL_SPLITTING   YES
#define  NTRACER                 0
#define  USER_DEF_PARAMETERS     1
#define  USER_DEF_CONSTANTS      0

/* -- physics dependent declarations -- */

#define  EOS                     IDEAL
#define  ENTROPY_SWITCH          NO
#define  USE_FOUR_VELOCITY       NO

/* -- user-defined parameters (labels) -- */

#define  SCRH                    0

/* -- user-defined symbolic constants -- */


/* -- supplementary constants (user editable) -- */ 

#define  INITIAL_SMOOTHING      NO
#define  WARNING_MESSAGES       YES
#define  PRINT_TO_FILE          NO
#define  INTERNAL_BOUNDARY      NO
#define  SHOCK_FLATTENING       NO
#define  ARTIFICIAL_VISCOSITY   YES
#define  CHAR_LIMITING          YES
#define  LIMITER                DEFAULT
