"""
`tcframe grade`: grade one solution against the test cases from a previous
`tcframe build`, using the compiled spec's `grade` command.
"""

import subprocess
from pathlib import Path
from typing import Mapping, Optional, Sequence

from tcframe_cli.build import DEFAULT_BUILD_DIR, DEFAULT_SOLUTION, SPEC_BINARY, SPEC_YML, TC_DIR, BuildError, anchor_program, default_scorer, resolve_helper, solution_path_for
from tcframe_cli.languages import available_languages
from tcframe_cli.multifile import MultifileError, is_multifile, multifile_args
from tcframe_cli.process import check_command


def grade_command(
    binary: Path,
    tc_dir: Path,
    solution: Optional[str],
    scorer: Optional[str] = None,
    communicator: Optional[str] = None,
    brief: bool = False,
    time_limit: Optional[int] = None,
    no_time_limit: bool = False,
    memory_limit: Optional[int] = None,
    no_memory_limit: bool = False,
    output_format: Optional[str] = None,
    extra_args: Sequence[str] = (),
) -> list[str]:
    """argv for the compiled spec's `grade` command. Program paths must already be resolvable from the cwd used to run it."""
    argv = [str(binary.resolve()), 'grade', f'--output={tc_dir.resolve()}']
    if solution is not None:
        argv.append(f'--solution={solution}')
    if scorer is not None:
        argv.append(f'--scorer={scorer}')
    if communicator is not None:
        argv.append(f'--communicator={communicator}')
    if brief:
        argv.append('--brief')
    if time_limit is not None:
        argv.append(f'--time-limit={time_limit}')
    if no_time_limit:
        argv.append('--no-time-limit')
    if memory_limit is not None:
        argv.append(f'--memory-limit={memory_limit}')
    if no_memory_limit:
        argv.append('--no-memory-limit')
    if output_format is not None:
        argv.append(f'--format={output_format}')
    argv.extend(extra_args)
    return argv


def grade_package(
    package_dir: Path,
    build_dir: Optional[Path] = None,
    solution: str = DEFAULT_SOLUTION,
    scorer: Optional[str] = None,
    communicator: Optional[str] = None,
    brief: bool = False,
    time_limit: Optional[int] = None,
    no_time_limit: bool = False,
    memory_limit: Optional[int] = None,
    no_memory_limit: bool = False,
    env: Optional[Mapping[str, str]] = None,
) -> int:
    """Grade `solution` in `package_dir`. Returns the runner's exit code."""
    build_dir = build_dir if build_dir is not None else package_dir / DEFAULT_BUILD_DIR
    binary = build_dir / SPEC_BINARY
    if not binary.is_file():
        raise BuildError(f"no compiled spec at '{binary}'; run 'tcframe build' first")
    if not (build_dir / TC_DIR).is_dir():
        raise BuildError(f"no test cases at '{build_dir / TC_DIR}'; run 'tcframe build' first")

    spec_yml = build_dir / SPEC_YML
    extra_args: list[str] = []
    solution_arg: Optional[str] = anchor_program(package_dir, solution)
    solution_dir = solution_path_for(package_dir, solution)
    if is_multifile_safe(solution_dir, spec_yml, env):
        try:
            extra_args = multifile_args(solution_dir, package_dir, spec_yml,
                                        available_languages(package_dir, env), env)
        except MultifileError as exc:
            raise BuildError(str(exc))
        solution_arg = None
    # Output-only problems grade a directory of submitted outputs, not a program to run.
    elif not solution_dir.is_dir():
        problem = check_command(solution_arg, 'solution')
        if problem is not None:
            raise BuildError(f'solution unavailable: {problem}')

    argv = grade_command(
        binary, build_dir / TC_DIR, solution_arg,
        scorer=resolve_helper(package_dir, build_dir, scorer or default_scorer(package_dir), 'scorer', env),
        communicator=resolve_helper(package_dir, build_dir, communicator, 'communicator', env),
        brief=brief, time_limit=time_limit, no_time_limit=no_time_limit,
        memory_limit=memory_limit, no_memory_limit=no_memory_limit,
        extra_args=extra_args,
    )
    return subprocess.run(argv, cwd=package_dir).returncode


def is_multifile_safe(solution_dir: Path, spec_yml: Path, env: Optional[Mapping[str, str]]) -> bool:
    """
    is_multifile, but a spec.yml without evaluator info (or an unknown evaluator) means
    "not multi-file", so the caller keeps the 1.x / output-only behaviour.
    """
    try:
        return is_multifile(solution_dir, spec_yml, env)
    except MultifileError:
        return False
