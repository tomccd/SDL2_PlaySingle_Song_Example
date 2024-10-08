//Using C Header to import C Library
extern "C"{
    //Ubuntu : sudo apt-get install libavformat-dev libavcodec-dev libavutil-dev (ffmpeg)
    #include <libavformat/avformat.h>
    #include <libavutil/opt.h>
}
//Install SDL on Ubuntu: https://gist.github.com/aaangeletakis/3187339a99f7786c25075d4d9c80fad5
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

int64_t getMp3Duration(const char* filename) {
    //Reference Link: https://stackoverflow.com/questions/6451814/how-to-use-libavcodec-ffmpeg-to-find-duration-of-video-file
    AVFormatContext* pFormatCtx = avformat_alloc_context();
    avformat_open_input(&pFormatCtx, filename, NULL, NULL);
    avformat_find_stream_info(pFormatCtx,NULL);
    int64_t duration = pFormatCtx->duration; // uSecond
    // etc
    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);
    return duration/AV_TIME_BASE; //seconds
}
std::string getMp3File(std::string dir){
    if(fs::is_directory(dir)){
        for(auto &file: fs::directory_iterator(dir)){
            // std::cout << file.path() << std::endl;
            if(file.path().extension() == ".mp3"){
                return file.path().string();
            }
        }
    }
    return "";
}
int main(int argc, char *argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    //Find
    std::string filename = getMp3File("/home/tomccd/Downloads");
    std::cout << filename << std::endl;
    // Load music
    Mix_Music *music = Mix_LoadMUS(filename.c_str());
    if (!music) {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        Mix_Quit();
        SDL_Quit();
        return -1;
    }
    int counterLoop = 0;
    int loop = 2;
    // Play music
    if (Mix_PlayMusic(music, loop) == 2) { // Loop indefinitely
        printf("SDL_mixer Error: %s\n", Mix_GetError());
    }
    // Get Duration of Song in seconds
    Uint32 duration = getMp3Duration(filename.c_str());
    std::cout << "Duration: " << duration << " s" << std::endl;
    // get time in milisecond since SDL2 was initialized (begin time)
    Uint32 begin_time = SDL_GetTicks(); 
    // SDL_Delay(getMp3Duration(filename)*1000); // Wait until finish song
    //get time in milisecond when finish song (finish time)
    Uint32 finish_time = SDL_GetTicks();
    Uint32 previous_time = 0;
    //Keep playing song until it finishes
    while(counterLoop < loop){
        finish_time = SDL_GetTicks();
        if((finish_time-begin_time)/1000 > duration){
            std::cout << "Loop " << counterLoop+0 << std::endl;
            begin_time = finish_time;
            counterLoop++;
        }
        if(finish_time-previous_time>1000){
            std::cout << "Seconds: " << (finish_time-begin_time)/1000 << " s" << std::endl;
            previous_time = finish_time;
        }
    }
    std::cout << "Finish\n";
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}