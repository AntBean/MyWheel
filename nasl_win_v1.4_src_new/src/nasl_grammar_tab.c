
/*  A Bison parser, made from ..\src\nasl_grammar.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define yyparse naslparse
#define yylex nasllex
#define yyerror naslerror
#define yylval nasllval
#define yychar naslchar
#define yydebug nasldebug
#define yynerrs naslnerrs
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

#line 1 "..\src\nasl_grammar.y"

/* Nessus Attack Scripting Language version 2
 *
 * Copyright (C) 2002 - 2004 Michel Arboi and Renaud Deraison
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2,
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#define YYPARSE_PARAM parm
#define YYLEX_PARAM parm
#include <malloc.h>
#define LNB	(((naslctxt*)parm)->line_nb)
#include "includes.h"
#include "nasl_tree.h"
#include "nasl_global_ctxt.h"
#include "nasl_func.h"
#include "nasl_var.h"
#include "nasl_lex_ctxt.h"
#include "nasl_debug.h"

static void naslerror(const char *);
#define YYERROR_VERBOSE

#line 36 "..\src\nasl_grammar.y"
typedef union {
  int		num;
  char		*str;
  struct asciiz {
    char	*val;
    int		len;
  } data;
  tree_cell	*node;
} YYSTYPE;

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		227
#define	YYFLAG		-32768
#define	YYNTBASE	73

#define YYTRANSLATE(x) ((unsigned)(x) <= 304 ? yytranslate[x] : 115)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    70,     2,     2,     2,    57,    52,     2,    61,
    62,    55,    53,    63,    54,    72,    56,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    67,    66,    48,
    47,    49,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    68,     2,    69,    51,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    64,    50,    65,    71,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    58,    59,    60
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     4,     7,     9,    11,    18,    19,    21,    23,
    27,    31,    34,    36,    39,    42,    44,    46,    48,    50,
    52,    54,    56,    58,    60,    62,    64,    66,    68,    69,
    72,    74,    80,    88,    90,    92,    94,    96,   106,   112,
   118,   125,   127,   129,   131,   132,   136,   138,   140,   145,
   150,   152,   153,   155,   159,   161,   165,   169,   173,   177,
   181,   185,   189,   193,   197,   201,   203,   205,   207,   209,
   214,   216,   219,   222,   225,   228,   232,   236,   239,   243,
   247,   251,   254,   257,   261,   265,   269,   273,   277,   281,
   285,   289,   293,   297,   299,   303,   307,   311,   315,   319,
   323,   327,   331,   335,   339,   341,   343,   345,   347,   349,
   353,   355,   359,   361,   365,   367,   369,   371,   373,   375,
   377,   379,   381,   389,   392
};

static const short yyrhs[] = {    74,
     0,    75,     0,    75,    74,     0,    81,     0,    76,     0,
    21,   100,    61,    77,    62,    79,     0,     0,    78,     0,
   100,     0,   100,    63,    78,     0,    64,    80,    65,     0,
    64,    65,     0,    81,     0,    81,    80,     0,    82,    66,
     0,    79,     0,    84,     0,    85,     0,    98,     0,   103,
     0,    91,     0,    94,     0,    83,     0,    93,     0,   113,
     0,   114,     0,    19,     0,    20,     0,     0,    22,   104,
     0,    22,     0,     3,    61,   104,    62,    81,     0,     3,
    61,   104,    62,    81,     4,    81,     0,    86,     0,    87,
     0,    88,     0,    89,     0,    14,    61,    90,    66,   104,
    66,    90,    62,    81,     0,    18,    61,   104,    62,    81,
     0,    15,    81,    16,   104,    66,     0,    17,   100,    61,
   104,    62,    81,     0,    98,     0,   103,     0,    94,     0,
     0,    94,    13,   104,     0,    44,     0,    45,     0,    23,
    61,    92,    62,     0,   100,    61,    95,    62,     0,    96,
     0,     0,    97,     0,    97,    63,    96,     0,   104,     0,
   100,    67,   104,     0,    99,    47,   104,     0,    99,    32,
   104,     0,    99,    33,   104,     0,    99,    34,   104,     0,
    99,    35,   104,     0,    99,    36,   104,     0,    99,    38,
   104,     0,    99,    39,   104,     0,    99,    37,   104,     0,
   100,     0,   101,     0,    43,     0,    13,     0,   100,    68,
   102,    69,     0,   104,     0,    26,    99,     0,    27,    99,
     0,    99,    26,     0,    99,    27,     0,    61,   104,    62,
     0,   104,    10,   104,     0,    70,   104,     0,   104,     9,
   104,     0,   104,    53,   104,     0,   104,    54,   104,     0,
    54,   104,     0,    71,   104,     0,   104,    55,   104,     0,
   104,    31,   104,     0,   104,    56,   104,     0,   104,    57,
   104,     0,   104,    52,   104,     0,   104,    51,   104,     0,
   104,    50,   104,     0,   104,    29,   104,     0,   104,    30,
   104,     0,   104,    28,   104,     0,   103,     0,   104,    11,
   104,     0,   104,    12,   104,     0,   104,    40,    92,     0,
   104,    41,    92,     0,   104,    48,   104,     0,   104,    49,
   104,     0,   104,     5,   104,     0,   104,     6,   104,     0,
   104,     7,   104,     0,   104,     8,   104,     0,   110,     0,
    98,     0,   112,     0,   108,     0,   105,     0,    68,   106,
    69,     0,   107,     0,   107,    63,   106,     0,   109,     0,
    92,    42,   109,     0,    46,     0,    45,     0,    44,     0,
   108,     0,   111,     0,   101,     0,    94,     0,   100,     0,
    46,    72,    46,    72,    46,    72,    46,     0,    24,    77,
     0,    25,    77,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   130,   135,   141,   148,   148,   151,   159,   159,   160,   161,
   169,   169,   170,   171,   185,   185,   185,   185,   188,   188,
   188,   189,   189,   189,   189,   189,   190,   194,   198,   201,
   206,   214,   220,   228,   228,   228,   228,   229,   239,   246,
   254,   263,   263,   263,   263,   266,   274,   274,   277,   310,
   317,   317,   318,   318,   324,   330,   338,   342,   343,   344,
   345,   346,   347,   348,   349,   352,   352,   354,   354,   356,
   363,   365,   367,   368,   369,   373,   374,   375,   376,   377,
   378,   379,   380,   381,   382,   383,   384,   385,   386,   387,
   388,   389,   390,   391,   392,   393,   394,   395,   396,   397,
   398,   399,   400,   401,   402,   402,   402,   402,   402,   405,
   407,   408,   412,   415,   421,   422,   426,   431,   433,   434,
   434,   436,   438,   448,   456
};

static const char * const yytname[] = {   "$","error","$undefined.","IF","ELSE",
"EQ","NEQ","SUPEQ","INFEQ","OR","AND","MATCH","NOMATCH","REP","FOR","REPEAT",
"UNTIL","FOREACH","WHILE","BREAK","CONTINUE","FUNCTION","RETURN","INCLUDE","LOCAL",
"GLOBAL","PLUS_PLUS","MINUS_MINUS","L_SHIFT","R_SHIFT","R_USHIFT","EXPO","PLUS_EQ",
"MINUS_EQ","MULT_EQ","DIV_EQ","MODULO_EQ","L_SHIFT_EQ","R_SHIFT_EQ","R_USHIFT_EQ",
"RE_MATCH","RE_NOMATCH","ARROW","IDENT","STRING1","STRING2","INTEGER","'='",
"'<'","'>'","'|'","'^'","'&'","'+'","'-'","'*'","'/'","'%'","NOT","UMINUS","BIT_NOT",
"'('","')'","','","'{'","'}'","';'","':'","'['","']'","'!'","'~'","'.'","tiptop",
"instr_decl_list","instr_decl","func_decl","arg_decl","arg_decl_1","block","instr_list",
"instr","simple_instr","ret","if_block","loop","for_loop","while_loop","repeat_loop",
"foreach_loop","aff_func","rep","string","inc","func_call","arg_list","arg_list_1",
"arg","aff","lvalue","identifier","array_elem","array_index","post_pre_incr",
"expr","const_array","list_array_data","array_data","atom","simple_array_data",
"var","var_name","ipaddr","loc","glob",""
};
#endif

static const short yyr1[] = {     0,
    73,    74,    74,    75,    75,    76,    77,    77,    78,    78,
    79,    79,    80,    80,    81,    81,    81,    81,    82,    82,
    82,    82,    82,    82,    82,    82,    82,    82,    82,    83,
    83,    84,    84,    85,    85,    85,    85,    86,    87,    88,
    89,    90,    90,    90,    90,    91,    92,    92,    93,    94,
    95,    95,    96,    96,    97,    97,    98,    98,    98,    98,
    98,    98,    98,    98,    98,    99,    99,   100,   100,   101,
   102,   103,   103,   103,   103,   104,   104,   104,   104,   104,
   104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
   104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
   104,   104,   104,   104,   104,   104,   104,   104,   104,   105,
   106,   106,   107,   107,   108,   108,   108,   109,   110,   110,
   110,   111,   112,   113,   114
};

static const short yyr2[] = {     0,
     1,     1,     2,     1,     1,     6,     0,     1,     1,     3,
     3,     2,     1,     2,     2,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     1,     0,     2,
     1,     5,     7,     1,     1,     1,     1,     9,     5,     5,
     6,     1,     1,     1,     0,     3,     1,     1,     4,     4,
     1,     0,     1,     3,     1,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     1,     1,     1,     1,     4,
     1,     2,     2,     2,     2,     3,     3,     2,     3,     3,
     3,     2,     2,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     1,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     1,     1,     1,     1,     1,     3,
     1,     3,     1,     3,     1,     1,     1,     1,     1,     1,
     1,     1,     7,     2,     2
};

static const short yydefact[] = {    29,
     0,    69,     0,    29,     0,     0,    27,    28,     0,    31,
     0,     7,     7,     0,     0,    68,    29,     1,     2,     5,
    16,     4,     0,    23,    17,    18,    34,    35,    36,    37,
    21,    24,    22,    19,     0,    66,    67,    20,    25,    26,
     0,    45,     0,     0,     0,     0,   117,   116,   115,     0,
     0,     0,     0,     0,   121,   106,   122,   120,    94,    30,
   109,   108,   105,   119,   107,     0,   124,     8,     9,   125,
    72,    66,    73,    12,     0,    13,     3,    15,     0,    74,
    75,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    52,     0,     0,     0,    44,    42,    43,     0,     0,     0,
     7,     0,    82,     0,   117,   116,   115,     0,     0,   111,
   118,   113,    78,    83,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,    47,    48,
     0,     0,    11,    14,    46,    58,    59,    60,    61,    62,
    65,    63,    64,    57,     0,    51,    53,   122,    55,     0,
    71,    29,     0,     0,     0,    29,     0,     0,    76,     0,
   110,     0,   101,   102,   103,   104,    79,    77,    95,    96,
    93,    91,    92,    85,    97,    98,    99,   100,    90,    89,
    88,    80,    81,    84,    86,    87,    49,    10,    50,     0,
     0,    70,    32,     0,    40,    29,    39,     0,     0,   114,
   112,    54,    56,    29,    45,    41,     6,     0,    33,     0,
     0,    29,   123,    38,     0,     0,     0
};

static const short yydefgoto[] = {   225,
    18,    19,    20,    67,    68,    21,    75,    22,    23,    24,
    25,    26,    27,    28,    29,    30,    94,    31,   108,    32,
    55,   155,   156,   157,    56,    35,    57,    58,   160,    59,
   159,    61,   109,   110,    62,   112,    63,    64,    65,    39,
    40
};

static const short yypact[] = {   482,
   -54,-32768,   -50,   509,    -5,   -30,-32768,-32768,    -5,    13,
   -28,    -5,    -5,    -5,    -5,-32768,   348,-32768,    90,-32768,
-32768,-32768,   -22,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,    23,-32768,   361,    -7,-32768,-32768,-32768,-32768,
    13,    19,    34,    -6,    13,     2,-32768,-32768,     1,    13,
    13,    25,    13,    13,-32768,-32768,   927,   593,-32768,   780,
-32768,-32768,-32768,-32768,-32768,    41,-32768,-32768,     5,-32768,
-32768,     7,-32768,-32768,    15,   121,-32768,-32768,    13,-32768,
-32768,    13,    13,    13,    13,    13,    13,    13,    13,    13,
    13,    13,   548,    21,-32768,-32768,-32768,    13,    13,   606,
    -5,    43,    61,   664,    55,    56,-32768,    57,    49,    39,
-32768,-32768,    61,    61,    13,    13,    13,    13,    13,    13,
    13,    13,    13,    13,    13,    13,    41,    41,    13,    13,
    13,    13,    13,    13,    13,    13,    13,    13,-32768,-32768,
    58,    -5,-32768,-32768,   780,   780,   780,   780,   780,   780,
   780,   780,   780,   780,    60,-32768,    62,   239,   780,    54,
   780,   509,    13,   374,   722,   509,    64,    59,-32768,    33,
-32768,    25,   864,   864,   864,   864,   833,   895,   864,   864,
   485,   485,   485,    61,-32768,-32768,   864,   864,   390,   434,
   537,    -4,    -4,    61,    61,    61,-32768,-32768,-32768,    13,
    13,-32768,   123,   427,-32768,   509,-32768,    65,    82,-32768,
-32768,-32768,   780,   509,    19,-32768,-32768,    77,-32768,    68,
    86,   509,-32768,-32768,   137,   150,-32768
};

static const short yypgoto[] = {-32768,
   132,-32768,-32768,   -11,    10,   -55,    79,    12,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,   -58,-32768,   -63,-32768,
    18,-32768,   -42,-32768,    24,    81,     0,     6,-32768,    30,
   206,-32768,   -13,-32768,   -51,   -10,-32768,-32768,-32768,-32768,
-32768
};


#define	YYLAST		995


static const short yytable[] = {    36,
   111,    70,   141,    36,    44,    37,    41,     2,    46,    37,
    42,    69,    69,    72,    72,    43,    36,    33,    36,    37,
    37,    33,    37,    34,    37,     2,   126,    34,    76,    38,
    45,     2,    66,    38,    33,    79,    33,    16,    14,    15,
    34,    36,    34,    78,    14,    15,    38,    37,    38,    98,
   136,   137,   138,    91,    99,    16,    47,    48,    49,    95,
    92,    16,   101,   185,   186,    96,    50,   142,   105,   106,
   107,    97,   102,    51,    92,    36,    47,    48,   107,   143,
    52,    37,    53,    54,   139,   140,   163,    76,   168,   167,
   158,   126,     1,    33,    71,    73,   -47,   -48,   170,    34,
    69,   172,     2,     3,     4,    38,     5,     6,     7,     8,
     9,    10,    11,    12,    13,    14,    15,   171,   111,   197,
   111,   199,   202,     1,   200,   208,   214,   218,    17,   222,
   209,   223,    16,     2,     3,     4,   226,     5,     6,     7,
     8,    69,    10,    11,    12,    13,    14,    15,   221,   227,
    77,   198,   217,    17,   144,   -29,   220,   212,   211,   210,
     0,    36,     0,    16,     0,    36,     0,    37,     0,     0,
     0,    37,     0,   203,     0,     0,     0,   207,     0,    33,
     0,     0,     0,    33,    17,    34,   -29,     0,     0,    34,
     0,    38,     0,     0,     0,    38,     0,     0,     0,   158,
     0,     0,     0,     0,     0,    36,     0,     0,     0,     0,
     0,    37,     0,    36,    36,    60,     0,   216,     0,    37,
    37,    36,     0,    33,     0,   219,     0,    37,     0,    34,
     0,    33,    95,   224,     0,    38,     0,    34,    96,    33,
     0,     0,     0,    38,    97,    34,    93,     0,     0,     0,
   100,    38,     0,     0,     0,   103,   104,     0,   113,   114,
     0,     0,     0,     0,   -66,   -66,     0,     0,     0,     0,
   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,     0,     0,
     0,     0,     0,     0,   145,   -66,     0,   146,   147,   148,
   149,   150,   151,   152,   153,   154,     0,   161,     0,    91,
     0,     0,     0,   164,   165,   201,    92,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
   183,   184,     0,     0,   187,   188,   189,   190,   191,   192,
   193,   194,   195,   196,     0,     0,     0,     0,     0,     0,
     1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     2,     3,     4,     0,     5,     6,     7,     8,   204,    10,
    11,    12,    13,    14,    15,     0,     0,     0,   115,   116,
   117,   118,   119,   120,   121,   122,    80,    81,     0,     0,
    16,     0,    82,    83,    84,    85,    86,    87,    88,    89,
     0,   123,   124,   125,   126,     0,   213,    90,     0,     0,
     0,    17,    74,   127,   128,     0,     0,   123,   124,   125,
   126,   129,   130,   131,   132,   133,   134,   135,   136,   137,
   138,   115,   116,   117,   118,   119,   120,   121,   122,   205,
   132,   133,   134,   135,   136,   137,   138,     0,     0,     0,
     0,     0,     0,     0,   123,   124,   125,   126,     0,     0,
     0,   123,   124,   125,   126,     0,   127,   128,     0,     0,
     0,     0,     0,     0,   129,   130,   131,   132,   133,   134,
   135,   136,   137,   138,     1,   133,   134,   135,   136,   137,
   138,     0,   215,     0,     2,     3,     4,     0,     5,     6,
     7,     8,     9,    10,    11,    12,    13,    14,    15,     0,
     0,     1,-32768,-32768,-32768,   126,     0,     0,     0,     0,
     0,     2,     3,     4,    16,     5,     6,     7,     8,     0,
    10,    11,    12,    13,    14,    15,     0,   134,   135,   136,
   137,   138,     0,     0,     0,    17,     0,     0,     0,     0,
     0,    16,   115,   116,   117,   118,   119,   120,   121,   122,
     0,     0,     0,     0,   123,   124,   125,   126,     0,     0,
     0,     0,    17,     0,     0,   123,   124,   125,   126,     0,
     0,     0,     0,     0,     0,     0,     0,   127,   128,   134,
   135,   136,   137,   138,     0,   129,   130,   131,   132,   133,
   134,   135,   136,   137,   138,     0,     0,     0,     0,   162,
   115,   116,   117,   118,   119,   120,   121,   122,   -67,   -67,
     0,     0,     0,     0,   -67,   -67,   -67,   -67,   -67,   -67,
   -67,   -67,     0,   123,   124,   125,   126,     0,     0,   -67,
     0,     0,     0,     0,     0,   127,   128,     0,     0,     0,
     0,     0,     0,   129,   130,   131,   132,   133,   134,   135,
   136,   137,   138,     0,     0,     0,     0,   166,   115,   116,
   117,   118,   119,   120,   121,   122,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,   123,   124,   125,   126,     0,     0,     0,     0,     0,
     0,     0,     0,   127,   128,     0,     0,     0,     0,     0,
     0,   129,   130,   131,   132,   133,   134,   135,   136,   137,
   138,     0,     0,     0,     0,   169,   115,   116,   117,   118,
   119,   120,   121,   122,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,   123,
   124,   125,   126,     0,     0,     0,     0,     0,     0,     0,
     0,   127,   128,     0,     0,     0,     0,     0,     0,   129,
   130,   131,   132,   133,   134,   135,   136,   137,   138,     0,
     0,     0,     0,   206,   115,   116,   117,   118,   119,   120,
   121,   122,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,   123,   124,   125,
   126,     0,     0,     0,     0,     0,     0,     0,     0,   127,
   128,     0,     0,     0,     0,     0,     0,   129,   130,   131,
   132,   133,   134,   135,   136,   137,   138,   115,   116,   117,
   118,     0,   120,   121,   122,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
   123,   124,   125,   126,     0,     0,     0,     0,-32768,-32768,
-32768,-32768,   127,   128,-32768,-32768,     0,     0,     0,     0,
   129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     0,   123,   124,   125,   126,     0,     0,     0,     0,   115,
   116,   117,   118,-32768,-32768,   121,   122,     0,     0,     0,
     0,-32768,-32768,   131,   132,   133,   134,   135,   136,   137,
   138,     0,   123,   124,   125,   126,     0,     0,     0,     0,
     0,     0,     0,     0,   127,   128,     0,     0,     0,     0,
     0,     0,   129,   130,   131,   132,   133,   134,   135,   136,
   137,   138,   -66,   -66,     0,     0,     0,     0,   -66,   -66,
   -66,   -66,   -66,   -66,   -66,   -66,     0,     0,     0,     0,
     0,     0,     0,   -66,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,    91,     0,     0,
     0,     0,     0,     0,    92
};

static const short yycheck[] = {     0,
    52,    13,    66,     4,     5,     0,    61,    13,     9,     4,
    61,    12,    13,    14,    15,     4,    17,     0,    19,    14,
    15,     4,    17,     0,    19,    13,    31,     4,    17,     0,
    61,    13,    61,     4,    17,    13,    19,    43,    26,    27,
    17,    42,    19,    66,    26,    27,    17,    42,    19,    16,
    55,    56,    57,    61,    61,    43,    44,    45,    46,    42,
    68,    43,    61,   127,   128,    42,    54,    63,    44,    45,
    46,    42,    72,    61,    68,    76,    44,    45,    46,    65,
    68,    76,    70,    71,    44,    45,    66,    76,    46,   101,
    91,    31,     3,    76,    14,    15,    42,    42,    42,    76,
   101,    63,    13,    14,    15,    76,    17,    18,    19,    20,
    21,    22,    23,    24,    25,    26,    27,    69,   170,    62,
   172,    62,    69,     3,    63,    62,     4,    46,    64,    62,
    72,    46,    43,    13,    14,    15,     0,    17,    18,    19,
    20,   142,    22,    23,    24,    25,    26,    27,    72,     0,
    19,   142,   208,    64,    76,    66,   215,   200,   172,   170,
    -1,   162,    -1,    43,    -1,   166,    -1,   162,    -1,    -1,
    -1,   166,    -1,   162,    -1,    -1,    -1,   166,    -1,   162,
    -1,    -1,    -1,   166,    64,   162,    66,    -1,    -1,   166,
    -1,   162,    -1,    -1,    -1,   166,    -1,    -1,    -1,   200,
    -1,    -1,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,
    -1,   206,    -1,   214,   215,    10,    -1,   206,    -1,   214,
   215,   222,    -1,   206,    -1,   214,    -1,   222,    -1,   206,
    -1,   214,   215,   222,    -1,   206,    -1,   214,   215,   222,
    -1,    -1,    -1,   214,   215,   222,    41,    -1,    -1,    -1,
    45,   222,    -1,    -1,    -1,    50,    51,    -1,    53,    54,
    -1,    -1,    -1,    -1,    26,    27,    -1,    -1,    -1,    -1,
    32,    33,    34,    35,    36,    37,    38,    39,    -1,    -1,
    -1,    -1,    -1,    -1,    79,    47,    -1,    82,    83,    84,
    85,    86,    87,    88,    89,    90,    -1,    92,    -1,    61,
    -1,    -1,    -1,    98,    99,    67,    68,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
   115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
   125,   126,    -1,    -1,   129,   130,   131,   132,   133,   134,
   135,   136,   137,   138,    -1,    -1,    -1,    -1,    -1,    -1,
     3,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    13,    14,    15,    -1,    17,    18,    19,    20,   163,    22,
    23,    24,    25,    26,    27,    -1,    -1,    -1,     5,     6,
     7,     8,     9,    10,    11,    12,    26,    27,    -1,    -1,
    43,    -1,    32,    33,    34,    35,    36,    37,    38,    39,
    -1,    28,    29,    30,    31,    -1,   201,    47,    -1,    -1,
    -1,    64,    65,    40,    41,    -1,    -1,    28,    29,    30,
    31,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,     5,     6,     7,     8,     9,    10,    11,    12,    66,
    51,    52,    53,    54,    55,    56,    57,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    28,    29,    30,    31,    -1,    -1,
    -1,    28,    29,    30,    31,    -1,    40,    41,    -1,    -1,
    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
    54,    55,    56,    57,     3,    52,    53,    54,    55,    56,
    57,    -1,    66,    -1,    13,    14,    15,    -1,    17,    18,
    19,    20,    21,    22,    23,    24,    25,    26,    27,    -1,
    -1,     3,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
    -1,    13,    14,    15,    43,    17,    18,    19,    20,    -1,
    22,    23,    24,    25,    26,    27,    -1,    53,    54,    55,
    56,    57,    -1,    -1,    -1,    64,    -1,    -1,    -1,    -1,
    -1,    43,     5,     6,     7,     8,     9,    10,    11,    12,
    -1,    -1,    -1,    -1,    28,    29,    30,    31,    -1,    -1,
    -1,    -1,    64,    -1,    -1,    28,    29,    30,    31,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,    53,
    54,    55,    56,    57,    -1,    48,    49,    50,    51,    52,
    53,    54,    55,    56,    57,    -1,    -1,    -1,    -1,    62,
     5,     6,     7,     8,     9,    10,    11,    12,    26,    27,
    -1,    -1,    -1,    -1,    32,    33,    34,    35,    36,    37,
    38,    39,    -1,    28,    29,    30,    31,    -1,    -1,    47,
    -1,    -1,    -1,    -1,    -1,    40,    41,    -1,    -1,    -1,
    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    54,
    55,    56,    57,    -1,    -1,    -1,    -1,    62,     5,     6,
     7,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    40,    41,    -1,    -1,    -1,    -1,    -1,
    -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    -1,    -1,    -1,    -1,    62,     5,     6,     7,     8,
     9,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,
    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    40,    41,    -1,    -1,    -1,    -1,    -1,    -1,    48,
    49,    50,    51,    52,    53,    54,    55,    56,    57,    -1,
    -1,    -1,    -1,    62,     5,     6,     7,     8,     9,    10,
    11,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    30,
    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,
    41,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
    51,    52,    53,    54,    55,    56,    57,     5,     6,     7,
     8,    -1,    10,    11,    12,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    28,    29,    30,    31,    -1,    -1,    -1,    -1,     5,     6,
     7,     8,    40,    41,    11,    12,    -1,    -1,    -1,    -1,
    48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
    -1,    28,    29,    30,    31,    -1,    -1,    -1,    -1,     5,
     6,     7,     8,    40,    41,    11,    12,    -1,    -1,    -1,
    -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
    57,    -1,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    40,    41,    -1,    -1,    -1,    -1,
    -1,    -1,    48,    49,    50,    51,    52,    53,    54,    55,
    56,    57,    26,    27,    -1,    -1,    -1,    -1,    32,    33,
    34,    35,    36,    37,    38,    39,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    -1,    61,    -1,    -1,
    -1,    -1,    -1,    -1,    68
};
#define YYPURE 1

/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */

