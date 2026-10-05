from pathlib import Path

import pytest

from tcframe_cli.paths import (
    LANGUAGES,
    REGISTRY,
    TEMPLATES,
    find_resource,
    search_roots,
    tcframe_home,
    user_home,
)


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


def test_tcframe_home_from_env(dirs):
    assert tcframe_home(env_with(dirs['tc_home'])) == dirs['tc_home']


def test_tcframe_home_unset_or_empty_is_none():
    assert tcframe_home({}) is None
    assert tcframe_home({'TCFRAME_HOME': ''}) is None


def test_user_home_is_dot_tcframe(dirs):
    assert user_home(dirs['home']) == dirs['home'] / '.tcframe'


def test_templates_search_user_before_home(dirs):
    roots = search_roots(TEMPLATES, env=env_with(dirs['tc_home']), home=dirs['home'])
    assert roots == [dirs['home'] / '.tcframe', dirs['tc_home']]


def test_languages_search_package_then_user_then_home(dirs):
    roots = search_roots(LANGUAGES, package_dir=dirs['pkg'],
                         env=env_with(dirs['tc_home']), home=dirs['home'])
    assert roots == [dirs['pkg'], dirs['home'] / '.tcframe', dirs['tc_home']]


def test_registry_search_home_before_user(dirs):
    roots = search_roots(REGISTRY, env=env_with(dirs['tc_home']), home=dirs['home'])
    assert roots == [dirs['tc_home'], dirs['home'] / '.tcframe']


def test_unset_roots_are_skipped(dirs):
    assert search_roots(REGISTRY, env={}, home=dirs['home']) == [dirs['home'] / '.tcframe']
    assert search_roots(LANGUAGES, env={}, home=dirs['home']) == [dirs['home'] / '.tcframe']


def test_unknown_resource_rejected(dirs):
    with pytest.raises(ValueError):
        search_roots('nope', env={}, home=dirs['home'])


def test_find_template_prefers_user_copy(dirs):
    for base in (dirs['home'] / '.tcframe', dirs['tc_home']):
        (base / 'templates').mkdir(parents=True)
        (base / 'templates' / 'batch.cpp').write_text(base.name)

    found = find_resource(TEMPLATES, 'batch.cpp', env=env_with(dirs['tc_home']), home=dirs['home'])

    assert found == dirs['home'] / '.tcframe' / 'templates' / 'batch.cpp'


def test_find_template_falls_back_to_tcframe_home(dirs):
    (dirs['tc_home'] / 'templates').mkdir()
    (dirs['tc_home'] / 'templates' / 'interactive.cpp').write_text('x')

    found = find_resource(TEMPLATES, 'interactive.cpp', env=env_with(dirs['tc_home']), home=dirs['home'])

    assert found == dirs['tc_home'] / 'templates' / 'interactive.cpp'


def test_language_package_overrides_user_and_home(dirs):
    for base in (dirs['pkg'], dirs['home'] / '.tcframe', dirs['tc_home']):
        (base / 'languages').mkdir(parents=True, exist_ok=True)
        (base / 'languages' / 'cpp17.yml').write_text(base.name)

    found = find_resource(LANGUAGES, 'cpp17.yml', package_dir=dirs['pkg'],
                          env=env_with(dirs['tc_home']), home=dirs['home'])

    assert found == dirs['pkg'] / 'languages' / 'cpp17.yml'


def test_language_without_package_uses_user_then_home(dirs):
    (dirs['tc_home'] / 'languages').mkdir()
    (dirs['tc_home'] / 'languages' / 'pascal.yml').write_text('x')

    found = find_resource(LANGUAGES, 'pascal.yml', env=env_with(dirs['tc_home']), home=dirs['home'])

    assert found == dirs['tc_home'] / 'languages' / 'pascal.yml'


def test_registry_directory_found_under_home(dirs):
    (dirs['tc_home'] / 'registry' / 'evaluators' / 'batch').mkdir(parents=True)

    found = find_resource(REGISTRY, 'evaluators/batch', env=env_with(dirs['tc_home']), home=dirs['home'])

    assert found == dirs['tc_home'] / 'registry' / 'evaluators' / 'batch'


def test_missing_resource_returns_none(dirs):
    assert find_resource(REGISTRY, 'nothing', env=env_with(dirs['tc_home']), home=dirs['home']) is None


def test_default_home_uses_path_home(monkeypatch, tmp_path):
    monkeypatch.setenv('HOME', str(tmp_path))
    assert user_home() == Path(tmp_path) / '.tcframe'
