from pathlib import Path

import pytest

from tcframe_cli.evaluators import (
    EvaluatorError,
    find_evaluator,
    load_evaluator,
    resolve_helpers,
)

REPO = Path(__file__).resolve().parents[2]


def write_evaluator(registry: Path, slug: str, text: str) -> Path:
    directory = registry / 'evaluators' / slug
    directory.mkdir(parents=True)
    (directory / 'evaluator.yml').write_text(text)
    return directory


@pytest.fixture
def registry(tmp_path):
    root = tmp_path / 'registry'
    (root / 'helpers' / 'scorer' / 'compare').mkdir(parents=True)
    (root / 'helpers' / 'scorer' / 'defaults.yml').write_text('slug: compare\n')
    return root


@pytest.fixture
def package(tmp_path):
    pkg = tmp_path / 'pkg'
    pkg.mkdir()
    return pkg


FUNCTIONAL = (
    'custom_solution_keys: true\n'
    'tc_output: optional\n'
    'helpers:\n'
    '- { slug: scorer, type: program, optional: true }\n'
    '- { slug: manager, type: files, optional: false }\n'
)

INTERACTIVE = (
    'custom_solution_keys: false\n'
    'tc_output: not_required\n'
    'helpers:\n'
    '- { slug: communicator, type: program, optional: false }\n'
)


def test_load_reads_keys_and_helpers(registry):
    config = load_evaluator(write_evaluator(registry, 'functional', FUNCTIONAL))

    assert config.slug == 'functional'
    assert config.custom_solution_keys is True
    assert config.tc_output == 'optional'
    assert [(h.slug, h.type, h.optional) for h in config.helpers] == [
        ('scorer', 'program', True),
        ('manager', 'files', False),
    ]


def test_load_defaults_when_keys_missing(registry):
    config = load_evaluator(write_evaluator(registry, 'bare', 'helpers:\n'))

    assert config.custom_solution_keys is False
    assert config.tc_output == 'optional'
    assert config.helpers == ()


@pytest.mark.parametrize('text, message', [
    ('tc_output: maybe\n', 'tc_output'),
    ('custom_solution_keys: yes\n', 'custom_solution_keys'),
    ('helpers:\n- { slug: x }\n', 'type'),
    ('helpers:\n- { slug: x, type: binary }\n', 'program or files'),
])
def test_load_rejects_bad_values(registry, text, message):
    with pytest.raises(EvaluatorError, match=message):
        load_evaluator(write_evaluator(registry, 'bad', text))


def test_find_evaluator_uses_tcframe_home_registry():
    config = find_evaluator('functional', env={'TCFRAME_HOME': str(REPO)}, home=REPO / 'no-home')

    assert config is not None
    assert config.custom_solution_keys is True
    assert config.directory == REPO / 'registry' / 'evaluators' / 'functional'


def test_find_evaluator_unknown_slug_is_none(tmp_path):
    assert find_evaluator('nope', env={'TCFRAME_HOME': str(tmp_path)}, home=tmp_path) is None


SCORER_ONLY = 'helpers:\n- { slug: scorer, type: program, optional: true }\n'


def test_package_program_file_wins_over_default(registry, package):
    config = load_evaluator(write_evaluator(registry, 'batch', SCORER_ONLY))
    (package / 'scorer.cpp').write_text('')

    helpers = resolve_helpers(config, package)

    assert helpers['scorer'].source == 'package'
    assert helpers['scorer'].path == package / 'scorer.cpp'


def test_missing_optional_program_uses_registry_default(registry, package):
    config = load_evaluator(write_evaluator(registry, 'batch', 'helpers:\n- { slug: scorer, type: program, optional: true }\n'))

    helpers = resolve_helpers(config, package)

    assert helpers['scorer'].source == 'registry'
    assert helpers['scorer'].path == registry / 'helpers' / 'scorer' / 'compare'


def test_package_program_directory_is_found(registry, package):
    config = load_evaluator(write_evaluator(registry, 'batch', 'helpers:\n- { slug: scorer, type: program, optional: true }\n'))
    (package / 'scorer').mkdir()

    assert resolve_helpers(config, package)['scorer'].path == package / 'scorer'


def test_optional_helper_without_default_is_omitted(registry, package):
    config = load_evaluator(write_evaluator(registry, 'x', 'helpers:\n- { slug: checker, type: program, optional: true }\n'))

    assert resolve_helpers(config, package) == {}


def test_required_program_missing_is_error(registry, package):
    config = load_evaluator(write_evaluator(registry, 'interactive', INTERACTIVE))

    with pytest.raises(EvaluatorError, match="requires helper 'communicator'"):
        resolve_helpers(config, package)


def test_required_files_helper_is_directory_in_package(registry, package):
    config = load_evaluator(write_evaluator(registry, 'functional', FUNCTIONAL))
    (package / 'manager').mkdir()

    helpers = resolve_helpers(config, package)

    assert helpers['manager'].source == 'package'
    assert helpers['manager'].path == package / 'manager'
    assert helpers['scorer'].source == 'registry'  # optional, default from the registry


def test_missing_files_helper_is_error(registry, package):
    config = load_evaluator(write_evaluator(registry, 'functional', FUNCTIONAL))

    with pytest.raises(EvaluatorError, match="requires helper 'manager'"):
        resolve_helpers(config, package)


def test_files_helper_given_as_file_is_error(registry, package):
    config = load_evaluator(write_evaluator(registry, 'functional', FUNCTIONAL))
    (package / 'manager').write_text('')

    with pytest.raises(EvaluatorError, match='must be a directory'):
        resolve_helpers(config, package)


def test_ambiguous_program_is_error(registry, package):
    config = load_evaluator(write_evaluator(registry, 'batch', 'helpers:\n- { slug: scorer, type: program, optional: true }\n'))
    (package / 'scorer.cpp').write_text('')
    (package / 'scorer').mkdir()

    with pytest.raises(EvaluatorError, match='ambiguous'):
        resolve_helpers(config, package)
