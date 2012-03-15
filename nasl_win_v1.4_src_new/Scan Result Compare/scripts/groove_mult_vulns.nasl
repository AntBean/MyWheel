#
# (C) Tenable Network Security
#
# 

if (description) {
  script_id(18355);
  script_version("$Revision: 1.3 $");
  script_bugtraq_id(13682, 13684, 13685, 13686, 13688);

  name["english"] = "Groove Virtual Office / Workspace Multiple Vulnerabilities";
  script_name(english:name["english"]);
 
  desc["english"] = "
According to its version number, the version of Groove Virtual Office
or Groove Workspace on the remote host suffers from multiple
vulnerabilities.  Some of these flaws may allow for arbitrary script
execution, disclosure of sensitive information, and denial of service,
all from remote users. 

Solution : Upgrade to Workspace v2.5n build 1871 or
           Virtual Office v3.1a build 2364 or later.
Risk factor : High";
  script_description(english:desc["english"]);
 
  summary["english"] = "Checks for multiple vulnerabilities in Groove Virtual Office / Workspace";
  script_summary(english:summary["english"]);
 
  script_category(ACT_GATHER_INFO);
  script_family(english:"Windows");

  script_copyright(english:"This script is Copyright (C) 2005 Tenable Network Security");

  script_dependencies("smb_hotfixes.nasl");
  script_require_keys("SMB/Registry/Enumerated");
  script_require_ports(139, 445);

  exit(0);
}


include("smb_func.inc");
include("smb_hotfixes.inc");


if (!get_kb_item("SMB/Registry/Enumerated")) exit(1);


# Look in the registry for the version of Groove installed.
name = kb_smb_name();
port = kb_smb_transport();
if (!get_port_state(port)) exit(1);
login = kb_smb_login();
pass = kb_smb_password();
domain = kb_smb_domain();


# Connect to the remote registry.
soc = open_sock_tcp(port);
if (!soc) exit(1);
session_init(socket:soc, hostname:name);

rc = NetUseAdd(login:login, password:pass, domain:domain, share:"IPC$");
if (rc != 1) exit(1);

hklm = RegConnectRegistry(hkey:HKEY_LOCAL_MACHINE);
if (isnull(hklm)) {
  NetUseDel();
  exit(1);
}


# Determine the version and build number of Groove.
#
# nb: the version number in the registry doesn't seem to use
#     alphabetic characters; eg, "3.1" rather than "3.1a".
key = "SOFTWARE\Groove Networks, Inc.\Groove";
key_h = RegOpenKey(handle:hklm, key:key, mode:MAXIMUM_ALLOWED);
if (!isnull(key_h)) {
  value = RegQueryValue(handle:key_h, item:"CurVer");
  if (!isnull(value)) ver = value[1];

  value = RegQueryValue(handle:key_h, item:"BuildNumber");
  if (!isnull(value)) build = int(value[1]);

  RegCloseKey(handle:key_h);

  # Check whether it's vulnerable.
  if (isnull(ver) || isnull(build)) {
    if (log_verbosity > 1) {
      debug_print("can't determine version / build number of Groove install!");
    }
  }
  else {
      iver = split(ver, sep:'.', keep:FALSE);

    if (
      ( int(iver[0]) < 2 || ( int(iver[0]) == 2 && int(iver[1]) < 5 ) ) ||
      (  ver == "2.5" && build < 1871 ) ||
      (  ver == "3.0" ) ||
      (  ver == "3.1" && build < 2364 )
     ) security_warning(port);
  }
}


# Clean up.
RegCloseKey(handle:hklm);
NetUseDel();