#line 3 "bison.simple"



/* Skeleton output parser for bison,

   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.



   This program is free software; you can redistribute it and/or modify

   it under the terms of the GNU General Public License as published by

   the Free Software Foundation; either version 2, or (at your option)

   any later version.



   This program is distributed in the hope that it will be useful,

   but WITHOUT ANY WARRANTY; without even the implied warranty of

   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the

   GNU General Public License for more details.



   You should have received a copy of the GNU General Public License

   along with this program; if not, write to the Free Software

   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */



/* As a special exception, when this file is copied by Bison into a

   Bison output file, you may use that output file without restriction.

   This special exception was added by the Free Software Foundation

   in version 1.24 of Bison.  */



#ifndef alloca

#ifdef __GNUC__

#define alloca __builtin_alloca

#else /* not GNU C.  */

#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)

#include <alloca.h>

#else /* not sparc */

#if defined (MSDOS) && !defined (__TURBOC__)

#include <malloc.h>

#else /* not MSDOS, or __TURBOC__ */

#if defined(_AIX)

#include <malloc.h>

 #pragma alloca

#else /* not MSDOS, __TURBOC__, or _AIX */

#ifdef __hpux

#ifdef __cplusplus

extern "C" {

void *alloca (unsigned int);

};

#else /* not __cplusplus */

