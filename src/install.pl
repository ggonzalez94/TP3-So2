#!/usr/bin/perl

print "Content-type: text/html \n\n";

print "Instalacion de modulos </br>";
print "\n\n";
system("/var/www/html/cgi-bin/install.sh") == 0
    or die "Bash Script failed";
