#
# (C) Tenable Network Security
#


 desc["english"] = "
This script detects whether the remote host is running Phorum and
extracts version numbers and locations of any instances found. 

Phorum is a web-based message board written in PHP.  See
http://phorum.org/ for more information.";


if (description) {
  script_id(17349);
  script_version("$Revision: 1.2 $");

  name["english"] = "Phorum Detection";
  script_name(english:name["english"]);
 
  script_description(english:desc["english"]);
 
  summary["english"] = "Checks for presence of Phorum";
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


# Search for Phorum.
installs = 0;
foreach dir (cgi_dirs()) {
  # nb: while the version number isn't always found in 'index.php',
  #     it does seem to be in 'admin.php'.
  req = http_get(item:string(dir, "/admin.php"), port:port);
  res = http_keepalive_send_recv(port:port, data:req);
  if (res == NULL) exit(0);

  # If it's Phorum.
  if (egrep(string:res, pattern:"title>Phorum Admin<")) {
    if (dir == "") dir = "/";

    # Try to identify the version number from the page itself.
    pat = "Phorum Admin.+version ([^<]+)<";
    matches = egrep(pattern:pat, string:res, icase:TRUE);
    foreach match (split(matches)) {
      match = chomp(match);
      ver = eregmatch(pattern:pat, string:match);
      if (!isnull(ver)) {
        ver = ver[1];
        break;
      }
    }
    # If unsuccessful, try to grab it from the changelog.
    if (isnull(ver)) {
      req = http_get(item:dir + "/docs/CHANGES", port:port);
      res = http_keepalive_send_recv(port:port, data:req);
      if (res == NULL) exit(0);

      pat = "^Release: phorum\.(.+)";
      matches = egrep(pattern:pat, string:res, icase:TRUE);
      foreach match (split(matches)) {
        match = chomp(match);
        ver = eregmatch(pattern:pat, string:match, icase:TRUE);
        if (!isnull(ver)) {
          ver = ver[1];
          break;
        }
      }
    }
    # Oh well, just mark it as "unknown".
    if (isnull(ver)) ver = "unknown";

    set_kb_item(
      name:string("www/", port, "/phorum"),
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
      info = string("An unknown version of Phorum was detected on the remote\nhost under the path ", dir, ".");
    }
    else {
      info = string("Phorum ", ver, " was detected on the remote host under\nthe path ", dir, ".");
    }
  }
  else {
    info = string(
      "Multiple instances of Phorum were detected on the remote host:\n",
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

