#!/usr/bin/perl

# HTTP Header
print "Content-Type:application/octet-stream; name=\"FileName\"\r\n";
print "Content-Disposition: attachment; filename=\"FileName\"\r\n\n";

# Actual File Content will go hear.
open( FILE, "</home/gustav/Desktop/TP3-So2" );
while(read(FILE, $buffer, 100) ){
   print("$buffer");
}