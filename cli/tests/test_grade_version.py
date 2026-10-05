import shutil
import subprocess
from pathlib import Path

import pytest

from tcframe_cli import __version__
from tcframe_cli.build import BuildError, build_package
from tcframe_cli.cli import main
from tcframe_cli.grade import grade_package
from tcframe_cli.version import version_string

REPO_ROOT = Path(__file__).resolve().parents[2]
needs_gxx = pytest.mark.skipif(shutil.which('g++') is None, reason='g++ not available')
SOLUTION_SH = '#!/bin/sh\nread a b\necho $((a + b))\n'
WRONG_SH = '#!/bin/sh\nread a b\necho 0\n'


def write_exec(path: Path, body: str) -> None:
    path.write_text(body)
    path.chmod(0o755)


def make_problem(root: Path) -> Path:
    pkg = root / 'aplusb'
    pkg.mkdir()
    (pkg / 'spec.cpp').write_text((REPO_ROOT / 'templates' / 'batch.cpp').read_text())
    write_exec(pkg / 'solution', SOLUTION_SH)
    return pkg


def test_grade_requires_prior_build(tmp_path):
    pkg = make_problem(tmp_path)
    with pytest.raises(BuildError, match="run 'tcframe build' first"):
        grade_package(pkg)


@needs_gxx
def test_grade_accepts_and_rejects_solutions(tmp_path, capfd):
    pkg = make_problem(tmp_path)
    build_package(pkg, env={'TCFRAME_HOME': str(REPO_ROOT)})
    capfd.readouterr()  # drop the build's generation log
    write_exec(pkg / 'wrong', WRONG_SH)

    rc_ok = grade_package(pkg, solution='./solution', brief=True)
    ok_out = capfd.readouterr().out
    rc_bad = grade_package(pkg, solution='./wrong', brief=True)
    bad_out = capfd.readouterr().out

    assert rc_ok == 0 and rc_bad == 0
    assert ok_out == 'AC 100\n'
    assert bad_out == 'WA 0\n'


@needs_gxx
def test_grade_output_only_submission_directory(tmp_path, capfd):
    pkg = make_problem(tmp_path)
    spec = (pkg / 'spec.cpp').read_text()
    (pkg / 'spec.cpp').write_text(spec.replace(
        '    void Constraints() {',
        '    void StyleConfig() {\n        OutputOnlyEvaluator();\n    }\n\n    void Constraints() {',
        1))
    build_package(pkg, env={'TCFRAME_HOME': str(REPO_ROOT)})
    capfd.readouterr()  # drop the build's generation log
    submissions = pkg / 'submissions'
    submissions.mkdir()
    for out in (pkg / 'build' / 'tc').glob('*.out'):
        shutil.copy(out, submissions / out.name)

    rc = grade_package(pkg, solution='submissions', brief=True)

    assert rc == 0
    assert capfd.readouterr().out == 'AC 100\n'


@needs_gxx
def test_grade_missing_solution_reported(tmp_path):
    pkg = make_problem(tmp_path)
    build_package(pkg, env={'TCFRAME_HOME': str(REPO_ROOT)})

    with pytest.raises(BuildError, match='solution unavailable'):
        grade_package(pkg, solution='./nope')


def test_version_falls_back_outside_git(tmp_path):
    assert version_string(tmp_path) == __version__


def test_version_from_git_tag(tmp_path):
    if shutil.which('git') is None:
        pytest.skip('git not available')
    run = lambda *a: subprocess.run(['git', *a], cwd=tmp_path, check=True, capture_output=True)
    run('init', '-q')
    run('-c', 'user.name=t', '-c', 'user.email=t@t', 'commit', '-q', '--allow-empty', '-m', 'a')
    run('tag', 'v1.2.3')
    assert version_string(tmp_path) == '1.2.3'
    run('-c', 'user.name=t', '-c', 'user.email=t@t', 'commit', '-q', '--allow-empty', '-m', 'b')
    assert version_string(tmp_path).startswith('1.2.3-1-g')
    (tmp_path / 'x').write_text('x')
    assert version_string(tmp_path).endswith('-dirty')


def test_version_command(capsys):
    assert main(['version']) == 0
    assert capsys.readouterr().out.strip()
