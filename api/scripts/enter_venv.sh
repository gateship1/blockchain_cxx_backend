#!/bin/sh

# Enter the virtual environment
if [[ ${os_id} -eq 2 ]]; then
	activate() { source ../${venv_name}/Scripts/Activate; }
else
	activate() { source ../${venv_name}/bin/activate; }
fi
activate
