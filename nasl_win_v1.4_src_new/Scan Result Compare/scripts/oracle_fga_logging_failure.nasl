#
# (C) Tenable Network Security
#
# 

if (description) {
  script_id(18205);
  script_version("$Revision: 1.2 $");
  script_bugtraq_id(13510);

  name["english"] = "Oracle Database 9i/10g Fine Grained Audit Logging Failure Vulnerability";
  script_name(english:name["english"]);
 
  desc["english"] = "
The remote host is running a version of Oracle Database that, according
to its version number, suffers from a flaw in which Fine Grained
Auditing (FGA) becomes disabled when the user SYS runs a SELECT
statement. 

See also : http://www.red-database-security.com/advisory/oracle-fine-grained-auditing-issue.html
Solution : Apply the 10.1.0.4 patch set for Oracle 10g.
Risk Factor : Medium";
  script_description(english:desc["english"]);
 
  summary["english"] = "Checks for fine grained audit logging failure vulnerability in Oracle Database 9i/10g";
  script_summary(english:summary["english"]);
 
  script_category(ACT_GATHER_INFO);
  script_copyright(english:"This script is Copyright (C) 2005 Tenable Network Security");

  family["english"] = "Misc.";
  script_family(english:family["english"]);

  script_dependencie("oracle_tnslsnr_version.nasl");
  script_require_ports("Services/oracle_tnslsnr");

  exit(0);
}

include('global_settings.inc');
if ( report_paranoia < 1 ) exit(0);

port = get_kb_item("Services/oracle_tnslsnr");
if (isnull(port)) exit(0);


ver = get_kb_item(string("oracle_tnslsnr/", port, "/version"));
if (ver) {
  if (ver =~ ".*Version (5\.0\.0\.([012]\.0\.0|2\.9\.0))") {
    security_warning(port);
    exit(0);
  }
  if (ver =~ ".*Version 8\.1\.7") {
    security_warning(port);
    exit(0);
  }
  if (ver =~ ".*Version 9\.(0\.([0124][^0-9]?|1\.[2-5]|4\.0)|2\.0\.0(\.[1-6])?)") {
    security_warning(port);
    exit(0);
  }
  if (ver =~ ".*Version 10\.1\.0\.0\.([2-4][^0-9]?|3\.1)") {
    security_warning(port);
    exit(0);
  }
}
