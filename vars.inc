
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
CLEAR='\033[0m'

ERROR=${RED}ERROR:${CLEAR}
WARN=${YELLOW}WARNING:${CLEAR}

: ${HACKBERRY_DIR:="~/.hackberry"}
: ${HACKBERRY_LOG:="hackberry.log"}

HACKBERRY_LOG=$( realpath $HACKBERRY_LOG )