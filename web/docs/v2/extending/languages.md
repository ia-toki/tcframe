# Languages

A language tells `tcframe` how to build and run a source file. Each language is one `*.yml` file.

## Shipped languages

`languages/cpp17.yml`:

```yaml
name: C++17 (GCC)
family: cpp
extensions: [cc, cpp, c++]
build: g++ -std=c++17 -O2 -o $BASE_FILENAME $FILENAME
run: ./$BASE_FILENAME
```

`languages/pascal.yml`:

```yaml
name: Pascal (FPC)
family: pascal
extensions: [pas]
build: fpc -O2 -o$BASE_FILENAME $FILENAME
run: ./$BASE_FILENAME
```

## Fields

| Field | Meaning |
| --- | --- |
| `name` | Display name |
| `family` | Group of related languages. Used to choose the functional build and run scripts. |
| `extensions` | File extensions this language handles |
| `build` | Command that builds the file. May be empty for interpreted languages. |
| `run` | Command that runs the built program. Required. |

In both commands, `$FILENAME` is the source file's name and `$BASE_FILENAME` is the name without its extension.

## Adding a language

For an interpreted language, leave `build` empty. This Python example runs `.py` solutions directly:

```yaml
name: Python 3
family: python
extensions: [py]
build:
run: python3 $FILENAME
```

Save it as `languages/python3.yml` in the package, or in `~/.tcframe/languages/` to use it everywhere.

## Where languages are found

`tcframe` searches these places, in order, and uses the first file it finds:

1. `languages/` in the problem package
2. `~/.tcframe/languages/`
3. `$TCFRAME_HOME/languages/`

A language's slug is the file name without `.yml`. If two languages claim the same extension, the one whose slug sorts first wins.
