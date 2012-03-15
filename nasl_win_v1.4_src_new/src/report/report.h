/* Nessuslib -- the Nessus Library
 * Copyright (C) 1998 Renaud Deraison
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */   
 
#ifndef _NESSUSC_REPORT_H
#define _NESSUSC_REPORT_H
#define HOLE_PRESENT 3
#define WARNING_PRESENT 2
#define NOTE_PRESENT 1

#define SAVE_NSR 0
#define SAVE_HTML 1
#define SAVE_TEXT 2
#define SAVE_LATEX 3
#define SAVE_HTML_GRAPH 4
#define SAVE_XML 5
#define SAVE_MAX SAVE_XML

typedef int(*cmp_func_t)(char*, char*);
#define show_error(x) fprintf(stderr, "%s\n", x)

void report_tests(struct arglist *, int);
int is_there_any_hole(struct arglist *);
#endif

void open_report_selectfile(void);
struct arglist *log_to_arglist(char *fname);
void save_report(char *fname, int type, struct arglist * hosts);
