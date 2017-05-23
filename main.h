#include <stdbool.h>

#define LINESIZE	100

extern long ABB[], ATAB[], ATLOC[], DLOC[], FIXED[],
		KTAB[], *LINES, LINK[], LNLENG, LNPOSN,
		PARMS[], PLACE[], PTEXT[], RTEXT[], TABSIZ;
extern signed char INLINE[LINESIZE+1], MAP1[], MAP2[];
extern FILE *logfp;
extern bool oldstyle;

struct game_t	{
    /* These members are in the save order of the individual variables
     * in the 2.5 code. */
    long abbnum;
    long blklin;
    long bonus;
    long clock1;
    long clock2;
    long closed;
    long closng;
    long detail;
    long dflag;
    long dkill;
    long dtotal;
    long foobar;
    long holdng;
    long iwest;
    long knfloc;
    long limit;
    /* LL may go here */
    long lmwarn;
    /* LOC will go here */
    long newloc;
    long numdie;
    /* more state will go here */
    long wzdark;
};
extern struct game_t game;
