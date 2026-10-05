import shutil
from pathlib import Path

import pytest

from tcframe_cli.build import BuildError, build_package, compile_command, compile_spec

REPO_ROOT = Path(__file__).resolve().parents[2]
HAS_GXX = shutil.which('g++') is not None
needs_gxx = pytest.mark.skipif(not HAS_GXX, reason='g++ not available')


def make_package(root: Path, name: str, spec_text: str) -> Path:
    pkg = root / name
    pkg.mkdir(parents=True)
    (pkg / 'spec.cpp').write_text(spec_text)
    return pkg


def test_compile_command_matches_1x_line(tmp_path):
    argv = compile_command(Path('/p/spec.cpp'), Path('/p/build/spec'), Path('/tc'), ['-O2'])

    assert argv[:3] == ['g++', '-std=c++17', '-D__TCFRAME_SPEC_FILE__="/p/spec.cpp"']
    assert argv[argv.index('-I') + 1] == '/tc/include'
    assert '-O2' in argv
    assert argv[argv.index('-o') + 1] == '/p/build/spec'
    assert argv[-1] == '/tc/src/tcframe/runner.cpp'


def test_missing_spec_is_error(tmp_path):
    with pytest.raises(BuildError, match='does not exist'):
        compile_spec(tmp_path, tmp_path / 'build', env={'TCFRAME_HOME': str(REPO_ROOT)})


def test_unset_tcframe_home_is_error(tmp_path):
    pkg = make_package(tmp_path, 'p', '')
    with pytest.raises(BuildError, match='TCFRAME_HOME is not set'):
        compile_spec(pkg, pkg / 'build', env={})


def test_bad_tcframe_home_is_error(tmp_path):
    pkg = make_package(tmp_path, 'p', '')
    with pytest.raises(BuildError, match='must point at the tcframe install root'):
        compile_spec(pkg, pkg / 'build', env={'TCFRAME_HOME': str(tmp_path)})


SOLUTION_SH = '#!/bin/sh\nread a b\necho $((a + b))\n'


def write_solution(pkg: Path, body: str = SOLUTION_SH) -> None:
    sol = pkg / 'solution'
    sol.write_text(body)
    sol.chmod(0o755)


@needs_gxx
def test_build_generates_test_cases(tmp_path):
    pkg = make_package(tmp_path, 'aplusb', (REPO_ROOT / 'templates' / 'batch.cpp').read_text())
    write_solution(pkg)

    build_dir = build_package(pkg, env={'TCFRAME_HOME': str(REPO_ROOT)})

    assert build_dir == pkg / 'build'
    assert (build_dir / 'spec').is_file()
    assert (build_dir / 'tc' / 'aplusb_sample_1.in').is_file()
    assert (build_dir / 'tc' / 'aplusb_sample_1.out').is_file()
    assert (build_dir / 'tc' / 'aplusb_1.in').is_file()
    assert (build_dir / 'tc' / 'aplusb_1.out').read_text().strip() == str(
        sum(int(x) for x in (build_dir / 'tc' / 'aplusb_1.in').read_text().split()))
    assert not list(pkg.glob('__tcframe_*'))


@needs_gxx
def test_missing_solution_reported_before_generation(tmp_path):
    pkg = make_package(tmp_path, 'aplusb', (REPO_ROOT / 'templates' / 'batch.cpp').read_text())

    with pytest.raises(BuildError, match='reference solution unavailable'):
        build_package(pkg, env={'TCFRAME_HOME': str(REPO_ROOT)})

    assert (pkg / 'build' / 'spec').is_file()
    assert not (pkg / 'build' / 'tc').exists()


@needs_gxx
def test_explicit_solution_path_is_anchored_to_package(tmp_path, monkeypatch):
    pkg = make_package(tmp_path, 'aplusb', (REPO_ROOT / 'templates' / 'batch.cpp').read_text())
    (pkg / 'bin').mkdir()
    write_solution(pkg / 'bin', SOLUTION_SH)
    (pkg / 'bin' / 'solution').rename(pkg / 'bin' / 'ref')
    monkeypatch.chdir(tmp_path)

    build_dir = build_package(pkg, solution='./bin/ref', env={'TCFRAME_HOME': str(REPO_ROOT)})

    assert (build_dir / 'tc' / 'aplusb_1.out').is_file()


@needs_gxx
def test_compile_error_reports_compiler_output(tmp_path):
    pkg = make_package(tmp_path, 'broken', 'int main() { this is not c++ }\n')

    with pytest.raises(BuildError) as exc:
        build_package(pkg, env={'TCFRAME_HOME': str(REPO_ROOT)})

    assert 'compiling' in str(exc.value)
    assert 'failed' in str(exc.value)
