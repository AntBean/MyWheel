#
# (C) Tenable Network Security
#

if(description)
{
 
 script_id(15477);  
 script_bugtraq_id(11435, 11432);
 script_version ("$Revision: 1.3 $");
 
 name["english"] = "MySQL multiple flaws (3)";
 script_name(english:name["english"], francais:name["francais"]);
 
 desc["english"] = "
You are running a version of MySQL which is older than version 4.0.21.

There are two flaws in the remote version of this database :

- There is an unauthorized database GRANT privilege vulnerability, which
may allow an attacker to misuse the GRANT privilege it has been given and
to use it against other databases

- A denial of service vulnerability may be triggered by the misuse of the
FULLTEXT search functionnality.

Solution : Upgrade to MySQL 4.0.21
Risk factor : Medium";

	


 script_description(english:desc["english"]);
 
 summary["english"] = "Checks for the remote MySQL version";
 summary["francais"] = "V�rifie la version de MySQL";
 script_summary(english:summary["english"], francais:summary["francais"]);
 
 script_category(ACT_GATHER_INFO);
 
 
 script_copyright(english:"This script is Copyright (C) 2004 Tenable Network Security");
 family["english"] = "Gain a shell remotely";
 family["francais"] = "Obtenir un shell � distance";
 script_family(english:family["english"], francais:family["francais"]);
 script_dependencie("find_service.nes", "mysql_version.nasl");
 script_require_ports("Services/mysql", 3306);
 exit(0);
}

#
# The script code starts here
#

include("misc_func.inc");

port = get_kb_item("Services/mysql");
if(!port)port = 3306;

ver=get_mysql_version(port); 
if(ver==NULL) exit(0);
if(ereg(pattern:"^([0-3]\.|4\.0\.([0-9]|1[0-9]|20)([^0-9]|$))", string:ver))security_warning(port);	
