import os
import subprocess
import multiprocessing
from argparse import ArgumentParser


def build(build_type, preset_name):
    full_preset_name = f"{preset_name}_{build_type.lower()}"
    preset_dir = f"build/{full_preset_name}"
    source_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    os.makedirs(preset_dir, exist_ok=True)

    conan_install(source_dir, preset_name, build_type, preset_dir)
    cmake_config(full_preset_name)
    cmake_build(preset_dir, build_type)


def conan_install(source_dir, preset_name, build_type, output_dir):
    subprocess.check_call(
        f"conan install {source_dir}/conan -pr:b={source_dir}/conan/profiles/{preset_name} "
        f"-s build_type={build_type} --build=missing -of={output_dir}",
        shell=True,
    )


def cmake_config(full_preset_name):
    subprocess.check_call(
        f"cmake --preset {full_preset_name}",
        shell=True,
    )


def cmake_build(binary_dir, build_type):
    subprocess.check_call(
        f"cmake --build {binary_dir} -j {multiprocessing.cpu_count()} --config {build_type}",
        shell=True,
    )


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument(
        "-p",
        "--preset",
        required=True,
        choices=["vs", "clang", "gcc"],
        help="CMake preset name to use",
    )
    parser.add_argument(
        "-b",
        "--build_type",
        default="Release",
        choices=["Release", "Debug"],
        help="CMake build type",
    )
    args = parser.parse_args()
    build(args.build_type, args.preset)
