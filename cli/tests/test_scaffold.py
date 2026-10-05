import pytest

from tcframe_cli.scaffold import ScaffoldError, create_package


@pytest.fixture
def env(tmp_path):
    tc_home = tmp_path / 'tcframe-home'
    (tc_home / 'templates').mkdir(parents=True)
    (tc_home / 'templates' / 'batch.cpp').write_text('// batch template\n')
    return {'TCFRAME_HOME': str(tc_home)}


@pytest.fixture
def home(tmp_path):
    h = tmp_path / 'home'
    h.mkdir()
    return h


def test_copies_template_to_spec_cpp(tmp_path, env, home):
    dest = tmp_path / 'problem'

    target = create_package('batch', dest, env=env, home=home)

    assert target == dest / 'spec.cpp'
    assert target.read_text() == '// batch template\n'


def test_user_template_overrides_tcframe_home(tmp_path, env, home):
    (home / '.tcframe' / 'templates').mkdir(parents=True)
    (home / '.tcframe' / 'templates' / 'batch.cpp').write_text('// user template\n')

    target = create_package('batch', tmp_path / 'p', env=env, home=home)

    assert target.read_text() == '// user template\n'


def test_refuses_to_overwrite_existing_spec(tmp_path, env, home):
    dest = tmp_path / 'problem'
    dest.mkdir()
    (dest / 'spec.cpp').write_text('mine')

    with pytest.raises(ScaffoldError, match='already exists'):
        create_package('batch', dest, env=env, home=home)

    assert (dest / 'spec.cpp').read_text() == 'mine'


def test_unknown_template_lists_searched_paths(tmp_path, env, home):
    with pytest.raises(ScaffoldError) as exc:
        create_package('nope', tmp_path / 'p', env=env, home=home)

    assert "template 'nope' not found" in str(exc.value)
    assert 'nope.cpp' in str(exc.value)


def test_unset_tcframe_home_reported(tmp_path, home):
    with pytest.raises(ScaffoldError, match='TCFRAME_HOME is unset'):
        create_package('batch', tmp_path / 'p', env={}, home=home)


@pytest.mark.parametrize('name', ['../evil', 'a/b', '', 'x.cpp'])
def test_rejects_path_like_template_names(tmp_path, env, home, name):
    with pytest.raises(ScaffoldError, match='invalid template name'):
        create_package(name, tmp_path / 'p', env=env, home=home)
