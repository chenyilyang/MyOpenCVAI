
#include "mediapipe/framework/calculator_graph.h"
#include "mediapipe/framework/port/logging.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include <iostream>

ABSL_FLAG(std::string, calculator_graph_config_file, "",
    "Name of file containing text format CalculatorGrapyConfig proto.");
namespace mediapipe {
    absl::Status PrintHelloMediaPipe() {
        //Configures a simple graph, which concatenates 2 PassThroughCalculators
        // CalculatorGraphConfig config =
        //     ParseTextProtoOrDie<CalculatorGraphConfig>(
        //         R"pb(
        //             input_stream: "in"
        //             output_stream: "out"
        //             node {
        //                 calculator: "PassThroughCalculator"
        //                 input_stream: "in"
        //                 output_stream: "out1"
        //             }
        //             node {
        //                 calculator: "PassThroughCalculator"
        //                 input_stream: "out1"
        //                 output_stream: "out"
        //             }
        //         )pb");
        std::string calculator_graph_config_contents;
        MP_RETURN_IF_ERROR(mediapipe::file::GetContents(
            absl::GetFlag(FLAGS_calculator_graph_config_file),
            &calculator_graph_config_contents
        ));
        LOG(INFO) << "Get calculator graph config contents: "
            << calculator_graph_config_contents;
        CalculatorGraphConfig config =
            ParseTextProtoOrDie<CalculatorGraphConfig>(calculator_graph_config_contents);
        CalculatorGraph graph;
        MP_RETURN_IF_ERROR(graph.Initialize(config));
        ASSIGN_OR_RETURN(OutputStreamPoller poller, graph.AddOutputStreamPoller("out1"));
        MP_RETURN_IF_ERROR(graph.StartRun({}));
        //Give 10 input packets that contains the same std::string "Hello mediapipe"
        for (size_t i = 0; i < 10; i++) {
            MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
                "in", MakePacket<std::string>(std::string("Hello mediapipe")).At(Timestamp(i))
            ));
        }
        //Close the input stream "in"
        MP_RETURN_IF_ERROR(graph.CloseInputStream("in"));
        mediapipe::Packet packet;
        //Get the output packets std::string
        while(poller.Next(&packet)) {
            LOG(INFO) << packet.Get<std::string>();
            // std::cout << packet.Get<std::string>() << std::endl;
        }
        absl::Status status = graph.WaitUntilDone();
        return status;
    }
}
int main(int argc, char** argv) {
    google::InitGoogleLogging(argv[0]);
    std::cout << argv[1] << std::endl;
    absl::ParseCommandLine(argc, argv);
    CHECK(mediapipe::PrintHelloMediaPipe().ok());
    return 0;
}