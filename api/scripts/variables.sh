#!/bin/sh

var=server

if [[ ${os_id} -eq 2 ]]; then
    set FLASK_APP=${var}
else
    export FLASK_APP=${var}
#    export PYPATH="/usr/include/python3.6m"
fi
