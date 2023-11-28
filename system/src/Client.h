/*
 * Client.h
 *
 *  Created on: Apr 24, 2021
 *      Author: eghabash
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#include <string>

class Client {

public:
  Client(std::string tilesPerFrameTracePath,
         std::string vpCorrPerFrameTracePath, std::string tileChunkSizesPath,
         std::string tileChunksQaulityPath,
         std::string backgroundDisplacementPath, std::string serverIp,
         std::string panoTilesGroupsPath, std::string panoVideoBitrate,
         std::string fullVideoChunkSizePath,
         std::string fullVideoChunkPSNRPath);
  virtual ~Client();
};

#endif /* CLIENT_H_ */

/*
sudo ./system/build/client 
tilesPerFrameTracePath =  /home/netlab/main/Dragonfly/traces_system/vid1_uid3_tiles_per_frame_user.txt 
vpCorrPerFrameTracePath = /home/netlab/main/Dragonfly/traces_system/vid1_uid3_vp_corr_per_frame.txt 
tileChunkSizesPath = /home/netlab/main/Dragonfly/Videos/v1_data/sizes.txt 
tileChunksQaulityPath = /home/netlab/main/Dragonfly/Videos/v1_data/psnr_avgs.txt 
backgroundDisplacementPath = /home/netlab/main/Dragonfly/Videos/various_displacements/v1_displacement_across_users_p100_all.txt  
serverIp = 100.64.0.2
/home/netlab/main/Dragonfly/Videos/Pano_tiles_grouping/v1_grouping.txt 
/home/netlab/main/Dragonfly/Videos/videos_bitrates/v1_bitrates.txt  
-model=Pano -predLR=1 -bufferModel=rebuffer -predictionWindow=3 

*/