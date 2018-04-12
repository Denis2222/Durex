#!/bin/bash
#
### BEGIN INIT INFO
# Provides:          durex
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Should-Start:      $network $time
# Should-Stop:       $network $time
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start and stop the mysql database server daemon
# Description:       Controls the main MySQL database server daemon "mysqld"
#                    and its wrapper script "mysqld_safe".
### END INIT INFO
#
set -e
set -u
${DEBIAN_SCRIPT_DEBUG:+ set -v -x};

. /lib/lsb/init-functions

PATH="/usr/bin/:/bin/"
DAEMON="/usr/bin/Durex"
SELF=$(cd $(dirname $0); pwd -P)/$(basename $0)
DUREX="/usr/bin/Durex"

# priority can be overriden and "-s" adds output to stderr
ERR_LOGGER="logger -p daemon.err -t /etc/init.d/Durex -i"

# Safeguard (relative paths, core dumps..)
cd /
umask 077

## Checks if there is a server running and if so if it is accessible.
#
# check_alive :
# $? == 1 is alive
# $? == 0 is dead
#
# Usage: boolean durex_status [check_alive|check_dead] [warn|nowarn]
durex_status () {
    status_output=`$DUREX status 2>&1`
	ping_alive=$?
    if [ "$1" = "check_alive"  -a  $ping_alive = 1 ] ||
       [ "$1" = "check_dead"   -a  $ping_alive = 0 ]; then
	return 0 # EXIT_SUCCESS
    else
  	if [ "$2" = "warn" ]; then
  	    echo -e "processes alive and '$DUREX status' resulted in\n" | $ERR_LOGGER -p daemon.debug
	fi
  	return 1 # EXIT_FAILURE
    fi
}

#
# main()
#

#case "${1:-''}" in
case "$1" in
  'start')
	$SELF system-reload
	# Start daemon
	log_daemon_msg "Starting Durex server" "durex"
	log_end_msg 0
	if durex_status check_alive nowarn; then
	   echo "Durex already running."
	else
	    # Start Durex!
		start_daemon -p `$DUREX pid` $DAEMON
		if !durex_status; then
			$DAEMON > /dev/null 2>&1 &
		fi
	fi
	;;

  'stop')
	$SELF system-reload
	log_daemon_msg "Stopping Durex server" "durex"
	log_end_msg 0
	if durex_status check_alive nowarn; then #service systemctl
		killproc -p `$DUREX pid` $DAEMON
	fi
	if durex_status check_alive nowarn; then #service init
		kill -15 `$DUREX pid`
	fi
	;;

  'restart')
	$SELF stop
	$SELF start
	;;

  'reload'|'force-reload')
	$SELF system-reload
	log_daemon_msg "Reloading Durex server" "durex"
	$DUREX reload
	log_end_msg 0
	;;

  'status')
	$SELF system-reload
	status_of_proc -p `$DUREX pid` $DAEMON Durex && exit 0 || exit $?
  	;;

  'system-reload')
	systemctl daemon-reload
	;;
  *)
	echo "Usage: $SELF start|stop|restart|reload|force-reload|status"
	exit 1
	;;
esac
