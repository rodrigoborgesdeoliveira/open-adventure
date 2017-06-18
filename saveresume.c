#include <stdlib.h>
#include <string.h>

#include "advent.h"
#include "database.h"
#include "newdb.h"
#include "linenoise/linenoise.h"

/*
 * (ESR) This replaces  a bunch of particularly nasty FORTRAN-derived code;
 * see the history.adoc file in the source distribution for discussion.
 */

#define VRSION	26	/* bump on save format change */

/*
 * If you change the first three members, the resume function may not properly
 * reject saves from older versions.  Yes, this glues us to a hardware-
 * dependent length of long.  Later members can change, but bump the version
 * when you do that.
 */
struct save_t {
    long savetime;
    long mode;		/* not used, must be present for version detection */
    long version;
    struct game_t game;
    long bird;
    long bivalve;
};
struct save_t save;

/* Suspend and resume */
int suspend(void)
{
    /*  Suspend.  Offer to save things in a file, but charging
     *  some points (so can't win by using saved games to retry
     *  battles or to start over after learning zzword).
     *  If ADVENT_NOSAVE is defined, do nothing instead. */

#ifdef ADVENT_NOSAVE
    return GO_UNKNOWN;
#endif
    long i, k;
    FILE *fp = NULL;

    RSPEAK(SUSPEND_WARNING);
    if (!YES(THIS_ACCEPTABLE, OK_MAN, OK_MAN)) return GO_CLEAROBJ;
    game.saved = game.saved + 5;

    while (fp == NULL) {
        char* name = linenoise("\nFile name: ");
        if (name == NULL)
            return GO_TOP;
        fp = fopen(name, WRITE_MODE);
        if (fp == NULL)
            printf("Can't open file %s, try again.\n", name);
        linenoiseFree(name);
    }

    DATIME(&i, &k);
    k = i + 650 * k;
    save.savetime = k;
    save.mode = -1;
    save.version = VRSION;
    memcpy(&save.game, &game, sizeof(struct game_t));
    save.bird = OBJSND[BIRD];
    save.bivalve = OBJTXT[OYSTER];
    IGNORE(fwrite(&save, sizeof(struct save_t), 1, fp));
    fclose(fp);
    RSPEAK(RESUME_HELP);
    exit(0);
}

int resume(void)
{
    /*  Resume.  Read a suspended game back from a file.
     *  If ADVENT_NOSAVE is defined, do nothing instead. */

#ifdef ADVENT_NOSAVE
    return GO_UNKNOWN;
#endif
    FILE *fp = NULL;

    if (game.loc != 1 || game.abbrev[1] != 1) {
        RSPEAK(RESUME_ABANDON);
        if (!YES(THIS_ACCEPTABLE, OK_MAN, OK_MAN)) return GO_CLEAROBJ;
    }

    while (fp == NULL) {
        char* name = linenoise("\nFile name: ");
        if (name == NULL)
            return GO_TOP;
        fp = fopen(name, READ_MODE);
        if (fp == NULL)
            printf("Can't open file %s, try again.\n", name);
        linenoiseFree(name);
    }

    return restore(fp);
}

int restore(FILE* fp)
{
    /*  Read and restore game state from file, assuming
     *  sane initial state.
     *  If ADVENT_NOSAVE is defined, do nothing instead. */
#ifdef ADVENT_NOSAVE
    return GO_UNKNOWN;
#endif

    IGNORE(fread(&save, sizeof(struct save_t), 1, fp));
    fclose(fp);
    if (save.version != VRSION) {
        SETPRM(1, save.version / 10, MOD(save.version, 10));
        SETPRM(3, VRSION / 10, MOD(VRSION, 10));
        RSPEAK(VERSION_SKEW);
    } else {
        memcpy(&game, &save.game, sizeof(struct game_t));
        OBJSND[BIRD] = save.bird;
        OBJTXT[OYSTER] = save.bivalve;
        game.zzword = RNDVOC(3, game.zzword);
    }
    return GO_TOP;
}

/* end */
