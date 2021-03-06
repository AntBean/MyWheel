#
# (C) Tenable Network Security
#

if(description)
{
 script_id(18064);
 script_bugtraq_id(12988, 13211, 13216, 13228, 13229, 13230, 13231, 13232, 13233);
 if ( NASL_LEVEL >= 2200 )script_cve_id(
   "CAN-2005-0752",
   "CAN-2005-0989",
   "CAN-2005-1153",
   "CAN-2005-1154",
   "CAN-2005-1154",
   "CAN-2005-1155",
   "CAN-2005-1156",
   "CAN-2005-1157",
   "CAN-2005-1158",
   "CAN-2005-1159",
   "CAN-2005-1160"
 );
 script_version("$Revision: 1.3 $");

 name["english"] = "Firefox < 1.0.3";

 script_name(english:name["english"]);
 
 desc["english"] = "
The remote host is using Firefox, an alternative web browser.

The remote version of this software contains various security issues which may
allow an attacker to execute arbitrary code on the remote host

Solution : Upgrade to Firefox 1.0.3
Risk factor : High";

 script_description(english:desc["english"]);
 
 summary["english"] = "Determines the version of Firefox";

 script_summary(english:summary["english"]);
 
 script_category(ACT_GATHER_INFO);
 
 script_copyright(english:"This script is Copyright (C) 2005 Tenable Network Security");
 family["english"] = "Windows";
 script_family(english:family["english"]);
 
 script_dependencies("mozilla_firefox_code_exec.nasl");
 exit(0);
}




fox = get_kb_item("Mozilla/Firefox/Version");
if (fox)
{
  if (ereg(pattern:"(0\.[0-9]\.|1\.0\.[0-2]([^0-9]|$))", string:fox) )
     security_hole(0);
}