void *alloca ();

#endif /* not __cplusplus */

#endif /* __hpux */

#endif /* not _AIX */

#endif /* not MSDOS, or __TURBOC__ */

#endif /* not sparc.  */

#endif /* not GNU C.  */

#endif /* alloca not defined.  */



/* This is the parser code that is written into each bison parser

  when the %semantic_parser declaration is not specified in the grammar.

  It was written by Richard Stallman by simplifying the hairy parser

  used when %semantic_parser is specified.  */



/* Note: there must be only one dollar sign in this file.

   It is replaced by the list of actions, each action

   as one case of the switch.  */



#define yyerrok		(yyerrstatus = 0)

#define yyclearin	(yychar = YYEMPTY)

#define YYEMPTY		-2

#define YYEOF		0

#define YYACCEPT	return(0)

#define YYABORT 	return(1)

#define YYERROR		goto yyerrlab1

/* Like YYERROR except do call yyerror.

   This remains here temporarily to ease the

   transition to the new meaning of YYERROR, for GCC.

   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)



#define YYTERROR	1

#define YYERRCODE	256



#ifndef YYPURE

#define YYLEX		yylex()

#endif



#ifdef YYPURE

#ifdef YYLSP_NEEDED

#ifdef YYLEX_PARAM

#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)

#else

#define YYLEX		yylex(&yylval, &yylloc)

#endif

#else /* not YYLSP_NEEDED */

