# export GLOG_logtostderr=1 # enable LOG output
'''
using video file and write to a video output or show the window of tracking results
bazel-bin/hand_tracking/hand_tracking_cpu \
 --calculator_graph_config_file=/Users/mac/OpenCV/mediapipe/mediapipe/graphs/hand_tracking/hand_tracking_desktop_live.pbtxt \
 --input_video_path=/Users/mac/Movies/hand_tracking_demo.mov \
 --output_video_path=/Users/mac/OpenCV/video_output/2.mp4
using camera(0)
bazel-bin/hand_tracking/hand_tracking_cpu \
 --calculator_graph_config_file=/Users/mac/OpenCV/mediapipe/mediapipe/graphs/hand_tracking/hand_tracking_desktop_live.pbtxt '''

cc_binary(
    name = "hand_tracking",
    srcs = ["hand_tracking.cc"],
    deps = [
        "//mediapipe/graphs/hand_tracking:desktop_tflite_calculators",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/formats:image_frame_opencv",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:opencv_highgui",
        "//mediapipe/framework/port:opencv_imgproc",
        "//mediapipe/framework/port:opencv_video",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)