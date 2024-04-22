# Blockchain Backend

This is a C++ engine (backend) for a simple Blockchain implementation.
The engine is accessed with a Python Flask API using PyBind11.
The backend can be run in "server" mode in a provided Docker container or it can be built and run locally using a Terminal.
Instructions for both methods are provided.

## Table of Contents

* [Prerequisites](#prerequisites)
* [Project Setup](#project-setup)
* [Build and Run Docker Server](#build-and-run-docker-server)
* [Build and Run Locally in Terminal](#build-and-run-locally-in-terminal)
* [Create Environment in Terminal to Build with CMake](#create-environment-in-terminal-to-build-with-cmake)
* [Build Backend](#build-backend)
* [Run Backend Server in Terminal](#run-backend-server-in-terminal)
* [Deactivate Build Environment](#deactivate-build-environment)
* [References](#references)

## Prerequisites

* [git](https://git-scm.com/)

    <details>
    <summary>Install Command</summary>
    
    - Debian / Ubuntu:
        
            sudo apt update
            sudo apt install git

    </details>

    <br>

* A [gcc compiler](https://gcc.gnu.org/releases.html) that supports modern C++ ([recommended minimum version is C++17](https://gcc.gnu.org/projects/cxx-status.html))

    <details>
    <summary>Install Command</summary>
    
    - Debian / Ubuntu (g++):
        
            sudo apt update
            sudo apt install build-essential
            sudo apt-get install manpages-dev

    </details>

    <br>

* [Docker](https://www.docker.com/)

    <details>
    <summary>Setup & Install Commands</summary>
    
    - Debian / Ubuntu:
        
            sudo apt-get update
            sudo apt-get install ca-certificates curl
            sudo install -m 0755 -d /etc/apt/keyrings
            sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
            sudo chmod a+r /etc/apt/keyrings/docker.asc
            echo \
              "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
              $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
              sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
            sudo apt-get update

            sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

            sudo docker run hello-world

    </details>

    <br>

* [CMake](https://cmake.org/) Version 3.13.4 (or later)

    <details>
    <summary>Install Command</summary>
    
    - Debian / Ubuntu:
        
            sudo apt update
            sudo apt-get install cmake

    </details>

    <br>

* [Python](https://www.python.org/downloads/) Version 3.6 (or later)

    <details>
    <summary>Install Command</summary>
    
    - Debian / Ubuntu:
        
            sudo apt update
            sudo apt install python3

    </details>

    <br>

* Internet access (for [doctest](https://github.com/onqtam/doctest) and [pybind11](https://github.com/pybind/pybind11.git) cloning as well as requirements installation and running a [Docker](https://www.docker.com/) server)

\[[toc](#table-of-contents)\]

<br>

<hr>

## Project Setup

Prior to building the code for the first time, i.e. after a fresh clone, the project's [git submodules](.gitmodules) need to be brought into the project. The process to achieve this task is listed for various operating systems as follows:

<details>
<summary>Debian / Ubuntu</summary>
    
  - open a terminal
  - `cd` into the `parent` directory (containing the `.gitmodules` file)
  - run the command string
        
            git submodule update --init --recursive --remote
    
</details>

<br>

Ensure the submodules have been brought into the project (it will not build without them).
The `cmake` directory directory should be fully populated.

\[[toc](#table-of-contents)\]

<br>

<hr>

## Build and Run [Docker](https://www.docker.com/) Server

The option to build and run the server in a [Docker](https://www.docker.com/) container is available with the provided ```Dockerfile```. In the directory containing the ```Dockerfile``` (**this** directory), execute the following command:

```console
docker build -t blockchain .
```

To run the container
```console
docker run -t -d -p 5000:5000 blockchain:latest
```

Visit ```http://localhost:5000``` in a browser to see the *Genesis* block (or current last block in the chain).

<details><summary>Other Useful Docker Commands</summary>

<br>

TO VIEW **ALL** [DOCKER](https://www.docker.com/) IMAGES
```console
docker images
```

TO VIEW RUNNING [DOCKER](https://www.docker.com/) CONTAINERS
```console
docker ps -a
```

TO STOP **ALL** RUNNING [DOCKER](https://www.docker.com/) CONTAINERS
```console
docker container stop $(docker container ls -aq)
```

TO **DESTROY ALL** STOPPED [DOCKER](https://www.docker.com/) CONTAINERS
```console
docker container rm $(docker container ls -aq)  
```

TO **DESTROY ALL** [DOCKER](https://www.docker.com/) IMAGES
```console
docker system prune -a
```

While not a [Docker](https://www.docker.com/) command, the following can be used to ensure the container is listening to correct port:
```console
sudo netstat -ntlp | grep LISTEN
```
</details>

<br>

\[[toc](#table-of-contents)\]

<br>

<hr>

## Build and Run Locally in Terminal

Alternatively, a local build running the backend in a virtual environment is provided can used for development.

### Create Environment in Terminal to Build with CMake

**NOTE:** all shell scripts may require

```console
chmod u+x <name>.sh
```

prior to executing, where `<name>` is the *filename* of the shell script.

In the ```api/scripts``` directory, execute the command:

```console
source create_and_update_venv.sh
```

This will install all requirements listed in ```requirements.txt```.

<br>

\[[toc](#table-of-contents)\]

<br>

<hr>

### Build Backend

The blockchain implementation is provided with [CMakeLists.txt](https://cmake.org/) files and a [CMakePresets.json](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html).

[SHA-256](https://en.wikipedia.org/wiki/SHA-2) is used for hashing.

The [CMakeLists.txt](https://cmake.org/) files are currently only configured for a Linux build, but that can be modified with relative ease.

The parent directory is the directory containing the *CMakePresets.txt* file.

The directory structure is:
```console
backend
├── api
│   └── scripts
├── cmake
├── src
│   └── CMakeLists.txt
├── test
│   └── CMakeLists.txt
├── .gitignore
├── .gitmodules
├── CMakeLists.txt
├── CMakePresets.json
├── Dockerfile
└── README.md
```

<br>

To review each directory and the files provided in the directory structure in detail, review the following Details.

<details>


### `parent` directory

This is the `parent` directory for the project.

In addition to the directories described below, the `parent` directory contains:
  * a `.gitignore` file,
  * a `.gitmodules` file (described in more detail in the [Project Setup](#project-setup) section),
  * the main (driver) `CMakeLists.txt` file,
  * a `CMakePresets.json` file (which contains each configuration / build / test preset, compiler settings, etc.), and
  * a `README.md` file that contains build instructions (i.e. **this** file).

<br>

### `cmake` directory

The `cmake` directory contains the [CMake](https://cmake.org/) include files which are used to both simplify the `CMakeLists.txt` files and to modularize the build system. The entire directory is a git submodule (listed in the `.gitmodules` file in the `parent` directory) and is brought into the project as described in the [Project Setup](#project-setup) section of **this** `README`.

<br>

### `src` directory

The `src` directory is where all of the source code for the blockchain C++ engine is provided:

* *.hpp for C++ header files
* *.cpp for C++ source files

The `src` directory contains a `CMakeLists.txt` file which handles compiling the blockchain C++ engine source code into libraries (defaults to *SHARED* library).

<br>

### `test` directory

The `test` directory is where all of the source code for unit testing the blockchain C++ engine functions is provided:

* *.cpp for C++ source files
* *.hpp for C++ header files

The unit testing is based on values provided in the [References](#references).

The test directory is constructed to mirror the `src` directory so that finding tests for associated functions / routines in the `src` directory should be relatively straightforward.

The [doctest](https://github.com/doctest/doctest) framework is used. It is automatically downloaded and included in the build by [CMake](https://cmake.org/).

<br>

</details>

<br>

\[[toc](#table-of-contents)\]

<br>


The provided [CMakePresets.json](CMakePresets.json) file controls the build and test configurations.

<br>

\[[toc](#table-of-contents)\]

<br>

<hr>


## Adding The Code To Another Project

The code and [CMake](https://cmake.org/) build system are designed to be added to another project as a library / backend.
The steps for adding the code to another project are detailed in this section.

### Main Project Prerequisites

 * the project must already be initialized as a git repo (`git init`)
 * the project must be a [CMake](https://cmake.org/) project using a C++ compiler with the C++17 standard enabled

### Import the code as a git submodule

This code must first be imported into the main project.

<details>
<summary>Debian / Ubuntu </summary>
    
  - open a terminal
  - `cd` into the project `parent` directory
  - run the command strings

            git submodule add https://github.com/gateship1/steam_tables <destination_directory>
            git submodule set-branch --branch main <destination_directory>

  where `<destination_directory>` is the directory where this cloned source will be located in the project

  - run the command strings

            git submodule update --init --recursive --remote

</details>

<br>

\[[toc](#table-of-contents)\]

<br>

<hr>

### Run Backend Server in Terminal

**NOTE:** This option is only guaranteed to work with the option 1 build.

**NOTE:** all shell scripts may require

```console
chmod u+x <name>.sh
```

In the ```api``` directory

```console
./server.py
```

<br>

\[[toc](#table-of-contents)\]

<br>

<hr>

### Deactivate Build Environment
```console
deactivate
```
\[[toc](#table-of-contents)\]

<br>

<hr>

## References

* [Blockchain Technology](https://bitcoin.org/bitcoin.pdf)
* [Blockchain Description](https://www.tutorialspoint.com/blockchain/index.htm)
* [SHA-256 Description](https://en.wikipedia.org/wiki/SHA-2)
* [SHA-256 Algorithm](https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf)
* [SHA-256 Implementation Notes](https://csrc.nist.gov/projects/cryptographic-standards-and-guidelines/example-values)
* [SHA-256 Test Data](https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA256.pdfhttps://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA256.pdf)
* [SHA-256 Additional Test Data](https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA2_Additional.pdf)


<br>

\[[toc](#table-of-contents)\]

<br>

<hr>
