#!/bin/sh

echo "run $0 begin!!!";
 
#CIPHER
himm 0x2003007c 0x00000002 

#TOE/GMAC
himm 0x200300c8 0x000000aa

#DMA
himm 0x200300e0 0x00000002 

echo "run $0 end!!!";
