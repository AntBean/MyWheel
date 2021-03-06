#
# (C) Tenable Network Security
#


if (description) {
  script_id(17650);
  script_version ("$Revision: 1.1 $"); 
  script_bugtraq_id(12943);

  name["english"] = "Horde Parent Page Title Cross-Site Scripting Vulnerability";
  script_name(english:name["english"]);

  desc["english"] = "
The version of Horde installed on the remote host suffers from a
cross-site scripting vulnerability in which an attacker can inject
arbitrary HTML and script code via the page title of a parent frame,
enabling him to steal cookie-based authentication credentials and
perform other such attacks. 

Solution : Upgrade to Horde version 3.0.4 or later.

Risk factor : Low";
  script_description(english:desc["english"]);
 
  summary["english"] = "Checks for parent page title XSS vulnerability in Horde";
  script_summary(english:summary["english"]);
 
  script_category(ACT_GATHER_INFO);
  script_family(english:"CGI abuses : XSS");
 
  script_copyright(english:"This script is Copyright (C) 2005 Tenable Network Security");

  script_dependencies("horde_detect.nasl");
  script_require_ports("Services/www", 80);

  exit(0);
}


include("http_func.inc");


port = get_http_port(default:80);
if (!get_port_state(port)) exit(0);
if (!can_host_php(port:port)) exit(0);


# Test an install.
install = get_kb_item(string("www/", port, "/horde"));
if (isnull(install)) exit(0);
matches = eregmatch(string:install, pattern:"^(.+) under (/.*)$");
if (!isnull(matches)) {
  ver = matches[1];

  if (ver =~ "^([12]\.|3\.0\.([0-3]|4-RC))") 
    security_warning(port);
}

