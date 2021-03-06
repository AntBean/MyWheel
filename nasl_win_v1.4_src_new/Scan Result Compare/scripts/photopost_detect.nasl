#
# (C) Tenable Network Security
#


 desc["english"] = "
This script detects whether the remote host is running PhotoPost PHP
and extracts version numbers and locations of any instances found. 

PhotoPost PHP is a picture gallery software suite written in PHP.  See
http://www.photopost.com/ for more information.";


if (description) {
  script_id(17648);
  script_version("$Revision: 1.1 $");

  name["english"] = "PhotoPost PHP Detection";
  script_name(english:name["english"]);
 
  script_description(english:desc["english"]);
 
  summary["english"] = "Checks for presence of PhotoPost PHP";
  script_summary(english:summary["english"]);
 
  script_category(ACT_GATHER_INFO);
  script_family(english:"CGI abuses");
 
  script_copyright(english:"This script is Copyright (C) 2005 Tenable Network Security");

  script_dependencies("global_settings.nasl", "http_version.nasl");
  script_require_ports("Services/www", 80);

  exit(0);
}


include("global_settings.inc");
include("http_func.inc");
include("http_keepalive.inc");


port = get_http_port(default:80);
if (!get_port_state(port)) exit(0);
if (!can_host_php(port:port)) exit(0);


# Search for PhotoPost PHP.
installs = 0;
foreach dir (cgi_dirs()) {
  # Try to pull up index.php.
  req = http_get(item:string(dir, "/index.php"), port:port);
  res = http_keepalive_send_recv(port:port, data:req);
  if (res == NULL) exit(0);

  # If it's PhotoPost PHP.
  if (egrep(string:res, pattern:"Powered by: <[^>]+>PhotoPost</a> PHP", icase:TRUE)) {

    # Try to identify the version number from index.php.
    # 
    # Sample headers:
    #  'Powered by: <A target="_blank" href="http://www.photopost.com">PhotoPost</a> PHP 3.0.6 <Br>Copyright 2002 All Enthusiast, Inc.'
    #  'Powered by: <A target="_blank" href="http://www.photopost.com">PhotoPost</a> PHP 3.2.1 <Br>Copyright 2002 All Enthusiast, Inc.'
    #  'Powered by: <a target="_blank" href="http://www.qksrv.net/click-xxxxxxx-xxxxxxx">PhotoPost</a> PHP 4.0.1 Copyright 2003 All Enthusiast, Inc.'
    #  'Powered by: <a target="_blank" href="http://www.photopost.com">PhotoPost</a> PHP 4.5.1<br />Copyright 2003 All Enthusiast, Inc.'
    #  'Powered by: <a target="_blank" href="http://www.photopost.com">PhotoPost</a> PHP 4.8d<br />Copyright &copy 2004 All Enthusiast, Inc.'
    #  'Powered by: <a target="_blank" href="http://www.photopost.com">PhotoPost</a> PHP 4.8.2<br />Copyright &copy; 2004 All Enthusiast, Inc.'
    #  'Powered by: <a href="http://www.qksrv.net/click-xxxxxxx-xxxxxxx">PhotoPost</a> PHP 4.86 vB3 Enhanced<br />Copyright 2005 All Enthusiast, Inc.'
    #  'Powered by: <a target="_blank" href="http://www.photopost.com">PhotoPost</a> PHP 5.02 vB3 Enhanced<br />Copyright 2005 All Enthusiast, Inc.'
    pat = "Powered by: <[^>]+>PhotoPost</a> PHP (.+)( C| <Br>C|<br />C)opyright";
    matches = egrep(string:res, pattern:pat, icase:TRUE);
    foreach match (split(matches)) {
      match = chomp(match);
      ver = eregmatch(pattern:pat, string:match);
      if (!isnull(ver)) {
        ver = ver[1];
        # nb: we're not particularly interested in vB3 enhancements.
        ver = ver - ' vB3 Enhanced';
        break;
      }
    }

    if (dir == "") dir = "/";
    set_kb_item(
      name:string("www/", port, "/photopost"),
      value:string(ver, " under ", dir)
    );
    installations[dir] = ver;
    ++installs;

    # Scan for multiple installations only if "Thorough Tests" is checked.
    if (!thorough_tests) break;
  }
}


# Report any instances found unless Report verbosity is "Quiet".
if (installs && report_verbosity > 0) {
  if (installs == 1) {
    foreach dir (keys(installations)) {
      # empty - just need to set 'dir'.
    }
    if (ver == "unknown") {
      info = string("An unknown version of PhotoPost PHP was detected on the remote\nhost under the path ", dir, ".");
    }
    else {
      info = string("PhotoPost PHP ", ver, " was detected on the remote host under\nthe path ", dir, ".");
    }
  }
  else {
    info = string(
      "Multiple instances of PhotoPost PHP were detected on the remote host:\n",
      "\n"
    );
    foreach dir (keys(installations)) {
      info = info + string("    ", installations[dir], ", installed under ", dir, "\n");
    }
    info = chomp(info);
  }

  desc = ereg_replace(
    string:desc["english"],
    pattern:"This script[^\.]+\.", 
    replace:info
  );
  security_note(port:port, data:desc);
}

