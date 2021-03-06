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
# From: "Matthew Murphy" <mattmurphy@kc.rr.com>
# To: full-disclosure@lists.netsys.com, 
#  "SecurITeam News" <news@securiteam.com>, bugtraq@securityfocus.com
# Subject: Three BadBlue Vulnerabilities
# Date: Fri, 12 Jul 2002 19:50:16 -0500
#

if(description)
{
 script_id(11064);
 script_bugtraq_id(5226);
 script_version ("$Revision: 1.16 $");
 script_cve_id("CAN-2002-1021");
 name["english"] = "BadBlue invalid null byte vulnerability";
 script_name(english:name["english"]);
 
 desc["english"] = "It was possible to read the content of /EXT.INI
(BadBlue configuration file) by sending an invalid GET request.

A cracker may exploit this vulnerability to steal the passwords.


Solution : upgrade your software or protect it with a filtering reverse proxy
Risk factor : High";

 script_description(english:desc["english"]);
 
 summary["english"] = "Read BadBlue protected configuration file";
 script_summary(english:summary["english"]);
 
 script_category(ACT_ATTACK);
 
 script_copyright(english:"This script is Copyright (C) 2002 Michel Arboi");
 family["english"] = "CGI abuses";
 script_family(english:family["english"]);
 script_require_ports("Services/www", 80);
 script_dependencies("find_service.nes", "no404.nasl", "http_version.nasl");
 exit(0);
}

########

include("http_func.inc");
include("http_keepalive.inc");

port = get_http_port(default:80);
if ( ! port ) exit(0);
banner = get_http_banner(port:port);
if ( ! banner ) exit(0);
if ("BadBlue" >!< banner ) exit(0);


r = string("/ext.ini.%00.txt");
res = is_cgi_installed_ka(item:r, port:port);
if( res ) security_hole(port);
