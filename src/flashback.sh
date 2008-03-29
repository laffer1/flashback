#!/bin/sh
# $Id: flashback.sh,v 1.1 2008/03/29 01:50:07 laffer1 Exp $
#
# PROVIDE: flashback
# REQUIRE: LOGIN cleanvar
#
# Add the following to /etc/rc.conf to start Flashback at boot time:
#
# flashback_enable="YES"
#

flashback_enable="${flashback_enable-NO}"

. /etc/rc.subr

name=flashback
rcvar=`set_rcvar`
command="/usr/local/sbin/flashback"
pidfile="/var/run/${name}.pid"
procname="/usr/local/sbin/flashback"

load_rc_config ${name}
run_rc_command "$1"
