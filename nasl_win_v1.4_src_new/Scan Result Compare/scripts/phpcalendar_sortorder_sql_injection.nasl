#
# (C) Tenable Network Security
#
# 

if (description) {
  script_id(18156);
  script_version("$Revision: 1.2 $");

  script_cve_id("CAN-2005-1397");
  script_bugtraq_id(13405);
  if (defined_func("script_xref")) {
    script_xref(name:"OSVDB", value:"15866");
  }

  name["english"] = "PHP-Calendar Search.PHP SQL Injection Vulnerability";
  script_name(english:name["english"]);
 
  desc["english"] = "
The version of PHP-Calendar installed on the remote host suffers from
a SQL injection vulnerability due to its failure to sanitize
user-supplied input to the 'sort' and 'order' parameters to the
'includes/search.php' script.  An attacker can exploit this flaw to
alter database queries, potentially revealing sensitive information or
even modifying data.

Solution : Upgrade to PHP-Calendar version 0.10.3 or later.
Risk factor : Low";
  script_description(english:desc["english"]);
 
  summary["english"] = "Checks for SQL injection vulnerability in PHP-Calendar search.php";
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


port = get_http_port(default:80);
if (!get_port_state(port)) exit(0);
if (!can_host_php(port:port)) exit(0);


# Try the exploit.
foreach dir (cgi_dirs()) {
  postdata = string(
    "submit=Submit&",
    "searchstring=nessus&",
    "action=search&",
    "sday=1&",
    "smonth=1&",
    "syear=2005&",
    "eday=31&",
    "emonth=12&",
    "eyear=2005&",
    # values for both these fields are passed as-is to MySQL; the
    # single quote will cause a syntax error in affected versions.
    "sort=startdate&",
    "order='", SCRIPT_NAME
  );
  req = string(
    "POST ", dir, "/index.php HTTP/1.1\r\n",
    "Host: ", get_host_name(), "\r\n",
    "Content-Type: application/x-www-form-urlencoded\r\n",
    "Content-Length: ", strlen(postdata), "\r\n",
    "\r\n",
    postdata
  );
  res = http_keepalive_send_recv(port:port, data:req, bodyonly:TRUE);

  # There's a problem if we see a syntax error.
  if (
    egrep(
      string:res, 
      pattern:string("an error in your SQL syntax.+ near ''", SCRIPT_NAME, "'"), 
      icase:TRUE
    )
  ) {
    security_warning(port);
    exit(0);
  }
}
