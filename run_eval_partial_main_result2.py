#!/usr/bin/python
# 这个玩意有些端口转发的问题，在vscode上的termial上跑有点问题
import sys
import os
import random
import math
import subprocess
from optparse import OptionParser
import time
from datetime import datetime
from os import walk
import time
import shlex
import signal
encoding ="utf8"


def kill_procs():
        os.system("sudo killall -9 server"+"  ")
        os.system("sudo killall -9 client"+"  ")
        os.system("sudo killall -9 mm-link"+"  ")
        os.system("sudo killall -9 mm-delay"+"  ")
        time.sleep(2)


def handler(signum, frame):
        print("Time out")
        kill_procs()


signal.signal(signal.SIGALRM, handler)

def run_server_and_get_ip(tracefile, videoPath, args):

        interfaces_cmd = "ls /sys/class/net/"
        pid1 = subprocess.Popen(shlex.split(interfaces_cmd), stdout=subprocess.PIPE, shell=False)
        interfaces_before_mahimahi, err1 = pid1.communicate()

        #mahimahi_preq_cmd = "sudo sysctl -w net.ipv4.ip_forward=1"
        #pid2 = subprocess.Popen(shlex.split(mahimahi_preq_cmd), stdout=subprocess.PIPE, shell=False)
        #out2, err2 = pid2.communicate()

        #This will create mahimahi with http-server @ port 7717 inside mahimahi shell"
        mahimahi_cmd = "mm-link "+tracefile+" "+tracefile+" sudo ./system/build/server " + videoPath + args
        pid3 = subprocess.Popen([mahimahi_cmd],shell=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)

        #wait for a second to update the interfaces (adding mahimahi interface)
        time.sleep(1)
        pid4 = subprocess.Popen(shlex.split(interfaces_cmd), stdout=subprocess.PIPE, shell=False)
        interfaces_after_mahimahi, err4 = pid4.communicate()

        #Figuring out what is the mahimahi interface name.
        interfaces_before_mahimahi=interfaces_before_mahimahi.split()
        interfaces_after_mahimahi=interfaces_after_mahimahi.split()
        mahimahi_interface=""
        for interface in interfaces_after_mahimahi:
                if interface not in interfaces_before_mahimahi:
                        mahimahi_interface=interface
                        break

        ip_cmd="ip address show "
        try:
                ip_cmd+=mahimahi_interface.decode(encoding)
        except:
                ip_cmd+=mahimahi_interface

        pid5 = subprocess.Popen(shlex.split(ip_cmd), stdout=subprocess.PIPE, shell=False)
        ipout, err5 = pid5.communicate()
        ip=""
        for x in ipout.split(b'\n'):
                if b'peer' in x:
                        ip=x.split()[3].split(b'/')[0]

        try:
                return str(ip.decode())
        except:
                return str(ip)

