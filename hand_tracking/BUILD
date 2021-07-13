''' bazel-bin/hand_tracking/hand_tracking_cpu \
 --calculator_graph_config_file=/Users/mac/OpenCV/mediapipe/mediapipe/graphs/hand_tracking/hand_tracking_desktop_live.pbtxt \
 --input_video_path=/Users/mac/Movies/hand_tracking_demo.mov \
 --output_video_path=/Users/mac/OpenCV/video_output/2.mp4 '''


cc_binary(
    name = "hand_tracking_cpu",
    deps = [
        "//mediapipe/examples/desktop:demo_run_graph_main",
        "//mediapipe/graphs/hand_tracking:desktop_tflite_calculators",
    ],
)