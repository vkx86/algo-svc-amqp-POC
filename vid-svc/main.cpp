#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>

#include "CommonDefs.h"
#include "VideoSvcSim.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initLog(const std::string & logFile)
{
    boost::log::add_common_attributes();
    boost::log::add_file_log(
            boost::log::keywords::file_name = logFile,
            boost::log::keywords::auto_flush = true,
            boost::log::keywords::format = "[%TimeStamp%] (%Severity%) %Message%",
            boost::log::keywords::rotation_size = 10 * 1024 * 1024,
            boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0)
    );

    boost::log::add_console_log(std::cout, boost::log::keywords::format = ">> %Message%");

//    boost::log::core::get()->set_filter(
//            boost::log::trivial::severity >= boost::log::trivial::info
//    );

}

int main(int argc, char *argv[]) {

    namespace po = boost::program_options;
    po::options_description desc("Options");
    desc.add_options()
            ("amqp-conn", po::value<std::string>()->default_value(AMQP_CONN_STR), "amqp connection string")
            ("log-path", po::value<std::string>()->default_value(""), "log file path");

    po::variables_map vm;
    po::store(po::parse_command_line(argc,argv,desc),vm);
    po::notify(vm);

    auto logFile = boost::any_cast<std::string>(vm["log-path"].value());
    if( !logFile.empty()){
        logFile += "VID_SVC_%N.log";
        initLog(logFile);
    }

    BOOST_LOG_TRIVIAL(info) << "VID_SVC startup...";

    auto amqp_conn_str = boost::any_cast<std::string>(vm["amqp-conn"].value());
    AmqpPub amqpPub(amqp_conn_str, {EXCH_VIDEO_IN});

    VideoSvcSim sim(amqpPub);

    AmqpSub amqpSub(amqp_conn_str, {{EXCH_VIDEO_OUT,ROUTE_VIDEO_OUT}}, "msg-sweeper-VID_SVC@75468adaef584f7aaf332d1b56a2d769", &sim);
    std::thread amqpSubThread(&AmqpSub::Start, std::ref(amqpSub));

    sim.Run();

    amqpSub.Stop();
    amqpSubThread.join();

    BOOST_LOG_TRIVIAL(info) << "VID_SVC shutdown...";
    return 0;
}
