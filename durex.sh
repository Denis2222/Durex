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
${DEBIAN_SCRIPT_DEBUG:+ set -v -x}

test -x /usr/bin/mysqld_safe || exit 0

. /lib/lsb/init-functions

SELF=$(cd $(dirname $0); pwd -P)/$(basename $0)
DUREX="/usr/bin/Durex"

# priority can be overriden and "-s" adds output to stderr
ERR_LOGGER="logger -p daemon.err -t /etc/init.d/durex -i"

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
    status_output=`$DUREX status 2>&1`; ping_alive=$(( ! $? ))
    ps_alive=0

    if [ "$1" = "check_alive"  -a  $ping_alive = 1 ] ||
       [ "$1" = "check_dead"   -a  $ping_alive = 0 ]; then
	return 0 # EXIT_SUCCESS
    else
  	if [ "$2" = "warn" ]; then
  	    echo -e "$ps_alive processes alive and '$DUREX status' resulted in\n$status_output\n" | $ERR_LOGGER -p daemon.debug
	fi
  	return 1 # EXIT_FAILURE
    fi
}

#
# main()
#

case "${1:-''}" in
  'start')
	# Start daemon
	log_daemon_msg "Starting Durex server" "durex"
	log_end_msg 0
	if durex_status check_alive nowarn; then
	   log_progress_msg "already running"
	   log_end_msg 0
	else
	    # Start Durex!
  	    /usr/bin/Durex > /dev/null 2>&1 &
	fi
	;;

  'stop')
	# * As a passwordless mysqladmin (e.g. via ~/.my.cnf) must be possible
	# at least for cron, we can rely on it here, too. (although we have
	# to specify it explicit as e.g. sudo environments points to the normal
	# users home and not /root)
	log_daemon_msg "Stopping Durex server" "durex"
	if ! durex_status check_dead nowarn; then
		set +e
		shutdown_out=`$DUREX shutdown 2>&1`; r=$?
		set -e
			if [ "$r" -ne 0 ]; then
				log_end_msg 1
				[ "$VERBOSE" != "no" ] && log_failure_msg "Error: $shutdown_out"
				log_daemon_msg "Killing Durex server by signal" "durex"
				killall -15 durex
				server_down=
				for i in 1 2 3 4 5 6 7 8 9 10; do
					sleep 1
					if durex_status check_dead nowarn; then server_down=1; break; fi
				done
					if test -z "$server_down"; then killall -9 durex;
					fi
			fi
	fi
	if ! durex_status check_dead warn; then
		log_end_msg 1
		log_failure_msg "Please stop Durex manually !"
		exit -1
	else
		log_end_msg 0
	fi
	;;

  'restart')
	set +e; $SELF stop; set -e
	$SELF start
	;;

  'reload'|'force-reload')
  	log_daemon_msg "Reloading Durex server" "durex"
	$DUREX reload
	log_end_msg 0
	;;

  'status')
	if durex_status check_alive nowarn; then
	  log_action_msg "$($DUREX version)"
	else
	  log_action_msg "Durex is stopped."
	  exit 3
	fi
  	;;

  *)
	echo "Usage: $SELF start|stop|restart|reload|force-reload|status"
	exit 1
	;;
esac
