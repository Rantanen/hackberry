# Hackberry
### Self contained embedded headless development environment running on Raspberry Pi

## Installation

With cURL: `curl -o- https://raw.githubusercontent.com/Rantanen/hackberry/master/install.sh | sh`  
Or Wget: `wget -qO- https://raw.githubusercontent.com/Rantanen/hackberry/master/install.sh | sh`  

The install script will clone the hackberry repository to `~/.hackberry` by
default and run the `update.sh` to set up the environment.

The same command can be used to update the installation to a newer version.

There are some environment variables to alter the installation behaviour:

- `HACKBERRY_DIR` - the installation directory in which hackberry is installed. Defaults to `~/.hackberry`
- `HACKBERRY_GIT` - the source repository from which hackberry is installed. Defaults to `https://github.com/Rantanen/hackberry`
- `HACKBERRY_LOG` - the update log file. Defaults to `hackberry.log` in the current directory.

## Toolset

### [ttyd](https://github.com/tsl0922/ttyd)

Terminal access over browser. Allows access to the Hackberry without SSH.
