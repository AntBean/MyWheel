#
# This script was written by Renaud Deraison <deraison@cvs.nessus.org>
#
# See the Nessus Scripts License for details
#

if(description)
{
 script_id(10269);
 script_bugtraq_id(843);
 script_version ("$Revision: 1.18 $");
 script_cve_id("CVE-1999-0834");
 
 name["english"] = "SSH Overflow";
 name["francais"] = "Buffer overflow dans SSH";
 script_name(english:name["english"], francais:name["francais"]);
 
 desc["english"] = "
You are running a version of SSH which is older than (or as old as) 
version 1.2.27.
If this version was compiled against the RSAREF library, then it is very 
likely to be vulnerable to a buffer overflow which may be exploited by an 
attacker to gain root privileges on your system.

To determine if you compiled ssh against the RSAREF library, type 
'ssh -V' on the remote host.

Risk factor : High
Solution : Use ssh 2.x, or do not compile ssh against the RSAREF library";

	
 desc["francais"] = "
Vous faites tourner une version de ssh
plus ancienne ou �gale � la version 1.2.27.

Cette version est vuln�rable � un d�passement
de buffer dans le cas o� elle serait compil�e
avec la biblioth�que RSAREF, ce qui permettrait
� un pirate de passer root sur ce syst�me.

Pour d�terminer si vous avez compil� SSH avec
RSAREF, tappez 'ssh -V' sur le syst�me distant.

Facteur de risque : Elev�
Solution : utilisez ssh 2.x, ou recompilez ssh en
d�sactivant le support rsaref.";


 script_description(english:desc["english"], francais:desc["francais"]);
 
 summary["english"] = "Checks for the remote SSH version";
 summary["francais"] = "V�rifie la version de SSH";
 script_summary(english:summary["english"], francais:summary["francais"]);
 
 script_category(ACT_GATHER_INFO);
 
 
 script_copyright(english:"This script is Copyright (C) 1999 Renaud Deraison",
		francais:"Ce script est Copyright (C) 1999 Renaud Deraison");
 family["english"] = "Gain a shell remotely";
 family["francais"] = "Obtenir un shell � distance";
 script_family(english:family["english"], francais:family["francais"]);
 script_dependencie("ssh_detect.nasl");
 script_require_ports("Services/ssh", 22);
 exit(0);
}

#
# The script code starts here
#


port = get_kb_item("Services/ssh");
if(!port)port = 22;


banner = get_kb_item("SSH/banner/" + port );
if ( ! banner ) exit(0);


if(ereg(string:banner, pattern:"SSH-.*-1\.([0-1]|2\.([0-1]..*|2[0-7]))[^0-9]*$", icase:TRUE))security_warning(port);