#ifdef YYLEX_PARAM

#define YYLEX		yylex(&yylval, YYLEX_PARAM)

#else

#define YYLEX		yylex(&yylval)

#endif

#endif /* not YYLSP_NEEDED */

#endif



/* If nonreentrant, generate the variables here */



#ifndef YYPURE



int	yychar;			/*  the lookahead symbol		*/

YYSTYPE	yylval;			/*  the semantic value of the		*/

				/*  lookahead symbol			*/



#ifdef YYLSP_NEEDED

YYLTYPE yylloc;			/*  location data for the lookahead	*/

				/*  symbol				*/

#endif



int yynerrs;			/*  number of parse errors so far       */

#endif  /* not YYPURE */



#if YYDEBUG != 0

int yydebug;			/*  nonzero means print parse trace	*/

/* Since this is uninitialized, it does not stop multiple parsers

   from coexisting.  */

#endif



/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/



#ifndef	YYINITDEPTH

#define YYINITDEPTH 200

#endif



/*  YYMAXDEPTH is the maximum size the stacks can grow to

    (effective only if the built-in stack extension method is used).  */



#if YYMAXDEPTH == 0

#undef YYMAXDEPTH

#endif



#ifndef YYMAXDEPTH

#define YYMAXDEPTH 10000

#endif



/* Prevent warning if -Wstrict-prototypes.  */

#ifdef __GNUC__

int yyparse (void);

#endif



#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */

#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)

#else				/* not GNU C or C++ */

#ifndef __cplusplus



/* This is the most reliable way to avoid incompatibilities

   in available built-in functions on various systems.  */

static void

__yy_memcpy (from, to, count)

     char *from;

     char *to;

     int count;

{

  register char *f = from;

  register char *t = to;

  register int i = count;



  while (i-- > 0)

    *t++ = *f++;

}



#else /* __cplusplus */



/* This is the most reliable way to avoid incompatibilities

   in available built-in functions on various systems.  */

static void

__yy_memcpy (char *from, char *to, int count)

{

  register char *f = from;

  register char *t = to;

  register int i = count;



  while (i-- > 0)

    *t++ = *f++;

}



#endif

#endif



#line 192 "bison.simple"



/* The user can define YYPARSE_PARAM as the name of an argument to be passed

   into yyparse.  The argument should have type void *.

   It should actually point to an object.

   Grammar actions can access the variable by casting it

   to the proper pointer type.  */



#ifdef YYPARSE_PARAM

#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;

#else

#define YYPARSE_PARAM

#define YYPARSE_PARAM_DECL

#endif



int

yyparse(YYPARSE_PARAM)

     YYPARSE_PARAM_DECL

{

  register int yystate;

  register int yyn;

  register short *yyssp;

  register YYSTYPE *yyvsp;

  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */

  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */



  short	yyssa[YYINITDEPTH];	/*  the state stack			*/

  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/



  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */

  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */



#ifdef YYLSP_NEEDED

  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/

  YYLTYPE *yyls = yylsa;

  YYLTYPE *yylsp;



#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)

#else

#define YYPOPSTACK   (yyvsp--, yyssp--)

#endif



  int yystacksize = YYINITDEPTH;



#ifdef YYPURE

  int yychar;

  YYSTYPE yylval;

  int yynerrs;

#ifdef YYLSP_NEEDED

  YYLTYPE yylloc;

#endif

#endif



  YYSTYPE yyval;		/*  the variable used to return		*/

				/*  semantic values from the action	*/

				/*  routines				*/



  int yylen;



#if YYDEBUG != 0

  if (yydebug)

    fprintf(stderr, "Starting parse\n");

#endif



  yystate = 0;

  yyerrstatus = 0;

  yynerrs = 0;

  yychar = YYEMPTY;		/* Cause a token to be read.  */



  /* Initialize stack pointers.

     Waste one element of value and location stack

     so that they stay on the same level as the state stack.

     The wasted elements are never initialized.  */



  yyssp = yyss - 1;

  yyvsp = yyvs;

#ifdef YYLSP_NEEDED

  yylsp = yyls;

#endif



/* Push a new state, which is found in  yystate  .  */

/* In all cases, when you get here, the value and location stacks

   have just been pushed. so pushing a state here evens the stacks.  */

yynewstate:



  *++yyssp = yystate;



  if (yyssp >= yyss + yystacksize - 1)

    {

      /* Give user a chance to reallocate the stack */

      /* Use copies of these so that the &'s don't force the real ones into memory. */

      YYSTYPE *yyvs1 = yyvs;

      short *yyss1 = yyss;

#ifdef YYLSP_NEEDED

      YYLTYPE *yyls1 = yyls;

#endif



      /* Get the current used size of the three stacks, in elements.  */

      int size = yyssp - yyss + 1;



#ifdef yyoverflow

      /* Each stack pointer address is followed by the size of

	 the data in use in that stack, in bytes.  */

#ifdef YYLSP_NEEDED

      /* This used to be a conditional around just the two extra args,

	 but that might be undefined if yyoverflow is a macro.  */

      yyoverflow("parser stack overflow",

		 &yyss1, size * sizeof (*yyssp),

		 &yyvs1, size * sizeof (*yyvsp),

		 &yyls1, size * sizeof (*yylsp),

		 &yystacksize);

#else

      yyoverflow("parser stack overflow",

		 &yyss1, size * sizeof (*yyssp),

		 &yyvs1, size * sizeof (*yyvsp),

		 &yystacksize);

#endif



      yyss = yyss1; yyvs = yyvs1;

#ifdef YYLSP_NEEDED

      yyls = yyls1;

#endif

#else /* no yyoverflow */

      /* Extend the stack our own way.  */

      if (yystacksize >= YYMAXDEPTH)

	{

	  yyerror("parser stack overflow");

	  return 2;

	}

      yystacksize *= 2;

      if (yystacksize > YYMAXDEPTH)

	yystacksize = YYMAXDEPTH;

      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));

      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));

      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));

      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));

#ifdef YYLSP_NEEDED

      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));

      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));

#endif

#endif /* no yyoverflow */



      yyssp = yyss + size - 1;

      yyvsp = yyvs + size - 1;

#ifdef YYLSP_NEEDED

      yylsp = yyls + size - 1;

#endif



#if YYDEBUG != 0

      if (yydebug)

	fprintf(stderr, "Stack size increased to %d\n", yystacksize);

#endif



      if (yyssp >= yyss + yystacksize - 1)

	YYABORT;

    }



#if YYDEBUG != 0

  if (yydebug)

    fprintf(stderr, "Entering state %d\n", yystate);

#endif



  goto yybackup;

 yybackup:



/* Do appropriate processing given the current state.  */

/* Read a lookahead token if we need one and don't already have one.  */

/* yyresume: */



  /* First try to decide what to do without reference to lookahead token.  */



  yyn = yypact[yystate];

  if (yyn == YYFLAG)

    goto yydefault;



  /* Not known => get a lookahead token if don't already have one.  */



  /* yychar is either YYEMPTY or YYEOF

     or a valid token in external form.  */



  if (yychar == YYEMPTY)

    {

#if YYDEBUG != 0

      if (yydebug)

	fprintf(stderr, "Reading a token: ");

#endif

      yychar = YYLEX;

    }



  /* Convert token to internal form (in yychar1) for indexing tables with */



  if (yychar <= 0)		/* This means end of input. */

    {

      yychar1 = 0;

      yychar = YYEOF;		/* Don't call YYLEX any more */



#if YYDEBUG != 0

      if (yydebug)

	fprintf(stderr, "Now at end of input.\n");

#endif

    }

  else

    {

      yychar1 = YYTRANSLATE(yychar);



#if YYDEBUG != 0

      if (yydebug)

	{

	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);

	  /* Give the individual parser a way to print the precise meaning

	     of a token, for further debugging info.  */

#ifdef YYPRINT

	  YYPRINT (stderr, yychar, yylval);

#endif

	  fprintf (stderr, ")\n");

	}

#endif

    }



  yyn += yychar1;

  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)

    goto yydefault;



  yyn = yytable[yyn];



  /* yyn is what to do for this token type in this state.

     Negative => reduce, -yyn is rule number.

     Positive => shift, yyn is new state.

       New state is final state => don't bother to shift,

       just return success.

     0, or most negative number => error.  */



  if (yyn < 0)

    {

      if (yyn == YYFLAG)

	goto yyerrlab;

      yyn = -yyn;

      goto yyreduce;

    }

  else if (yyn == 0)

    goto yyerrlab;



  if (yyn == YYFINAL)

    YYACCEPT;



  /* Shift the lookahead token.  */



#if YYDEBUG != 0

  if (yydebug)

    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);

#endif



  /* Discard the token being shifted unless it is eof.  */

  if (yychar != YYEOF)

    yychar = YYEMPTY;



  *++yyvsp = yylval;

#ifdef YYLSP_NEEDED

  *++yylsp = yylloc;

#endif



  /* count tokens shifted since error; after three, turn off error status.  */

  if (yyerrstatus) yyerrstatus--;



  yystate = yyn;

  goto yynewstate;



/* Do the default action for the current state.  */

yydefault:



  yyn = yydefact[yystate];

  if (yyn == 0)

    goto yyerrlab;



/* Do a reduction.  yyn is the number of a rule to reduce with.  */

yyreduce:

  yylen = yyr2[yyn];

  if (yylen > 0)

    yyval = yyvsp[1-yylen]; /* implement default value of the action */



