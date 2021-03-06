#
# Script by Noam Rathaus
#
# From: "Donato Ferrante" <fdonato@autistici.org>
# Date: 5.7.2004 10:54
# XSS in 12Planet Chat Server 2.9

if(description)
{
 script_id(12299);
 script_cve_id("CAN-2004-0678");
 script_bugtraq_id(10659);
 script_version("$Revision: 1.4 $");
 
 name["english"] = "12Planet Chat Server one2planet.infolet.InfoServlet XSS";

 script_name(english:name["english"]);
 
 desc["english"] = "
The remote host is using 12Planet Chat Server.

There is a bug in this software which makes it vulnerable to cross site
scripting attacks.

An attacker may use this bug to steal the credentials of the legitimate users
of this site.

Risk factor: Medium";

 script_description(english:desc["english"]);
 
 summary["english"] = "Checks for the presence of an XSS bug in 12Planet Chat Server";
 
 script_summary(english:summary["english"]);
 
 script_category(ACT_ATTACK);
 
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
if (  get_kb_item(string("www/", port, "/generic_xss")) ) exit(0);

function check(loc)
{
 req = http_get(item:string(loc, "/servlet/one2planet.infolet.InfoServlet?page=<script>foo</script>"), port:port);

 r = http_keepalive_send_recv(port:port, data:req, bodyonly:1);
 if( r == NULL )exit(0);
 if(egrep(pattern:"<script>foo</script>", string:r))
 {
 	security_warning(port);
	exit(0);
 }
}

check(loc:"");
foreach dir (cgi_dirs())
{
 check(loc:dir);
}

