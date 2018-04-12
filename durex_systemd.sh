[Unit]
Description= Durex 42 project
After=network-online.target

[Service]
Type=simple

User=root
Group=root

ExecStart=/usr/bin/Durex

Restart=on-failure

TimeoutStopSec=300

[Install]
WantedBy=multi-user.target
