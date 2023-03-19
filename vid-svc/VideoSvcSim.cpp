//
// Created by developer on 3/19/23.
//

#include "VideoSvcSim.h"
#include "CommonDefs.h"

#include <iostream>
#include <csignal>
#include <sys/random.h>

#include "Utils.h"

using namespace std;
using namespace std::chrono;

static volatile int keepRunning = 1;

void intHandler(__attribute__((unused)) int dummy) {
    cout << "\nInterrupting/terminating..." << endl;
    keepRunning = 0;
}

VideoSvcSim::VideoSvcSim(AmqpPub &amqpPub) : _amqpPub(amqpPub)
{
    signal(SIGINT, intHandler);
    signal(SIGTERM, intHandler);

    _timer_30Hz = _timer.add(microseconds(33333), [this](CppTime::timer_id) {
        this->OnTimer_30Hz();
    }, microseconds(33333));
}

VideoSvcSim::~VideoSvcSim() {
    _timer.remove(_timer_30Hz);
}

void VideoSvcSim::Run() {
    while (keepRunning) {
        usleep(25000);
    }
}

void VideoSvcSim::OnTimer_30Hz() {
    SendSimulatedEncodedFrame();
}

void VideoSvcSim::OnMsgReceived(const char *fromExchange, const char *fromRoute, const char *data, size_t dataSize, const char *correlationId,
                           uint64_t deliveryTag) {

    auto* pFrame = (VideoFrameMsg*)data;

    timespec currentTime{};
    clock_gettime(CLOCK_REALTIME, &currentTime);

    auto diff = Utils::timespec_diff(pFrame->SentTime, currentTime);

    BOOST_LOG_TRIVIAL(trace) << "FrameId: " << pFrame->FrameId << " diff sec: "
                             << diff.tv_sec
                             << " | nano-sec: " << diff.tv_nsec;

}

void VideoSvcSim::SendSimulatedEncodedFrame(){

    //const std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    auto ret = getrandom(_videoOutBuff, sizeof(_videoOutBuff), GRND_NONBLOCK);
    //const auto end = std::chrono::steady_clock::now();
    //BOOST_LOG_TRIVIAL(trace) << "Time spent (micro-sec): " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    if(ret == -1){
        BOOST_LOG_TRIVIAL(trace) << "Failed to get random bytes...";
    }

    auto* pFrame = (VideoFrameMsg*)_videoOutBuff;
    //const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();

    pFrame->FrameId = _frameId++;
    pFrame->FrameLength = sizeof(_videoOutBuff) - sizeof(VideoFrameMsg);

    clock_gettime(CLOCK_REALTIME, &pFrame->SentTime);

    _amqpPub.Publish(EXCH_VIDEO_IN, ROUTE_VIDEO_IN, _videoOutBuff, sizeof(_videoOutBuff));

}