#if YYDEBUG != 0

  if (yydebug)

    {

      int i;



      fprintf (stderr, "Reducing via rule %d (line %d), ",

	       yyn, yyrline[yyn]);



      /* Print the symbols being reduced, and their result.  */

      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)

	fprintf (stderr, "%s ", yytname[yyrhs[i]]);

      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);

    }

#endif




  switch (yyn) {

case 1:
#line 131 "..\src\nasl_grammar.y"
{
	  ((naslctxt*)parm)->tree = yyvsp[0].node;
	;
    break;}
case 2:
#line 136 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type = NODE_INSTR_L;
	  yyval.node->link[0] = yyvsp[0].node;
	;
    break;}
case 3:
#line 142 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type = NODE_INSTR_L;
	  yyval.node->link[0] = yyvsp[-1].node;
	  yyval.node->link[1] = yyvsp[0].node;
	;
    break;}
case 6:
#line 152 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, yyvsp[-4].str);
	  yyval.node->type = NODE_FUN_DEF;
	  yyval.node->link[0] = yyvsp[-2].node;
	  yyval.node->link[1] = yyvsp[0].node;
	;
    break;}
case 7:
#line 159 "..\src\nasl_grammar.y"
{ yyval.node = NULL; ;
    break;}
case 8:
#line 159 "..\src\nasl_grammar.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 9:
#line 160 "..\src\nasl_grammar.y"
{ yyval.node = alloc_tree_cell(LNB, yyvsp[0].str); yyval.node->type = NODE_DECL; ;
    break;}
case 10:
#line 162 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, yyvsp[-2].str);
	  yyval.node->type = NODE_DECL;
	  yyval.node->link[0] = yyvsp[0].node; 
	;
    break;}
case 11:
#line 169 "..\src\nasl_grammar.y"
{ yyval.node = yyvsp[-1].node; ;
    break;}
case 12:
#line 169 "..\src\nasl_grammar.y"
{ yyval.node = NULL; ;
    break;}
case 14:
#line 172 "..\src\nasl_grammar.y"
{
	  if (yyvsp[-1].node == NULL)
	    yyval.node = yyvsp[0].node;
	  else
	    {
	      yyval.node = alloc_tree_cell(LNB, NULL);
	      yyval.node->type = NODE_INSTR_L;
	      yyval.node->link[0] = yyvsp[-1].node;
	      yyval.node->link[1] = yyvsp[0].node; 
	    }
	;
    break;}
case 15:
#line 185 "..\src\nasl_grammar.y"
{ yyval.node = yyvsp[-1].node; ;
    break;}
case 27:
#line 190 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type =  NODE_BREAK;
	;
    break;}
case 28:
#line 194 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type =  NODE_CONTINUE;
	;
    break;}
case 29:
#line 198 "..\src\nasl_grammar.y"
{ yyval.node = NULL; ;
    break;}
case 30:
#line 202 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type =  NODE_RETURN;
	  yyval.node->link[0] = yyvsp[0].node;
	;
    break;}
case 31:
#line 208 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type =  NODE_RETURN;
	;
    break;}
case 32:
#line 215 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type = NODE_IF_ELSE;
	  yyval.node->link[0] = yyvsp[-2].node; yyval.node->link[1] = yyvsp[0].node;
	;
    break;}
case 33:
#line 221 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type = NODE_IF_ELSE;
	  yyval.node->link[0] = yyvsp[-4].node; yyval.node->link[1] = yyvsp[-2].node; yyval.node->link[2] = yyvsp[0].node;
	;
    break;}
case 38:
#line 230 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type = NODE_FOR;
	  yyval.node->link[0] = yyvsp[-6].node;
	  yyval.node->link[1] = yyvsp[-4].node;
	  yyval.node->link[2] = yyvsp[-2].node;
	  yyval.node->link[3] = yyvsp[0].node;
	;
    break;}
case 39:
#line 240 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type = NODE_WHILE;
	  yyval.node->link[0] = yyvsp[-2].node;
	  yyval.node->link[1] = yyvsp[0].node;
	;
    break;}
case 40:
#line 247 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type = NODE_REPEAT_UNTIL;
	  yyval.node->link[0] = yyvsp[-3].node;
	  yyval.node->link[1] = yyvsp[-1].node;
	;
    break;}
case 41:
#line 255 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, yyvsp[-4].str);
	  yyval.node->type = NODE_FOREACH;
	  yyval.node->link[0] = yyvsp[-2].node;
	  yyval.node->link[1] = yyvsp[0].node;
	;
    break;}
case 45:
#line 263 "..\src\nasl_grammar.y"
{ yyval.node = NULL; ;
    break;}
case 46:
#line 267 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type = NODE_REPEATED;
	  yyval.node->link[0] = yyvsp[-2].node;
	  yyval.node->link[1] = yyvsp[0].node;
	;
    break;}
case 47:
#line 274 "..\src\nasl_grammar.y"
{ yyval.str = yyvsp[0].data.val; ;
    break;}
case 49:
#line 278 "..\src\nasl_grammar.y"
{ 
	  naslctxt	subctx;
	  int		x;

 	  subctx.always_authenticated = ((naslctxt*)parm)->always_authenticated;
	  x = init_nasl_ctx(&subctx, yyvsp[-1].str);
	  yyval.node = NULL;
	  if (x >= 0)
	    {
	      if (! naslparse(&subctx))
		{
		  yyval.node = subctx.tree;
		}
	      else
		nasl_perror(NULL, "%s: Parse error at or near line %d\n",
			yyvsp[-1].str, subctx.line_nb);
	      efree(&subctx.buffer);
	      fclose(subctx.fp); 
	      subctx.fp = NULL;
	      /* If we are an authenticated script and the script we include is *NOT* authenticated,
   		 then we lose our authentication status */
	      if ( ((naslctxt*)parm)->always_authenticated == 0 &&
	          ((naslctxt*)parm)->authenticated != 0 && subctx.authenticated == 0 )
			{
			((naslctxt*)parm)->authenticated = 0;
			nasl_perror(NULL, "Including %s which is not authenticated - losing our authenticated status\n", yyvsp[-1].str);
			}
	    }
	  efree(& yyvsp[-1].str);
	;
    break;}
case 50:
#line 311 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, yyvsp[-3].str);
	  yyval.node->type = NODE_FUN_CALL;
	  yyval.node->link[0] = yyvsp[-1].node;
	;
    break;}
case 52:
#line 317 "..\src\nasl_grammar.y"
{ yyval.node = NULL; ;
    break;}
case 54:
#line 319 "..\src\nasl_grammar.y"
{
	  yyvsp[-2].node->link[1] = yyvsp[0].node;
	  yyval.node = yyvsp[-2].node;
	;
    break;}
case 55:
#line 325 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type = NODE_ARG;
	  yyval.node->link[0] = yyvsp[0].node;
	;
    break;}
case 56:
#line 331 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, yyvsp[-2].str);
	  yyval.node->type = NODE_ARG;
	  yyval.node->link[0] = yyvsp[0].node;
	;
    break;}
case 57:
#line 339 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_expr_cell(LNB, NODE_AFF, yyvsp[-2].node, yyvsp[0].node);
	;
    break;}
case 58:
#line 342 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, NODE_PLUS_EQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 59:
#line 343 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, NODE_MINUS_EQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 60:
#line 344 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, NODE_MULT_EQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 61:
#line 345 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, NODE_DIV_EQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 62:
#line 346 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, NODE_MODULO_EQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 63:
#line 347 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, NODE_R_SHIFT_EQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 64:
#line 348 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, NODE_R_USHIFT_EQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 65:
#line 349 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, NODE_L_SHIFT_EQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 66:
#line 352 "..\src\nasl_grammar.y"
{ yyval.node = alloc_tree_cell(LNB, yyvsp[0].str); yyval.node->type = NODE_VAR; ;
    break;}
case 69:
#line 354 "..\src\nasl_grammar.y"
{ yyval.str = strdup("x"); ;
    break;}
case 70:
#line 357 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_tree_cell(LNB, yyvsp[-3].str);
	  yyval.node->type = NODE_ARRAY_EL;
	  yyval.node->link[0] = yyvsp[-1].node;
	;
    break;}
case 72:
#line 366 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_INCR, NULL, yyvsp[0].node); ;
    break;}
case 73:
#line 367 "..\src\nasl_grammar.y"
{yyval.node = alloc_expr_cell(LNB, EXPR_DECR, NULL, yyvsp[0].node); ;
    break;}
case 74:
#line 368 "..\src\nasl_grammar.y"
{ yyval.node= alloc_expr_cell(LNB, EXPR_INCR, yyvsp[-1].node, NULL); ;
    break;}
case 75:
#line 369 "..\src\nasl_grammar.y"
{ yyval.node= alloc_expr_cell(LNB, EXPR_DECR, yyvsp[-1].node, NULL); ;
    break;}
case 76:
#line 373 "..\src\nasl_grammar.y"
{ yyval.node = yyvsp[-1].node; ;
    break;}
case 77:
#line 374 "..\src\nasl_grammar.y"
{  yyval.node = alloc_expr_cell(LNB, EXPR_AND, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 78:
#line 375 "..\src\nasl_grammar.y"
{  yyval.node = alloc_expr_cell(LNB, EXPR_NOT, yyvsp[0].node, NULL); ;
    break;}
case 79:
#line 376 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_OR, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 80:
#line 377 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_PLUS, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 81:
#line 378 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_MINUS, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 82:
#line 379 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_U_MINUS, yyvsp[0].node, NULL);;
    break;}
case 83:
#line 380 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_BIT_NOT, yyvsp[0].node, NULL);;
    break;}
