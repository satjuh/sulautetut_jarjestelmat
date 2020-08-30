
if command -v python3 >/dev/null 2>&1 ; then
	echo installing 
	python -m venv .pythonenv
	source ./.pythonenv/bin/activate
	which pip
	pip install -r requirements.txt
else 
	echo Python not installed
	echo exiting...
fi
