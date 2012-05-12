The Voreen directory structure:
===============================
include               - contains all necessary headers
src                     - contains the source files
modules             - contains all files for plugin modules

apps                    - contains applications using voreen
apps/voreenve      - the main voreen application - the Voreen Visualization Environment
apps/voltool         - perform various preprocessings on volume data

ext                       - external dependencies to other libraries

data                     - contains volume datasets
data/cache            - a temporary folder used for volume caching
data/fonts             - (possibly) necessary fonts
data/networks       - predefined networks which can be used in VoreenVE
data/scripts           - python scripts which can be executed from within voreen
data/workspaces    - predefined workspace to be used with VoreenVE
data/volume          - default directory for volume data
data/transferfuncs  - contains transfer functions in various formats