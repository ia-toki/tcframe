import shlex
import shutil
import subprocess
import sys
from pathlib import Path

import pytest

from tcframe_cli.compiler import CompileError, compile_program, language_for
from tcframe_cli.languages import Language

GXX = shutil.which('g++')
needs_gxx = pytest.mark.skipif(GXX is None, reason='g++ not installed')


def make_lang(slug, exts, build='', run='./$BASE_FILENAME', family='x'):
    return Language(slug=slug, name=slug, family=family, extensions=tuple(exts),
                    build=build, run=run, source=Path(f'{slug}.yml'))


def cpp17(gxx=GXX or 'g++'):
    return make_lang('cpp17', ['cc', 'cpp'],
                     build=f'{gxx} -std=c++17 -o $BASE_FILENAME $FILENAME', family='cpp')


def python3():
    return make_lang('py', ['py'], build='', run=f'{shlex.quote(sys.executable)} $FILENAME')


def write_exec(path: Path, body: str) -> Path:
    path.write_text('#!/bin/sh\n' + body)
    path.chmod(0o755)
    return path


def run(compiled):
    return subprocess.run(compiled.argv, cwd=compiled.workdir,
                          capture_output=True, text=True)


@pytest.fixture
def dirs(tmp_path):
    pkg = tmp_path / 'problem'
    build = pkg / 'build'
    pkg.mkdir()
    return {'pkg': pkg, 'build': build}


# --- extension -> language -------------------------------------------------

def test_language_for_matches_extension_case_insensitive():
    langs = {'cpp17': make_lang('cpp17', ['cc', 'cpp'])}
    assert language_for(Path('a.CPP'), langs).slug == 'cpp17'
    assert language_for(Path('a.cc'), langs).slug == 'cpp17'


def test_language_for_unknown_or_missing_extension_is_none():
    langs = {'cpp17': make_lang('cpp17', ['cpp'])}
    assert language_for(Path('a.py'), langs) is None
    assert language_for(Path('Makefile'), langs) is None


def test_language_for_shared_extension_picks_first_slug():
    langs = {'cpp17': make_lang('cpp17', ['cpp']), 'cpp11': make_lang('cpp11', ['cpp'])}
    assert language_for(Path('a.cpp'), langs).slug == 'cpp11'


# --- single-file programs --------------------------------------------------

@needs_gxx
def test_compile_cpp_file_builds_into_build_dir(dirs):
    src = dirs['pkg'] / 'solution.cpp'
    src.write_text('#include <cstdio>\nint main(){ std::puts("hi"); }\n')

    compiled = compile_program(src, dirs['build'], {'cpp17': cpp17()})

    assert compiled.name == 'solution'
    assert compiled.workdir == dirs['build'] / 'solution'
    assert (compiled.workdir / 'solution').is_file()
    assert sorted(p.name for p in dirs['pkg'].iterdir()) == ['build', 'solution.cpp']
    assert run(compiled).stdout == 'hi\n'


@needs_gxx
def test_compile_error_reports_compiler_output(dirs):
    src = dirs['pkg'] / 'bad.cpp'
    src.write_text('int main() { return undefined_symbol; }\n')

    with pytest.raises(CompileError, match='undefined_symbol'):
        compile_program(src, dirs['build'], {'cpp17': cpp17()})


def test_interpreted_language_runs_without_build(dirs):
    src = dirs['pkg'] / 'my sol.py'
    src.write_text('print("ran")\n')

    compiled = compile_program(src, dirs['build'], {'py': python3()})

    assert compiled.argv[-1] == 'my sol.py'
    assert run(compiled).stdout == 'ran\n'


def test_file_without_language_is_error(dirs):
    src = dirs['pkg'] / 'scorer.rs'
    src.write_text('')
    with pytest.raises(CompileError, match="no language for 'scorer.rs' .*extension .rs"):
        compile_program(src, dirs['build'], {})


def test_rebuild_replaces_previous_output(dirs):
    src = dirs['pkg'] / 'a.py'
    src.write_text('print(1)\n')
    compile_program(src, dirs['build'], {'py': python3()})
    stale = dirs['build'] / 'a' / 'stale.txt'
    stale.write_text('x')

    compiled = compile_program(src, dirs['build'], {'py': python3()})

    assert not stale.exists()
    assert run(compiled).stdout == '1\n'


# --- directory programs ----------------------------------------------------

def test_directory_program_build_then_run(dirs):
    prog = dirs['pkg'] / 'encoder'
    prog.mkdir()
    write_exec(prog / 'build', 'echo built > artifact.txt\n')
    write_exec(prog / 'run', 'cat artifact.txt\n')

    compiled = compile_program(prog, dirs['build'], {})

    assert compiled.argv == ['./run']
    assert compiled.workdir == dirs['build'] / 'encoder'
    assert run(compiled).stdout == 'built\n'
    assert not (prog / 'artifact.txt').exists()


def test_directory_program_without_build_script(dirs):
    prog = dirs['pkg'] / 'scorer'
    prog.mkdir()
    write_exec(prog / 'run', 'echo ok\n')

    compiled = compile_program(prog, dirs['build'], {})
    assert run(compiled).stdout == 'ok\n'


def test_directory_program_build_failure(dirs):
    prog = dirs['pkg'] / 'broken'
    prog.mkdir()
    write_exec(prog / 'build', 'echo oops >&2\nexit 3\n')
    write_exec(prog / 'run', 'true\n')

    with pytest.raises(CompileError, match=r'build broken failed \(exit code 3\):\noops'):
        compile_program(prog, dirs['build'], {})


def test_directory_program_needs_run_script(dirs):
    prog = dirs['pkg'] / 'nope'
    prog.mkdir()
    write_exec(prog / 'build', 'true\n')
    with pytest.raises(CompileError, match="has no 'run' script"):
        compile_program(prog, dirs['build'], {})


def test_non_executable_run_script_is_error(dirs):
    prog = dirs['pkg'] / 'noexec'
    prog.mkdir()
    (prog / 'run').write_text('#!/bin/sh\necho hi\n')
    (prog / 'run').chmod(0o644)
    with pytest.raises(CompileError, match='not executable'):
        compile_program(prog, dirs['build'], {})


def test_missing_program_is_error(dirs):
    with pytest.raises(CompileError, match='does not exist'):
        compile_program(dirs['pkg'] / 'ghost.cpp', dirs['build'], {})


def test_explicit_name_overrides_stem(dirs):
    src = dirs['pkg'] / 'a.py'
    src.write_text('print(2)\n')
    compiled = compile_program(src, dirs['build'], {'py': python3()}, name='ref_a')
    assert compiled.name == 'ref_a'
    assert compiled.workdir == dirs['build'] / 'ref_a'
