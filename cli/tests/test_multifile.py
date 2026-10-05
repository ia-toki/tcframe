import shutil
from pathlib import Path

import pytest

from tcframe_cli.build import BuildError, build_package
from tcframe_cli.checker import PASS, run_tests
from tcframe_cli.languages import Language, available_languages
from tcframe_cli.multifile import (
    MultifileError,
    family_of,
    key_files,
    multifile_args,
    read_spec_info,
)

REPO_ROOT = Path(__file__).resolve().parents[2]
ENV = {'TCFRAME_HOME': str(REPO_ROOT)}
GXX = shutil.which('g++')
FPC = shutil.which('fpc')
needs_gxx = pytest.mark.skipif(GXX is None, reason='g++ not available')
needs_fpc = pytest.mark.skipif(FPC is None, reason='fpc not available')

SPEC_YML = (
    'subtasks: []\n'
    'evaluator:\n'
    '  slug: functional\n'
    '  solution_keys: [encoder, decoder]\n'
    '  tc_output_present: false\n'
    'helpers: {}\n'
    'limits: { time: 2000, memory: 65536 }\n'
)


def language(slug, family, exts):
    return Language(slug, slug, family, tuple(exts), build='', run='', source=Path(slug + '.yml'))


LANGUAGES = {
    'cpp17': language('cpp17', 'cpp', ['cc', 'cpp', 'c++']),
    'pascal': language('pascal', 'pascal', ['pas']),
}


def write(path: Path, text: str = ''):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)


def test_read_spec_info_from_emitted_yaml(tmp_path):
    spec_yml = tmp_path / 'spec.yml'
    spec_yml.write_text(SPEC_YML)

    info = read_spec_info(spec_yml)

    assert info.evaluator_slug == 'functional'
    assert info.solution_keys == ('encoder', 'decoder')


def test_read_spec_info_custom_evaluator_has_no_slug(tmp_path):
    spec_yml = tmp_path / 'spec.yml'
    spec_yml.write_text('evaluator:\n  solution_keys: [source]\n  tc_output_present: true\n')

    assert read_spec_info(spec_yml).evaluator_slug is None


def test_read_spec_info_needs_keys(tmp_path):
    spec_yml = tmp_path / 'spec.yml'
    spec_yml.write_text('evaluator:\n  slug: batch\n')

    with pytest.raises(MultifileError, match='solution_keys'):
        read_spec_info(spec_yml)


def test_key_files_match_stems_and_ignore_extra_files(tmp_path):
    d = tmp_path / 'fushar'
    write(d / 'encoder.cpp')
    write(d / 'decoder.cpp')
    write(d / 'encoder.h')  # a header shares the stem but is not a source of any language
    write(d / '.hidden.cpp')

    files = key_files(d, ('encoder', 'decoder'), LANGUAGES)

    assert {k: p.name for k, p in files.items()} == {'encoder': 'encoder.cpp', 'decoder': 'decoder.cpp'}


def test_key_files_missing_key_is_error(tmp_path):
    d = tmp_path / 'fushar'
    write(d / 'encoder.cpp')

    with pytest.raises(MultifileError, match="no file for solution key 'decoder'"):
        key_files(d, ('encoder', 'decoder'), LANGUAGES)


def test_key_files_two_files_for_one_key_is_error(tmp_path):
    d = tmp_path / 'fushar'
    write(d / 'encoder.cpp')
    write(d / 'encoder.pas')
    write(d / 'decoder.cpp')

    with pytest.raises(MultifileError, match="more than one file for key 'encoder'"):
        key_files(d, ('encoder', 'decoder'), LANGUAGES)


def test_family_of_one_family(tmp_path):
    files = {'encoder': tmp_path / 'encoder.cpp', 'decoder': tmp_path / 'decoder.cc'}

    assert family_of(files, LANGUAGES) == 'cpp'


def test_family_of_mixed_families_is_error(tmp_path):
    files = {'encoder': tmp_path / 'encoder.cpp', 'decoder': tmp_path / 'decoder.pas'}

    with pytest.raises(MultifileError, match='one language family'):
        family_of(files, LANGUAGES)


def test_family_of_unknown_extension_is_error(tmp_path):
    with pytest.raises(MultifileError, match='no language for'):
        family_of({'encoder': tmp_path / 'encoder.rs'}, LANGUAGES)


def test_multifile_args_carry_keys_evaluator_family_and_manager(tmp_path):
    pkg = tmp_path / 'pkg'
    d = pkg / 'solutions' / 'ref' / 'fushar'
    write(d / 'encoder.cpp')
    write(d / 'decoder.cpp')
    spec_yml = tmp_path / 'spec.yml'
    spec_yml.write_text(SPEC_YML)

    args = multifile_args(d, pkg, spec_yml, LANGUAGES, ENV)

    assert args == [
        f'--solution-file=encoder={(d / "encoder.cpp").resolve()}',
        f'--solution-file=decoder={(d / "decoder.cpp").resolve()}',
        f'--evaluator-dir={(REPO_ROOT / "registry" / "evaluators" / "functional").resolve()}',
        '--solution-family=cpp',
        f'--manager={(pkg / "manager").resolve()}',
    ]


