import re
from pathlib import Path

import pytest

from tcframe_cli.languages import (
    LanguageError,
    available_languages,
    check_slug,
    find_language,
    load_language,
)
from tcframe_cli.simple_yaml import YamlError, parse

CPP17 = """\
name: C++17 (GCC)
family: cpp
extensions:
- cc
- cpp
- c++

build: /usr/bin/g++ -std=c++17 -o $BASE_FILENAME $FILENAME
run: ./$BASE_FILENAME
"""


def write_lang(folder: Path, slug: str, text: str = CPP17) -> Path:
    folder.mkdir(parents=True, exist_ok=True)
    path = folder / f'{slug}.yml'
    path.write_text(text, encoding='utf-8')
    return path


@pytest.fixture
def dirs(tmp_path):
    home = tmp_path / 'home'
    tc_home = tmp_path / 'tcframe-home'
    pkg = tmp_path / 'problem'
    for d in (home, tc_home, pkg):
        d.mkdir()
    return {'home': home, 'tc_home': tc_home, 'pkg': pkg}


def env_with(tc_home):
    return {'TCFRAME_HOME': str(tc_home)}


# --- simple_yaml -----------------------------------------------------------

def test_yaml_scalars_quotes_and_flow_list():
    data = parse('name: "C++ 17"\nfamily: \'cpp\'\nextensions: [cc, .cpp, ]\n')
    assert data == {'name': 'C++ 17', 'family': 'cpp', 'extensions': ['cc', '.cpp']}


def test_yaml_block_list_and_comments_and_blank_lines():
    data = parse('# comment\n\nexts:\n  - cc\n  - cpp\nrun: ./x\n')
    assert data == {'exts': ['cc', 'cpp'], 'run': './x'}


def test_yaml_empty_value_is_empty_string():
    assert parse('build:\nrun: ./x\n') == {'build': '', 'run': './x'}


def test_yaml_value_keeps_colons_and_dollars():
    data = parse('build: /usr/bin/g++ -o $BASE_FILENAME $FILENAME:x\n')
    assert data['build'] == '/usr/bin/g++ -o $BASE_FILENAME $FILENAME:x'


@pytest.mark.parametrize('text', [
    'a:\n  nested: 1\n',      # nested mapping
    '- orphan\n',             # list item without key
    'a: 1\na: 2\n',           # duplicate key
    'just text\n',            # no colon
    'a: 1\n- item\n',         # list item after scalar key
])
def test_yaml_rejects_unsupported(text):
    with pytest.raises(YamlError):
        parse(text)


# --- load_language ---------------------------------------------------------

def test_load_language_fields(tmp_path):
    lang = load_language(write_lang(tmp_path, 'cpp17'))
    assert lang.slug == 'cpp17'
    assert lang.name == 'C++17 (GCC)'
    assert lang.family == 'cpp'
    assert lang.extensions == ('cc', 'cpp', 'c++')
    assert lang.source == tmp_path / 'cpp17.yml'


def test_extensions_are_normalized(tmp_path):
    text = CPP17.replace('extensions:\n- cc\n- cpp\n- c++', 'extensions: [.CC, cpp]')
    assert load_language(write_lang(tmp_path, 'x', text)).extensions == ('cc', 'cpp')


def test_empty_build_allowed_for_interpreted(tmp_path):
    text = CPP17.replace('build: /usr/bin/g++ -std=c++17 -o $BASE_FILENAME $FILENAME', 'build:')
    lang = load_language(write_lang(tmp_path, 'py', text))
    assert lang.build == ''
    assert lang.build_argv('a.py') == []


@pytest.mark.parametrize('replace_from, replace_to, message', [
    ('family: cpp\n', '', re.escape('missing key(s): family')),
    ('run: ./$BASE_FILENAME', 'run:', 'run must not be empty'),
    ('extensions:\n- cc\n- cpp\n- c++', 'extensions: cc', 'extensions must be a non-empty list'),
    ('extensions:\n- cc\n- cpp\n- c++', 'extensions:', 'extensions must be a non-empty list'),
])
def test_invalid_language_rejected(tmp_path, replace_from, replace_to, message):
    text = CPP17.replace(replace_from, replace_to)
    with pytest.raises(LanguageError, match=message):
        load_language(write_lang(tmp_path, 'x', text))


