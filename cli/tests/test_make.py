import shutil
import subprocess
from pathlib import Path

import pytest

from tcframe_cli.build import SPEC_YML, BuildError, compile_spec, emit_spec_yml
from tcframe_cli.make import make_package

REPO_ROOT = Path(__file__).resolve().parents[2]
needs_gxx = pytest.mark.skipif(shutil.which('g++') is None, reason='g++ not available')
SOLUTION_SH = '#!/bin/sh\nread a b\necho $((a + b))\n'


def make_problem(root: Path, name: str, template: str) -> Path:
    pkg = root / name
    pkg.mkdir(parents=True)
    (pkg / 'spec.cpp').write_text((REPO_ROOT / 'templates' / f'{template}.cpp').read_text())
    sol = pkg / 'solution'
    sol.write_text(SOLUTION_SH)
    sol.chmod(0o755)
    return pkg


@needs_gxx
def test_make_writes_spec_yml_and_tc(tmp_path):
    pkg = make_problem(tmp_path, 'aplusb', 'batch-subtasks')

    build_dir = make_package(pkg, env={'TCFRAME_HOME': str(REPO_ROOT)})

    assert build_dir == pkg / 'build'
    spec_yml = (build_dir / SPEC_YML).read_text()
    assert spec_yml.startswith('slug: aplusb\n')
    assert '  - points: 40\n  - points: 60\n' in spec_yml
    assert 'slug: batch' in spec_yml
    assert (build_dir / 'tc' / 'aplusb_1_1.in').is_file()
    assert (build_dir / 'tc' / 'aplusb_2_1.in').is_file()
    assert (build_dir / 'validator').is_file()


@needs_gxx
def test_make_validator_accepts_every_generated_input(tmp_path):
    pkg = make_problem(tmp_path, 'aplusb', 'batch-subtasks')
    build_dir = make_package(pkg, env={'TCFRAME_HOME': str(REPO_ROOT)})

    inputs = sorted((build_dir / 'tc').glob('*.in'))
    assert inputs
    for tc_input in inputs:
        with tc_input.open('rb') as stdin:
            result = subprocess.run([str(build_dir / 'validator'), 'validate'], stdin=stdin, capture_output=True, text=True)
        assert result.returncode == 0, tc_input.name
        assert result.stdout.split()[0] != '0', tc_input.name


@needs_gxx
def test_make_without_solution_fails_before_generation(tmp_path):
    pkg = make_problem(tmp_path, 'aplusb', 'batch')
    (pkg / 'solution').unlink()

    with pytest.raises(BuildError, match='reference solution unavailable'):
        make_package(pkg, env={'TCFRAME_HOME': str(REPO_ROOT)})

    assert not (pkg / 'build' / 'tc').exists()


@needs_gxx
def test_emit_spec_reports_interactive_evaluator(tmp_path):
    # Interactive packages need a communicator to generate, so check only the
    # spec.yml step: compile, then emit without running generation.
    pkg = make_problem(tmp_path, 'inter', 'interactive')
    build = pkg / 'build'
    binary = compile_spec(pkg, build, env={'TCFRAME_HOME': str(REPO_ROOT)})

    emit_spec_yml(binary, pkg, build / SPEC_YML)

    spec_yml = (build / SPEC_YML).read_text()
    assert 'slug: interactive' in spec_yml
    assert '  slug: batch' not in spec_yml


@needs_gxx
def test_make_compiles_source_reference_solution(tmp_path):
    # SPEC.md T7.3: a judge builds from the source package, whose reference is source only.
    pkg = make_problem(tmp_path, 'aplusb', 'batch')
    (pkg / 'solution').unlink()
    ref = pkg / 'solutions' / 'ref'
    ref.mkdir(parents=True)
    (ref / 'ref.cpp').write_text('#include <cstdio>\nint main() { int a, b; scanf("%d %d", &a, &b); printf("%d\\n", a + b); }\n')

    build_dir = make_package(pkg, solution='solutions/ref/ref.cpp', env={'TCFRAME_HOME': str(REPO_ROOT)})

    outputs = sorted((build_dir / 'tc').glob('*.out'))
    assert outputs
    assert (build_dir / 'solutions' / 'ref' / 'ref').is_file()