def test_multifile_args_rejects_single_file_evaluator(tmp_path):
    d = tmp_path / 'sol'
    write(d / 'source.cpp')
    spec_yml = tmp_path / 'spec.yml'
    spec_yml.write_text('evaluator:\n  slug: batch\n  solution_keys: [source]\n  tc_output_present: true\n')

    with pytest.raises(MultifileError, match='functional evaluator'):
        multifile_args(d, tmp_path, spec_yml, LANGUAGES, ENV)


def test_shipped_languages_load():
    languages = available_languages(env=ENV, home=REPO_ROOT / 'no-home')

    assert languages['cpp17'].family == 'cpp'
    assert 'cpp' in languages['cpp17'].extensions
    assert languages['pascal'].family == 'pascal'
    assert languages['pascal'].extensions == ('pas',)


def write_functional_package(root: Path, spec_dir: str, ext: str, keys_ext: dict[str, str]) -> Path:
    """
    A source package for a functional problem: spec.cpp and manager/ from the ete fixture,
    the reference in solutions/ref/fushar/, a second correct pair in solutions/ac/copy/ and
    a wrong decoder in solutions/wa/wrong/. `spec_dir` is the ete fixture name.
    """
    fixture = REPO_ROOT / 'test' / 'ete' / 'resources' / spec_dir
    pkg = root / 'fn'
    write(pkg / 'spec.cpp', (fixture / 'spec.cpp').read_text())
    shutil.copytree(fixture / 'manager', pkg / 'manager')
    ref = fixture / 'ref'
    wrong = fixture / 'wrong'
    for name in keys_ext:
        ext_name = keys_ext[name]
        write(pkg / 'solutions' / 'ref' / 'fushar' / f'{name}.{ext_name}', (ref / f'{name}.{ext_name}').read_text())
        write(pkg / 'solutions' / 'ac' / 'copy' / f'{name}.{ext_name}', (ref / f'{name}.{ext_name}').read_text())
    write(pkg / 'solutions' / 'wa' / 'wrong' / f'encoder.{keys_ext["encoder"]}',
          (ref / f'encoder.{keys_ext["encoder"]}').read_text())
    write(pkg / 'solutions' / 'wa' / 'wrong' / f'decoder.{keys_ext["decoder"]}',
          (wrong / f'decoder.{keys_ext["decoder"]}').read_text())
    return pkg


@needs_gxx
def test_functional_cpp_package_tested_end_to_end(tmp_path):
    pkg = write_functional_package(tmp_path, 'functional', 'cpp', {'encoder': 'cpp', 'decoder': 'cpp'})

    checks = run_tests(pkg, env=ENV)

    by_label = {c.result.solution.label: c for c in checks}
    assert by_label['ref/fushar'].status == PASS
    assert by_label['ac/copy'].status == PASS
    assert by_label['wa/wrong'].status == PASS, by_label['wa/wrong'].detail
    assert by_label['wa/wrong'].detail.startswith('WA')


@needs_gxx
def test_directory_solution_with_batch_evaluator_is_error(tmp_path):
    pkg = tmp_path / 'aplusb'
    pkg.mkdir()
    shutil.copy(REPO_ROOT / 'templates' / 'batch.cpp', pkg / 'spec.cpp')
    write(pkg / 'sol' / 'source.cpp', '#include <cstdio>\nint main(){ int a,b; scanf("%d %d",&a,&b); printf("%d\\n",a+b); }\n')

    with pytest.raises(BuildError, match='functional evaluator'):
        build_package(pkg, solution=str(pkg / 'sol'), env=ENV)


@needs_gxx
def test_mixed_family_key_files_is_build_error(tmp_path):
    pkg = write_functional_package(tmp_path, 'functional', 'cpp', {'encoder': 'cpp', 'decoder': 'cpp'})
    mixed = pkg / 'mixed'
    write(mixed / 'encoder.cpp', (REPO_ROOT / 'test/ete/resources/functional/ref/encoder.cpp').read_text())
    write(mixed / 'decoder.pas', 'unit decoder; interface implementation end.\n')

    with pytest.raises(BuildError, match='one language family'):
        build_package(pkg, solution=str(mixed), env=ENV)


@needs_fpc
def test_functional_pascal_package_tested_end_to_end(tmp_path):
    pkg = write_functional_package(tmp_path, 'functional-pascal', 'pas', {'encoder': 'pas', 'decoder': 'pas'})

    checks = run_tests(pkg, env=ENV)

    by_label = {c.result.solution.label: c for c in checks}
    assert by_label['ref/fushar'].status == PASS
    assert by_label['ac/copy'].status == PASS
    assert by_label['wa/wrong'].status == PASS, by_label['wa/wrong'].detail


@needs_gxx
def test_grade_package_accepts_multifile_directory(tmp_path, capfd):
    from tcframe_cli.grade import grade_package

    pkg = write_functional_package(tmp_path, 'functional', 'cpp', {'encoder': 'cpp', 'decoder': 'cpp'})
    build_package(pkg, solution=str(pkg / 'solutions' / 'ref' / 'fushar'), env=ENV)

    code = grade_package(pkg, solution=str(pkg / 'solutions' / 'wa' / 'wrong'), env=ENV)

    assert code == 0
    assert 'Wrong Answer' in capfd.readouterr().out