def test_malformed_yaml_wrapped_as_language_error(tmp_path):
    with pytest.raises(LanguageError, match='expected "key: value"'):
        load_language(write_lang(tmp_path, 'x', 'garbage\n'))


def test_slug_must_be_safe_stem(tmp_path):
    with pytest.raises(LanguageError, match='invalid language slug'):
        check_slug('../evil')
    with pytest.raises(LanguageError, match='invalid language slug'):
        load_language(write_lang(tmp_path, 'my.lang', CPP17))


# --- substitution ----------------------------------------------------------

def test_build_and_run_argv_substitution(tmp_path):
    lang = load_language(write_lang(tmp_path, 'cpp17'))
    assert lang.build_argv('solution.cpp') == [
        '/usr/bin/g++', '-std=c++17', '-o', 'solution', 'solution.cpp']
    assert lang.run_argv('solution.cpp') == ['./solution']


def test_substitution_keeps_spaces_in_filename(tmp_path):
    lang = load_language(write_lang(tmp_path, 'cpp17'))
    assert lang.build_argv('dir/my sol.cpp') == [
        '/usr/bin/g++', '-std=c++17', '-o', 'my sol', 'my sol.cpp']


def test_substitution_only_matches_whole_placeholders(tmp_path):
    text = CPP17.replace('run: ./$BASE_FILENAME', 'run: ./$BASE_FILENAME_x $FILENAMEX')
    lang = load_language(write_lang(tmp_path, 'x', text))
    assert lang.run_argv('a.cpp') == ['./$BASE_FILENAME_x', '$FILENAMEX']


# --- resolution ------------------------------------------------------------

def test_package_language_wins_over_user_and_home(dirs):
    write_lang(dirs['tc_home'] / 'languages', 'cpp17', CPP17.replace('C++17 (GCC)', 'home'))
    write_lang(dirs['home'] / '.tcframe' / 'languages', 'cpp17', CPP17.replace('C++17 (GCC)', 'user'))
    write_lang(dirs['pkg'] / 'languages', 'cpp17', CPP17.replace('C++17 (GCC)', 'pkg'))

    lang = find_language('cpp17', dirs['pkg'], env_with(dirs['tc_home']), dirs['home'])
    assert lang.name == 'pkg'


def test_user_language_wins_over_home(dirs):
    write_lang(dirs['tc_home'] / 'languages', 'cpp17', CPP17.replace('C++17 (GCC)', 'home'))
    write_lang(dirs['home'] / '.tcframe' / 'languages', 'cpp17', CPP17.replace('C++17 (GCC)', 'user'))

    lang = find_language('cpp17', None, env_with(dirs['tc_home']), dirs['home'])
    assert lang.name == 'user'


def test_home_language_used_when_only_source(dirs):
    write_lang(dirs['tc_home'] / 'languages', 'cpp17')
    lang = find_language('cpp17', dirs['pkg'], env_with(dirs['tc_home']), dirs['home'])
    assert lang.source == dirs['tc_home'] / 'languages' / 'cpp17.yml'


def test_find_language_missing_returns_none(dirs):
    assert find_language('nope', dirs['pkg'], env_with(dirs['tc_home']), dirs['home']) is None


def test_unset_tcframe_home_is_skipped(dirs):
    write_lang(dirs['home'] / '.tcframe' / 'languages', 'cpp17')
    assert find_language('cpp17', None, {}, dirs['home']) is not None


def test_available_languages_first_root_hides_same_slug(dirs):
    write_lang(dirs['tc_home'] / 'languages', 'cpp17', CPP17.replace('C++17 (GCC)', 'home'))
    write_lang(dirs['pkg'] / 'languages', 'cpp17', CPP17.replace('C++17 (GCC)', 'pkg'))
    write_lang(dirs['tc_home'] / 'languages', 'pascal', CPP17.replace('C++17 (GCC)', 'fpc'))

    langs = available_languages(dirs['pkg'], env_with(dirs['tc_home']), dirs['home'])
    assert sorted(langs) == ['cpp17', 'pascal']
    assert langs['cpp17'].name == 'pkg'
    assert langs['pascal'].name == 'fpc'


def test_available_languages_empty(dirs):
    assert available_languages(dirs['pkg'], env_with(dirs['tc_home']), dirs['home']) == {}
