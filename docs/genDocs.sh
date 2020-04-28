#!/bin/sh
echo "# Tabla de KeyCodes" > keyCodes.md
echo "Key codes tomados del archivo /usr/include/linux/input-event-codes.h" >> keyCodes.md
echo "   " >> keyCodes.md
echo "| Nombre Tecla | Código |" >> keyCodes.md
echo "|--------------|--------|" >> keyCodes.md
cat /usr/include/linux/input-event-codes.h | grep KEY | grep -v 0x | awk '{print "| " $2 " | " $3 " |"}' >> keyCodes.md
