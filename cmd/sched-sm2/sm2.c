#include <stdio.h>
#include <stdlib.h>

typedef int bool;
static char *argv0;
#define inline

/* SM-2
 * http://super-memory.com/english/ol/sm2.htm
 *
 * SPDX-License-Identifier: Unlicense
 * C99 (or C89 if you remove inlines)
 */

inline static long
new_ivl(long reps, double ease)
{
	if(reps <= 1)
		return 1;
	if(reps == 2)
		return 6;

	return new_ivl(reps-1, ease)*ease;
}

inline static double
new_ease(double ease, int score)
{
	return ease+(0.1-(5-score)*(0.08+(5-score)*0.02));
}

inline static void
usage(void)
{
	fprintf(stderr, "usage: %s reps ease score\n", argv0);
	exit(EXIT_FAILURE);
}

int
main(int argc, char **argv)
{
	char *endp;
	long reps, r_ivl;
	double ease, r_ease;
	int score;
	bool lapse;

	argv0 = argv[0];

	if(argc < 4)
		usage();

	reps = strtol(argv[1], &endp, 10);
	if(*endp != '\0' || *argv[1] == '\0')
		usage();

	ease = strtod(argv[2], &endp);
	if(*endp != '\0' || *argv[2] == '\0')
		usage();

	score = (int)strtol(argv[3], &endp, 10);
	if(*endp != '\0' || *argv[3] == '\0')
		usage();

	if(score < 3) {
		r_ease = ease;
		r_ivl = 1;
		lapse = 1;
	} else {
		r_ease = new_ease(ease, score);
		r_ivl = new_ivl(reps, r_ease);
		lapse = 0;
	}

	/* I think you should write the initial working implementation on 9front */
	/* TODO: implement anki fork of sm2 too (in a separate file obviously)
	         https://apps.ankiweb.net/docs/manual20.html#what-spaced-repetition-algorithm-does-anki-use
	         https://github.com/ankitects/anki/blob/2.1.21/pylib/anki/sched.py#L959
	 */
	/* TODO: ease is supposed to be clamped between [1.3, 2.5] */
	/* TODO: check if reps aren't reset after a lapse (doesn't need to be implemented here, just reset rep count if "lapse"d) */
	/* TODO: After each repetition session of a given day repeat again
	 *       all items that scored below four in the quality assessment.
	 *       Continue the repetitions until all of these items score at
	 *       least four.
	 *      (probably out of scope, but good to keep in mind as other
	 *       algorithms might not actually want that, so you might need
	 *       to account for this here)
	 */
	printf("%ld\t%.2f\t%s\n", r_ivl, r_ease, lapse ? "lapse" : "");

	return EXIT_SUCCESS;
}
