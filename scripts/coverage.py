import os
import glob
import subprocess

from argparse import ArgumentParser
from github_tools import GitHubLogger


def generate_coverage_report(llvm_profdata, llvm_cov, output_line_coverage_percent):
    source_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    build_type = "Debug"
    build_dir = f"{source_dir}/build/{build_type}"
    binary_dir = f"{build_dir}/bin"
    lib_dir = f"{build_dir}/lib"
    output_dir = f"{build_dir}/coverage_report"

    merge_raw_profiles(llvm_profdata, binary_dir)
    generate_html(
        llvm_cov,
        source_dir,
        binary_dir,
        lib_dir,
        output_dir,
        output_line_coverage_percent,
    )


def merge_raw_profiles(llvm_profdata, binary_dir):
    GitHubLogger.print("::group::Merge coverage data")

    raw_profiles = glob.glob(f"{binary_dir}/*.profraw")
    cmd = f"{llvm_profdata} merge {' '.join(raw_profiles)} -o coverage.profdata"
    GitHubLogger.print("Executing command:", cmd)

    subprocess.check_call(
        cmd,
        shell=True,
        cwd=binary_dir,
    )
    GitHubLogger.print("::endgroup::")


def generate_html(
    llvm_cov, src_dir, binary_dir, lib_dir, output_dir, output_line_coverage_percent
):
    GitHubLogger.print("::group::Generate coverage HTML report")
    library_files = glob.glob(f"{lib_dir}/*.a") + glob.glob(f"{lib_dir}/*.so")
    exec_files = list(filter(lambda f: f.find(".") == -1, glob.glob(f"{binary_dir}/*")))

    object_flags = ["-object " + file for file in (library_files + exec_files)]

    subprocess.check_call(
        f"{llvm_cov} show -format=html --ignore-filename-regex='.*_tests.cpp' -output-dir={output_dir} -instr-profile coverage.profdata {' '.join(object_flags)}",
        shell=True,
        cwd=binary_dir,
    )

    show_branch_summary = (
        "-show-branch-summary=false" if llvm_cov.endswith("14") else ""
    )

    process = subprocess.run(
        f"{llvm_cov} report -show-region-summary=false --ignore-filename-regex='.*_tests.cpp' -show-functions=false {show_branch_summary} "
        f"-instr-profile coverage.profdata {' '.join(object_flags)}",
        shell=True,
        cwd=binary_dir,
        capture_output=True,
        check=True,
    )

    print(process.stdout.decode())

    if output_line_coverage_percent is not None:
        with open(output_line_coverage_percent, "w") as out:
            stripped_line_percentage = (
                process.stdout.decode().splitlines()[-1].split()[-1][:-1]
            )
            out.write(stripped_line_percentage)

    GitHubLogger.print("::endgroup::")


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument(
        "-p",
        "--llvm_profdata",
        required=False,
        default="llvm-profdata",
        help="llvm-profdata executable path",
    )
    parser.add_argument(
        "-c",
        "--llvm_cov",
        required=False,
        default="llvm-cov",
        help="llvm-cov executable path",
    )
    parser.add_argument(
        "-o",
        "--output_line_coverage_percent",
        required=False,
        default=None,
        help="Output file for percentage coverage result",
    )

    args = parser.parse_args()
    generate_coverage_report(
        args.llvm_profdata, args.llvm_cov, args.output_line_coverage_percent
    )
    print("Done")
