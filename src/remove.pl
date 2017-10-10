#!/usr/bin/perl

print "Content-type: text/html \n\n";

print "Modulos desinstalados </br>";

system("/var/www/html/cgi-bin/remove.sh") == 0
    or die "Bash Script failed";
