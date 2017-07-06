#pragma once

/**
 * EXPAND(...) simply expands to its arguments without any modification.
 *
 * This seemingly useless macro is extensively used throughout other macro
 * definitions. It is important to understand why, so here is the explanation.
 *
 * This macro is required due to strange rules of __VA_ARGS__ processing in MSVC
 * compilers. Specifically, __VA_ARGS__ is treated as a single token when being
 * passed to another macro.
 *
 * This is demonstrated by the following code:
 *  
 * #define MY_MACRO(...) GET_FIRST_ARG(__VA_ARGS__)
 * #define GET_FIRST_ARG(X, ...) (X)
 *
 * MY_MACRO(1, 2, 3)
 *
 * Here, MY_MACRO is expanded to:
 * (1, 2, 3)
 * as if "1, 2, 3" (passed in __VA_ARGS__) was the one and only argument for
 * GET_FIRST_ARG.
 *
 * AFAIK, this behavior is only demonstrated by (some versions of?) MSVC
 * compiler.
 *
 * This does not happen, however, if macro call with __VA_ARGS__ argument is
 * wrapped in another macro. So, to fix the example above, we can write:
 *
 * #define EXPAND(...) __VA_ARGS__
 *
 * #define MY_MACRO(...) EXPAND(GET_FIRST_ARG(__VA_ARGS__))
 * #define GET_FIRST_ARG(X, ...) (X)
 *
 * MY_MACRO(1, 2, 3)
 *
 * I am not sure why this happens. EXPAND(...) probably gets expanded before
 * GET_FIRST_ARG, so we have something like this chain of macro expansions:
 * 1. MY_MACRO(1, 2, 3)
 * 2. EXPAND(GET_FIRST_ARG(__VA_ARGS__))
 * 3. EXPAND(GET_FIRST_ARG(1, 2, 3))
 * 4. GET_FIRST_ARG(1, 2, 3)
 * 5. (1)
 * The magic probably happens somewhere in step 3 --> 4, where "1, 2, 3" is just
 * a string, resulting in 3 separate tokens passed to GET_FIRST_ARG.
 *
 * The related part of C standard seems kinda vague, so I'm not even sure if
 * MSVC is to blame.
 */
#define EXPAND(...) __VA_ARGS__

/**
 * Helper macro for JOIN.
 */
#define POST_JOIN(X1, X2) X1 ## X2

/**
 * Join X1 and X2 with ##, expanding them first.
 *
 * If X1 and X2 are macro expressions, expand them first, then join the result
 * with ##.
 *
 * Extra macro call (POST_JOIN) is used to make sure the expansion of X1 and X2
 * happens. Maybe it's MSVC macro-bullshit again, I don't know.
 */
#define JOIN(X1, X2) POST_JOIN(X1, X2)

/**
 * Perform a call of MACRO, passing all successive arguments to it.
 *
 * This macro is required when the name of the macro to call is constructed
 * dynamically.
 */
#define CALL(MACRO, ...) EXPAND(MACRO(__VA_ARGS__))

/**
 * Internal helper macro for calculating number of arguments passed to a macro
 * call. The macro to be used is ARG_NUM.
 *
 * This is essentially the famous PP_NARG implementation (without any
 * differences, I think):
 * https://groups.google.com/forum/#!topic/comp.std.c/d-6Mj5Lko_s
 *
 * Number of arguments that can be calculated is limited to 63. If we want to
 * calculate N arguments using this method, we have to make a macro call
 * with 2*N arguments at one point; and the C standard only guarantees that 127
 * macro arguments are supported. The exact number is implementation-defined,
 * and may be much bigger, but we want to be on the safe side here.
 */
#define ARG_NUM_HELPER(                                 \
     _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10,   \
    _11, _12, _13, _14, _15, _16, _17, _18, _19, _20,   \
    _21, _22, _23, _24, _25, _26, _27, _28, _29, _30,   \
    _31, _32, _33, _34, _35, _36, _37, _38, _39, _40,   \
    _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,   \
    _51, _52, _53, _54, _55, _56, _57, _58, _59, _60,   \
    _61, _62, _63,                                      \
    NAME, ...) NAME

/**
 * Expands to number of arguments passed to this macro, in [1, 63] range.
 *
 * Take into account the supported range (you can pass from 1 to 63 arguments).
 * When called with no arguments, as in "ARG_NUM()", will expand to "0". When
 * called with more than 63 arguments, most certainly DOES NOT expand to the
 * correct result (I'm not sure if you may get UB, or are guaranteed to receive
 * inconsistent crap; who cares anyway).
 *
 * Empty arguments are also counted, so "ARG_NUM(1,,2)" expands to "3".
 *
 * The macro always expands to a single decimal literal (e.g. "1", "10", "58").
 *
 * See ARG_NUM_HELPER for implementation details.
 */
