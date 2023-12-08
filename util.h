#CASE
#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#define UINTMAX_MAX  UINTMAX_C  // was not defined in stdint.h
#include <stddef.h> // for size_t
#include <string.h> // for memcpy
#include <stdarg.h>  // for die handling

#include <limits.h>

/*
 * BUILD_ASSERT_OR_ZERO - assert a build-time dependency, as an expression.
 * @cond: the compile-time condition which must be true.
 *
 * Your compile will fail if the condition isn't true, or can't be evaluated
 * by the compiler.  This can be used in an expression: its value is "0".
 *
 * Example:
 *   #define foo_to_char(foo)               \
 *       ((char *)(foo)                     \
 *        + BUILD_ASSERT_OR_ZERO(offsetof(struct foo, string) == 0))
 *
 * Reference : Git Source 
 * https://github.com/git/git/blob/60566cbb5890abc84fa18c56da70e35ace0b23bf/git-compat-util.h#L46-L59
 */
#define BUILD_ASSERT_OR_ZERO(cond) \
   (sizeof(char [1 - 2*!(cond)]) - 1)

#define bitsizeof(x)  (CHAR_BIT * sizeof(x))

#define maximum_signed_value_of_type(a) \
    (INTMAX_MAX >> (bitsizeof(intmax_t) - bitsizeof(a)))

#define maximum_unsigned_value_of_type(a) \
    (UINTMAX_MAX >> (bitsizeof(uintmax_t) - bitsizeof(a)))

/* Safe(r) Copy Array
 *
 * Specify source, destination and the number of elements; the size of an
 * element is inferred automatically.
 * It checks if the multiplication of size and element count overflows.
 * The inferred size is passed first to st_mult, which allows the division
 * there to be done at compilation time.
 *
 * As a basic type safety check it makes sure the sizes of source and
 * destination elements are the same. That's evaluated at compilation
 * time as well.
 * COPY_ARRAY is safe to use with NULL as source pointer iff 0 elements
 * are to be copied.
 * That convention is used in some cases for initializing arrays.
 * Raw memcpy(3) does not support it -- compilers are allowed to assume that
 * only valid pointers are passed to it and can optimize away NULL checks
 * after such a call.
 * 
 * Reference : Git Source : 
 * https://github.com/git/git/blob/60566cbb5890abc84fa18c56da70e35ace0b23bf/git-compat-util.h#L788-L794
 */

#define COPY_ARRAY(dst, src, n) copy_array((dst), (src), (n), sizeof(*(dst)) + \
    BUILD_ASSERT_OR_ZERO(sizeof(*(dst)) == sizeof(*(src))))

//Ref: https://github.com/git/git/blob/564d0252ca632e0264ed670534a51d18a689ef5d/git-compat-util.h#L144
/*
 * Returns true if the multiplication of "a" and "b" will
 * overflow. The types of "a" and "b" must match and must be unsigned.
 * Note that this macro evaluates "a" twice!
 */
#define unsigned_mult_overflows(a, b) ((a) && (b) > maximum_unsigned_value_of_type(a) / (a))

/* If we're not using GNU C, elide __attribute__ */
#ifndef __GNUC__
#  define  __attribute__(x)  /*NOTHING*/
#endif

/* General helper functions
 */
void die( char *err, ...) __attribute__((format (printf, 1, 2)))
{
   va_list params;
   va_start(params, err);

  //    We have none of these error reporting tools yet   
      //   trace2_cmd_error_va(err, params);
      //   reportf(_("fatal: "), err, params);
   // so, we reboot.
   reset_cpu();
}


// Ref: https://github.com/git/git/blob/564d0252ca632e0264ed670534a51d18a689ef5d/git-compat-util.h#L983
static inline size_t st_mult(size_t a, size_t b)
{
   if (unsigned_mult_overflows(a, b))
      die("size_t overflow: %Ld and %Ld",
          (uintmax_t)a, (uintmax_t)b);
   return a * b;
}



static inline void copy_array(void *dst, void *src, size_t n, size_t size)
{
    if (n)
        memcpy(dst, src, st_mult(size, n));
}

#endif

