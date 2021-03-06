#
# (C) Tenable Network Security
#

if(description)
{
 script_id(12057);
 script_bugtraq_id(9659);
 script_version ("$Revision: 1.3 $");

 
 name["english"] = "ASP Portal XSS";

 script_name(english:name["english"]);
 
 desc["english"] = "
The remote host is running the ASP Portal CGI suite.

There is a cross site scripting issue in this suite
which may allow an attacker to steal your users cookies.


Solution : See http://www.aspportal.net/downloadsviewer.asp?theurl=38
Risk factor : Medium";




 script_description(english:desc["english"]);
 
 summary["english"] = "Checks for ASP Portal";
 
 script_summary(english:summary["english"]);
 
 script_category(ACT_ATTACK);
 
 
 script_copyright(english:"This script is Copyright (C) 2004 Tenable Network Security"); 
 family["english"] = "CGI abuses : XSS";
 family["francais"] = "Abus de CGI";
 script_family(english:family["english"], francais:family["francais"]);
 script_dependencie("find_service.nes", "http_version.nasl", "cross_site_scripting.nasl");
 script_require_ports("Services/www", 80);
 exit(0);
}

# The script code starts here

include("http_func.inc");
include("http_keepalive.inc");

port = get_http_port(default:80);

if(!get_port_state(port))exit(0);
if(get_kb_item(string("www/", port, "/generic_xss"))) exit(0);
if(!can_host_php(port:port))exit(0);

foreach d ( cgi_dirs() )
{
 req = http_get(item:string(d, "/index.asp?inc=<script>foo</script>"), port:port);
 res = http_keepalive_send_recv(port:port, data:req);
 if( res == NULL ) exit(0);
 if(ereg(pattern:"^HTTP/[0-9]\.[0-9] 200 ", string:res) &&
    egrep(pattern:"<script>foo</script>", string:res)){
 	security_warning(port);
	exit(0);
 }
}
