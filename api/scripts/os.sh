#!/bin/sh
os=$(uname -s)
os_v=$(uname -v)

case ${os} in
    Darwin|Linux)
        python_version="$(python3 -V 3>&1)"
        activate() { . $PWD/${venv_name}/bin/activate; }
        os_id=1
        ;;
    CYGWIN*|MINGW32*|MINGW64*|MSYS*|MINGW*)
        python_version="$(python.exe -V 3>&1)"
        activate() { . $PWD/${venv_name}/Scripts/activate; }
        os_id=2
        ;;
    *)
        echo "Operating system unknown"
        os_id=0
        return
        ;;
esac

echo ${os} ${python_exe} ${python_version}
