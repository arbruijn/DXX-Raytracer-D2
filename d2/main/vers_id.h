/* Version defines */

#ifndef _VERS_ID
#define _VERS_ID

#define __stringize2(X)	#X
#define __stringize(X)	__stringize2(X)

#define D1X_RAYTRACER_VERSION_MAJOR __stringize(D1X_RAYTRACER_VERSION_MAJORi)
#define D1X_RAYTRACER_VERSION_MINOR __stringize(D1X_RAYTRACER_VERSION_MINORi)
#define D1X_RAYTRACER_VERSION_MICRO __stringize(D1X_RAYTRACER_VERSION_MICROi)

#define BASED_VERSION "Full Version v1.2"
#define VERSION D1X_RAYTRACER_VERSION_MAJOR "." D1X_RAYTRACER_VERSION_MINOR "." D1X_RAYTRACER_VERSION_MICRO
#define DESCENT_VERSION "D2X Raytracer " VERSION

#define VERSION_EXTRA_MAIN_SCREEN " - based on D1X Raytracer 1.1.0 by Breda University"

extern const char g_descent_version[40];
extern const char g_descent_build_datetime[21];

#endif /* _VERS_ID */
