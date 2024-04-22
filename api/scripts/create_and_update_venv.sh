#!/bin/sh
echo "Creating / Updating / Starting Virtual Environment"

source venv_name.sh
source os.sh

# Go to where the venv should be
cd ..

# Check if virtual environment exits
if [ ! -d ${venv_name} ]; then
    
    # create the virtual environment
    echo "Creating virtual environment ${venv_name}"
    python -m pip install virtualenv
    case ${os} in
        Darwin|Linux)
            virtualenv -p /usr/bin/python ${venv_name}
            ;;
        CYGWIN*|MINGW32*|MINGW64*|MSYS*|MINGW*)
            python -m venv ${venv_name}
            ;;
        *)
            echo "Operating system unknown"
            return
            ;;
    esac

else
    echo "${venv_name} already exists - update only"
fi

# activate the virtual environment
echo "Activating ${venv_name}"
cd scripts
source enter_venv.sh
cd ..

# Get latest pip and requirements
python -m pip install --upgrade pip

# get /update requirements
cd scripts
pip install -r requirements.txt --no-cache-dir
cd ..

# set venv variables
cd scripts
source variables.sh
cd ../
