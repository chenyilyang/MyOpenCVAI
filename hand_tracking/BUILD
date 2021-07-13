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
    name = "hand_tracking_cpu",
    deps = [
        "//mediapipe/examples/desktop:demo_run_graph_main",
        "//mediapipe/graphs/hand_tracking:desktop_tflite_calculators",
    ],
)