typedef union {
  int		num;
  char		*str;
  struct asciiz {
    char	*val;
    int		len;
  } data;
  tree_cell	*node;
} YYSTYPE;
#define	IF	258
#define	ELSE	259
#define	EQ	260
#define	NEQ	261
#define	SUPEQ	262
#define	INFEQ	263
#define	OR	264
#define	AND	265
#define	MATCH	266
#define	NOMATCH	267
#define	REP	268
#define	FOR	269
#define	REPEAT	270
#define	UNTIL	271
#define	FOREACH	272
#define	WHILE	273
#define	BREAK	274
#define	CONTINUE	275
#define	FUNCTION	276
#define	RETURN	277
#define	INCLUDE	278
#define	LOCAL	279
#define	GLOBAL	280
#define	PLUS_PLUS	281
#define	MINUS_MINUS	282
#define	L_SHIFT	283
#define	R_SHIFT	284
#define	R_USHIFT	285
#define	EXPO	286
#define	PLUS_EQ	287
#define	MINUS_EQ	288
#define	MULT_EQ	289
#define	DIV_EQ	290
#define	MODULO_EQ	291
#define	L_SHIFT_EQ	292
#define	R_SHIFT_EQ	293
#define	R_USHIFT_EQ	294
#define	RE_MATCH	295
#define	RE_NOMATCH	296
#define	ARROW	297
#define	IDENT	298
#define	STRING1	299
#define	STRING2	300
#define	INTEGER	301
#define	NOT	302
#define	UMINUS	303
#define	BIT_NOT	304

