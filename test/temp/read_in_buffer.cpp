/*
这个文件用来测试ffmpeg的内存读取模式
*/
#include <iostream>
extern "C" {
    #include <stdio.h>
    #include <libavformat/avio.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}
#include <string>

struct BufferData{
    uint8_t* ptr;
    size_t size;
};

const int BUFF_SIZE = 1024 * 4;
/* opaque 指向用户数据， buffer是缓冲区
   我们将用户数据写入缓冲区中交给ffmpeg处理
*/
int read_packet(void *opaque, uint8_t *buffer, int bufferSize) {
  BufferData *br = (BufferData *)opaque;
  bufferSize = FFMIN(bufferSize, br->size);
  memcpy(buffer, br->ptr, bufferSize);
  br->ptr += bufferSize;
  br->size -= bufferSize;
  return bufferSize;
}


class AVIO{
    public:
        // decoder
        AVFormatContext* de_formatc;
        AVCodecContext* de_codecc;
        AVIOContext* de_ioc;
        BufferData user_data;
        uint8_t* buffer;

        AVIO(const std::string& input_pth, const std::string& output_pth);
        AVFormatContext* create_de_formatc(const std::string& input_pth );
        uint8_t* read_file_buffer(const std::string& input_pth, size_t& length);
};

inline AVIO:: AVIO(const std::string& input_pth, const std::string& output_pth) {
    
    this->user_data.ptr = read_file_buffer( input_pth, this->user_data.size );

}

inline AVFormatContext* AVIO:: create_de_formatc(const std::string& input_pth ) {

    AVFormatContext* avfc = avformat_alloc_context();
    if ( !avfc ) {
        throw std::runtime_error( "failed to alloc memory for avformat" );
    }
    
    this->buffer = (uint8_t*)av_malloc( BUFF_SIZE );
    if ( !this->buffer ){
        throw std::runtime_error( "failed to alloc memory to buffer" );
    }

    this->de_ioc = avio_alloc_context( this->buffer, BUFF_SIZE, 0, &(this->user_data), &read_packet, NULL, NULL); 
    if ( !this->de_ioc ){
        throw std::runtime_error( "failed to get the avio_alloc_context" );
    } 
}

// 将文件读取到内存中
inline uint8_t*  AVIO:: read_file_buffer(const std::string& input_pth, size_t& length) {
    FILE* pfile;
    uint8_t* data;
    pfile = fopen(input_pth.c_str(), "rb");
    if (pfile == NULL)
        return NULL;
    fseek(pfile, 0, SEEK_END);
    length = ftell(pfile);
    data = (uint8_t *)malloc(length * sizeof(uint8_t));
    rewind(pfile);  // fseek(pfile, 0, SEEK_SET) 设置指针位置到开头
    length = fread(data, 1, length, pfile);
    fclose(pfile);
    return data;
}

int main() {

    return 0;
}