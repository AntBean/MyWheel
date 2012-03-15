 
#
# Msadcs.dll locate.
#
# This plugin was written in NASL by RWT roelof@sensepost.com
#
# Changes by rd: 
# - french


if(description)
{
 script_id(10357);
 script_bugtraq_id(529);
 script_version ("$Revision: 1.17 $");
 script_cve_id("CVE-1999-1011");


 name["english"] = "RDS / MDAC Vulnerability (msadcs.dll) located";
 name["francais"] = "RDS / MDAC Vulnerability (msadcs.dll) possible";

  
 script_name(english:name["english"], francais:name["francais"], deutsch:name["deutsch"]);
 
 desc["english"] = "
The web server is probably susceptible to a common IIS vulnerability discovered by
'Rain Forest Puppy'. This vulnerability enables an attacker to execute arbitrary
commands on the server with Administrator Privileges. 

*** Nessus solely relied on the presence of the file /msadc/msadcs.dll
*** so this might be a false positive

See Microsoft security bulletin (MS99-025) for patch information.
Also, BUGTRAQ ID 529 on www.securityfocus.com ( http://www.securityfocus.com/bid/529 )

Risk factor : High";



 desc["francais"] = "Le script msadcs.dll a �t� trouv� dans /msadc/.
C'est un script de d�mo qui permet a des pirates
d'executer du code arbitraire sur le syst�me
distant.

Solution : effacez-le
Facteur de risque : Elev�
Voir aussi : BUGTRAQ ID 529 on www.securityfocus.com ( http://www.securityfocus.com/bid/529 )";


 

 script_description(english:desc["english"], francais:desc["francais"]);
 
 summary["english"] = "Determines the presence of msadcs.dll";
 summary["francais"] = "V�rifie la pr�sence de /msadc/msadcs.dll";


 script_summary(english:summary["english"], francais:summary["francais"]);
 
 script_category(ACT_GATHER_INFO);
 
 
 script_copyright(english:"This script is Copyright (C) 2000 Roelof Temmingh <roelof@sensepost.com>",
		francais:"Ce script est Copyright (C) 2000 Roelof Temmingh <roelof@sensepost.com>"
		);

 family["english"] = "CGI abuses";
 family["francais"] = "Abus de CGI";

 script_family(english:family["english"], francais:family["francais"]);
 
 script_dependencie("find_service.nes", "http_version.nasl", "www_fingerprinting_hmap.nasl");
 script_require_ports("Services/www", 80);
 exit(0);
}

#
# The script code starts here
#


include("http_func.inc");
include("http_keepalive.inc");
port = get_http_port(default:80);

if ( ! get_port_state(port) )  exit(0);
sig = get_kb_item("www/hmap/" + port + "/description");
if ( sig && "IIS" >!< sig ) exit(0);

cgi = "/msadc/msadcs.dll";
res = is_cgi_installed_ka(item:cgi, port:port);
if(res)security_hole(port);
