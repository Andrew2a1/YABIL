#!/usr/bin/python

import os
import subprocess
import multiprocessing
from argparse import ArgumentParser

from github_tools import GitHubLogger


def build(build_type, preset_name):
    full_preset_name = f"{preset_name}_{build_type.lower()}"
    preset_dir = f"build/{full_preset_name}"
    source_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

    os.makedirs(preset_dir, exist_ok=True)
    build_type = "Debug" if build_type.lower() == "dev" else build_type
    build_type = "Release" if build_type.lower() == "prof" else build_type

    conan_install(source_dir, preset_name, build_type, preset_dir)
    cmake_config(full_preset_name)
    cmake_build(preset_dir, build_type)


def conan_install(source_dir, preset_name, build_type, output_dir):
    GitHubLogger.print("::group::Conan install")
    os.makedirs(output_dir, exist_ok=True)

    preset_file = f"{source_dir}/conan/profiles/{preset_name}"
    if not os.path.exists(preset_file):
        preset_file = "default"

    profile_host = f"-pr:h={preset_file}"
    config = ""
    if preset_name == "android":
        profile_host = f"-pr:h={preset_file}"
        preset_file = "default"
        config = f"-c tools.android:ndk_path={os.environ['NDK']}"

    subprocess.check_call(
        f"conan install {source_dir}/conan -pr:b={preset_file} {profile_host} {config} "
        f"-s build_type={build_type} --build=missing -of=.",
        shell=True,
        cwd=output_dir,
    )
    GitHubLogger.print("::endgroup::")


def cmake_config(full_preset_name):
    GitHubLogger.print("::group::CMake config")
    subprocess.check_call(
        f"cmake --preset {full_preset_name}",
        shell=True,
    )
    GitHubLogger.print("::endgroup::")


def cmake_build(binary_dir, build_type):
    GitHubLogger.print("::group::CMake build")
    subprocess.check_call(
        f"cmake --build {binary_dir} -j {multiprocessing.cpu_count()} --config {build_type}",
        shell=True,
    )
    GitHubLogger.print("::endgroup::")


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument(
        "-p",
        "--preset",
        required=True,
        choices=["vs", "clang", "gcc", "apple_clang", "android"],
        help="CMake preset name to use",
    )
    parser.add_argument(
        "-b",
        "--build_type",
        default="Release",
        choices=["Release", "Debug", "Dev", "Prof"],
        help="CMake build type",
    )
    args = parser.parse_args()
    build(args.build_type, args.preset)
