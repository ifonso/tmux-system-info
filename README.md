# Tmux System Info

This project is a simple program for getting system statistics on macOS. It's written in C and uses the Mach kernel API 
to get the statistics. It was built for using in tmux status bar.

It should appear like (RAM usage, CPU usage):
```sh
10.0/16GB 10.00% ▄
```

## Usage

You'll need to clone and compile the project. For compiling just use this command:

```sh
make
```

You shoul get a binary file called `tmux_sysinfo` in the project directory. Put this binary in a folder that is in your 
`$PATH` and add the following line to your `.tmux.conf` file:

```sh
set -g status-right "#(/mypath/tmux_sysinfo)"
```
> Make sure to replace `/mypath/tmux_sysinfo` with the correct path to the binary file, and dont use relative paths.

## Note

This script uses the Mach kernel API to get system statistics. It's only supported on BSD-based systems, including macOS.