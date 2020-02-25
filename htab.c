#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"

#define HTAB_BUCKETS	997	/* Prime; table doesn't have to be too big */

typedef u_int32_t hash_t;
typedef u_int32_t count_t;

typedef struct htab_ent {
    struct htab_ent *next;	/* Bucket chain pointer */
    hash_t hash;		/* Hash of signature */
    count_t count;		/* Number of occurrences */
    char *sig;			/* Signature */
} htab_ent_t;

htab_ent_t *htab[HTAB_BUCKETS];

/* Bob Jenkins' "One-At-a-Time Hash" */
static hash_t
hash_buf(const u_int8_t *key, int len)
{
    hash_t h = 0;
    int i;

    for (i = 0; i < len; i++) {
	h += key[i];
	h += (h << 10);
	h ^= (h >> 6);
    }

    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h;
}

void
htab_insert(char *sig, int sig_len)
{
    hash_t hash = hash_buf((u_int8_t *)sig, sig_len);
    int bucket = hash % HTAB_BUCKETS;
    htab_ent_t **p;
    htab_ent_t *e;

    //printf("%08x: %s\n", hash, sig);

    for (p = &htab[bucket]; (e = *p) != NULL; p = &e->next)
	if (e->hash == hash && strcmp(sig, e->sig) == 0) {
	    e->count++;
	    return;
	}

    if ((e = malloc(sizeof (htab_ent_t))) == NULL) {
	fprintf(stderr, "Out of memory\n");
	exit(1);
    }

    memset(e, 0, sizeof (htab_ent_t));

    e->next = NULL;
    e->hash = hash;
    e->count = 1;

    if ((e->sig = strdup(sig)) == NULL) {
	fprintf(stderr, "Out of memory\n");
	exit(1);
    }

    *p = e;
}

void
htab_dump(void)
{
    int bucket;
    htab_ent_t *e;

    for (bucket = 0; bucket < HTAB_BUCKETS; bucket++)
	for (e = htab[bucket]; e != NULL; e = e->next)
	    printf("%d: %s\n", e->count, e->sig);
}

void
htab_clear(void)
{
    int bucket;
    htab_ent_t *e, *f;

    for (bucket = 0; bucket < HTAB_BUCKETS; bucket++) {
	for (e = htab[bucket]; e != NULL; e = f) {
	    f = e->next;
	    free(e->sig);
	    free(e);
	}

	htab[bucket] = NULL;
    }
}
