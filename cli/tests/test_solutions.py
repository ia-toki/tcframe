import pytest

from tcframe_cli.solutions import (
    AC, OK, RTE, TLE, WA, SolutionsError, parse_group, parse_solutions)


def make_tree(pkg, groups):
    """groups: {dir name: [entry names]}; entries ending in '/' are directories."""
    for group, entries in groups.items():
        folder = pkg / 'solutions' / group
        folder.mkdir(parents=True, exist_ok=True)
        for entry in entries:
            if entry.endswith('/'):
                (folder / entry.rstrip('/')).mkdir()
            else:
                (folder / entry).write_text('int main(){}\n')
    return pkg


@pytest.fixture
def pkg(tmp_path):
    return tmp_path / 'problem'


def test_full_layout_ordering_and_expectations(pkg):
    make_tree(pkg, {
        'ref': ['fushar.cpp'],
        'ac': ['gyosh.cpp', 'afaji321.cpp'],
        'ok-75': ['xxx.cpp'],
        'tle-75': ['yyy.cpp'],
        'failed': ['zzz.cpp'],
    })
    sols = parse_solutions(pkg)

    assert [s.label for s in sols] == [
        'ref/fushar.cpp', 'ac/afaji321.cpp', 'ac/gyosh.cpp',
        'failed/zzz.cpp', 'ok-75/xxx.cpp', 'tle-75/yyy.cpp']
    by_label = {s.label: s for s in sols}
    assert by_label['ref/fushar.cpp'].expected.verdicts == {AC}
    assert by_label['ac/gyosh.cpp'].expected.verdicts == {AC}
    assert by_label['failed/zzz.cpp'].expected.verdicts == {WA, TLE, RTE}
    assert by_label['ok-75/xxx.cpp'].expected.verdicts == {OK}
    assert by_label['ok-75/xxx.cpp'].expected.score == 75
    assert by_label['tle-75/yyy.cpp'].expected.verdicts == {TLE}
    assert by_label['ac/gyosh.cpp'].expected.score is None


def test_functional_solutions_are_directories(pkg):
    make_tree(pkg, {'ref': ['fushar/'], 'ac': ['afaji/']})
    sols = parse_solutions(pkg)
    assert [s.label for s in sols] == ['ref/fushar', 'ac/afaji']
    assert all(s.is_dir for s in sols)


def test_hidden_entries_ignored(pkg):
    make_tree(pkg, {'ref': ['ref.cpp', '.DS_Store'], 'wa': ['.keep', 'bad.cpp']})
    assert [s.label for s in parse_solutions(pkg)] == ['ref/ref.cpp', 'wa/bad.cpp']


def test_empty_verdict_directory_has_no_solutions(pkg):
    make_tree(pkg, {'ref': ['ref.cpp']})
    (pkg / 'solutions' / 'rte').mkdir()
    assert [s.label for s in parse_solutions(pkg)] == ['ref/ref.cpp']


@pytest.mark.parametrize('group, verdicts, score', [
    ('ref', {AC}, None),
    ('ac', {AC}, None),
    ('wa', {WA}, None),
    ('tle', {TLE}, None),
    ('rte', {RTE}, None),
    ('ok', {OK}, None),
    ('ok-0', {OK}, 0),
    ('ok-100', {OK}, 100),
    ('failed', {WA, TLE, RTE}, None),
    ('failed-25', {WA, TLE, RTE}, 25),
])
def test_parse_group(group, verdicts, score):
    exp = parse_group(group)
    assert exp.verdicts == verdicts
    assert exp.score == score


@pytest.mark.parametrize('group', ['AC', 'ok-', 'ok-x', 'accepted', 'ok-75-1', 'pe'])
def test_unknown_group_rejected(group):
    with pytest.raises(SolutionsError, match='unknown solutions directory'):
        parse_group(group)


def test_missing_solutions_dir(pkg):
    pkg.mkdir()
    with pytest.raises(SolutionsError, match="does not exist"):
        parse_solutions(pkg)


def test_missing_ref_rejected(pkg):
    make_tree(pkg, {'ac': ['a.cpp']})
    with pytest.raises(SolutionsError, match="no 'ref/' directory"):
        parse_solutions(pkg)


@pytest.mark.parametrize('entries', [[], ['a.cpp', 'b.cpp']])
def test_ref_must_have_exactly_one_entry(pkg, entries):
    make_tree(pkg, {'ac': ['a.cpp']})
    (pkg / 'solutions' / 'ref').mkdir()
    for e in entries:
        (pkg / 'solutions' / 'ref' / e).write_text('')
    with pytest.raises(SolutionsError, match='exactly one reference solution'):
        parse_solutions(pkg)


def test_stray_file_in_solutions_root_is_ignored(pkg):
    make_tree(pkg, {'ref': ['ref.cpp']})
    (pkg / 'solutions' / 'README.txt').write_text('notes')
    assert [s.label for s in parse_solutions(pkg)] == ['ref/ref.cpp']


def test_unknown_directory_rejected(pkg):
    make_tree(pkg, {'ref': ['ref.cpp'], 'mle': ['m.cpp']})
    with pytest.raises(SolutionsError, match="unknown solutions directory 'mle'"):
        parse_solutions(pkg)
