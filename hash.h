/*
 * Hash table size should be a fraction of the number of prototype lines
 * expected.
 */

#define HASH_INIT(hacc)					\
    ((hacc) = 0)

#define HASH_CHAR(hacc, c) ({				\
    (hacc) = ((hacc) << 4) ^ ((hacc) >> 28);		\
    (hacc) ^= (u_int8_t)(c);				\
})

#define HASH_STRING(hacc, s) ({				\
    char _ch, *_t = (s);				\
    while ((_ch = *_t++) != '\0')			\
	HASH_CHAR((hacc), _ch);				\
})
