# Installation

TCFrame is a **header-only** C++ library, with a few helper shell scripts. In other words, you only need to get TCFrame's source code.

## Getting the source code

The recommended way is to **clone** the GitHub repository: **https://github.com/ia-toki/tcframe**. This way, you can just do `git pull` to fetch the latest updates. Alternatively, you can also **download** the latest release: **https://github.com/ia-toki/tcframe/releases**.

Suppose that you cloned or downloaded TCFrame to `~/tcframe`. We will call this directory "TCFrame home". Confirm that this directory is correct by verifying that the `include` directory exists directly inside the TCFrame home.

## Configure your environment

Next, add the following lines to your `~/.bashrc`. This will set the environment variable `TCFRAME_HOME` to our TCFrame home directory, and make the `tcframe` command available everywhere.

```sh
export TCFRAME_HOME=~/tcframe
alias tcframe=$TCFRAME_HOME/scripts/tcframe
```

:::note

If your `TCFRAME_HOME` contains space, you can escape it as follows:

```sh
export TCFRAME_HOME=~/path\ with\ space/tcframe
alias tcframe='$TCFRAME_HOME/scripts/tcframe'
```

:::

## Verifying setup

Restart your terminal session. Now, if you run

```
tcframe
```

You should see the following output

```
usage: tcframe <command>

Available commands:
  build         Compile spec file into runner program
  version       Print tcframe version
```

You're good to go!
