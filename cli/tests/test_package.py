import shutil
import zipfile
from pathlib import Path

import pytest

from tcframe_cli.package import package_package, write_archives

REPO_ROOT = Path(__file__).resolve().parents[2]
needs_gxx = pytest.mark.skipif(shutil.which('g++') is None, reason='g++ not available')


def names(zip_path: Path) -> set[str]:
    with zipfile.ZipFile(zip_path) as zf:
        return set(zf.namelist())


@pytest.fixture
def problem(tmp_path):
    pkg = tmp_path / 'aplusb'
    (pkg / 'solutions' / 'ref').mkdir(parents=True)
    (pkg / 'spec.cpp').write_text('spec')
    (pkg / 'metadata.yml').write_text('author: x\n')
    (pkg / 'solutions' / 'ref' / 'fushar.cpp').write_text('sol')
    (pkg / 'manager' / 'cpp').mkdir(parents=True)
    (pkg / 'manager' / 'cpp' / 'grader.cpp').write_text('g')
    build = pkg / 'build'
    (build / 'tc').mkdir(parents=True)
    (build / 'spec.yml').write_text('slug: aplusb\n')
    (build / 'tc' / 'aplusb_1.in').write_text('1 2\n')
    (build / 'tc' / 'aplusb_1.out').write_text('3\n')
    (build / 'spec').write_text('binary')  # compiled binary: never packaged
    (build / 'validator').write_text('validator')
    return pkg, build


def test_source_zip_has_sources_only(problem):
    pkg, build = problem

    source_zip, _ = write_archives(pkg, build, 'aplusb')

    assert source_zip == build / 'aplusb-source.zip'
    assert names(source_zip) == {
        'aplusb/spec.cpp',
        'aplusb/metadata.yml',
        'aplusb/solutions/ref/fushar.cpp',
        'aplusb/manager/cpp/grader.cpp',
    }


def test_dist_zip_adds_spec_yml_and_tc(problem):
    pkg, build = problem

    _, dist_zip = write_archives(pkg, build, 'aplusb')

    assert dist_zip == build / 'aplusb.zip'
    assert names(dist_zip) == names(build / 'aplusb-source.zip') | {
        'aplusb/spec.yml',
        'aplusb/tc/aplusb_1.in',
        'aplusb/tc/aplusb_1.out',
        'aplusb/validator',
    }


def test_missing_optional_entries_are_skipped(tmp_path):
    pkg = tmp_path / 'bare'
    pkg.mkdir()
    (pkg / 'spec.cpp').write_text('spec')
    build = pkg / 'build'
    build.mkdir()

    source_zip, dist_zip = write_archives(pkg, build, 'bare')

    assert names(source_zip) == {'bare/spec.cpp'}
    assert names(dist_zip) == {'bare/spec.cpp'}


def test_archives_overwrite_previous_run(problem):
    pkg, build = problem
    write_archives(pkg, build, 'aplusb')
    (pkg / 'metadata.yml').unlink()

    source_zip, _ = write_archives(pkg, build, 'aplusb')

    assert 'aplusb/metadata.yml' not in names(source_zip)


@needs_gxx
def test_package_runs_make_then_zips(tmp_path):
    pkg = tmp_path / 'aplusb'
    pkg.mkdir()
    (pkg / 'spec.cpp').write_text((REPO_ROOT / 'templates' / 'batch.cpp').read_text())
    sol = pkg / 'solution'
    sol.write_text('#!/bin/sh\nread a b\necho $((a + b))\n')
    sol.chmod(0o755)

    source_zip, dist_zip = package_package(pkg, env={'TCFRAME_HOME': str(REPO_ROOT)})

    assert source_zip == pkg / 'build' / 'aplusb-source.zip'
    assert 'aplusb/solution' in names(source_zip)
    assert {'aplusb/spec.yml', 'aplusb/tc/aplusb_sample_1.in'} <= names(dist_zip)
    assert 'aplusb/build/spec' not in names(dist_zip)


def test_metadata_is_copied_verbatim_into_both_archives(tmp_path):
    # SPEC.md T7.2: metadata.yml is opaque to tcframe, so its bytes must survive unchanged.
    pkg = tmp_path / 'meta'
    pkg.mkdir()
    (pkg / 'spec.cpp').write_text('spec')
    raw = 'author: Ashar Fuadi\r\ntags:\r\n- math\r\ntitle: Café\r\nx: [unparsed, "anything"]\r\n'
    (pkg / 'metadata.yml').write_bytes(raw.encode('utf-8'))
    build = pkg / 'build'
    build.mkdir()
    (build / 'spec.yml').write_text('slug: meta\n')

    source_zip, dist_zip = write_archives(pkg, build, 'meta')

    for archive in (source_zip, dist_zip):
        with zipfile.ZipFile(archive) as zf:
            assert zf.read('meta/metadata.yml') == raw.encode('utf-8')
