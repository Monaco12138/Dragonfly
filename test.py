import sys
import os
import shlex
# tracefile = '/home/netlab/main/Dragonfly/mmlink_traces_raw_to_use/report_bus_0003_subtrace1.txt'
# videoPath = '/home/netlab/main/Dragonfly/Videos/v1_data'
# args = ' 0 ' 
# mahimahi_cmd = "mm-link " +tracefile+ " " +tracefile+" sudo ./system/build/server " + videoPath + args 
# print( mahimahi_cmd )
# os.system( mahimahi_cmd )

interfaces_cmd = "ls /sys/class/net/"
x = shlex.split(interfaces_cmd)
print(x)