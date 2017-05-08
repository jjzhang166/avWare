#!/bin/sh

######## MISC QOS setting! ######
# If VO screen split, we suggest you to decrease ARM priority,						#
# set register 0x20120058 to 0x55563624, which may cause cpu performance down.		#
himm 0x20120058 0x55563621  ## cipher-dma2-dma1-usb3-sata3-vapu-gmac-a9
himm 0x2012005c 0x43055555  ## vedu-vdh0-vdp-pcie-sfc-usb2-nandc-sdio
himm 0x20120060 0x34443443  ## aio-vgs-jpge-tde-vdh1-mdu/ddrt-jpgd-vpss

####### MISC timeout setting! #########
himm 0x20120064 0x00000000   ## cipher-dma2-dma1-usb3-sata3-vapu-gmac-a9
himm 0x20120068 0x00100000   ## vedu-vdh0-vdp-pcie-sfc-usb2-nandc-sdio
himm 0x2012006c 0x00000000   ## aio-vgs-jpge-tde-vdh1-mdu/ddrt-jpgd-vpss

##############################
# mddrc0 pri&timeout setting #
##############################
himm  0x20110040  0x81001000  # ports0 选择随路QOS模式
himm  0x20110044  0x81001000  # ports1 
himm  0x20110048  0x81001000  # ports2 
himm  0x2011004c  0x81001000  # ports3 
himm  0x20110050  0x81001000  # ports4 
himm  0x20110054  0x81001000  # ports5 
himm  0x20110058  0x81001000  # ports6 

#DDRC AXI pri ports0 - 6
############# WR pri ##############
himm  0x201100c0  0x76543210  # ports0         
himm  0x201100c4  0x76543210  # ports1         
himm  0x201100c8  0x76543210  # ports2
himm  0x201100cc  0x76543210  # ports3
himm  0x201100d0  0x76543210  # ports4
himm  0x201100d4  0x76543210  # ports5
himm  0x201100d8  0x76543210  # ports6
############# RD pri ##############
himm  0x20110100  0x76543210  # ports0         
himm  0x20110104  0x76543210  # ports1         
himm  0x20110108  0x76543210  # ports2
himm  0x2011010c  0x76543210  # ports3
himm  0x20110110  0x76543210  # ports4
himm  0x20110114  0x76543210  # ports5
himm  0x20110118  0x76543210  # ports6

############# WR timeout ###########
himm  0x20110140  0x30080200  # ports0         
himm  0x20110144  0x30080200  # ports1         
himm  0x20110148  0x30080200  # ports2
himm  0x2011014c  0x30080200  # ports3
himm  0x20110150  0x30080200  # ports4
himm  0x20110154  0x30080200  # ports5
himm  0x20110158  0x30080200  # ports6
############# RD timeout ###########
himm  0x20110180  0x30080200  # ports0         
himm  0x20110184  0x30080200  # ports1         
himm  0x20110188  0x30080200  # ports2
himm  0x2011018c  0x30080200  # ports3
himm  0x20110190  0x30080200  # ports4
himm  0x20110194  0x30080200  # ports5
himm  0x20110198  0x30080200  # ports6

############ END ##########

############port2 otd水线配置##############
himm  0x20110208  0x1F1F1F01   ##port2 PCIE
himm  0x20110244  0x00000030   ##port2 非绿色端口模式1

