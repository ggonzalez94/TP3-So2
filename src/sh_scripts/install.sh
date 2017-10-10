#!/bin/bash
find /var/www/html/uploads/ -type f | while read line; do
    name=$(basename "$line")
	echo "Modulo instalado: $line"
	sudo insmod $line
done
echo
