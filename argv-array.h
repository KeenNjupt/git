#ifndef ARGV_ARRAY_H
#define ARGV_ARRAY_H

/**
 * The argv-array API allows one to dynamically build and store
 * NULL-terminated lists.  An argv-array maintains the invariant that the
 * `argv` member always points to a non-NULL array, and that the array is
 * always NULL-terminated at the element pointed to by `argv[argc]`. This
 * makes the result suitable for passing to functions expecting to receive
 * argv from main().
 *
 * The string-list API (documented in string-list.h) is similar, but cannot be
 * used for these purposes; instead of storing a straight string pointer,
 * it contains an item structure with a `util` field that is not compatible
 * with the traditional argv interface.
 *
 * Each `strvec` manages its own memory. Any strings pushed into the
 * array are duplicated, and all memory is freed by strvec_clear().
 */

extern const char *empty_strvec[];

/**
 * A single array. This should be initialized by assignment from
 * `STRVEC_INIT`, or by calling `strvec_init`. The `argv`
 * member contains the actual array; the `argc` member contains the
 * number of elements in the array, not including the terminating
 * NULL.
 */
struct strvec {
	const char **argv;
	size_t argc;
	size_t alloc;
};

#define STRVEC_INIT { empty_strvec, 0, 0 }

/**
 * Initialize an array. This is no different than assigning from
 * `STRVEC_INIT`.
 */
void strvec_init(struct strvec *);

/* Push a copy of a string onto the end of the array. */
const char *strvec_push(struct strvec *, const char *);

/**
 * Format a string and push it onto the end of the array. This is a
 * convenience wrapper combining `strbuf_addf` and `strvec_push`.
 */
__attribute__((format (printf,2,3)))
const char *strvec_pushf(struct strvec *, const char *fmt, ...);

/**
 * Push a list of strings onto the end of the array. The arguments
 * should be a list of `const char *` strings, terminated by a NULL
 * argument.
 */
LAST_ARG_MUST_BE_NULL
void strvec_pushl(struct strvec *, ...);

/* Push a null-terminated array of strings onto the end of the array. */
void strvec_pushv(struct strvec *, const char **);

/**
 * Remove the final element from the array. If there are no
 * elements in the array, do nothing.
 */
void strvec_pop(struct strvec *);

/* Splits by whitespace; does not handle quoted arguments! */
void strvec_split(struct strvec *, const char *);

/**
 * Free all memory associated with the array and return it to the
 * initial, empty state.
 */
void strvec_clear(struct strvec *);

/**
 * Disconnect the `argv` member from the `strvec` struct and
 * return it. The caller is responsible for freeing the memory used
 * by the array, and by the strings it references. After detaching,
 * the `strvec` is in a reinitialized state and can be pushed
 * into again.
 */
const char **strvec_detach(struct strvec *);

/* compatibility for historic argv_array interface */
#define argv_array strvec
#define ARGV_ARRAY_INIT STRVEC_INIT
#define argv_array_init strvec_init
#define argv_array_push strvec_push
#define argv_array_pushf strvec_pushf
#define argv_array_pushl strvec_pushl
#define argv_array_pushv strvec_pushv
#define argv_array_pop strvec_pop
#define argv_array_split strvec_split
#define argv_array_clear strvec_clear
#define argv_array_detach strvec_detach

#endif /* ARGV_ARRAY_H */
