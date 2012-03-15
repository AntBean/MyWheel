/* Nessus
 * Copyright (C) 1998 - 2001 Renaud Deraison
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
 *
 * In addition, as a special exception, Renaud Deraison
 * gives permission to link the code of this program with any
 * version of the OpenSSL library which is distributed under a
 * license identical to that listed in the included COPYING.OpenSSL
 * file, and distribute linked combinations including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * this file, you may extend this exception to your version of the
 * file, but you are not obligated to do so.  If you do not wish to
 * do so, delete this exception statement from your version.
 */
 
#include "../includes.h"
#include "../libnessus.h"
#include "report.h"
#include "report_utils.h"
//#include "error_dialog.h"
//#include "globals.h"


static int line_length(char * text)
{
 if((!text[0]) ||
    (text[0]=='\n'))
     return 0;
  else 
    return 1 + line_length(&(text[1]));
}
 
static char * printf_line(FILE * file, char * text, int offset)
{
 int n = 0;
 if(text[0]=='\n')
 	return &(text[1]);
	
 while(text[n] && (n != offset))
 {
  fprintf(file, "%c", text[n]);
  if(text[n+1]=='\n')return &(text[n+2]);
  else n++;
 }
 return &(text[n]);
}


void printf_formatted_text(FILE * file, char * text, int width, char * prefix)
{
 int length = strlen(text);
 int pl = prefix ? strlen(prefix):0;
 char *c;
 
 
 c = text;
	
 while(length)
 {
  /* go to the end and rewind until we find a space */
  char * t = text + width - pl;	
  while((t[0]!=' ') && (t[0]!='\n'))
  {
   t--;
   if(t == text)
    {
     /* Too long. Abort */
     if(prefix)fprintf(file, "\n%s", prefix);
     fprintf(file, "%s\n", text);
     return;
    }
  }
  fprintf(file, "\n%s", prefix?prefix:"");
  text = printf_line(file, text, t - text);
  length = strlen(text);
 }
}
 
 
int 
arglist_to_text(hosts, filename)
 struct arglist * hosts;
 char * filename;
{
 FILE * file;
 struct arglist * h;
 
 if(!strcmp(filename, "-"))file = stdout;
 else file = fopen(filename, "w");
 if(!file){
 	show_error("Could not create this file !");
	perror("fopen ");
	return(-1);
	}

 fprintf(file, "Windows扫描报告\n");
 fprintf(file, "------------------\n\n\n\n");
 
 
 /*
  * Write a (small) summary
  */
 fprintf(file, "概要\n\n");
 fprintf(file, " - 扫描期间主机存在的数目 : %d\n",
 			 arglist_length(hosts));
 fprintf(file, " - 发现的安全漏洞的数目 : %d\n", 
 			number_of_holes(hosts));
 fprintf(file, " - 发现的安全警告的数目 : %d\n",
 			number_of_warnings(hosts));
 fprintf(file, " - 发现的安全记录的数目 : %d\n",
 			number_of_notes(hosts));
 h = hosts;
 
 fprintf(file, "\n\n\n");
 fprintf(file, "测试主机\n\n");
 while(h && h->next)
 {
  int result;
  fprintf(file, " %s", h->name);
  result = is_there_any_hole(h->value);
  if(result == HOLE_PRESENT)fprintf(file, " (发现安全漏洞)\n");
  else if(result == WARNING_PRESENT)fprintf(file, " (发现安全警告)\n");
  else if(result == NOTE_PRESENT)fprintf(file, " (发现安全记录)\n");
  else fprintf(file, " (没有发现明显的问题)\n");
  h = h->next;
 }

 fprintf(file, "\n\n\n");
 
 fprintf(file, "详细\n\n");
 while(hosts && hosts->next)
 {
  char * port;
  struct arglist * ports;
  
 
  fprintf(file, "+ %s :\n", hosts->name);
  ports = arg_get_value(hosts->value, "端口");
  if(ports)
  {
   struct arglist * open = ports;
   if(open->next)
   {
  
    fprintf(file, " . 打开端口列表 :\n");
 

    while(open && open->next){
	if(arg_get_value(open->value, "报告") ||
	   arg_get_value(open->value, "信息") ||
	   arg_get_value(open->value, "记录"))
	   {
	     fprintf(file, "   o %s",open->name);
	     if(arg_get_value(open->value, "报告"))  fprintf(file, " (发现安全漏洞)\n");
	     else if(arg_get_value(open->value, "INFO")) fprintf(file, " (发现安全警告)\n");
	     else fprintf(file, " (发现安全记录)\n");
	   }	 
	 else		  
		fprintf(file, "   o %s\n", open->name);
	open = open->next;
      }
   }
  /*
   * Write the summary of the open ports here
   */
    
   
   while(ports && ports->next)
   {
    struct arglist * report;
    struct arglist * info;
    struct arglist * note;
    
    port = ports->name;
    report = arg_get_value(ports->value, "报告");
    if(report)while(report && report->next)
     {
     if(strlen(report->value))
     {
      fprintf(file, "\n . 发现端口漏洞 %s : \n\n",port);
      printf_formatted_text(file, report->value, 80, "    ");
      fprintf(file, "\n");
     }
      report = report->next;
     }
   info = arg_get_value(ports->value, "信息");
   if(info)while(info && info->next)
    {
     if(strlen(info->value))
     {
     fprintf(file, "\n . 发现端口警告 %s\n\n", port);
     printf_formatted_text(file, info->value, 80, "    ");
     fprintf(file, "\n");
     }  
     info = info->next;
    }
   note = arg_get_value(ports->value, "记录");
   if(note)while(note && note->next)
    {
     if(strlen(note->value))
     {
     fprintf(file, "\n . 发现端口信息 %s\n\n", port);
     printf_formatted_text(file, note->value, 80, "    ");
     fprintf(file, "\n");
     }  
     note = note->next;
    }
    ports = ports->next;
   }
  }
  fprintf(file, "\n");
  hosts = hosts->next;
 }
 fprintf(file, "\n\n\n");
 fprintf(file, "------------------------------------------------------\n");
 //fprintf(file, "This file was generated by the Nasl For Windows\n");
 fclose(file);
 return(0);
}
