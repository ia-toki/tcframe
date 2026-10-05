import shlex
import shutil
import subprocess
from pathlib import Path

import pytest

from tcframe_cli.build import BuildError, resolve_helper

REPO_ROOT = Path(__file__).resolve().parents[2]
needs_gxx = pytest.mark.skipif(shutil.which('g++') is None, reason='g++ not available')
ENV = {'TCFRAME_HOME': str(REPO_ROOT)}
COMMUNICATOR_CPP = '#include <cstdio>\nint main(int argc, char** argv) { printf("ran %s\\n", argv[1]); return 0; }\n'


def test_non_source_command_is_only_anchored(tmp_path):
    assert resolve_helper(tmp_path, tmp_path / 'build', None, 'scorer', ENV) is None
    assert resolve_helper(tmp_path, tmp_path / 'build', 'my-scorer', 'scorer', ENV) == 'my-scorer'
    assert resolve_helper(tmp_path, tmp_path / 'build', './bin/scorer', 'scorer', ENV) == str(tmp_path / 'bin' / 'scorer')


@needs_gxx
def test_source_file_is_compiled_to_absolute_command(tmp_path):
    (tmp_path / 'communicator.cpp').write_text(COMMUNICATOR_CPP)
    build_dir = tmp_path / 'build'

    command = resolve_helper(tmp_path, build_dir, 'communicator.cpp', 'communicator', ENV)

    argv = shlex.split(command)
    assert Path(argv[0]).is_absolute()
    assert Path(argv[0]).is_file()
    assert Path(argv[0]).is_relative_to(build_dir / 'helpers')
    result = subprocess.run(argv + ['tc.in'], capture_output=True, text=True)
    assert result.stdout == 'ran tc.in\n'


@needs_gxx
def test_extra_words_after_source_are_kept_as_arguments(tmp_path):
    (tmp_path / 'communicator.cpp').write_text(COMMUNICATOR_CPP)

    command = resolve_helper(tmp_path, tmp_path / 'build', 'communicator.cpp --flag', 'communicator', ENV)

    assert shlex.split(command)[-1] == '--flag'


@needs_gxx
def test_compile_failure_is_a_build_error(tmp_path):
    (tmp_path / 'scorer.cpp').write_text('this is not C++\n')

    with pytest.raises(BuildError, match='scorer failed to compile'):
        resolve_helper(tmp_path, tmp_path / 'build', 'scorer.cpp', 'scorer', ENV)
