import pytest

from tcframe_cli.simple_yaml import YamlError, parse


def test_flow_map_at_top_level():
    assert parse('aggregator: { slug: min, args: 16 }\n') == {
        'aggregator': {'slug': 'min', 'args': '16'}}


def test_flow_map_list_items():
    data = parse('helpers:\n- { slug: a, optional: true }\n- { slug: b }\n')
    assert data == {'helpers': [{'slug': 'a', 'optional': 'true'}, {'slug': 'b'}]}


def test_empty_flow_map():
    assert parse('m: {}\n') == {'m': {}}


@pytest.mark.parametrize('text', [
    'm: { a }\n',           # no colon inside braces
    'm: { a: 1, a: 2 }\n',  # duplicate key
    'items:\n- { a: 1\n',   # unterminated
])
def test_bad_flow_maps_rejected(text):
    with pytest.raises(YamlError):
        parse(text)