case 84:
#line 381 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_MULT, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 85:
#line 382 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_EXPO, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 86:
#line 383 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_DIV, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 87:
#line 384 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_MODULO, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 88:
#line 385 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_BIT_AND, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 89:
#line 386 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_BIT_XOR, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 90:
#line 387 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_BIT_OR, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 91:
#line 388 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_R_SHIFT, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 92:
#line 389 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_R_USHIFT, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 93:
#line 390 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, EXPR_L_SHIFT, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 95:
#line 392 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, COMP_MATCH, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 96:
#line 393 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, COMP_NOMATCH, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 97:
#line 394 "..\src\nasl_grammar.y"
{ yyval.node = alloc_RE_cell(LNB, COMP_RE_MATCH, yyvsp[-2].node, yyvsp[0].str); ;
    break;}
case 98:
#line 395 "..\src\nasl_grammar.y"
{ yyval.node = alloc_RE_cell(LNB, COMP_RE_NOMATCH, yyvsp[-2].node, yyvsp[0].str); ;
    break;}
case 99:
#line 396 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, COMP_LT, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 100:
#line 397 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, COMP_GT, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 101:
#line 398 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, COMP_EQ, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 102:
#line 399 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, COMP_NE, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 103:
#line 400 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, COMP_GE, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 104:
#line 401 "..\src\nasl_grammar.y"
{ yyval.node = alloc_expr_cell(LNB, COMP_LE, yyvsp[-2].node, yyvsp[0].node); ;
    break;}
case 110:
#line 405 "..\src\nasl_grammar.y"
{ yyval.node = make_array_from_elems(yyvsp[-1].node); ;
    break;}
case 111:
#line 407 "..\src\nasl_grammar.y"
{ yyval.node = yyvsp[0].node; ;
    break;}
case 112:
#line 408 "..\src\nasl_grammar.y"
{
		yyvsp[-2].node->link[1] = yyvsp[0].node; yyval.node = yyvsp[-2].node;
	;
    break;}
case 113:
#line 412 "..\src\nasl_grammar.y"
{ 
	  yyval.node = alloc_typed_cell(ARRAY_ELEM); 
	  yyval.node->link[0] = yyvsp[0].node;
	;
    break;}
case 114:
#line 415 "..\src\nasl_grammar.y"
{
	  yyval.node = alloc_typed_cell(ARRAY_ELEM);
	  yyval.node->link[0] = yyvsp[0].node;
	  yyval.node->x.str_val = yyvsp[-2].str;
	;
    break;}
case 115:
#line 421 "..\src\nasl_grammar.y"
{  yyval.node = alloc_typed_cell(CONST_INT); yyval.node->x.i_val = yyvsp[0].num; ;
    break;}
case 116:
#line 422 "..\src\nasl_grammar.y"
{ 
	  yyval.node = alloc_typed_cell(CONST_STR); yyval.node->x.str_val = yyvsp[0].str;
	  yyval.node->size = strlen(yyvsp[0].str);
	;
    break;}
case 117:
#line 426 "..\src\nasl_grammar.y"
{ 
	  yyval.node = alloc_typed_cell(CONST_DATA); yyval.node->x.str_val = yyvsp[0].data.val;
	  yyval.node->size = yyvsp[0].data.len;
	;
    break;}
case 119:
#line 433 "..\src\nasl_grammar.y"
{ yyval.node = alloc_tree_cell(LNB, yyvsp[0].str); yyval.node->type = NODE_VAR; ;
    break;}
case 123:
#line 439 "..\src\nasl_grammar.y"
{
	  char	*s = emalloc(44);
	  snprintf(s, 44, "%d.%d.%d.%d", yyvsp[-6].num, yyvsp[-4].num, yyvsp[-2].num, yyvsp[0].num);
	  yyval.node = alloc_tree_cell(LNB, s);
	  yyval.node->type = CONST_STR;
	  yyval.node->size = strlen(s);
	;
    break;}
case 124:
#line 449 "..\src\nasl_grammar.y"
{ 
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type = NODE_LOCAL;
	  yyval.node->link[0] = yyvsp[0].node;
	;
    break;}
case 125:
#line 457 "..\src\nasl_grammar.y"
{ 
	  yyval.node = alloc_tree_cell(LNB, NULL);
	  yyval.node->type = NODE_GLOBAL;
	  yyval.node->link[0] = yyvsp[0].node;
	;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */

#line 487 "bison.simple"



  yyvsp -= yylen;

  yyssp -= yylen;

#ifdef YYLSP_NEEDED

  yylsp -= yylen;

#endif



#if YYDEBUG != 0

  if (yydebug)

    {

      short *ssp1 = yyss - 1;

      fprintf (stderr, "state stack now");

      while (ssp1 != yyssp)

	fprintf (stderr, " %d", *++ssp1);

      fprintf (stderr, "\n");

    }

#endif



  *++yyvsp = yyval;



#ifdef YYLSP_NEEDED

  yylsp++;

  if (yylen == 0)

    {

      yylsp->first_line = yylloc.first_line;

      yylsp->first_column = yylloc.first_column;

      yylsp->last_line = (yylsp-1)->last_line;

      yylsp->last_column = (yylsp-1)->last_column;

      yylsp->text = 0;

    }

  else

    {

      yylsp->last_line = (yylsp+yylen-1)->last_line;

      yylsp->last_column = (yylsp+yylen-1)->last_column;

    }

#endif



  /* Now "shift" the result of the reduction.

     Determine what state that goes to,

     based on the state we popped back to

     and the rule number reduced by.  */



  yyn = yyr1[yyn];



  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;

  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)

    yystate = yytable[yystate];

  else

    yystate = yydefgoto[yyn - YYNTBASE];



  goto yynewstate;



yyerrlab:   /* here on detecting error */



  if (! yyerrstatus)

    /* If not already recovering from an error, report this error.  */

    {

      ++yynerrs;



#ifdef YYERROR_VERBOSE

      yyn = yypact[yystate];



      if (yyn > YYFLAG && yyn < YYLAST)

	{

	  int size = 0;

	  char *msg;

	  int x, count;



	  count = 0;

	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */

	  for (x = (yyn < 0 ? -yyn : 0);

	       x < (sizeof(yytname) / sizeof(char *)); x++)

	    if (yycheck[x + yyn] == x)

	      size += strlen(yytname[x]) + 15, count++;

	  msg = (char *) malloc(size + 15);

	  if (msg != 0)

	    {

	      strcpy(msg, "parse error");



	      if (count < 5)

		{

		  count = 0;

		  for (x = (yyn < 0 ? -yyn : 0);

		       x < (sizeof(yytname) / sizeof(char *)); x++)

		    if (yycheck[x + yyn] == x)

		      {

			strcat(msg, count == 0 ? ", expecting `" : " or `");

			strcat(msg, yytname[x]);

			strcat(msg, "'");

			count++;

		      }

		}

	      yyerror(msg);

	      free(msg);

	    }

	  else

	    yyerror ("parse error; also virtual memory exceeded");

	}

      else

#endif /* YYERROR_VERBOSE */

	yyerror("parse error");

    }



  goto yyerrlab1;

yyerrlab1:   /* here on error raised explicitly by an action */



  if (yyerrstatus == 3)

    {

      /* if just tried and failed to reuse lookahead token after an error, discard it.  */



      /* return failure if at end of input */

      if (yychar == YYEOF)

	YYABORT;



#if YYDEBUG != 0

      if (yydebug)

	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);

#endif



      yychar = YYEMPTY;

    }



  /* Else will try to reuse lookahead token

     after shifting the error token.  */



  yyerrstatus = 3;		/* Each real token shifted decrements this */



  goto yyerrhandle;



yyerrdefault:  /* current state does not do anything special for the error token. */



#if 0

  /* This is wrong; only states that explicitly want error tokens

     should shift them.  */

  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/

  if (yyn) goto yydefault;

#endif



yyerrpop:   /* pop the current state because it cannot handle the error token */



  if (yyssp == yyss) YYABORT;

  yyvsp--;

  yystate = *--yyssp;

#ifdef YYLSP_NEEDED

  yylsp--;

#endif



#if YYDEBUG != 0

  if (yydebug)

    {

      short *ssp1 = yyss - 1;

      fprintf (stderr, "Error: state stack now");

      while (ssp1 != yyssp)

	fprintf (stderr, " %d", *++ssp1);

      fprintf (stderr, "\n");

    }

#endif



yyerrhandle:



  yyn = yypact[yystate];

  if (yyn == YYFLAG)

    goto yyerrdefault;



  yyn += YYTERROR;

  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)

    goto yyerrdefault;



  yyn = yytable[yyn];

  if (yyn < 0)

    {

      if (yyn == YYFLAG)

	goto yyerrpop;

      yyn = -yyn;

      goto yyreduce;

    }

  else if (yyn == 0)

    goto yyerrpop;



  if (yyn == YYFINAL)

    YYACCEPT;



#if YYDEBUG != 0

  if (yydebug)

    fprintf(stderr, "Shifting error token, ");

#endif



  *++yyvsp = yylval;

#ifdef YYLSP_NEEDED

  *++yylsp = yylloc;

#endif



  yystate = yyn;

  goto yynewstate;

}

#line 463 "..\src\nasl_grammar.y"


#include <stdio.h>
#include <stdlib.h>

static void 
naslerror(const char *s)
{
  fputs(s, stderr);
}

static char scripts_dir[MAX_PATH] = {0}, include_dir[MAX_PATH] = {0};

void set_work_dir(char *scripts, char *include)
{
    char exe_path[MAX_PATH] = {0}, *ptr = NULL;

    GetModuleFileName(0, exe_path, sizeof(exe_path)-1);
    ptr = strrchr(exe_path, '\\');
    if (ptr) ptr[0] = '\0';

    if (scripts) {
        strcpy(scripts_dir, scripts);
        if (scripts_dir[strlen(scripts_dir)-1] == '\\') scripts_dir[strlen(scripts_dir)-1] = '\0';
    }
    else {
        strcpy(scripts_dir, "\0");
    }

    if (include) {
        strcpy(include_dir, include);
        if (include_dir[strlen(include_dir)-1] == '\\') include_dir[strlen(include_dir)-1] = '\0';
    }
    else {
        strcpy(include, "\0");
    }
}