#define ARG_NUM(...)                                        \
    EXPAND(ARG_NUM_HELPER(__VA_ARGS__,                      \
                                           63,  62,  61,    \
        60,  59,  58,  57,  56,  55,  54,  53,  52,  51,    \
        50,  49,  48,  47,  46,  45,  44,  43,  42,  41,    \
        40,  39,  38,  37,  36,  35,  34,  33,  32,  31,    \
        30,  29,  28,  27,  26,  25,  24,  23,  22,  21,    \
        20,  19,  18,  17,  16,  15,  14,  13,  12,  11,    \
        10,   9,   8,   7,   6,   5,   4,   3,   2,   1     \
    ))

/**
 * APPLY_<N> applies operation OP to each of N successive arguments. It is an
 * internal set of helper macros for APPLY.
 *
 * OP must be a function-like macro, accepting a single argument.
 *
 * Example:
 * APPLY_3(MY_MACRO, ARG1, ARG2, ARG3)
 *
 * Expands to:
 * MY_MACRO(ARG1)
 * MY_MACRO(ARG2)
 * MY_MACRO(ARG3)
 */
#define APPLY_1(OP, X1) OP(X1)
#define APPLY_2(OP, X1, ...) OP(X1), EXPAND(APPLY_1(OP, __VA_ARGS__))
#define APPLY_3(OP, X1, ...) OP(X1), EXPAND(APPLY_2(OP, __VA_ARGS__))
#define APPLY_4(OP, X1, ...) OP(X1), EXPAND(APPLY_3(OP, __VA_ARGS__))
#define APPLY_5(OP, X1, ...) OP(X1), EXPAND(APPLY_4(OP, __VA_ARGS__))
#define APPLY_6(OP, X1, ...) OP(X1), EXPAND(APPLY_5(OP, __VA_ARGS__))
#define APPLY_7(OP, X1, ...) OP(X1), EXPAND(APPLY_6(OP, __VA_ARGS__))
#define APPLY_8(OP, X1, ...) OP(X1), EXPAND(APPLY_7(OP, __VA_ARGS__))
#define APPLY_9(OP, X1, ...) OP(X1), EXPAND(APPLY_8(OP, __VA_ARGS__))

#define APPLY_10(OP, X1, ...) OP(X1), EXPAND(APPLY_9(OP, __VA_ARGS__))
#define APPLY_11(OP, X1, ...) OP(X1), EXPAND(APPLY_10(OP, __VA_ARGS__))
#define APPLY_12(OP, X1, ...) OP(X1), EXPAND(APPLY_11(OP, __VA_ARGS__))
#define APPLY_13(OP, X1, ...) OP(X1), EXPAND(APPLY_12(OP, __VA_ARGS__))
#define APPLY_14(OP, X1, ...) OP(X1), EXPAND(APPLY_13(OP, __VA_ARGS__))
#define APPLY_15(OP, X1, ...) OP(X1), EXPAND(APPLY_14(OP, __VA_ARGS__))
#define APPLY_16(OP, X1, ...) OP(X1), EXPAND(APPLY_15(OP, __VA_ARGS__))
#define APPLY_17(OP, X1, ...) OP(X1), EXPAND(APPLY_16(OP, __VA_ARGS__))
#define APPLY_18(OP, X1, ...) OP(X1), EXPAND(APPLY_17(OP, __VA_ARGS__))
#define APPLY_19(OP, X1, ...) OP(X1), EXPAND(APPLY_18(OP, __VA_ARGS__))
#define APPLY_20(OP, X1, ...) OP(X1), EXPAND(APPLY_19(OP, __VA_ARGS__))

#define APPLY_21(OP, X1, ...) OP(X1), EXPAND(APPLY_20(OP, __VA_ARGS__))
#define APPLY_22(OP, X1, ...) OP(X1), EXPAND(APPLY_21(OP, __VA_ARGS__))
#define APPLY_23(OP, X1, ...) OP(X1), EXPAND(APPLY_22(OP, __VA_ARGS__))
#define APPLY_24(OP, X1, ...) OP(X1), EXPAND(APPLY_23(OP, __VA_ARGS__))
#define APPLY_25(OP, X1, ...) OP(X1), EXPAND(APPLY_24(OP, __VA_ARGS__))
#define APPLY_26(OP, X1, ...) OP(X1), EXPAND(APPLY_25(OP, __VA_ARGS__))
#define APPLY_27(OP, X1, ...) OP(X1), EXPAND(APPLY_26(OP, __VA_ARGS__))
#define APPLY_28(OP, X1, ...) OP(X1), EXPAND(APPLY_27(OP, __VA_ARGS__))
#define APPLY_29(OP, X1, ...) OP(X1), EXPAND(APPLY_28(OP, __VA_ARGS__))
#define APPLY_30(OP, X1, ...) OP(X1), EXPAND(APPLY_29(OP, __VA_ARGS__))

