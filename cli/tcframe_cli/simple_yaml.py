"""
Minimal YAML reader for the flat config files the CLI reads (language `*.yml`,
later `evaluator.yml` / `defaults.yml`). The package has zero runtime
dependencies, so only the subset those files use is supported:

  key: scalar           plain or "quoted" / 'quoted' scalar
  key: [a, b]           flow list of scalars
  key: { a: x, b: y }   flow map of scalars (single level)
  key:                  block list: following "- item" lines (any indent)
  - a
  - b
  - { slug: x, type: program }     list items may be flow maps

Empty `key:` with no items reads as the empty string. Anything else (nested
mappings, multi-line scalars, anchors, trailing comments) raises YamlError.
Keys must be unique.
"""

from typing import Union

Map = dict[str, str]
Value = Union[str, Map, list[Union[str, Map]]]


class YamlError(ValueError):
    """Raised when text is outside the supported YAML subset."""


def _scalar(raw: str) -> str:
    s = raw.strip()
    if len(s) >= 2 and s[0] == s[-1] and s[0] in '"\'':
        return s[1:-1]
    return s


def _flow_map(raw: str, lineno: int) -> Map:
    s = raw.strip()
    if not (s.startswith('{') and s.endswith('}')):
        raise YamlError(f'line {lineno}: expected a flow map "{{ key: value }}"')
    result: Map = {}
    for part in s[1:-1].split(','):
        if not part.strip():
            continue
        key, sep, value = part.partition(':')
        key = key.strip()
        if not sep or not key:
            raise YamlError(f'line {lineno}: expected "key: value" inside {{ }}')
        if key in result:
            raise YamlError(f'line {lineno}: duplicate key {key!r}')
        result[key] = _scalar(value)
    return result


def parse(text: str) -> dict[str, Value]:
    result: dict[str, Value] = {}
    block_key = None  # key whose "- item" lines are being collected

    for lineno, line in enumerate(text.splitlines(), 1):
        stripped = line.strip()
        if not stripped or stripped.startswith('#'):
            continue

        if stripped == '-' or stripped.startswith('- '):
            if block_key is None:
                raise YamlError(f'line {lineno}: list item without a key')
            if isinstance(result[block_key], str):
                result[block_key] = []
            item = stripped[1:].strip()
            if item.startswith('{'):
                result[block_key].append(_flow_map(item, lineno))
            else:
                result[block_key].append(_scalar(item))
            continue

        if line[0] in ' \t':
            raise YamlError(f'line {lineno}: nested mappings are not supported')

        key, sep, value = stripped.partition(':')
        key = key.strip()
        value = value.strip()
        if not sep or not key:
            raise YamlError(f'line {lineno}: expected "key: value"')
        if key in result:
            raise YamlError(f'line {lineno}: duplicate key {key!r}')

        block_key = None
        if not value:
            result[key] = ''
            block_key = key
        elif value.startswith('[') and value.endswith(']'):
            items = [_scalar(v) for v in value[1:-1].split(',')]
            result[key] = [v for v in items if v]
        elif value.startswith('{'):
            result[key] = _flow_map(value, lineno)
        else:
            result[key] = _scalar(value)

    return result
