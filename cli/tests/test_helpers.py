import shlex
import shutil
import subprocess
from pathlib import Path

import pytest

from tcframe_cli.build import BuildError, default_scorer, resolve_helper
from tcframe_cli.checker import FAIL, PASS, run_tests

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


# A lenient scorer: accepts every output, so a wrong solution only passes with it.
SCORER_ALWAYS_AC = '#include <cstdio>\nint main() { puts("AC"); return 0; }\n'
WRONG = '#include <cstdio>\nint main(){ int a, b; scanf("%d %d", &a, &b); printf("%d\\n", a - b); }\n'
SUM = '#include <cstdio>\nint main(){ int a, b; scanf("%d %d", &a, &b); printf("%d\\n", a + b); }\n'
LANG = """\
name: C++17 (GCC)
family: cpp
extensions: [cc, cpp]
build: {gxx} -std=c++17 -o $BASE_FILENAME $FILENAME
run: ./$BASE_FILENAME
"""


def _write(path: Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)


def scorer_problem(tmp_path: Path, custom_scorer: bool) -> Path:
    pkg = tmp_path / 'aplusb'
    pkg.mkdir()
    spec = (REPO_ROOT / 'templates' / 'batch.cpp').read_text()
    if custom_scorer:
        spec = spec.replace('    void GradingConfig() {',
                            '    void StyleConfig() {\n        CustomScorer();\n    }\n\n    void GradingConfig() {', 1)
    _write(pkg / 'spec.cpp', spec)
    _write(pkg / 'languages' / 'cpp17.yml', LANG.format(gxx=shutil.which('g++') or 'g++'))
    _write(pkg / 'solutions' / 'ref' / 'ref.cpp', SUM)
    _write(pkg / 'solutions' / 'wa' / 'wrong.cpp', WRONG)
    _write(pkg / 'scorer.cpp', SCORER_ALWAYS_AC)
    return pkg


def test_default_scorer_looks_for_source_then_executable(tmp_path):
    assert default_scorer(tmp_path) is None
    (tmp_path / 'scorer').write_text('x')
    assert default_scorer(tmp_path) == './scorer'
    (tmp_path / 'scorer.cpp').write_text('x')
    assert default_scorer(tmp_path) == './scorer.cpp'


@needs_gxx
def test_package_scorer_is_used_when_custom_scorer_declared(tmp_path):
    pkg = scorer_problem(tmp_path, custom_scorer=True)

    checks = run_tests(pkg, env=ENV, home=tmp_path / 'nohome')
    by_label = {c.result.solution.label: c for c in checks}

    assert by_label['wa/wrong.cpp'].status == FAIL
    assert by_label['wa/wrong.cpp'].detail == 'got AC 100, expected WA'


@needs_gxx
def test_package_scorer_is_unused_without_custom_scorer(tmp_path):
    pkg = scorer_problem(tmp_path, custom_scorer=False)

    checks = run_tests(pkg, env=ENV, home=tmp_path / 'nohome')
    by_label = {c.result.solution.label: c for c in checks}

    assert by_label['wa/wrong.cpp'].status == PASS