#define APPLY_31(OP, X1, ...) OP(X1), EXPAND(APPLY_30(OP, __VA_ARGS__))
#define APPLY_32(OP, X1, ...) OP(X1), EXPAND(APPLY_31(OP, __VA_ARGS__))
#define APPLY_33(OP, X1, ...) OP(X1), EXPAND(APPLY_32(OP, __VA_ARGS__))
#define APPLY_34(OP, X1, ...) OP(X1), EXPAND(APPLY_33(OP, __VA_ARGS__))
#define APPLY_35(OP, X1, ...) OP(X1), EXPAND(APPLY_34(OP, __VA_ARGS__))
#define APPLY_36(OP, X1, ...) OP(X1), EXPAND(APPLY_35(OP, __VA_ARGS__))
#define APPLY_37(OP, X1, ...) OP(X1), EXPAND(APPLY_36(OP, __VA_ARGS__))
#define APPLY_38(OP, X1, ...) OP(X1), EXPAND(APPLY_37(OP, __VA_ARGS__))
#define APPLY_39(OP, X1, ...) OP(X1), EXPAND(APPLY_38(OP, __VA_ARGS__))
#define APPLY_40(OP, X1, ...) OP(X1), EXPAND(APPLY_39(OP, __VA_ARGS__))

#define APPLY_41(OP, X1, ...) OP(X1), EXPAND(APPLY_40(OP, __VA_ARGS__))
#define APPLY_42(OP, X1, ...) OP(X1), EXPAND(APPLY_41(OP, __VA_ARGS__))
#define APPLY_43(OP, X1, ...) OP(X1), EXPAND(APPLY_42(OP, __VA_ARGS__))
#define APPLY_44(OP, X1, ...) OP(X1), EXPAND(APPLY_43(OP, __VA_ARGS__))
#define APPLY_45(OP, X1, ...) OP(X1), EXPAND(APPLY_44(OP, __VA_ARGS__))
#define APPLY_46(OP, X1, ...) OP(X1), EXPAND(APPLY_45(OP, __VA_ARGS__))
#define APPLY_47(OP, X1, ...) OP(X1), EXPAND(APPLY_46(OP, __VA_ARGS__))
#define APPLY_48(OP, X1, ...) OP(X1), EXPAND(APPLY_47(OP, __VA_ARGS__))
#define APPLY_49(OP, X1, ...) OP(X1), EXPAND(APPLY_48(OP, __VA_ARGS__))
#define APPLY_50(OP, X1, ...) OP(X1), EXPAND(APPLY_49(OP, __VA_ARGS__))

#define APPLY_51(OP, X1, ...) OP(X1), EXPAND(APPLY_50(OP, __VA_ARGS__))
#define APPLY_52(OP, X1, ...) OP(X1), EXPAND(APPLY_51(OP, __VA_ARGS__))
#define APPLY_53(OP, X1, ...) OP(X1), EXPAND(APPLY_52(OP, __VA_ARGS__))
#define APPLY_54(OP, X1, ...) OP(X1), EXPAND(APPLY_53(OP, __VA_ARGS__))
#define APPLY_55(OP, X1, ...) OP(X1), EXPAND(APPLY_54(OP, __VA_ARGS__))
#define APPLY_56(OP, X1, ...) OP(X1), EXPAND(APPLY_55(OP, __VA_ARGS__))
#define APPLY_57(OP, X1, ...) OP(X1), EXPAND(APPLY_56(OP, __VA_ARGS__))
#define APPLY_58(OP, X1, ...) OP(X1), EXPAND(APPLY_57(OP, __VA_ARGS__))
#define APPLY_59(OP, X1, ...) OP(X1), EXPAND(APPLY_58(OP, __VA_ARGS__))
#define APPLY_60(OP, X1, ...) OP(X1), EXPAND(APPLY_59(OP, __VA_ARGS__))

#define APPLY_61(OP, X1, ...) OP(X1), EXPAND(APPLY_60(OP, __VA_ARGS__))
#define APPLY_62(OP, X1, ...) OP(X1), EXPAND(APPLY_61(OP, __VA_ARGS__))
//#define APPLY_63(OP, X1, ...) OP(X1), EXPAND(APPLY_62(OP, __VA_ARGS__))
#define APPLY_63(OP, X1, ...) static_assert(false, "Sorry")

/**
 * Apply operation OP to each of successive arguments.
 *
 * OP must be a function-like macro, accepting a single argument.
 *
 * Example:
 * APPLY(MY_MACRO, ARG1, ARG2, ARG3)
 *
 * This results in:
 * MY_MACRO(ARG1)
 * MY_MACRO(ARG2)
 * MY_MACRO(ARG3)
 */
#define APPLY(OP, ...)                                                      \
    CALL(JOIN(APPLY_, ARG_NUM(__VA_ARGS__)), OP, __VA_ARGS__)

#define FIELD_NAME(FIELD) #FIELD
#define FIELD_REF(FIELD) FIELD

#define FIELD_REF_TYPE(FIELD) decltype(FIELD) &

#define DEFINE_FIELDS(...)                                                  \
    ::ecosnail::config::Fields< EXPAND(APPLY(FIELD_REF_TYPE, __VA_ARGS__)) >            \
    define_fields()                                                         \
    {                                                                       \
        return {                                                            \
            { EXPAND(APPLY(FIELD_NAME, __VA_ARGS__)) },                     \
            { EXPAND(APPLY(FIELD_REF, __VA_ARGS__)) }                       \
        };                                                                  \
    }
