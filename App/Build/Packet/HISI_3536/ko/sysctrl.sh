#!/bin/sh

######### MISC QOS setting! ######
himm 0x12120130 0x53356633  ## JPGE-VPSS1-VDH1-TDE-MDU-JPGD-VPSS0-VDH0
himm 0x12120134 0x05251554  ## VICAP-PGD-AVC-IVE-AIO-VGS0-GZIP
himm 0x12120138 0x56555006  ## USB2-SDIO0-NFC-DMA1-DMA0-VDP1-VDP0-GPU
himm 0x1212013c 0x77444666  ## A7-A17-USB3-PCIE-BPD-SHA-SDIO1-CIPHER
himm 0x12120140 0x33411116  ## VPSS2-VDH2-SFC-XOR-SATA-  -TOE-VDMA

######### media0 timeout ctrl #######
himm 0x12120034 0x8020801c
himm 0x12120038 0x00000000
himm 0x1212003c 0x00008370
######### media0 pri #######
himm 0x12120040 0x00050066
######### media1 timeout ctrl #######
himm 0x12120044 0x8020801c
himm 0x12120048 0x00000000
himm 0x1212004c 0x00008170
######### media1 pri #######
himm 0x12120050 0x00050066
######### media2 timeout ctrl #######
himm 0x12120054 0x00000000
himm 0x12120058 0x00008032
himm 0x1212005c 0x8020801c
himm 0x12120060 0x00000000
######### media2 pri #######
himm 0x12120064 0x00560600
######### media3 timeout ctrl #######
himm 0x12120144 0x00000000
himm 0x12120148 0x00000000
######### media3 pri #######
himm 0x1212014c 0x00000000
######### hp_peri timeout ctrl #######
himm 0x12120068 0x81708032
himm 0x1212006c 0x00000000

###############################
## mddrc0 pri&timeout setting #
###############################
himm  0x12110200  0x00370000  # ports0 选择随路QOS模式
himm  0x12110210  0x00370000  # ports1
himm  0x12110220  0x00370000  # ports2 
himm  0x12110230  0x00370000  # ports3 
himm  0x12110240  0x00370000  # ports4 
himm  0x12110250  0x00370000  # ports5 
himm  0x12110260  0x00370000  # ports6 
himm  0x12110270  0x00370000  # ports7 
himm  0x12110280  0x00370000  # ports8 
##DDRC AXI pri ports0 - 6
############## WR pri ##############
himm  0x12110204  0x76543210  # ports0         
himm  0x12110214  0x76543210  # ports1         
himm  0x12110224  0x76543210  # ports2
himm  0x12110234  0x76543210  # ports3
himm  0x12110244  0x76543210  # ports4
himm  0x12110254  0x76543210  # ports5
himm  0x12110264  0x76543210  # ports6   
himm  0x12110274  0x76543210  # ports7
himm  0x12110284  0x76543210  # ports8
############## RD pri ##############
himm  0x12110208  0x76543210  # ports0         
himm  0x12110218  0x76543210  # ports1         
himm  0x12110228  0x76543210  # ports2
himm  0x12110238  0x76543210  # ports3
himm  0x12110248  0x76543210  # ports4
himm  0x12110258  0x76543210  # ports5
himm  0x12110268  0x76543210  # ports6
himm  0x12110278  0x76543210  # ports7
himm  0x12110288  0x76543210  # ports8
##############  qosbuf #############
himm  0x12114000  0x00000002   #qosb_push_ctrl
himm  0x1211410c  0x0000000a   #qosb_dmc_lvl
himm  0x12114110  0x0000000a   #qosb_dmc_lvl
himm  0x1211408c  0xb3032010   #qosb_wbuf_ctrl
himm  0x12114090  0xb3032010   #qosb_wbuf_ctrl
himm  0x121140f4  0x00000033   #row-hit enable
himm  0x121140ec  0x00000055   #row-hit 
himm  0x121140f0  0x00003333   #row-hit
himm  0x121141f4  0x00000000   #qosb_wbuf_pri_ctrl

himm  0x121141f0  0x00000001   #enable qosbuf timeout,through prilvl to remap timeout level
############## WR timeout ###########
himm  0x1211409c  0x0000000a  # wr_tout3 ~wr_tout0         
himm  0x121140a0  0x00000000  # wr_tout7 ~wr_tout4         
himm  0x121140a4  0x00000000  # wr_tout11~wr_tout8
himm  0x121140a8  0x00000000  # wr_tout15~wr_tout12

############## RD timeout ###########
himm  0x121140ac  0x0000000a  # rd_tout3 ~rd_tout0          
himm  0x121140b0  0x00000000  # rd_tout7 ~rd_tout4          
himm  0x121140b4  0x00000000  # rd_tout11~rd_tout8 
himm  0x121140b8  0x00000000  # rd_tout15~rd_tout12

