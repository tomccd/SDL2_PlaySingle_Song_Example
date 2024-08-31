extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
}

#include <iostream>

int64_t getMp3Duration(const char* filename) {
    AVFormatContext* pFormatCtx = avformat_alloc_context();
    avformat_open_input(&pFormatCtx, filename, NULL, NULL);
    avformat_find_stream_info(pFormatCtx,NULL);
    int64_t duration = pFormatCtx->duration;
    // etc
    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);
    return duration/AV_TIME_BASE;
}

int main() {
    const char* filename = "./music.mp3";
    double duration = getMp3Duration(filename);
    std::cout << duration;
    if (duration >= 0) {
        std::cout << "Duration: " << duration << " seconds" << std::endl;
    } else {
        std::cerr << "Failed to get duration." << std::endl;
    }

    return 0;
}