def main():

    user_trace_dir = "/home/netlab/main/Dragonfly/traces_system/"
    video_dir = "/home/netlab/main/Dragonfly/Videos/"
    bw_trace_dir ="/home/netlab/main/Dragonfly/mmlink_traces_raw_to_use/"
    tile_size ="sizes.txt"
    quality_name = "psnr_avgs.txt"

    args_client = { "journal":" -model=Journal -predLR=1 -bufferModel=journalRebuffer -JournalCoraseABR=true",
                                    "flare_rebuffer_3sec":" -model=Flare -predLR=1 -bufferModel=rebuffer -predictionWindow=3 ",
                                    "pano_rebuffer_3sec":" -model=Pano -predLR=1 -bufferModel=rebuffer -predictionWindow=3 ",
                                    "utility_360_background":" -model=Utility -predLR=1 -bufferModel=UtilityJskip -UtilityCoraseBackgroundStream=coarse ",
                                    }

    args_server = { "journal":" 0 ",
                                    "flare_rebuffer_3sec":" 0 ",
                                    "pano_rebuffer_3sec":" 0 ",
                                    "utility_360_background":" 1 ",
                            }

    models = ["pano_rebuffer_3sec","flare_rebuffer_3sec","utility_360_background","journal"]
    videos =  ["v1_data","v8_data","v28_data"]
    bw_traces = ["report_bus_0003_subtrace1","report_car_0004_subtrace2","report_foot_0004_subtrace1"]
    users = {
                    "v1_data":[3,9,14],
                    "v2_data":[2,6,8],
                    "v7_data":[2,7,15],
                    "v8_data":[4,8,12],
                    "v14_data":[6,9,15],
                    "v27_data":[1,7,11],
                    "v28_data":[2,3,14]
                    }

    iteration_num = 0
    iteration_id = 0
    total_num_of_exp = len(videos) * len(bw_traces) * len(users["v1_data"] * len(models))
    os.system("sudo mkdir main-partial-results  ")
    for video in videos:
            os.system("sudo mkdir main-partial-results/"+video+"  ")
            videoId = int(video.split("_")[0].replace("v",""))
            videoPath = video_dir + video
            videoPsnr = videoPath+"/" + quality_name
            vidoePsnrChunk = videoPath+"/psnr_chunk.txt"
            videoSizes = videoPath+"/"  + tile_size
            displacementTrainingPath = video_dir+"various_displacements/v"+str(videoId)+"_displacement_across_users_p100_all.txt"
            baseLayerSize = videoPath+"/QP00/sizes_b.txt "
            for bw_trace in bw_traces:
                    bw_trace_path = bw_trace_dir+bw_trace+".txt"
                    for userId in users[video]:
                        for model in models:
                            iteration_id += 1
                            if iteration_id - 1  < iteration_num:
                                    continue
                            print("\x1b[1;41;96m==> Running exp#:\x1b[0m\x1b[1;103;91m",iteration_num + 1,"\x1b[0m\x1b[1;103;32m/",total_num_of_exp,"\x1b[0m", end='\r')
                            sys.stdout.flush()

                            user_frame_vp_path = user_trace_dir+ "vid"+str(videoId)+"_uid"+ str(userId)+"_vp_corr_per_frame.txt"
                            user_frame_tiles_path = user_trace_dir+ "vid"+str(videoId)+"_uid"+ str(userId)+"_tiles_per_frame_user.txt"

                            kill_procs()
                            out_dir = "main-partial-results/"+video+"/"+str(userId)+"_"+model+"_"+str(bw_trace)
                            os.system("sudo rm -r "+out_dir+"  ")
                            os.system("sudo mkdir "+out_dir+"  ")
                            server_ip = run_server_and_get_ip(bw_trace_path,videoPath,args_server[model])
                            time.sleep(1)

                            client_cmd = "sudo ./system/build/client "+user_frame_tiles_path+" "+user_frame_vp_path+" "+videoSizes+" "+videoPsnr+" "
                            client_cmd += displacementTrainingPath+" "+server_ip+" "


                            if "pano" in model:
                                    client_cmd += " /home/netlab/main/Dragonfly/Videos/Pano_tiles_grouping/v"+str(videoId)+"_grouping.txt" +" /home/netlab/main/Dragonfly/Videos/videos_bitrates/v"+str(videoId)+"_bitrates.txt "
                            elif "journal" in model:
                                    client_cmd += baseLayerSize
                            elif "utility_360_background" in model:
                                    client_cmd += baseLayerSize +" "+vidoePsnrChunk

                            client_cmd += args_client[model]
                            print("#####")
                            print( client_cmd )
                            print("####")
                            pid = subprocess.Popen(client_cmd, shell=True,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
                            try:
                                    signal.alarm(15*60)
                                    out,err = pid.communicate()
                            except :
                                    print("TIME out")
                            os.system("sudo mv client* "+out_dir+"  ")
                            os.system("sudo mv play* "+out_dir+"  ")
                            os.system("sudo mv pred* "+out_dir+"  ")
                            os.system("sudo mv recv* "+out_dir+"  ")
                            os.system("sudo mv server* "+out_dir+"  ")
                            os.system("sudo rm -r yuv* "+"  ")
                            os.system("sudo rm /home/dfly/core*  ")
                            kill_procs()
                            time.sleep(5)
                            #os.system("sudo sed \"s/iteration_num = "+str(iteration_num)+"/iteration_num = "+str(iteration_num+1)+"/g\" -i run_eval_partial_main_results.py"+"  ")
                            iteration_num += 1
    print("\n          \x1b[1;102;91m    vvvv    \x1b[0m\n          \x1b[1;102;91m>>>>\x1b[0m\x1b[1;102;34mDONE\x1b[0m\x1b[1;102;91m<<<<\x1b[0m\n          \x1b[1;102;91m    ^^^^    \x1b[0m")



if __name__ == "__main__":
        main()
