#
# This script was written by Renaud Deraison <deraison@cvs.nessus.org>
#
# See the Nessus Scripts License for details
#

if(description)
{
 script_id(10433);
 script_bugtraq_id(1236);
 script_version ("$Revision: 1.22 $");
 script_cve_id("CVE-2000-0305");
 name["english"] = "NT IP fragment reassembly patch not applied (jolt2)";
 name["francais"] = "Patch for le reassemblage de fragments IP non appliqu� (jolt2)";
 
 script_name(english:name["english"],
 	     francais:name["francais"]);
 
 desc["english"] = "
The hotfix for the 'IP Fragment Reassembly' vulnerability
has not been applied on the remote Windows host.

This vulnerability allows an attacker to send malformed packets
which will hog this computer CPU to 100%, making
it nearly unusable for the legitimate users.


Solution : See http://www.microsoft.com/technet/security/bulletin/ms00-029.mspx
Risk factor : High";


 desc["francais"] = "
Le hotfix r�glant la vuln�rabilit� de r�assemblage
de paquets IP n'a pas �t� appliqu� sur le Windows
distant.

Cette vuln�rabilit� permet � un pirate d'envoyer des paquets
malform�s qui vont consommer 100% du temps CPU de l'hote
distant, le rendant inutilisable pour les utilisateurs
l�gitimes.

Solution : cf http://www.microsoft.com/technet/security/bulletin/ms00-029.mspx
Facteur de risque : S�rieux";


 script_description(english:desc["english"],
 		    francais:desc["francais"]);
 
 summary["english"] = "Determines whether the hotfix Q259728 is installed";
 summary["francais"] = "D�termine si le hotfix Q258728 est install�";
 script_summary(english:summary["english"],
 		francais:summary["francais"]);
 
 script_category(ACT_GATHER_INFO);
 
 script_copyright(english:"This script is Copyright (C) 2000 Renaud Deraison");
 family["english"] = "Windows : Microsoft Bulletins";
 script_family(english:family["english"]);
 
 script_dependencies("smb_hotfixes.nasl");
 script_require_keys("SMB/Registry/Enumerated");
 script_require_ports(139, 445);
 exit(0);
}




include("smb_hotfixes.inc");


if ( hotfix_check_sp(nt:7, win2k:2) <= 0 ) exit(0);

if ( hotfix_missing(name:"Q299444") > 0 &&
     hotfix_missing(name:"Q259728") > 0 ) 
	{
	  security_hole(get_kb_item("SMB/transport"));
	}