void get_work_dir(char *scripts, char *include)
{
    if (scripts) strcpy(scripts, scripts_dir);
    if (include) strcpy(include, include_dir);
}

int
init_nasl_ctx(naslctxt* pc, const char* name)
{
  char line[1024];
  char full_name[MAX_PATH];
#ifdef MULTIPLE_INCLUDE_DIRS
  static const char* inc_dirs[] = { ".", "/tmp" }; /* TBD */
#endif
  pc->line_nb = 1;
  pc->tree = NULL;
  pc->buffer = emalloc(80);
  pc->maxlen = 80;
  pc->authenticated = 0;

#ifdef MULTIPLE_INCLUDE_DIRS
  if (name[0] == '/')		/* absolute path */
#endif
    {
      char *ptr = strchr(name, ':');
      if (ptr) strcpy(full_name, name); /* include path */
      else if (!strchr(name, '\\')) sprintf(full_name, "%s\\%s", scripts_dir, name);
      else {
        GetModuleFileName(0, line, sizeof(line)-1);
        ptr = strrchr(line, '\\'); if (ptr) ptr[0] = '\0';
        sprintf(full_name, "%s\\%s", line, name);
      }
      pc->fp = fopen(full_name, "r");
      if (!pc->fp && !ptr)
      {
        sprintf(full_name, "%s\\%s", include_dir, name);
        pc->fp = fopen(full_name, "r");
      }
      /* Shouldn't we reject the file? */
      if (!pc->fp)
      {
        if ((pc->fp = fopen(name, "r")) == NULL)
    	{
    	  perror(name);
    	  return -1;
    	}
      }
      strncpy(full_name, name, sizeof(full_name) - 1);
      goto authenticate;
    }
#ifdef MULTIPLE_INCLUDE_DIRS
  else
    {
      int	i;

      for (i = 0; i < sizeof(inc_dirs) / sizeof(*inc_dirs); i ++)
	{
	  snprintf(full_name, sizeof(full_name),  "%s/%s", inc_dirs[i], name);
	  if ((pc->fp = fopen(full_name, "r")) != NULL)
	    goto authenticate;
	  perror(full_name);
	}
      return -1;
    }
#endif

authenticate:
 if ( pc->always_authenticated )
	pc->authenticated = 1;
 else 
 {
 fgets(line, sizeof(line) - 1, pc->fp);
 line[sizeof(line) - 1] = '\0';
 if ( strncmp(line, "#TRUSTED", strlen("#TRUSTED") ) == 0 )
 {
  int sig;
  full_name[sizeof(full_name) - 1] = '\0';
  sig = verify_script_signature(full_name);
  if ( sig == 0 ) 
	pc->authenticated = 1;
  else
	pc->authenticated = 0;
 
  if ( sig > 0  ) 
	{
	  fprintf(stderr, "%s: bad signature. Will not execute this script\n", full_name);
	  fclose(pc->fp);
	  pc->fp = NULL;
	  return -1;
	}
   else if ( sig < 0 )
	  fprintf(stderr, "%s: Could not verify the signature - this script will be run in non-authenticated mode\n", full_name);
 }
 rewind(pc->fp);
 }
 return 0;
}

void
nasl_clean_ctx(naslctxt* c)
{
#if 0
  nasl_dump_tree(c->tree);
#endif
  deref_cell(c->tree);
  efree(&c->buffer);
  if (c->fp)
    {
      fclose(c->fp);
      c->fp = NULL;
    }
}


#if 0
int
main(int argc,char **argv)
{
  naslctxt	ctx;

  ctx.line_nb = 1;
  ctx.tree = NULL;
  if (argc != 2)
    {
      nasl_perror(NULL, "Usage : nasl <input_file>\n");
      return 1;
    }
  if ((ctx.fp = fopen(argv[1], "r")) == NULL)
    {
      perror(argv[1]);
      return 1;
    }

  return naslparse((void*) &ctx);
}
#endif

enum lex_state {
  ST_START = 0,
  ST_SPACE,
  ST_IDENT,
  ST_ZERO,
  ST_ZEROX,
  ST_OCT,
  ST_DEC,
  ST_HEX,
  ST_COMMENT,
  ST_SUP,
  ST_INF,
  ST_SUP_EXCL,  
  ST_STRING1,
  ST_STRING1_ESC,
  ST_STRING2,
  ST_PLUS,
  ST_MINUS,
  ST_MULT,
  ST_DIV,
  ST_MODULO,
  ST_R_SHIFT,
  ST_R_USHIFT,
  ST_L_SHIFT,
  ST_NOT,
  ST_EQ,
  ST_AND,
  ST_OR };

