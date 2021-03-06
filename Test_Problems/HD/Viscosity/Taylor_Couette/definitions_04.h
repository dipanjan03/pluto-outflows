#define  PHYSICS                 HD
#define  DIMENSIONS              2
#define  COMPONENTS              3
#define  GEOMETRY                CYLINDRICAL
#define  BODY_FORCE              NO
#define  COOLING                 NO
#define  INTERPOLATION           LINEAR
#define  TIME_STEPPING           HANCOCK
#define  DIMENSIONAL_SPLITTING   NO
#define  NTRACER                 0
#define  USER_DEF_PARAMETERS     2
#define  USER_DEF_CONSTANTS      0

/* -- physics dependent declarations -- */

#define  EOS                     IDEAL
#define  ENTROPY_SWITCH          NO
#define  THERMAL_CONDUCTION      NO
#define  VISCOSITY               SUPER_TIME_STEPPING
#define  ROTATING_FRAME          NO

/* -- user-defined parameters (labels) -- */

#define  OMEGA                   0
#define  REYN                    1

/* -- user-defined symbolic constants -- */


/* -- supplementary constants (user editable) -- */ 

#define  INITIAL_SMOOTHING      NO
#define  WARNING_MESSAGES       YES
#define  PRINT_TO_FILE          YES
#define  INTERNAL_BOUNDARY      NO
#define  SHOCK_FLATTENING       NO
#define  ARTIFICIAL_VISCOSITY   NO
#define  CHAR_LIMITING          NO
#define  LIMITER                MC_LIM
#define  PRIMITIVE_HANCOCK      YES
#define  STS_nu                 0.01
