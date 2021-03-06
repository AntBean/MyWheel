#
# (C) Tenable Network Security
#
# 

if (description) {
  script_id(17687);
  script_version("$Revision: 1.3 $");

  script_cve_id("CAN-2005-0524", "CAN-2005-0525");
  script_bugtraq_id(12962, 12963);

  name["english"] = "PHP Image File Format Denial Of Service Vulnerabilities";
  script_name(english:name["english"]);
 
  desc["english"] = "
According to its banner, the version of PHP installed on the remote
host is vulnerable to a denial of service attack due to its failure to
properly validate file data in the routines 'php_handle_iff' and
'php_handle_jpeg', which are called by the PHP function
'getimagesize'.  Using a specially crafted image file, an attacker can
trigger an infinite loop when 'getimagesize' is called, perhaps even
remotely in the case image uploads are allowed. 

Solution : Upgrade to PHP 4.3.11 / PHP 5.0.4 or later.
Risk factor : Medium";
  script_description(english:desc["english"]);
 
  summary["english"] = "Checks for image file format denial of service vulnerabilities in PHP";
  script_summary(english:summary["english"]);
 
  script_category(ACT_GATHER_INFO);
  script_family(english:"Denial of Service");

  script_copyright(english:"This script is Copyright (C) 2005 Tenable Network Security");

  script_dependencie("http_version.nasl");
  script_require_ports("Services/www", 80);

  exit(0);
}


include("http_func.inc");
include("backport.inc");


port = get_http_port(default:80);
if (!get_port_state(port)) exit(0);
if (!can_host_php(port:port)) exit(0);


banner = get_http_banner(port:port);
if ( ! banner )exit(0);

php = get_php_version(banner:banner);
if ( ! php ) exit(0);
if (ereg(string:php, pattern:"PHP/([0-3]\.|4\.[0-2]\.|4\.3\.([0-9][^0-9]+|10[^0-9]+)|5\.0\.[0-3][^0-9]+)") )
	security_hole(port);