static int
mylex(lvalp, parm)
     YYSTYPE *lvalp;
     void	*parm;
{
  char		*p;
  naslctxt	*ctx = parm;
  FILE		*fp = ctx->fp;
  int		c, st = ST_START, len, r;
  int		x, i;

  p = ctx->buffer;
  len = 0;

  while ((c = getc(fp)) != EOF)
    {
      if (c ==  '\n')
	ctx->line_nb ++;

      switch(st)
	{
	case ST_START:
	  if (c == '#')
	    st = ST_COMMENT;
	  else if (isalpha(c) || c == '_')
	    {
	      st = ST_IDENT;
	      *p++ = c;
	      len ++;
	    }
	  else if (isspace(c))
	    st = ST_SPACE;
	  else if (c == '0')
	    st = ST_ZERO;
	  else if (isdigit(c))
	    {
	      st = ST_DEC;
	      *p++ = c;
	      len ++;
	    }
	  else if (c == '\'')
	    st = ST_STRING1;
	  else if (c == '"')
	    st = ST_STRING2;
	  else if (c == '+')
	    st = ST_PLUS;
	  else if (c == '-')
	    st = ST_MINUS;
	  else if (c == '*')
	    st = ST_MULT;
	  else if (c == '/')
	    st = ST_DIV;
	  else if (c == '%')
	    st = ST_MODULO;
	  else if (c == '>')
	    st =  ST_SUP;
	  else if (c == '<')
	    st = ST_INF;
	  else if (c == '=')
	    st = ST_EQ;
	  else if (c == '|')
	    st = ST_OR;
	  else if (c == '!')
	    st = ST_NOT;
	  else if (c == '&')
	    st = ST_AND;
	  else
	    {
	      return c;
	    }
	  break;

	case ST_STRING2:
	  if (c == '"')
	    goto exit_loop;
	  *p++ = c;
	  len ++;
	  break;

	case ST_STRING1:
	  if (c == '\'')
	    goto exit_loop;
	  else if (c == '\\')
	    {
	      c = getc(fp);
	      switch (c)
		{
		case EOF:
		  nasl_perror(NULL, "Unfinished string\n");
		  goto exit_loop; /* parse error? */

		case '\n':	/* escaped end of line */
		  ctx->line_nb ++;
		  break;
		case '\\':
		  *p++ ='\\'; len ++;
		  break;
		case 'n':
		  *p++ = '\n'; len++;
		  break;
		case 'f':
		  *p++ = '\f'; len ++;
		  break;
		case 't':
		  *p++ = '\t'; len ++;
		  break;
		case 'r':
		  *p++ = '\r'; len++;
		  break;
		case 'v':
		  *p++ = '\v'; len ++;
		  break;
		case '"':
		  *p ++ = '"'; len ++;
		  break;
	  /* Not yet, as we do not return the length of the string */
		case '0':
		  *p++ = '\0'; len++;
		  break;
		case '\'':
		  *p++ = '\''; len++;
		  break;

		case 'x':
		  x = 0;
		  for (i = 0; i < 2; i ++)
		    {
		      c = getc(fp);
		      if (c == EOF)
			{
			  nasl_perror(NULL, "Unfinished \\x escape sequence (EOF)\n");
			  goto exit_loop;
			}
		      if (c == '\n')
			ctx->line_nb ++;

		      c = tolower(c);
		      if (c >= '0' && c <= '9') 
			x = x * 16 + (c - '0');
		      else if (c >= 'a' && c <= 'f')
			x = x * 16 + 10 + (c - 'a');
		      else
			{
			  nasl_perror(NULL, "Unfinished \\x escape sequence\n");
			  ungetc(c, fp);
			  if (c == '\n')
			    ctx->line_nb --;
			  break;
			}
		    }
		  *p++ = x; len ++;		    
		  break;

		default:
		  nasl_perror(NULL, "Unknown escape sequence \\%c\n", c);
		  *p++ = c; len ++;
		  break;
		}
	    }
	  else
	    {
	      *p++ = c;
	      len ++;
	    }
	  break;

	case ST_IDENT:
	  if (isalnum(c) || c == '_')
	    {
	      st = ST_IDENT;
	      *p++ = c;
	      len ++;
	    }
	  else
	    {
	      ungetc(c, fp);
	      if (c == '\n') 
		ctx->line_nb --;
	      goto exit_loop;
	    }
	  break;

	case ST_ZERO:
	  if (c == 'x' || c == 'X')
	    st = ST_ZEROX;
	  else if (isdigit(c))
	    {
	      if (c <= '7')
		st = ST_OCT;
	      else
		st = ST_DEC;
	      *p ++ = c;
	      len ++;
	    }
	  else
	    {
	      ungetc(c, fp);
	      if (c == '\n')
		ctx->line_nb --;
	      goto exit_loop;
	    }
	  break;

	case ST_ZEROX:
	  if (isxdigit(c))
	    {
	      st = ST_HEX;
	      *p++ = c;
	      len ++;
	    }
	  else
	    {
	      /* This should be a parse error */
	      ungetc(c, fp);
	      if (c ==  '\n')
		ctx->line_nb --;
	      goto exit_loop;
	    }
	  break;

	case ST_OCT:
	  if (c >= '0')
	    {
	    if (c <= '7')
	      {
		*p++ = c;
		len ++;
		break;
	      }
	    else if (c <= '9')
	      {
		*p++ = c;
		len ++;
		st = ST_DEC;
		break;
	      }
	    }
	  ungetc(c, fp);
	  if (c ==  '\n')
	    ctx->line_nb --;
	  goto exit_loop;

	case ST_DEC:
	  if (isdigit(c))
	    {
	      *p++ = c;
	      len ++;
	    }
	  else
	    {
	      ungetc(c, fp);
	      if (c ==  '\n')
		ctx->line_nb --;
	      goto exit_loop;
	    }
	  break;

	case ST_HEX:
	  if (isxdigit(c))
	    {
	      *p++ = c;
	      len ++;
	    }
	  else
	    {
	      ungetc(c, fp);
	      if (c ==  '\n')
		ctx->line_nb --;
	      goto exit_loop;
	    }
	  break;
	    
	case ST_SPACE:
	  if (! isspace(c))
	    {
	      ungetc(c, fp);
	      if (c ==  '\n')
		ctx->line_nb --;
	      st = ST_START;
	    }
	  break;

	case ST_COMMENT:
	  if (c == '\n')
	    st = ST_START;
	  break;

	case ST_SUP_EXCL:
	  if (c == '<')
	    return NOMATCH;
	  else
	    {
	      ungetc(c, fp);
	      if (c ==  '\n')
		ctx->line_nb --;
	      if (! isprint(c)) c = '.';
	      fprintf(stderr, "lexer error: invalid token >!%c parsed as >!< %c\n", c, c);
	      return NOMATCH;
	    }
	  break;

	case ST_SUP:
	  if (c == '=')
	    return SUPEQ;
	  else if (c == '<')
	    return MATCH;
	  else if (c == '>')
	    st = ST_R_SHIFT;
	  else if (c == '!')
	    st = ST_SUP_EXCL;
	  else
	    {
	      ungetc(c, fp);
	      if (c ==  '\n')
		ctx->line_nb --;
	      return '>';
	    }
	  break;

	case ST_INF:
	  if (c == '=')
	    return INFEQ;
	  else if (c == '<')
	    st = ST_L_SHIFT;
	  else
	    {
	      ungetc(c, fp);
	      if (c ==  '\n')
		ctx->line_nb --;
	      return '<';
	    }
	  break;

	case ST_R_SHIFT:
	  if (c == '=')
	    return R_SHIFT_EQ;
	  else if (c == '>')
	    st = ST_R_USHIFT;
	  else
	    {
	      ungetc(c, fp);
	      if (c ==  '\n')
		ctx->line_nb --;
	      return R_SHIFT;
	    }
	  /*NOTREACHED*/
	  break;

	case ST_R_USHIFT:
	  if (c == '=')
	    return R_USHIFT_EQ;
	  else
	    {
	      ungetc(c, fp);
	      if (c ==  '\n')
		ctx->line_nb --;
	      return R_USHIFT;
	    }
	  /*NOTREACHED*/
	  break;

	case ST_L_SHIFT:
	  if (c == '=')
	    return L_SHIFT_EQ;
	  else
	    {
	      ungetc(c, fp);
	      if (c ==  '\n')
		ctx->line_nb --;
	      return L_SHIFT;
	    }
	  /*NOTREACHED*/
	  break;

	case ST_AND:
	  if (c == '&')
	    return AND;
	  ungetc(c, fp);
	  if (c ==  '\n')
	    ctx->line_nb --;
	  return '&';

	case ST_OR:
	  if (c == '|')
	    return OR;
	  ungetc(c, fp);
	  if (c ==  '\n')
	    ctx->line_nb --;
	  return '|';

	case ST_NOT:
	  if (c == '=')
	    return NEQ;
	  else if (c == '~')
	    return RE_NOMATCH;
	  ungetc(c, fp);
	  if (c ==  '\n')
	    ctx->line_nb --;
	  return '!';

	case ST_EQ:
	  if (c == '=')
	    return EQ;
	  else if (c == '~')
	    return RE_MATCH;
	  else if (c == '>')
	    return ARROW;
	  ungetc(c, fp);
	  if (c ==  '\n')
	    ctx->line_nb --;
	  return '='; 

	case ST_PLUS:
	  if (c == '+')
	    return PLUS_PLUS;
	  else if (c == '=')
	    return PLUS_EQ;

	  ungetc(c, fp);
	  if (c ==  '\n')
	    ctx->line_nb --;
	  return '+'; 
	  
	case ST_MINUS:
	  if (c == '-')
	    return MINUS_MINUS;
	  else if (c == '=')
	    return MINUS_EQ;

	  ungetc(c, fp);
	  if (c ==  '\n')
	    ctx->line_nb --;
	  return '-'; 
	  
	case ST_MULT:
	  if (c == '=')
	    return MULT_EQ;
	  else if (c == '*')
	    return EXPO;
	  ungetc(c, fp);
	  if (c ==  '\n')
	    ctx->line_nb --;
	  return '*'; 
	  
	case ST_DIV:
	  if (c == '=')
	    return DIV_EQ;

	  ungetc(c, fp);
	  if (c ==  '\n')
	    ctx->line_nb --;
	  return '/'; 
	  
	case ST_MODULO:
	  if (c == '=')
	    return MODULO_EQ;

	  ungetc(c, fp);
	  if (c ==  '\n')
	    ctx->line_nb --;
	  return '%'; 
	  
	}

      if (len >= ctx->maxlen) 
	{
	  int	offs = p - ctx->buffer;
	  char	*buf2;
	  ctx->maxlen += 80;
	  buf2 = erealloc(ctx->buffer, ctx->maxlen + 1);
	  if (buf2 == NULL)
	    {
	      perror("realloc");
	      abort();
	    }
	  p = buf2 + offs;
	  ctx->buffer = buf2;
	}
    }

 exit_loop:
  ctx->buffer[len] = '\0';
  switch (st)
    {
    case ST_START:
    case ST_COMMENT:
    case ST_SPACE:
      return 0;

    case ST_STRING2:
      r = STRING2;
      lvalp->str = estrdup(ctx->buffer);
      return r;
      
    case ST_STRING1:
      r = STRING1;
      lvalp->data.val = emalloc(len+2);
      memcpy(lvalp->data.val, ctx->buffer, len+1);
      lvalp->data.len = len;
      return r;
      
    case ST_IDENT:
      if (strcmp(ctx->buffer, "if") == 0)
	r = IF;
      else if (strcmp(ctx->buffer, "else") == 0)
	r = ELSE;
      else if (strcmp(ctx->buffer, "for") == 0)
	r = FOR;
      else if (strcmp(ctx->buffer, "while") == 0)
	r = WHILE;
      else if (strcmp(ctx->buffer, "repeat") == 0)
	r = REPEAT;
      else if (strcmp(ctx->buffer, "until") == 0)
	r = UNTIL;
      else if (strcmp(ctx->buffer, "foreach") == 0)
	r = FOREACH;
      else if (strcmp(ctx->buffer, "function") == 0)
	r = FUNCTION;
      else if (strcmp(ctx->buffer, "return") == 0)
	r = RETURN;
      else if (strcmp(ctx->buffer, "x") == 0)
	r = REP;
      else if (strcmp(ctx->buffer, "include") == 0)
	r = INCLUDE;
      else if (strcmp(ctx->buffer, "break") == 0)
	r = BREAK;
      else if (strcmp(ctx->buffer, "continue") == 0)
	r = CONTINUE;
      else if (strcmp(ctx->buffer, "local_var") == 0)
	r = LOCAL;
      else if (strcmp(ctx->buffer, "global_var") == 0)
	r = GLOBAL;
      else
	{
	  r = IDENT;
	  lvalp->str = estrdup(ctx->buffer);
	  return r;
	}
      return r;

    case ST_DEC:
      /* -123 is parsed as "-" and "123" so that we can write "4-2" without
       * inserting a white space after the minus operator
       * Note that strtoul would also work on negative integers */
      lvalp->num = x = strtoul(ctx->buffer, NULL, 10);
#if NASL_DEBUG > 1 && defined(ULONG_MAX) && defined(ERANGE)
      if (x == ULONG_MAX && errno == ERANGE)
	nasl_perror(NULL, "Integer overflow while converting %s at or near line %d\n", ctx->buffer, ctx->line_nb);
#endif
      return INTEGER;

    case ST_OCT:
      lvalp->num = x = strtoul(ctx->buffer, NULL, 8);
#if NASL_DEBUG > 1 && defined(ULONG_MAX) && defined(ERANGE)
      if (x == ULONG_MAX && errno == ERANGE)
	nasl_perror(NULL, "Integer overflow while converting %s at or near line %d\n", ctx->buffer, ctx->line_nb);
#endif
      return INTEGER;

    case ST_HEX:
      lvalp->num = x = strtoul(ctx->buffer, NULL, 16);
#if NASL_DEBUG > 1 && defined(ULONG_MAX)
      if (x == ULONG_MAX)
	nasl_perror(NULL, "Possible integer overflow while converting %s at or near line %d\n", ctx->buffer, ctx->line_nb);
#endif
      return INTEGER;

    case ST_ZEROX:
      nasl_perror(NULL, "Invalid token 0x parsed as 0 at line %d\n",
	      ctx->line_nb);
    case ST_ZERO:
      lvalp->num = 0;
      return INTEGER;
    default:
      abort();
    }
}

int
nasllex(lvalp, parm)
     YYSTYPE *lvalp;
     void	*parm;
{

  int	x = mylex(lvalp, parm);
#if 0
  naslctxt	*ctx = parm;
  if (isprint(x))
    fprintf(stderr, "Line %d\t: '%c'\n", ctx->line_nb, x);
  else
    fprintf(stderr, "Line %d\t:  %d\n", ctx->line_nb, x);
#endif
  return x;
}

