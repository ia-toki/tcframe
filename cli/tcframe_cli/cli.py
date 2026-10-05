import argparse
import sys
from pathlib import Path
from typing import Optional

from tcframe_cli.build import DEFAULT_SOLUTION, BuildError, build_package
from tcframe_cli.checker import CheckError, exit_code, format_report, run_tests
from tcframe_cli.grade import grade_package
from tcframe_cli.make import make_package
from tcframe_cli.package import package_package
from tcframe_cli.scaffold import DEFAULT_TEMPLATE, ScaffoldError, create_package
from tcframe_cli.version import version_string

USAGE = """usage: tcframe <command>

Available commands:
  new [--template=<t>]          Create spec.cpp in the current directory from a template
  build [options]               Compile spec.cpp and generate test cases
  make [options]                Build, leaving spec.yml next to tc/
  package [options]             Make, then zip source and distribution packages
  grade --solution=<cmd> [...]  Grade a solution against the built test cases
  test [--build-dir=<dir>]      Grade every solutions/ entry and check its expected verdict
  version                       Print tcframe version

build/make/package options: --build-dir=<dir> --solution=<cmd>
                            --scorer=<cmd> --communicator=<cmd>
"""


def _build_parser(name: str) -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(prog=f'tcframe {name}', add_help=False)
    parser.add_argument('--build-dir', default=None)
    parser.add_argument('--solution', default=DEFAULT_SOLUTION)
    parser.add_argument('--scorer', default=None)
    parser.add_argument('--communicator', default=None)
    return parser


def _parse(name: str, argv: list[str]):
    try:
        return _build_parser(name).parse_args(argv)
    except SystemExit:
        sys.stderr.write(f'usage: tcframe {name} [--build-dir=<dir>] [--solution=<cmd>] '
                         f'[--scorer=<cmd>] [--communicator=<cmd>]\n')
        return None


def _build_dir(parsed) -> Optional[Path]:
    return Path(parsed.build_dir) if parsed.build_dir is not None else None


def _cmd_new(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(prog='tcframe new', add_help=False)
    parser.add_argument('--template', default=DEFAULT_TEMPLATE)
    try:
        parsed = parser.parse_args(argv)
    except SystemExit:
        sys.stderr.write('usage: tcframe new [--template=<t>]\n')
        return 1

    try:
        target = create_package(parsed.template, Path.cwd())
    except ScaffoldError as exc:
        sys.stderr.write(f'tcframe: new error: {exc}\n')
        return 1

    print(f'created {target}')
    return 0


def _cmd_build(argv: list[str]) -> int:
    parsed = _parse('build', argv)
    if parsed is None:
        return 1
    try:
        out = build_package(Path.cwd(), _build_dir(parsed), solution=parsed.solution,
                            scorer=parsed.scorer, communicator=parsed.communicator)
    except BuildError as exc:
        sys.stderr.write(f'tcframe: build error: {exc}\n')
        return 1

    print(f'build output in {out}')
    return 0


def _cmd_make(argv: list[str]) -> int:
    parsed = _parse('make', argv)
    if parsed is None:
        return 1
    try:
        out = make_package(Path.cwd(), _build_dir(parsed), solution=parsed.solution)
    except BuildError as exc:
        sys.stderr.write(f'tcframe: make error: {exc}\n')
        return 1

    print(f'make output in {out}')
    return 0


def _cmd_package(argv: list[str]) -> int:
    parsed = _parse('package', argv)
    if parsed is None:
        return 1
    try:
        source_zip, dist_zip = package_package(Path.cwd(), _build_dir(parsed), solution=parsed.solution)
    except BuildError as exc:
        sys.stderr.write(f'tcframe: package error: {exc}\n')
        return 1

    print(f'source package: {source_zip}')
    print(f'distribution package: {dist_zip}')
    return 0


def _cmd_test(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(prog='tcframe test', add_help=False)
    parser.add_argument('--build-dir', default=None)
    parser.add_argument('--scorer', default=None)
    parser.add_argument('--communicator', default=None)
    try:
        parsed = parser.parse_args(argv)
    except SystemExit:
        sys.stderr.write('usage: tcframe test [--build-dir=<dir>] [--scorer=<cmd>] [--communicator=<cmd>]\n')
        return 1

    try:
        checks = run_tests(Path.cwd(), _build_dir(parsed), scorer=parsed.scorer,
                           communicator=parsed.communicator)
    except CheckError as exc:
        sys.stderr.write(f'tcframe: test error: {exc}\n')
        return 1

    print(format_report(checks))
    return exit_code(checks)


def _cmd_grade(argv: list[str]) -> int:
    parser = argparse.ArgumentParser(prog='tcframe grade', add_help=False)
    parser.add_argument('--build-dir', default=None)
    parser.add_argument('--solution', default=DEFAULT_SOLUTION)
    parser.add_argument('--scorer', default=None)
    parser.add_argument('--communicator', default=None)
    parser.add_argument('--brief', action='store_true')
    parser.add_argument('--time-limit', type=int, default=None)
    parser.add_argument('--no-time-limit', action='store_true')
    parser.add_argument('--memory-limit', type=int, default=None)
    parser.add_argument('--no-memory-limit', action='store_true')
    try:
        parsed = parser.parse_args(argv)
    except SystemExit:
        sys.stderr.write('usage: tcframe grade --solution=<cmd> [--scorer=<cmd>] [--communicator=<cmd>] '
                         '[--brief] [--time-limit=<s>] [--no-time-limit] [--memory-limit=<mb>] '
                         '[--no-memory-limit]\n')
        return 1

    try:
        return grade_package(
            Path.cwd(), _build_dir(parsed), solution=parsed.solution,
            scorer=parsed.scorer, communicator=parsed.communicator, brief=parsed.brief,
            time_limit=parsed.time_limit, no_time_limit=parsed.no_time_limit,
            memory_limit=parsed.memory_limit, no_memory_limit=parsed.no_memory_limit,
        )
    except BuildError as exc:
        sys.stderr.write(f'tcframe: grade error: {exc}\n')
        return 1


def main(argv: Optional[list[str]] = None) -> int:
    if argv is None:
        argv = sys.argv[1:]

    if not argv:
        sys.stderr.write(USAGE)
        return 1

    command, rest = argv[0], argv[1:]
    if command == 'version':
        print(version_string(Path.cwd()))
        return 0
    commands = {
        'new': _cmd_new,
        'build': _cmd_build,
        'make': _cmd_make,
        'package': _cmd_package,
        'grade': _cmd_grade,
        'test': _cmd_test,
    }
    if command in commands:
        return commands[command](rest)

    sys.stderr.write(USAGE)
    return 1


if __name__ == '__main__':
    sys.exit(main())
